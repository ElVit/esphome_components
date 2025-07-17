#include "panasonic_heatpump.h"
#include "esphome/core/application.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    static const char *const TAG = "panasonic_heatpump";

    void PanasonicHeatpumpComponent::dump_config()
    {
      ESP_LOGCONFIG(TAG, "Panasonic Heatpump");
      delay(10);  // NOLINT
    }

    void PanasonicHeatpumpComponent::setup()
    {
      ESP_LOGCONFIG(TAG, "Setting up Panasonic Heatpump ...");
      delay(10);  // NOLINT
      this->check_uart_settings(9600, 1, uart::UART_CONFIG_PARITY_EVEN, 8);

      this->update();
    }

    void PanasonicHeatpumpComponent::update()
    {
      if (this->uart_client_ != nullptr) return;
      this->next_request_ = this->send_extra_request_ ? RequestType::POLLING_EXTRA : RequestType::POLLING;
    }

    void PanasonicHeatpumpComponent::loop()
    {
      switch (this->loop_state_)
      {
        case LoopState::READ_RESPONSE:
          this->read_response();
          this->loop_state_ = LoopState::CHECK_RESPONSE;
          break;
        case LoopState::CHECK_RESPONSE:
          this->current_response_ = this->check_response(this->response_message_);
          switch (this->current_response_)
          {
            case ResponseType::UNKNOWN:
              this->loop_state_ = LoopState::SEND_REQUEST;
              break;
            case ResponseType::DEFAULT:
              this->loop_state_ = LoopState::PUBLISH_SENSOR;
              break;
            case ResponseType::EXTRA:
              this->loop_state_ = LoopState::PUBLISH_EXTRA_SENSOR;
              break;
          };
          break;
        case LoopState::PUBLISH_SENSOR:
          for (auto *entity : this->sensors_)
          {
            entity->publish_new_state(this->heatpump_default_message_);
          }
          this->loop_state_ = LoopState::PUBLISH_BINARY_SENSOR;
          break;
        case LoopState::PUBLISH_BINARY_SENSOR:
          for (auto *entity : this->binary_sensors_)
          {
            entity->publish_new_state(this->heatpump_default_message_);
          }
          this->loop_state_ = LoopState::PUBLISH_TEXT_SENSOR;
          break;
        case LoopState::PUBLISH_TEXT_SENSOR:
          for (auto *entity : this->text_sensors_)
          {
            entity->publish_new_state(this->heatpump_default_message_);
          }
          this->loop_state_ = LoopState::PUBLISH_NUMBER;
          break;
        case LoopState::PUBLISH_NUMBER:
          for (auto *entity : this->numbers_)
          {
            entity->publish_new_state(this->heatpump_default_message_);
          }
          this->loop_state_ = LoopState::PUBLISH_SELECT;
          break;
        case LoopState::PUBLISH_SELECT:
          for (auto *entity : this->selects_)
          {
            entity->publish_new_state(this->heatpump_default_message_);
          }
          this->loop_state_ = LoopState::PUBLISH_SWITCH;
          break;
        case LoopState::PUBLISH_SWITCH:
          for (auto *entity : this->switches_)
          {
            entity->publish_new_state(this->heatpump_default_message_);
          }
          this->loop_state_ = LoopState::PUBLISH_CLIMATE;
          break;
        case LoopState::PUBLISH_CLIMATE:
          for (auto *entity : this->climates_)
          {
            entity->publish_new_state(this->heatpump_default_message_);
          }
          this->loop_state_ = LoopState::SEND_REQUEST;
          break;
        case LoopState::PUBLISH_EXTRA_SENSOR:
          for (auto *entity : this->extra_sensors_)
          {
            entity->publish_new_state(this->heatpump_extra_message_);
          }
          this->loop_state_ = LoopState::SEND_REQUEST;
          break;
        case LoopState::SEND_REQUEST:
          this->send_request(this->next_request_);
          this->loop_state_ = LoopState::READ_REQUEST;
          break;
        case LoopState::READ_REQUEST:
          this->read_request();
          this->loop_state_ = LoopState::RESTART_LOOP;
          break;
        default:
          this->loop_state_ = LoopState::READ_RESPONSE;
          break;
      };
    }

    void PanasonicHeatpumpComponent::read_response()
    {
      while (this->available())
      {
        // Read each byte from heatpump and forward it directly to the client (CZ-TAW1)
        this->read_byte(&byte_);
        if (this->uart_client_ != nullptr)
        {
          this->uart_client_->write_byte(byte_);
        }

        // Message shall start with 0x31, 0x71 or 0xF1, if not skip this byte
        if (!this->response_receiving_)
        {
          if (byte_ != 0x31 && byte_ != 0x71 && byte_ != 0xF1) continue;
          this->response_message_.clear();
          this->response_receiving_ = true;
        }
        // Add current byte to message buffer
        this->response_message_.push_back(byte_);

        // 2. byte contains the payload size
        if (this->response_message_.size() == 2)
        {
          this->payload_length_ = byte_;
        }
        // Discard message if format is wrong
        if ((this->response_message_.size() == 3 ||
            this->response_message_.size() == 4) &&
            byte_ != 0x01 && byte_ != 0x10 && byte_ != 0x21)
        {
          this->response_receiving_ = false;
          ESP_LOGW(TAG, "Invalid response message: %d. byte is 0x%02X but expexted is 0x01 or 0x10",
            response_message_.size(), byte_);
          delay(10);  // NOLINT
          continue;
        }

        // Check if message is complete
        if (this->response_message_.size() > 2 &&
            this->response_message_.size() == this->payload_length_ + 3)
        {
          this->response_receiving_ = false;
          this->current_response_count_++;
          if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_RX, this->response_message_, ',');
        }
      }
    }

    void PanasonicHeatpumpComponent::send_request(RequestType requestType)
    {
      switch (requestType)
      {
        case RequestType::COMMAND:
        {
          if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, this->command_message_, ',');
          this->write_array(this->command_message_);
          this->flush();
          break;
        }
        case RequestType::INITIAL:
        {
          // Probably not necessary but CZ-TAW1 sends this query on boot
          if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, PanasonicCommand::InitialRequest, INIT_REQUEST_SIZE, ',');
          this->write_array(PanasonicCommand::InitialRequest, INIT_REQUEST_SIZE);
          this->flush();
          break;
        }
        case RequestType::POLLING:
        {
          if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, PanasonicCommand::PollingMessage, DATA_MESSAGE_SIZE, ',');
          this->write_array(PanasonicCommand::PollingMessage, DATA_MESSAGE_SIZE);
          this->flush();
          break;
        }
        case RequestType::POLLING_EXTRA:
        {
          if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, PanasonicCommand::PollingExtraMessage, DATA_MESSAGE_SIZE, ',');
          this->write_array(PanasonicCommand::PollingMessage, DATA_MESSAGE_SIZE);
          this->flush();
          break;
        }
      };

      this->next_request_ = RequestType::NONE;
    }

    void PanasonicHeatpumpComponent::read_request()
    {
      if (this->uart_client_ == nullptr) return;

      while (this->uart_client_->available())
      {
        // Read each byte from client and forward it directly to the heatpump
        this->uart_client_->read_byte(&byte_);
        this->write_byte(byte_);

        // Message shall start with 0x31, 0x71 or 0xF1, if not skip this byte
        if (!this->request_receiving_)
        {
          if (byte_ != 0x31 && byte_ != 0x71 && byte_ != 0xF1) continue;
          this->request_message_.clear();
          this->request_receiving_ = true;
        }
        // Add current byte to message buffer
        this->request_message_.push_back(byte_);

        // 2. byte contains the payload size
        if (this->request_message_.size() == 2)
        {
          this->payload_length_ = byte_;
        }
        // Discard message if format is wrong
        if ((this->request_message_.size() == 3 ||
            this->request_message_.size() == 4) &&
            byte_ != 0x01 && byte_ != 0x10 && byte_ != 0x21)
        {
          this->request_receiving_ = false;
          ESP_LOGW(TAG, "Invalid request message: %d. byte is 0x%02X but expexted is 0x01 or 0x10",
            request_message_.size(), byte_);
          delay(10);  // NOLINT
          continue;
        }

        // Check if message is complete
        if (this->request_message_.size() > 2 &&
            this->request_message_.size() == this->payload_length_ + 3)
        {
          this->request_receiving_ = false;
          if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, this->request_message_, ',');
        }
      }
    }

    int PanasonicHeatpumpComponent::getResponseByte(const int index)
    {
      if (this->heatpump_default_message_.size() > index) return this->heatpump_default_message_[index];
      return -1;
    }

    int PanasonicHeatpumpComponent::getExtraResponseByte(const int index)
    {
      if (this->heatpump_extra_message_.size() > index) return this->heatpump_extra_message_[index];
      return -1;
    }

    ResponseType PanasonicHeatpumpComponent::check_response(const std::vector<uint8_t>& data)
    {
      // Read response message:
      // format:          0x71 [payload_length] 0x01 [0x10 || 0x21] [[TOP0 - TOP114] ...] 0x00 [checksum]
      // payload_length:  payload_length + 3 = packet_length
      // checksum:        if (sum(all bytes) & 0xFF == 0) ==> valid packet

      if (data.empty()) return ResponseType::UNKNOWN;
      if (data[0] != 0x71) return ResponseType::UNKNOWN;
      if (this->response_receiving_) return ResponseType::UNKNOWN;
      if (data.size() != RESPONSE_MSG_SIZE)
      {
        ESP_LOGW(TAG, "Invalid response message length: recieved %d - expected %d", data.size(), RESPONSE_MSG_SIZE);
        delay(10);  // NOLINT
        return ResponseType::UNKNOWN;
      }

      // Verify checksum
      uint8_t checksum = 0;
      for (int i = 0; i < data.size(); i++)
      {
        checksum += data[i];
      }
      // all bytes (including checksum byte) shall be 0x00
      if (checksum != 0)
      {
        ESP_LOGW(TAG, "Invalid response message: checksum = 0x%02X, last_byte = 0x%02X", checksum, data[202]);
        delay(10);  // NOLINT
        return ResponseType::UNKNOWN;
      }

      // Get response type and save the response
      auto responseType = ResponseType::UNKNOWN;
      if (data[3] == 0x10)
      {
        responseType = ResponseType::DEFAULT;
        this->heatpump_default_message_ = data;
        this->send_extra_request_ = data[199] > 0x02 ? true : false;
      }
      else if (data[3] == 0x21) 
      {
        responseType = ResponseType::EXTRA;
        this->heatpump_extra_message_ = data;
        this->send_extra_request_ = false;
      }
      if (responseType == ResponseType::UNKNOWN)
      {
        ESP_LOGW(TAG, "Unknown response type (4. byte): 0x%02X. Expected 0x10 or 0x21.", data[3]);
        delay(10);  // NOLINT
        this->send_extra_request_ = false;
        return ResponseType::UNKNOWN;
      }

      // Check if the current response is a new response
      if (this->last_response_count_ == this->current_response_count_) return ResponseType::UNKNOWN;
      this->last_response_count_ = this->current_response_count_;

      return responseType;
    }

    void PanasonicHeatpumpComponent::set_command_high_nibble(const uint8_t value, const uint8_t index)
    {
      if (this->next_request_ != RequestType::COMMAND)
      {
        // initialize the command
        this->command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }
      uint8_t lowNibble = this->heatpump_default_message_[index] & 0b1111;
      uint8_t highNibble = value << 4;
      // set command byte
      this->command_message_[index] = highNibble + lowNibble;
      // calculate and set set checksum (last element)
      this->command_message_.back() = PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

      // command will be send on next loop
      this->next_request_ = RequestType::COMMAND;
    }

    void PanasonicHeatpumpComponent::set_command_low_nibble(const uint8_t value, const uint8_t index)
    {
      if (this->next_request_ != RequestType::COMMAND)
      {
        // initialize the command
        this->command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }
      uint8_t highNibble = this->heatpump_default_message_[index] & 0b11110000;
      uint8_t lowNibble = value & 0b1111;
      // set command byte
      this->command_message_[index] = highNibble + lowNibble;
      // calculate and set set checksum (last element)
      this->command_message_.back() = PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

      // command will be send on next loop
      this->next_request_ = RequestType::COMMAND;
    }

    void PanasonicHeatpumpComponent::set_command_byte(const uint8_t value, const uint8_t index)
    {
      if (this->next_request_ != RequestType::COMMAND)
      {
        // initialize the command
        this->command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }
      // set command byte
      this->command_message_[index] = value;
      // calculate and set set checksum (last element)
      this->command_message_.back() = PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

      // command will be send on next loop
      this->next_request_ = RequestType::COMMAND;
    }

    void PanasonicHeatpumpComponent::set_command_curve(const uint8_t value, const uint8_t index)
    {
      if (this->next_request_ != RequestType::COMMAND)
      {
        // initialize the command
        this->command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }

      // Set zone 1 curve bytes
      if (index == 75 || index == 76 || index == 77 || index == 78 ||
          index == 86 || index == 87 || index == 88 || index == 89)
      {
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
      if (index == 79 || index == 80 || index == 81 || index == 82 ||
          index == 90 || index == 91 || index == 92 || index == 93)
      {
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
      this->command_message_.back() = PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

      // command will be send on next loop
      this->next_request_ = RequestType::COMMAND;
    }
  }  // namespace panasonic_heatpump
}  // namespace esphome
