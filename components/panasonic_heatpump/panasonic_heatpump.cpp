#include "panasonic_heatpump.h"
#include "esphome/core/application.h"

namespace esphome {
namespace panasonic_heatpump {
static const char* const TAG = "panasonic_heatpump";

void PanasonicHeatpumpComponent::dump_config() {
  ESP_LOGW(TAG, "*** Panasonic Heatpump Component v%s ***", PANASONIC_HEATPUMP_VERSION);
}

void PanasonicHeatpumpComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Panasonic Heatpump ...");
  this->check_uart_settings(9600, 1, uart::UART_CONFIG_PARITY_EVEN, 8);

  this->response_queue_handle_ = xQueueCreate(8, sizeof(std::vector<uint8_t>*));
  if (this->response_queue_handle_ == nullptr) {
    ESP_LOGE(TAG, "Failed to create response queue!");
    this->mark_failed();
    return;
  }
  this->request_queue_handle_ = xQueueCreate(8, sizeof(std::vector<uint8_t>*));
  if (this->request_queue_handle_ == nullptr) {
    ESP_LOGE(TAG, "Failed to create request queue!");
    this->mark_failed();
    return;
  }

  // Start task
  xTaskCreatePinnedToCore(PanasonicHeatpumpComponent::uart_task, "uart_handler", 4096, this,
                          tskIDLE_PRIORITY + 1,  // Low priority, important for single-core C3
                          &this->uart_task_handle_,
                          tskNO_AFFINITY  // important for single-core C3
  );
  if (this->uart_client_ != nullptr) {
    xTaskCreatePinnedToCore(PanasonicHeatpumpComponent::uart_client_task, "uart_client_handler", 4096, this,
                            tskIDLE_PRIORITY + 1,  // Low priority, important for single-core C3
                            &this->uart_client_task_handle_,
                            tskNO_AFFINITY  // important for single-core C3
    );
  }

