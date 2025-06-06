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

      // initialize vector
      for (int i = 0; i < RESPONSE_MSG_SIZE; i++)
      {
        this->last_message_.push_back(0);
      }

      this->update();
    }

    void PanasonicHeatpumpComponent::update()
    {
      if (this->uart_client_ == nullptr)
        this->next_request_ = RequestType::POLLING;
    }

    void PanasonicHeatpumpComponent::loop()
    {
      switch (this->loop_state_)
      {
        case LoopState::READ_RESPONSE:
        {
          this->read_response();
          this->loop_state_ = LoopState::CHECK_RESPONSE;
          break;
        }
        case LoopState::CHECK_RESPONSE:
        {
          bool result = this->check_response(this->heatpump_message_);
          this->loop_state_ = result ?
            LoopState::SET_NUMBER_TRAITS : LoopState::SEND_REQUEST;
          break;
        }
        case LoopState::SET_NUMBER_TRAITS:
        {
          this->set_number_traits(this->heatpump_message_);
          this->loop_state_ = LoopState::PUBLISH_SENSOR;
          break;
        }
        case LoopState::PUBLISH_SENSOR:
        {
          this->publish_sensor(this->heatpump_message_, this->last_message_);
          this->loop_state_ = LoopState::PUBLISH_BINARY_SENSOR;
          break;
        }
        case LoopState::PUBLISH_BINARY_SENSOR:
        {
          this->publish_binary_sensor(this->heatpump_message_, this->last_message_);
          this->loop_state_ = LoopState::PUBLISH_TEXT_SENSOR;
          break;
        }
        case LoopState::PUBLISH_TEXT_SENSOR:
        {
          this->publish_text_sensor(this->heatpump_message_, this->last_message_);
          this->loop_state_ = LoopState::PUBLISH_NUMBER;
          break;
        }
        case LoopState::PUBLISH_NUMBER:
        {
          this->publish_number(this->heatpump_message_, this->last_message_);
          this->loop_state_ = LoopState::PUBLISH_SELECT;
          break;
        }
        case LoopState::PUBLISH_SELECT:
        {
          this->publish_select(this->heatpump_message_, this->last_message_);
          this->loop_state_ = LoopState::PUBLISH_SWITCH;
          break;
        }
        case LoopState::PUBLISH_SWITCH:
        {
          this->publish_switch(this->heatpump_message_, this->last_message_);
          this->loop_state_ = LoopState::STORE_RESPONSE;
          break;
        }
        case LoopState::STORE_RESPONSE:
        {
          this->last_message_ = this->heatpump_message_;
          this->loop_state_ = LoopState::SEND_REQUEST;
          break;
        }
        case LoopState::SEND_REQUEST:
        {
          this->send_request(this->next_request_);
          this->loop_state_ = LoopState::READ_REQUEST;
          break;
        }
        case LoopState::READ_REQUEST:
        {
          this->read_request();
          this->loop_state_ = LoopState::RESTART_LOOP;
          break;
        }
        default:
        {
          // Perform reboot only if a traits (e.g. min/max value of a number entity) was changed the second time.
          // The first traits change should happen before controller is connected to home assistant,
          // because the default min/max value is 0.
          if (this->trait_update_counter_ > 1)
          {
            ESP_LOGW(TAG, "Min/max values have changed. Rebooting so Home Assistant reconfigures the number component.");
            delay(100);   // NOLINT
            App.safe_reboot();
          }

          this->loop_state_ = LoopState::READ_RESPONSE;
          break;
        }
      };
    }

    void PanasonicHeatpumpComponent::read_response()
    {
      while (this->available())
      {
        // Read byte from heatpump and forward it directly to the client (CZ-TAW1)
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
          this->heatpump_message_ = this->response_message_;
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
      };

      this->next_request_ = RequestType::NONE;
    }

    void PanasonicHeatpumpComponent::read_request()
    {
      if (this->uart_client_ == nullptr) return;

      while (this->uart_client_->available())
      {
        // Read byte from client and forward it directly to the heatpump
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
      if (this->heatpump_message_.size() > index) return this->heatpump_message_[index];
      if (this->response_message_.size() > index) return this->response_message_[index];
      return -1;
    }

    bool PanasonicHeatpumpComponent::check_response(const std::vector<uint8_t>& data)
    {
      // Read response message:
      // format:          0x71 [payload_length] 0x01 0x10 [[TOP0 - TOP114] ...] 0x00 [checksum]
      // payload_length:  payload_length + 3 = packet_length
      // checksum:        if (sum(all bytes) & 0xFF == 0) ==> valid packet

      if (data.empty()) return false;
      if (data[0] != 0x71) return false;
      if (data.size() != RESPONSE_MSG_SIZE)
      {
        ESP_LOGW(TAG, "Invalid response message length: recieved %d - expected %d", data.size(), RESPONSE_MSG_SIZE);
        delay(10);  // NOLINT
        return false;
      }

      uint8_t checksum = 0;
      for (int i = 0; i < data.size(); i++)
      {
        checksum += data[i];
      }
      // checksum = checksum & 0xFF;
      if (checksum != 0)
      {
        ESP_LOGW(TAG, "Invalid response message: checksum = 0x%02X, last_byte = 0x%02X", checksum, data[202]);
        delay(10);  // NOLINT
        return false;
      }

      if (this->last_response_count_ == this->current_response_count_) return false;
      this->last_response_count_ = this->current_response_count_;
      return true;
    }

    void PanasonicHeatpumpComponent::set_command_high_nibble(const uint8_t value, const uint8_t index)
    {
      if (this->next_request_ != RequestType::COMMAND)
      {
        // initialize the command
        this->command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }
      uint8_t lowNibble = this->heatpump_message_[index] & 0b1111;
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
      uint8_t highNibble = this->heatpump_message_[index] & 0b11110000;
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

    void PanasonicHeatpumpComponent::set_command_bytes(const std::vector<std::tuple<uint8_t, uint8_t>>& data)
    {
      if (this->next_request_ != RequestType::COMMAND)
      {
        // initialize the command
        this->command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }
      // set command bytes
      for (size_t i = 0; i < data.size(); ++i)
      {
        uint8_t value = std::get<0>(data[i]);
        uint8_t index = std::get<1>(data[i]);
        this->command_message_[index] = value;
      }
      // calculate and set set checksum (last element)
      this->command_message_.back() = PanasonicCommand::calcChecksum(this->command_message_, this->command_message_.size() - 1);

      // command will be send on next loop
      this->next_request_ = RequestType::COMMAND;
    }

    void PanasonicHeatpumpComponent::set_number_traits(const std::vector<uint8_t>& data)
    {
#ifdef USE_TEXT_SENSOR
#ifdef USE_NUMBER
      if (data.empty()) return;

      // traits can be changed anytime, but entities will only be updated in home assistant,
      // if the traits was set before connecting to home assistant. If now a traits must be changed later,
      // a reboot of the ESP controller is required to see the changes in home assistant.

      bool change_detected = false;
      std::string top76 = PanasonicDecode::getTextState(
        PanasonicDecode::WaterTempControl, PanasonicDecode::getBit7and8(data[28]));   // Heating Mode
      std::string top81 = PanasonicDecode::getTextState(
        PanasonicDecode::WaterTempControl, PanasonicDecode::getBit5and6(data[28]));   // Cooling Mode
      float set5_min = this->set5_number_->traits.get_min_value();    // Z1 Heat Request Temperature
      float set6_min = this->set6_number_->traits.get_min_value();    // Z1 Cool Request Temperature
      float set7_min = this->set7_number_->traits.get_min_value();    // Z2 Heat Request Temperature
      float set8_min = this->set8_number_->traits.get_min_value();    // Z2 Cool Request Temperature

      if (set5_min >= 0.0 && top76 == PanasonicDecode::WaterTempControl[1])
      {
        this->set5_number_->traits.set_min_value(-5.0);
        this->set5_number_->traits.set_max_value(5.0);
        change_detected = true;
      }
      if (set6_min >= 0.0 && top81 == PanasonicDecode::WaterTempControl[1])
      {
        this->set6_number_->traits.set_min_value(-5.0);
        this->set6_number_->traits.set_max_value(5.0);
        change_detected = true;
      }
      if (set7_min >= 0.0 && top76 == PanasonicDecode::WaterTempControl[1])
      {
        this->set7_number_->traits.set_min_value(-5.0);
        this->set7_number_->traits.set_max_value(5.0);
        change_detected = true;
      }
      if (set8_min >= 0.0 && top81 == PanasonicDecode::WaterTempControl[1])
      {
        this->set8_number_->traits.set_min_value(-5.0);
        this->set8_number_->traits.set_max_value(5.0);
        change_detected = true;
      }

      if (set5_min <= 0.0 && top76 == PanasonicDecode::WaterTempControl[2])
      {
        this->set5_number_->traits.set_min_value(20.0);
        this->set5_number_->traits.set_max_value(60.0);
        change_detected = true;
      }
      if (set6_min <= 0.0 && top81 == PanasonicDecode::WaterTempControl[2])
      {
        this->set6_number_->traits.set_min_value(20.0);
        this->set6_number_->traits.set_max_value(60.0);
        change_detected = true;
      }
      if (set7_min <= 0.0 && top76 == PanasonicDecode::WaterTempControl[2])
      {
        this->set7_number_->traits.set_min_value(20.0);
        this->set7_number_->traits.set_max_value(60.0);
        change_detected = true;
      }
      if (set8_min <= 0.0 && top81 == PanasonicDecode::WaterTempControl[2])
      {
        this->set8_number_->traits.set_min_value(20.0);
        this->set8_number_->traits.set_max_value(60.0);
        change_detected = true;
      }

      if (change_detected)
      {
        this->trait_update_counter_++;
      }
#endif
#endif
    }

    void PanasonicHeatpumpComponent::publish_sensor(const std::vector<uint8_t>& data, const std::vector<uint8_t>& last)
    {
#ifdef USE_SENSOR
      if (data.empty()) return;
      if (this->top1_sensor_ && last[169] != data[169]) this->top1_sensor_->publish_state(PanasonicDecode::getPumpFlow(data, 169));
      if (this->top5_sensor_ && (last[143] != data[143] || last[118] != data[118])) this->top5_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[143]) + PanasonicDecode::getFractional(data[118], 0));
      if (this->top6_sensor_ && (last[144] != data[144] || last[118] != data[118])) this->top6_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[144]) + PanasonicDecode::getFractional(data[118], 3));
      if (this->top7_sensor_ && last[153] != data[153]) this->top7_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[153]));
      if (this->top8_sensor_ && last[166] != data[166]) this->top8_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[166]));
      if (this->top9_sensor_ && last[42] != data[42]) this->top9_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[42]));
      if (this->top10_sensor_ && last[141] != data[141]) this->top10_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[141]));
      if (this->top11_sensor_ && last[182] != data[182]) this->top11_sensor_->publish_state(PanasonicDecode::getWordMinus1(data, 182));
      if (this->top12_sensor_ && last[179] != data[179]) this->top12_sensor_->publish_state(PanasonicDecode::getWordMinus1(data, 179));
      if (this->top14_sensor_ && last[142] != data[142]) this->top14_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[142]));
      if (this->top15_sensor_ && last[194] != data[194]) this->top15_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[194]));
      if (this->top16_sensor_ && last[193] != data[193]) this->top16_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[193]));
      if (this->top21_sensor_ && last[158] != data[158]) this->top21_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[158]));
      if (this->top22_sensor_ && last[99] != data[99]) this->top22_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[99]));
      if (this->top23_sensor_ && last[84] != data[84]) this->top23_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[84]));
      if (this->top24_sensor_ && last[94] != data[94]) this->top24_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[94]));
      if (this->top25_sensor_ && last[44] != data[44]) this->top25_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[44]));
      if (this->top27_sensor_ && last[38] != data[38]) this->top27_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[38]));
      if (this->top28_sensor_ && last[39] != data[39]) this->top28_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[39]));
      if (this->top29_sensor_ && last[75] != data[75]) this->top29_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[75]));
      if (this->top30_sensor_ && last[76] != data[76]) this->top30_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[76]));
      if (this->top31_sensor_ && last[78] != data[78]) this->top31_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[78]));
      if (this->top32_sensor_ && last[77] != data[77]) this->top32_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[77]));
      if (this->top33_sensor_ && last[156] != data[156]) this->top33_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[156]));
      if (this->top34_sensor_ && last[40] != data[40]) this->top34_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[40]));
      if (this->top35_sensor_ && last[41] != data[41]) this->top35_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[41]));
      if (this->top36_sensor_ && last[145] != data[145]) this->top36_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[145]));
      if (this->top37_sensor_ && last[146] != data[146]) this->top37_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[146]));
      if (this->top38_sensor_ && last[196] != data[196]) this->top38_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[196]));
      if (this->top39_sensor_ && last[195] != data[195]) this->top39_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[195]));
      if (this->top40_sensor_ && last[198] != data[198]) this->top40_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[198]));
      if (this->top41_sensor_ && last[197] != data[197]) this->top41_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[197]));
      if (this->top42_sensor_ && last[147] != data[147]) this->top42_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[147]));
      if (this->top43_sensor_ && last[148] != data[148]) this->top43_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[148]));
      if (this->top45_sensor_ && last[43] != data[43]) this->top45_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[43]));
      if (this->top46_sensor_ && last[149] != data[149]) this->top46_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[149]));
      if (this->top47_sensor_ && last[150] != data[150]) this->top47_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[150]));
      if (this->top48_sensor_ && last[151] != data[151]) this->top48_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[151]));
      if (this->top49_sensor_ && last[154] != data[154]) this->top49_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[154]));
      if (this->top50_sensor_ && last[155] != data[155]) this->top50_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[155]));
      if (this->top51_sensor_ && last[157] != data[157]) this->top51_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[157]));
      if (this->top52_sensor_ && last[159] != data[159]) this->top52_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[159]));
      if (this->top53_sensor_ && last[160] != data[160]) this->top53_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[160]));
      if (this->top54_sensor_ && last[161] != data[161]) this->top54_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[161]));
      if (this->top55_sensor_ && last[162] != data[162]) this->top55_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[162]));
      if (this->top56_sensor_ && last[139] != data[139]) this->top56_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[139]));
      if (this->top57_sensor_ && last[140] != data[140]) this->top57_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[140]));
      if (this->top62_sensor_ && last[173] != data[173]) this->top62_sensor_->publish_state(PanasonicDecode::getByteMinus1Times10(data[173]));
      if (this->top63_sensor_ && last[174] != data[174]) this->top63_sensor_->publish_state(PanasonicDecode::getByteMinus1Times10(data[174]));
      if (this->top64_sensor_ && last[163] != data[163]) this->top64_sensor_->publish_state(PanasonicDecode::getByteMinus1Div5(data[163]));
      if (this->top65_sensor_ && last[171] != data[171]) this->top65_sensor_->publish_state(PanasonicDecode::getByteMinus1Times50(data[171]));
      if (this->top66_sensor_ && last[164] != data[164]) this->top66_sensor_->publish_state(PanasonicDecode::getByteMinus1Times50(data[164]));
      if (this->top67_sensor_ && last[165] != data[165]) this->top67_sensor_->publish_state(PanasonicDecode::getByteMinus1Div5(data[165]));
      if (this->top70_sensor_ && last[100] != data[100]) this->top70_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[100]));
      if (this->top71_sensor_ && last[101] != data[101]) this->top71_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[101]));
      if (this->top72_sensor_ && last[86] != data[86]) this->top72_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[86]));
      if (this->top73_sensor_ && last[87] != data[87]) this->top73_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[87]));
      if (this->top74_sensor_ && last[89] != data[89]) this->top74_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[89]));
      if (this->top75_sensor_ && last[88] != data[88]) this->top75_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[88]));
      if (this->top77_sensor_ && last[83] != data[83]) this->top77_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[83]));
      if (this->top78_sensor_ && last[85] != data[85]) this->top78_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[85]));
      if (this->top79_sensor_ && last[95] != data[95]) this->top79_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[95]));
      if (this->top80_sensor_ && last[96] != data[96]) this->top80_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[96]));
      if (this->top82_sensor_ && last[79] != data[79]) this->top82_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[79]));
      if (this->top83_sensor_ && last[80] != data[80]) this->top83_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[80]));
      if (this->top84_sensor_ && last[82] != data[82]) this->top84_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[82]));
      if (this->top85_sensor_ && last[81] != data[81]) this->top85_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[81]));
      if (this->top86_sensor_ && last[90] != data[90]) this->top86_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[90]));
      if (this->top87_sensor_ && last[91] != data[91]) this->top87_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[91]));
      if (this->top88_sensor_ && last[93] != data[93]) this->top88_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[93]));
      if (this->top89_sensor_ && last[92] != data[92]) this->top89_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[92]));
      if (this->top90_sensor_ && last[185] != data[185]) this->top90_sensor_->publish_state(PanasonicDecode::getWordMinus1(data, 185));
      if (this->top91_sensor_ && last[188] != data[188]) this->top91_sensor_->publish_state(PanasonicDecode::getWordMinus1(data, 188));
      if (this->top93_sensor_ && last[172] != data[172]) this->top93_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[172]));
      if (this->top95_sensor_ && last[45] != data[45]) this->top95_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[45]));
      if (this->top96_sensor_ && last[104] != data[104]) this->top96_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[104]));
      if (this->top97_sensor_ && last[105] != data[105]) this->top97_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[105]));
      if (this->top98_sensor_ && last[106] != data[106]) this->top98_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[106]));
      if (this->top102_sensor_ && last[61] != data[61]) this->top102_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[61]));
      if (this->top103_sensor_ && last[62] != data[62]) this->top103_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[62]));
      if (this->top104_sensor_ && last[63] != data[63]) this->top104_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[63]));
      if (this->top105_sensor_ && last[64] != data[64]) this->top105_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[64]));
      if (this->top113_sensor_ && last[59] != data[59]) this->top113_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[59]));
      if (this->top115_sensor_ && last[125] != data[125]) this->top115_sensor_->publish_state(PanasonicDecode::getByteMinus1Div50(data[125]));
      if (this->top116_sensor_ && last[126] != data[126]) this->top116_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[126]));
      if (this->top117_sensor_ && last[127] != data[127]) this->top117_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[127]));
      if (this->top118_sensor_ && last[128] != data[128]) this->top118_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[128]));
      if (this->top127_sensor_ && last[177] != data[177]) this->top127_sensor_->publish_state(PanasonicDecode::getByteMinus1Div2(data[177]));
      if (this->top128_sensor_ && last[178] != data[178]) this->top128_sensor_->publish_state(PanasonicDecode::getByteMinus1Div2(data[178]));
      if (this->top131_sensor_ && last[65] != data[65]) this->top131_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[65]));
      if (this->top134_sensor_ && last[66] != data[66]) this->top134_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[66]));
      if (this->top135_sensor_ && last[68] != data[68]) this->top135_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[68]));
      if (this->top136_sensor_ && last[67] != data[67]) this->top136_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[67]));
      if (this->top137_sensor_ && last[69] != data[69]) this->top137_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[69]));
      if (this->top138_sensor_ && last[70] != data[70]) this->top138_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[70]));