  if (this->uart_client_ != nullptr && this->uart_client_timeout_ < 100) {
    ESP_LOGI(TAG, "Self polling disabled (uart_client_timeout_ < 100ms). Not sending initial request.");
    return;
  }
}

void PanasonicHeatpumpComponent::update() {
  // Do not send polling requests if a uart client (CZ-TAW1) is configured and timeout is set too low.
  if (this->uart_client_ != nullptr && this->uart_client_timeout_ < 100)
    return;

  // If a uart client (CZ-TAW1) is configured, check if the last request from the client is too long ago.
  // If so, send polling request to heatpump again.
  if (this->uart_client_ != nullptr && !this->uart_client_timeout_exceeded_) {
    if (millis() - this->last_client_request_time_ > uart_client_timeout_)
      this->uart_client_timeout_exceeded_ = true;
    else
      return;
  }

  ESP_LOGD(TAG, "Queue polling request");
  this->queue_request(build_message(PanasonicCommand::PollingMessage));
}

void PanasonicHeatpumpComponent::loop() {
  switch (this->loop_state_) {
  case LoopState::READ_RESPONSE: {
    switch (this->read_response()) {
    case ResponseType::STANDARD:
      this->loop_state_ = LoopState::PUBLISH_SENSOR;
      break;
    case ResponseType::EXTRA:
      this->loop_state_ = LoopState::PUBLISH_EXTRA_SENSOR;
      break;
    default:
      this->loop_state_ = LoopState::SEND_REQUEST;
      break;
    };
    break;
  }
  case LoopState::PUBLISH_SENSOR:
    for (auto* entity : this->sensors_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::PUBLISH_BINARY_SENSOR;
    break;
  case LoopState::PUBLISH_BINARY_SENSOR:
    for (auto* entity : this->binary_sensors_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::PUBLISH_TEXT_SENSOR;
    break;
  case LoopState::PUBLISH_TEXT_SENSOR:
    for (auto* entity : this->text_sensors_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::PUBLISH_NUMBER;
    break;
  case LoopState::PUBLISH_NUMBER:
    for (auto* entity : this->numbers_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::PUBLISH_SELECT;
    break;
  case LoopState::PUBLISH_SELECT:
    for (auto* entity : this->selects_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::PUBLISH_SWITCH;
    break;
  case LoopState::PUBLISH_SWITCH:
    for (auto* entity : this->switches_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::PUBLISH_CLIMATE;
    break;
  case LoopState::PUBLISH_CLIMATE:
    for (auto* entity : this->climates_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::PUBLISH_WATER_HEATER;
    break;
  case LoopState::PUBLISH_WATER_HEATER:
    for (auto* entity : this->water_heaters_) {
      entity->publish_new_state(this->heatpump_default_message_);
    }
    this->loop_state_ = LoopState::SEND_REQUEST;
    break;
  case LoopState::PUBLISH_EXTRA_SENSOR:
    for (auto* entity : this->extra_sensors_) {
      entity->publish_new_state(this->heatpump_extra_message_);
    }
    this->loop_state_ = LoopState::SEND_REQUEST;
    break;
  case LoopState::SEND_REQUEST:
    this->send_request();
    // fallthrough
  default:
    this->loop_state_ = LoopState::READ_RESPONSE;
    break;
  };
}

void PanasonicHeatpumpComponent::uart_task(void* pvParameters) {
  auto* self = static_cast<PanasonicHeatpumpComponent*>(pvParameters);
  std::vector<uint8_t> rx_buffer;
  rx_buffer.reserve(256);

  while (true) {
    // Process the data from the UART interface connected to the heatpump
    if (self->receive_from_uart(self->parent_, rx_buffer)) {
      auto* message = new std::vector<uint8_t>(rx_buffer);
      if (xQueueSend(self->response_queue_handle_, &message, 0) != pdPASS) {
        ESP_LOGW(TAG, "Response queue full or unavailable, dropping message");
        delete message;
      }
      // ... and pass on a copy to CZ-TAW1
      if (self->uart_client_ != nullptr) {
        self->uart_client_->write_array(rx_buffer);
      }
    } else {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}

void PanasonicHeatpumpComponent::uart_client_task(void* pvParameters) {
  auto* self = static_cast<PanasonicHeatpumpComponent*>(pvParameters);
  std::vector<uint8_t> rx_buffer;
  rx_buffer.reserve(256);

  while (true) {
    // Process the data from the UART interface connected to the client (CZ-TAW1)
    if (self->receive_from_uart(self->uart_client_, rx_buffer)) {
      auto* message = new std::vector<uint8_t>(rx_buffer);
      if (xQueueSend(self->request_queue_handle_, &message, 0) != pdPASS) {
        ESP_LOGW(TAG, "Request queue full or unavailable, dropping message");
        delete message;
      }
      self->last_client_request_time_ = millis();
      self->uart_client_timeout_exceeded_ = false;
    } else {
      vTaskDelay(pdMS_TO_TICKS(10));
    }
  }
}

// Used for both uart interfaces
bool PanasonicHeatpumpComponent::receive_from_uart(uart::UARTComponent* uartComp, std::vector<uint8_t>& buffer) {
  uint8_t start_byte;

  // Wait for the start byte to be available
  while (!uartComp->available())
    vTaskDelay(pdMS_TO_TICKS(5));

  // Read the first byte
  if (!uartComp->read_byte(&start_byte))
    return false;

  // Message shall start with 0x31, 0x71 or 0xF1, if not skip this byte
  if (start_byte != 0x31 && start_byte != 0x71 && start_byte != 0xF1) {
    return false;
  }

  // Prepare buffer for header reading, header is 4 bytes long and first byte is already read.
  // Message may be up to 256 bytes long, so reserve enough space to avoid dynamic resizing during reading.
  buffer.clear();
  buffer.reserve(256);
  buffer.resize(HEADER_SIZE);
  buffer[0] = start_byte;

  // Read the rest of the header
  if (uartComp->available() < HEADER_SIZE - 1)
    vTaskDelay(pdMS_TO_TICKS(5));
  auto succeed = uartComp->read_array(&buffer[1], HEADER_SIZE - 1);

  // Verify header (start byte, message type and length)
  if (!verify_message_header(buffer, succeed))
    return false;

  // Read the rest of the message according to the length specified in the header
  size_t total_expected = buffer[1] + 3;
  size_t remaining = total_expected - buffer.size();

  while (remaining > 0) {
    size_t current_size = buffer.size();
    size_t to_read = std::min((size_t)8, remaining);
    buffer.resize(current_size + to_read);
    if (uartComp->available() < to_read)
      vTaskDelay(pdMS_TO_TICKS(10));
    if (!uartComp->read_array(&buffer[current_size], to_read)) {
      ESP_LOGW(TAG, "Timeout while reading message body");
      return false;
    }
    remaining -= to_read;
  }

  // Verify checksum (should be 0 if all bytes are summed up)
  if (!verify_message_checksum(buffer)) {
    return false;
  }

  // message is complete
  return true;
}

bool PanasonicHeatpumpComponent::verify_message_header(const std::vector<uint8_t>& message, bool reading_succeeded) {
  if (!reading_succeeded) {
    ESP_LOGW(TAG, "Timeout while reading message header");
    return false;
  }

  if (message.size() < HEADER_SIZE) {
    ESP_LOGW(TAG, "Message too short to contain valid header");
    return false;
  }

  if ((message[2] != 0x01 && message[2] != 0x10) ||                        // 3. byte shall be 0x01 or 0x10
      (message[3] != 0x01 && message[3] != 0x10 && message[3] != 0x21)) {  // 4. byte shall be 0x01, 0x10 or 0x21
    ESP_LOGW(TAG, "Invalid message header: 0x%s. Drop message.",
             PanasonicHelpers::byte_array_to_hex_string(message, ',').c_str());
    return false;
  }

  return true;
}

bool PanasonicHeatpumpComponent::verify_message_checksum(const std::vector<uint8_t>& message) {
  uint8_t checksum = 0;
  for (const auto b : message)
    checksum += b;

  if (checksum != 0) {
    ESP_LOGW(TAG, "Invalid message checksum: 0x%02X. Last byte: 0x%02X", checksum, message.back());
    return false;
  }
  return true;
}

ResponseType PanasonicHeatpumpComponent::read_response() {
  // Get message from queue
  std::vector<uint8_t>* message{nullptr};
  if (xQueueReceive(this->response_queue_handle_, &message, 0) != pdPASS || message == nullptr) {
    return ResponseType::UNKNOWN; // nothing queued
  }
  PanasonicHelpers::write_uart_log(UART_LOG_RX, *message, ',', this->log_uart_msg_);

  if (!this->check_response_length(*message)) {
    delete message;
    return ResponseType::UNKNOWN;
  }

  // Get response type and save the response
  auto responseType = ResponseType::UNKNOWN;
  if (message->at(3) == 0x10) {
    responseType = ResponseType::STANDARD;
    this->heatpump_default_message_ = std::move(*message);

    // Is an extra request required?
    if (message->at(199) > 0x02) {
      ESP_LOGD(TAG, "Queue extra polling request");
      this->queue_request(build_message(PanasonicCommand::PollingExtraMessage));
    }
  } else if (message->at(3) == 0x21) {
    responseType = ResponseType::EXTRA;
    this->heatpump_extra_message_ = std::move(*message);
  }

  delete message;
  return responseType;
}

bool PanasonicHeatpumpComponent::check_response_length(const std::vector<uint8_t>& message) {
  // Read response message:
  // format:          0x71 [payload_length] 0x01 [0x10 || 0x21] [[TOP0 - TOP114] ...] 0x00 [checksum]
  // payload_length:  payload_length + 3 = packet_length
  // checksum:        if (sum(all bytes) & 0xFF == 0) ==> valid packet
  if (message.size() == RESPONSE_MSG_SIZE)
    return true;

  ESP_LOGW(TAG, "Invalid response message length: recieved %d - expected %d", message.size(), RESPONSE_MSG_SIZE);
  return false;
}

void PanasonicHeatpumpComponent::send_request() {
  if (millis() - request_send_time_ < REQUEST_SEND_INTERVAL) {
    // wait until the interval is over
    return;
  }

  // Get message from queue
  std::vector<uint8_t>* message{nullptr};
  if (xQueueReceive(this->request_queue_handle_, &message, 0) != pdPASS || message == nullptr) {
    return;  // nothing queued
  }
  PanasonicHelpers::write_uart_log(UART_LOG_TX, *message, ',', this->log_uart_msg_);

  // Send vector content over UART (robust API usage)
  this->write_array(message->data(), message->size());
  delete message;
  request_send_time_ = millis();
}

void PanasonicHeatpumpComponent::set_command_high_nibble(const uint8_t value, const uint8_t index) {
  this->command_message_ = build_message(PanasonicCommand::CommandMessage);

  uint8_t lowNibble = this->heatpump_default_message_[index] & 0b1111;
  uint8_t highNibble = value << 4;
  // set command byte
  this->command_message_[index] = highNibble + lowNibble;
  // calculate and set set checksum (last element)
  this->command_message_.back() =
      PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

  ESP_LOGD(TAG, "Queue command request");
  this->queue_request(this->command_message_);
}

void PanasonicHeatpumpComponent::set_command_low_nibble(const uint8_t value, const uint8_t index) {
  this->command_message_ = build_message(PanasonicCommand::CommandMessage);

  uint8_t highNibble = this->heatpump_default_message_[index] & 0b11110000;
  uint8_t lowNibble = value & 0b1111;
  // set command byte
  this->command_message_[index] = highNibble + lowNibble;
  // calculate and set set checksum (last element)
  this->command_message_.back() =
      PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

  ESP_LOGD(TAG, "Queue command request");
  this->queue_request(this->command_message_);
}

void PanasonicHeatpumpComponent::set_command_byte(const uint8_t value, const uint8_t index) {
  this->command_message_ = build_message(PanasonicCommand::CommandMessage);

  // set command byte
  this->command_message_[index] = value;
  // calculate and set set checksum (last element)
  this->command_message_.back() =
      PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

  ESP_LOGD(TAG, "Queue command request");
  this->queue_request(this->command_message_);
}

void PanasonicHeatpumpComponent::set_command_curve(const uint8_t value, const uint8_t index) {
  this->command_message_ = build_message(PanasonicCommand::CommandMessage);

  // Set zone 1 curve bytes
  if (index == 75 || index == 76 || index == 77 || index == 78 || index == 86 || index == 87 || index == 88 ||
      index == 89) {
    this->command_message_[75] = this->heatpump_default_message_[75];
    this->command_message_[76] = this->heatpump_default_message_[76];
    this->command_message_[77] = this->heatpump_default_message_[77];
    this->command_message_[78] = this->heatpump_default_message_[78];
    this->command_message_[86] = this->heatpump_default_message_[86];
    this->command_message_[87] = this->heatpump_default_message_[87];
    this->command_message_[88] = this->heatpump_default_message_[88];
    this->command_message_[89] = this->heatpump_default_message_[89];
  }
  // Set zone 2 curve bytes
  if (index == 79 || index == 80 || index == 81 || index == 82 || index == 90 || index == 91 || index == 92 ||
      index == 93) {
    this->command_message_[79] = this->heatpump_default_message_[79];
    this->command_message_[80] = this->heatpump_default_message_[80];
    this->command_message_[81] = this->heatpump_default_message_[81];
    this->command_message_[82] = this->heatpump_default_message_[82];
    this->command_message_[90] = this->heatpump_default_message_[90];
    this->command_message_[91] = this->heatpump_default_message_[91];
    this->command_message_[92] = this->heatpump_default_message_[92];
    this->command_message_[93] = this->heatpump_default_message_[93];
  }

  // set command byte
  this->command_message_[index] = value;
  // calculate and set set checksum (last element)
  this->command_message_.back() =
      PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

  ESP_LOGD(TAG, "Queue command request");
  this->queue_request(this->command_message_);
}

void PanasonicHeatpumpComponent::queue_request(const std::vector<uint8_t>& message) {
  auto* cmd = new std::vector<uint8_t>(message);

  // Check request_queue_handle_, function is called before setup() initializes it!
  if (this->request_queue_handle_ == nullptr || xQueueSend(this->request_queue_handle_, &cmd, 0) != pdPASS) {
    ESP_LOGW(TAG, "Request queue full or unavailable, dropping message");
    delete cmd;
  }
}

// This function can be used in esphome lambda to get a specific byte
int PanasonicHeatpumpComponent::get_response_byte(const int index) {
  if (this->heatpump_default_message_.size() > index)
    return this->heatpump_default_message_[index];
  return -1;
}

// This function can be used in esphome lambda to get a specific byte
int PanasonicHeatpumpComponent::get_extra_response_byte(const int index) {
  if (this->heatpump_extra_message_.size() > index)
    return this->heatpump_extra_message_[index];
  return -1;
}
}  // namespace panasonic_heatpump
}  // namespace esphome