#endif
    }

    void PanasonicHeatpumpComponent::publish_binary_sensor(const std::vector<uint8_t>& data, const std::vector<uint8_t>& last)
    {
#ifdef USE_BINARY_SENSOR
      if (data.empty()) return;
      if (this->top0_binary_sensor_ && last[4] != data[4]) this->top0_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[4])));
      if (this->top2_binary_sensor_ && last[4] != data[4]) this->top2_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[4])));
      if (this->top3_binary_sensor_ && last[7] != data[7]) this->top3_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[7])));
      if (this->top13_binary_sensor_ && last[5] != data[5]) this->top13_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[5])));
      if (this->top26_binary_sensor_ && last[111] != data[111]) this->top26_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[111])));
      if (this->top60_binary_sensor_ && last[112] != data[112]) this->top60_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[112])));
      if (this->top61_binary_sensor_ && last[112] != data[112]) this->top61_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[112])));
      if (this->top68_binary_sensor_ && last[5] != data[5]) this->top68_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[5])));
      if (this->top69_binary_sensor_ && last[117] != data[117]) this->top69_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[117])));
      if (this->top99_binary_sensor_ && last[24] != data[24]) this->top99_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[24])));
      if (this->top100_binary_sensor_ && last[24] != data[24]) this->top100_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[24])));
      if (this->top108_binary_sensor_ && last[20] != data[20]) this->top108_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[20])));
      if (this->top109_binary_sensor_ && last[20] != data[20]) this->top109_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[20])));
      if (this->top110_binary_sensor_ && last[20] != data[20]) this->top110_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[20])));
      if (this->top119_binary_sensor_ && last[23] != data[23]) this->top119_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[23])));
      if (this->top120_binary_sensor_ && last[23] != data[23]) this->top120_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[23])));
      if (this->top121_binary_sensor_ && last[23] != data[23]) this->top121_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[23])));
      if (this->top122_binary_sensor_ && last[23] != data[23]) this->top122_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[23])));
      if (this->top123_binary_sensor_ && last[116] != data[116]) this->top123_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[116])));
      if (this->top124_binary_sensor_ && last[116] != data[116]) this->top124_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[116])));
      if (this->top129_binary_sensor_ && last[26] != data[26]) this->top129_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[26])));
      if (this->top132_binary_sensor_ && last[26] != data[26]) this->top132_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[26])));
      if (this->top133_binary_sensor_ && last[26] != data[26]) this->top133_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[26])));
#endif
    }

    void PanasonicHeatpumpComponent::publish_text_sensor(const std::vector<uint8_t>& data, const std::vector<uint8_t>& last)
    {
#ifdef USE_TEXT_SENSOR
      if (data.empty()) return;
      if (this->top4_text_sensor_ && last[6] != data[6]) this->top4_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::OperationMode, PanasonicDecode::getOperationMode(data[6])));
      if (this->top17_text_sensor_ && last[7] != data[7]) this->top17_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::PowerfulMode, PanasonicDecode::getBit6and7and8(data[7])));
      if (this->top18_text_sensor_ && last[7] != data[7]) this->top18_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::QuietMode, PanasonicDecode::getBit3and4and5(data[7])));
      if (this->top19_text_sensor_ && last[5] != data[5]) this->top19_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::HolidayState, PanasonicDecode::getBit3and4(data[5])));
      if (this->top20_text_sensor_ && last[111] != data[111]) this->top20_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ThreeWayValve, PanasonicDecode::getBit7and8(data[111])));
      if (this->top44_text_sensor_ && (last[113] != data[113] || last[114] != data[114])) this->top44_text_sensor_->publish_state(PanasonicDecode::getErrorInfo(data[113], data[114]));
      if (this->top58_text_sensor_ && last[9] != data[9]) this->top58_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::BlockedFree, PanasonicDecode::getBit5and6(data[9])));
      if (this->top59_text_sensor_ && last[9] != data[9]) this->top59_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::BlockedFree, PanasonicDecode::getBit7and8(data[9])));
      if (this->top76_text_sensor_ && last[28] != data[28]) this->top76_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::WaterTempControl, PanasonicDecode::getBit7and8(data[28])));
      if (this->top81_text_sensor_ && last[28] != data[28]) this->top81_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::WaterTempControl, PanasonicDecode::getBit5and6(data[28])));
      if (this->top92_text_sensor_ && last[129] != data[129]) this->top92_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ModelNames, PanasonicDecode::getModel(data, 129)));
      if (this->top94_text_sensor_ && last[6] != data[6]) this->top94_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZoneState, PanasonicDecode::getBit1and2(data[6])));
      if (this->top101_text_sensor_ && last[24] != data[24]) this->top101_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::SolarMode, PanasonicDecode::getBit3and4(data[24])));
      if (this->top106_text_sensor_ && last[29] != data[29]) this->top106_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::PumpFlowRateMode, PanasonicDecode::getBit3and4(data[29])));
      if (this->top107_text_sensor_ && last[20] != data[20]) this->top107_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::LiquidType, PanasonicDecode::getBit1(data[20])));
      if (this->top111_text_sensor_ && last[22] != data[22]) this->top111_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZoneSensorType, PanasonicDecode::getLowNibbleMinus1(data[22])));
      if (this->top112_text_sensor_ && last[22] != data[22]) this->top112_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZoneSensorType, PanasonicDecode::getHighNibbleMinus1(data[22])));
      if (this->top114_text_sensor_ && last[25] != data[25]) this->top114_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ExtPadHeaterType, PanasonicDecode::getBit3and4(data[25])));
      if (this->top125_text_sensor_ && last[116] != data[116]) this->top125_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::TwoWayValve, PanasonicDecode::getBit5and6(data[116])));
      if (this->top126_text_sensor_ && last[116] != data[116]) this->top126_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ThreeWayValve, PanasonicDecode::getBit7and8(data[116])));
      if (this->top130_text_sensor_ && last[26] != data[26]) this->top130_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::BivalentMode, PanasonicDecode::getBit5and6(data[26])));
#endif
    }

    void PanasonicHeatpumpComponent::publish_number(const std::vector<uint8_t>& data, const std::vector<uint8_t>& last)
    {
#ifdef USE_NUMBER
      if (data.empty()) return;
      if (this->set11_number_ && last[42] != data[42]) this->set11_number_->publish_state(PanasonicDecode::getByteMinus128(data[42]));
      if (this->set20_number_ && last[99] != data[99]) this->set20_number_->publish_state(PanasonicDecode::getByteMinus128(data[99]));
      if (this->set18_number_ && last[84] != data[84]) this->set18_number_->publish_state(PanasonicDecode::getByteMinus128(data[84]));
      if (this->set19_number_ && last[94] != data[94]) this->set19_number_->publish_state(PanasonicDecode::getByteMinus128(data[94]));
      if (this->set5_number_ && last[38] != data[38]) this->set5_number_->publish_state(PanasonicDecode::getByteMinus128(data[38]));
      if (this->set6_number_ && last[39] != data[39]) this->set6_number_->publish_state(PanasonicDecode::getByteMinus128(data[39]));
      if (this->set16_01_number_ && last[75] != data[75]) this->set16_01_number_->publish_state(PanasonicDecode::getByteMinus128(data[75]));
      if (this->set16_02_number_ && last[76] != data[76]) this->set16_02_number_->publish_state(PanasonicDecode::getByteMinus128(data[76]));
      if (this->set16_04_number_ && last[78] != data[78]) this->set16_04_number_->publish_state(PanasonicDecode::getByteMinus128(data[78]));
      if (this->set16_03_number_ && last[77] != data[77]) this->set16_03_number_->publish_state(PanasonicDecode::getByteMinus128(data[77]));
      if (this->set7_number_ && last[40] != data[40]) this->set7_number_->publish_state(PanasonicDecode::getByteMinus128(data[40]));
      if (this->set8_number_ && last[41] != data[41]) this->set8_number_->publish_state(PanasonicDecode::getByteMinus128(data[41]));
      if (this->set16_09_number_ && last[86] != data[86]) this->set16_09_number_->publish_state(PanasonicDecode::getByteMinus128(data[86]));
      if (this->set16_10_number_ && last[87] != data[87]) this->set16_10_number_->publish_state(PanasonicDecode::getByteMinus128(data[87]));
      if (this->set16_12_number_ && last[89] != data[89]) this->set16_12_number_->publish_state(PanasonicDecode::getByteMinus128(data[89]));
      if (this->set16_11_number_ && last[88] != data[88]) this->set16_11_number_->publish_state(PanasonicDecode::getByteMinus128(data[88]));
      if (this->set29_number_ && last[83] != data[83]) this->set29_number_->publish_state(PanasonicDecode::getByteMinus128(data[83]));
      if (this->set16_05_number_ && last[79] != data[79]) this->set16_05_number_->publish_state(PanasonicDecode::getByteMinus128(data[79]));
      if (this->set16_06_number_ && last[80] != data[80]) this->set16_06_number_->publish_state(PanasonicDecode::getByteMinus128(data[80]));
      if (this->set16_08_number_ && last[82] != data[82]) this->set16_08_number_->publish_state(PanasonicDecode::getByteMinus128(data[82]));
      if (this->set16_07_number_ && last[81] != data[81]) this->set16_07_number_->publish_state(PanasonicDecode::getByteMinus128(data[81]));
      if (this->set16_13_number_ && last[90] != data[90]) this->set16_13_number_->publish_state(PanasonicDecode::getByteMinus128(data[90]));
      if (this->set16_14_number_ && last[91] != data[91]) this->set16_14_number_->publish_state(PanasonicDecode::getByteMinus128(data[91]));
      if (this->set16_16_number_ && last[93] != data[93]) this->set16_16_number_->publish_state(PanasonicDecode::getByteMinus128(data[93]));
      if (this->set16_15_number_ && last[92] != data[92]) this->set16_15_number_->publish_state(PanasonicDecode::getByteMinus128(data[92]));
      if (this->set15_number_ && last[45] != data[45]) this->set15_number_->publish_state(PanasonicDecode::getByteMinus1(data[45]));
      if (this->set21_number_ && last[104] != data[104]) this->set21_number_->publish_state(PanasonicDecode::getByteMinus1(data[104]));
      if (this->set22_number_ && last[105] != data[105]) this->set22_number_->publish_state(PanasonicDecode::getByteMinus128(data[105]));
      if (this->set23_number_ && last[106] != data[106]) this->set23_number_->publish_state(PanasonicDecode::getByteMinus128(data[106]));
      if (this->set27_number_ && last[59] != data[59]) this->set27_number_->publish_state(PanasonicDecode::getByteMinus128(data[59]));
      if (this->set36_number_ && last[65] != data[65]) this->set36_number_->publish_state(PanasonicDecode::getByteMinus128(data[65]));
      if (this->set37_number_ && last[66] != data[66]) this->set37_number_->publish_state(PanasonicDecode::getByteMinus128(data[66]));
      if (this->set38_number_ && last[68] != data[68]) this->set38_number_->publish_state(PanasonicDecode::getByteMinus128(data[68]));
#endif
    }

    void PanasonicHeatpumpComponent::publish_select(const std::vector<uint8_t>& data, const std::vector<uint8_t>& last)
    {
#ifdef USE_SELECT
      if (data.empty()) return;
      if (this->set9_select_ && last[6] != data[6]) this->set9_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::OperationMode, PanasonicDecode::getOperationMode(data[6])));
      if (this->set4_select_ && last[7] != data[7]) this->set4_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::PowerfulMode, PanasonicDecode::getBit6and7and8(data[7])));
      if (this->set3_select_ && last[7] != data[7]) this->set3_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::QuietMode, PanasonicDecode::getBit3and4and5(data[7])));
      if (this->set2_select_ && last[5] != data[5]) this->set2_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::HolidayState, PanasonicDecode::getBit3and4(data[5])));
      if (this->set17_select_ && last[6] != data[6]) this->set17_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZoneState, PanasonicDecode::getBit1and2(data[6])));
      if (this->set26_select_ && last[25] != data[25]) this->set26_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ExtPadHeaterType, PanasonicDecode::getBit3and4(data[25])));
      if (this->set35_select_ && last[26] != data[26]) this->set35_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::BivalentMode, PanasonicDecode::getBit5and6(data[26])));
#endif
    }

    void PanasonicHeatpumpComponent::publish_switch(const std::vector<uint8_t>& data, const std::vector<uint8_t>& last)
    {
#ifdef USE_SWITCH
      if (data.empty()) return;
      if (this->set1_switch_ && last[4] != data[4]) this->set1_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[4])));
      if (this->set10_switch_ && last[4] != data[4]) this->set10_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[4])));
      if (this->set24_switch_ && last[5] != data[5]) this->set24_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[5])));
      if (this->set12_switch_ && last[111] != data[111]) this->set12_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[111])));
      if (this->set13_switch_ && last[117] != data[117]) this->set13_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[117])));
      if (this->set28_switch_ && last[24] != data[24]) this->set28_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[24])));
      if (this->set25_switch_ && last[20] != data[20]) this->set25_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[20])));
      if (this->set30_switch_ && last[23] != data[23]) this->set30_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[23])));
      if (this->set33_switch_ && last[23] != data[23]) this->set33_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[23])));
      if (this->set31_switch_ && last[23] != data[23]) this->set31_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[23])));
      if (this->set32_switch_ && last[23] != data[23]) this->set32_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[23])));
      if (this->set34_switch_ && last[26] != data[26]) this->set34_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[26])));
#endif
    }

#ifdef USE_NUMBER
    void PanasonicHeatpumpComponent::control_number(number::Number* object, float value)
    {
      if (object == this->set5_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 38);
      else if (object == this->set6_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 39);
      else if (object == this->set7_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 40);
      else if (object == this->set8_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 41);
      else if (object == this->set11_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 42);
      else if (object == this->set15_number_) this->set_command_byte(PanasonicCommand::setPlus1(value), 45);
      else if (object == this->set18_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 84);
      else if (object == this->set19_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 94);
      else if (object == this->set20_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 99);
      else if (object == this->set21_number_) this->set_command_byte(PanasonicCommand::setPlus1(value), 104);
      else if (object == this->set22_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 105);
      else if (object == this->set23_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 106);
      else if (object == this->set27_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 59);
      else if (object == this->set29_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 83);
      else if (object == this->set36_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 65);
      else if (object == this->set37_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 66);
      else if (object == this->set38_number_) this->set_command_byte(PanasonicCommand::setPlus128(value), 68);
      // Set zone 1 curve
      if (object == this->set16_01_number_ || object == this->set16_02_number_
        || object == this->set16_03_number_ || object == this->set16_04_number_
        || object == this->set16_09_number_ || object == this->set16_10_number_
        || object == this->set16_11_number_ || object == this->set16_12_number_)
      {
        std::vector<std::tuple<uint8_t, uint8_t>> curve;
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_01_number_->state), 75));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_02_number_->state), 76));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_03_number_->state), 77));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_04_number_->state), 78));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_09_number_->state), 86));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_10_number_->state), 87));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_11_number_->state), 88));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_12_number_->state), 89));
        this->set_command_bytes(curve);
      }
      // Set zone 2 curve
      if (object == this->set16_05_number_ || object == this->set16_06_number_
        || object == this->set16_07_number_ || object == this->set16_08_number_
        || object == this->set16_13_number_ || object == this->set16_14_number_
        || object == this->set16_15_number_ || object == this->set16_16_number_)
      {
        std::vector<std::tuple<uint8_t, uint8_t>> curve;
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_05_number_->state), 79));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_06_number_->state), 80));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_07_number_->state), 81));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_08_number_->state), 82));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_13_number_->state), 90));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_14_number_->state), 91));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_15_number_->state), 92));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_16_number_->state), 93));
        this->set_command_bytes(curve);
      }
    }
#endif

#ifdef USE_SELECT
    void PanasonicHeatpumpComponent::control_select(select::Select* object, size_t value)
    {
      if (object == this->set2_select_) this->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 5);
      else if (object == this->set3_select_) this->set_command_byte(PanasonicCommand::setPlus1Multiply8(value), 7);
      else if (object == this->set4_select_) this->set_command_byte(PanasonicCommand::setPlus73(value), 7);
      else if (object == this->set9_select_) this->set_command_byte(PanasonicCommand::setOperationMode(value), 6);
      else if (object == this->set17_select_) this->set_command_byte(PanasonicCommand::setPlus1Multiply64(value), 6);
      else if (object == this->set26_select_) this->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 25);
      else if (object == this->set35_select_) this->set_command_byte(PanasonicCommand::setPlus1Multiply4(value), 26);
    }
#endif

#ifdef USE_SWITCH
    void PanasonicHeatpumpComponent::control_switch(switch_::Switch* object, bool state)
    {
      size_t value = state ? 1 : 0;

      if (object == this->set1_switch_) this->set_command_byte(PanasonicCommand::setPlus1(value), 4);
      else if (object == this->set10_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply64(value), 4);
      else if (object == this->set12_switch_) this->set_command_byte(PanasonicCommand::setMultiply2(value), 8);
      else if (object == this->set13_switch_) this->set_command_byte(PanasonicCommand::setMultiply4(value), 8);
      else if (object == this->set14_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 4);
      else if (object == this->set24_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply64(value), 5);
      else if (object == this->set25_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 20);
      else if (object == this->set28_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply4(value), 24);
      else if (object == this->set30_switch_) this->set_command_byte(PanasonicCommand::setPlus1(value), 23);
      else if (object == this->set31_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 23);
      else if (object == this->set32_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply64(value), 23);
      else if (object == this->set33_switch_) this->set_command_byte(PanasonicCommand::setPlus1Multiply4(value), 23);
      else if (object == this->set34_switch_) this->set_command_byte(PanasonicCommand::setPlus1(value), 26);
    }
#endif
  }  // namespace panasonic_heatpump
}  // namespace esphome
