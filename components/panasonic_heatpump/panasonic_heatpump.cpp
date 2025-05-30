#include "panasonic_heatpump.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    static const char *const TAG = "panasonic_heatpump";

    void PanasonicHeatpumpComponent::dump_config()
    {
      ESP_LOGCONFIG(TAG, "Panasonic Heatpump");
      delay(10);
    }

    void PanasonicHeatpumpComponent::setup()
    {
      ESP_LOGCONFIG(TAG, "Setting up Panasonic Heatpump ...");
      delay(10);
      this->check_uart_settings(9600, 1, uart::UART_CONFIG_PARITY_EVEN, 8);
      // Trigger initial request
      if (this->uart_client_ == nullptr)
      {
        this->next_request_ = 0;
        this->trigger_request_ = true;
      }
      else
      {
        this->trigger_request_ = false;
      }
    }

    void PanasonicHeatpumpComponent::update()
    {
      if (this->uart_client_ == nullptr)
        this->trigger_request_ = true;
    }

    void PanasonicHeatpumpComponent::loop()
    {
      switch (this->loop_state_)
      {
        case 0:
        {
          this->read_response();
          this->loop_state_ = 1;
          break;
        }
        case 1:
        {
          bool result = this->check_response(this->heatpump_message_);
          this->loop_state_ = result ? 10 : 90;
          break;
        }
        case 10:
        {
          this->set_number_min_max_value(this->heatpump_message_);
          this->loop_state_ = 20;
          break;
        }
        case 20:
        {
          this->publish_sensor(this->heatpump_message_);
          this->loop_state_ = 21;
          break;
        }
        case 21:
        {
          this->publish_binary_sensor(this->heatpump_message_);
          this->loop_state_ = 22;
          break;
        }
        case 22:
        {
          this->publish_text_sensor(this->heatpump_message_);
          this->loop_state_ = 23;
          break;
        }
        case 23:
        {
          this->publish_number(this->heatpump_message_);
          this->loop_state_ = 24;
          break;
        }
        case 24:
        {
          this->publish_select(this->heatpump_message_);
          this->loop_state_ = 25;
          break;
        }
        case 25:
        {
          this->publish_switch(this->heatpump_message_);
          this->loop_state_ = 90;
          break;
        }
        case 90:
        {
          this->send_request();
          this->loop_state_ = 91;
          break;
        }
        case 91:
        {
          this->read_request();
          this->loop_state_ = 92;
          break;
        }
        default:
        {
          // Next request will be polling
          this->next_request_ = 1;
          this->trigger_request_ = false;
          this->loop_state_ = 0;
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
          delay(10);
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

    void PanasonicHeatpumpComponent::send_request()
    {
      if (this->trigger_request_ == false) return;

      if (this->next_request_ == 2) // command
      {
        if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, this->command_message_, ',');
        this->write_array(this->command_message_);
        this->flush();
        return;
      }

      if (this->uart_client_ != nullptr) return;

      if (this->next_request_ == 0) // initial
      {
        // Probably not necessary but CZ-TAW1 sends this query on boot
        if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, PanasonicCommand::InitialRequest, INIT_REQUEST_SIZE, ',');
        this->write_array(PanasonicCommand::InitialRequest, INIT_REQUEST_SIZE);
        this->flush();
      }
      else if (this->next_request_ == 1) // polling
      {
        if (this->log_uart_msg_) PanasonicHelpers::log_uart_hex(UART_LOG_TX, PanasonicCommand::PollingMessage, DATA_MESSAGE_SIZE, ',');
        this->write_array(PanasonicCommand::PollingMessage, DATA_MESSAGE_SIZE);
        this->flush();
      }
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
          delay(10);
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
        delay(10);
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
        delay(10);
        return false;
      }

      if (this->last_response_count_ == this->current_response_count_) return false;
      this->last_response_count_ = this->current_response_count_;
      return true;
    }

    void PanasonicHeatpumpComponent::set_command_byte(const uint8_t value, const uint8_t index)
    {
      if (this->next_request_ == 1)
      {
        // initialize the command
        command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }
      // set command byte
      command_message_[index] = value;
      // calculate and set set checksum (last element)
      command_message_.back() = PanasonicCommand::calcChecksum(command_message_, command_message_.size() - 1);

      // command will be send on next loop
      this->next_request_ = 2;
      this->trigger_request_ = true;
    }

    void PanasonicHeatpumpComponent::set_command_bytes(const std::vector<std::tuple<uint8_t, uint8_t>>& data)
    {
      if (this->next_request_ == 1)
      {
        // initialize the command
        command_message_.assign(std::begin(PanasonicCommand::CommandMessage),
                                std::end(PanasonicCommand::CommandMessage));
      }
      // set command bytes
      for (size_t i = 0; i < data.size(); ++i)
      {
        uint8_t value = std::get<0>(data[i]);
        uint8_t index = std::get<1>(data[i]);
        command_message_[index] = value;
      }
      // calculate and set set checksum (last element)
      command_message_.back() = PanasonicCommand::calcChecksum(command_message_, command_message_.size() - 1);

      // command will be send on next loop
      this->next_request_ = 2;
      this->trigger_request_ = true;
    }

    void PanasonicHeatpumpComponent::set_number_min_max_value(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;
#ifdef USE_TEXT_SENSOR
#ifdef USE_NUMBER
      if (this->set5_number_->traits.get_min_value() > 0.0 &&               // Z1 Heat Request Temperature
          this->top76_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[1])  // Heating Mode
      {
        this->set5_number_->traits.set_min_value(-5.0);
        this->set5_number_->traits.set_max_value(5.0);
      }
      if (this->set6_number_->traits.get_min_value() > 0.0 &&               // Z1 Cool Request Temperature
          this->top81_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[1])  // Cooling Mode
      {
        this->set6_number_->traits.set_min_value(-5.0);
        this->set6_number_->traits.set_max_value(5.0);
      }
      if (this->set7_number_->traits.get_min_value() > 0.0 &&               // Z2 Heat Request Temperature
          this->top76_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[1])   // Heating Mode
      {
        this->set7_number_->traits.set_min_value(-5.0);
        this->set7_number_->traits.set_max_value(5.0);
      }
      if (this->set8_number_->traits.get_min_value() > 0.0 &&               // Z2 Cool Request Temperature
          this->top81_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[1])  // Cooling Mode
      {
        this->set8_number_->traits.set_min_value(-5.0);
        this->set8_number_->traits.set_max_value(5.0);
      }

      if (this->set5_number_->traits.get_min_value() < 0.0 &&               // Z1 Heat Request Temperature
          this->top76_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[2])  // Heating Mode
      {
        this->set5_number_->traits.set_min_value(20.0);
        this->set5_number_->traits.set_max_value(60.0);
      }
      if (this->set6_number_->traits.get_min_value() < 0.0 &&               // Z1 Cool Request Temperature
          this->top81_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[2])  // Cooling Mode
      {
        this->set6_number_->traits.set_min_value(20.0);
        this->set6_number_->traits.set_max_value(60.0);
      }
      if (this->set7_number_->traits.get_min_value() < 0.0 &&               // Z2 Heat Request Temperature
          this->top76_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[2])  // Heating Mode
      {
        this->set7_number_->traits.set_min_value(20.0);
        this->set7_number_->traits.set_max_value(60.0);
      }
      if (this->set8_number_->traits.get_min_value() < 0.0 &&               // Z2 Cool Request Temperature
          this->top81_text_sensor_->get_state() == PanasonicDecode::HeatCoolModeDesc[2])  // Cooling Mode
      {
        this->set8_number_->traits.set_min_value(20.0);
        this->set8_number_->traits.set_max_value(60.0);
      }
#endif
#endif
    }

    void PanasonicHeatpumpComponent::publish_sensor(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;
#ifdef USE_SENSOR
      if (this->top1_sensor_) this->top1_sensor_->publish_state(PanasonicDecode::getPumpFlow(data[169], data[170]));
      if (this->top5_sensor_) this->top5_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[143]) + PanasonicDecode::getFractional(data[118], 0));
      if (this->top6_sensor_) this->top6_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[144]) + PanasonicDecode::getFractional(data[118], 3));
      if (this->top7_sensor_) this->top7_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[153]));
      if (this->top8_sensor_) this->top8_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[166]));
      if (this->top9_sensor_) this->top9_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[42]));
      if (this->top10_sensor_) this->top10_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[141]));
      if (this->top11_sensor_) this->top11_sensor_->publish_state(PanasonicDecode::getWordMinus1(data[182], data[183]));
      if (this->top12_sensor_) this->top12_sensor_->publish_state(PanasonicDecode::getWordMinus1(data[179], data[180]));
      if (this->top14_sensor_) this->top14_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[142]));
      if (this->top15_sensor_) this->top15_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[194]));
      if (this->top16_sensor_) this->top16_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[193]));
      if (this->top21_sensor_) this->top21_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[158]));
      if (this->top22_sensor_) this->top22_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[99]));
      if (this->top23_sensor_) this->top23_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[84]));
      if (this->top24_sensor_) this->top24_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[94]));
      if (this->top25_sensor_) this->top25_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[44]));
      if (this->top27_sensor_) this->top27_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[38]));
      if (this->top28_sensor_) this->top28_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[39]));
      if (this->top29_sensor_) this->top29_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[75]));
      if (this->top30_sensor_) this->top30_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[76]));
      if (this->top31_sensor_) this->top31_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[78]));
      if (this->top32_sensor_) this->top32_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[77]));
      if (this->top33_sensor_) this->top33_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[156]));
      if (this->top34_sensor_) this->top34_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[40]));
      if (this->top35_sensor_) this->top35_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[41]));
      if (this->top36_sensor_) this->top36_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[145]));
      if (this->top37_sensor_) this->top37_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[146]));
      if (this->top38_sensor_) this->top38_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[196]));
      if (this->top39_sensor_) this->top39_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[195]));
      if (this->top40_sensor_) this->top40_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[198]));
      if (this->top41_sensor_) this->top41_sensor_->publish_state(PanasonicDecode::getByteMinus1Times200(data[197]));
      if (this->top42_sensor_) this->top42_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[147]));
      if (this->top43_sensor_) this->top43_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[148]));
      if (this->top45_sensor_) this->top45_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[43]));
      if (this->top46_sensor_) this->top46_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[149]));
      if (this->top47_sensor_) this->top47_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[150]));
      if (this->top48_sensor_) this->top48_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[151]));
      if (this->top49_sensor_) this->top49_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[154]));
      if (this->top50_sensor_) this->top50_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[155]));
      if (this->top51_sensor_) this->top51_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[157]));
      if (this->top52_sensor_) this->top52_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[159]));
      if (this->top53_sensor_) this->top53_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[160]));
      if (this->top54_sensor_) this->top54_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[161]));
      if (this->top55_sensor_) this->top55_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[162]));
      if (this->top56_sensor_) this->top56_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[139]));
      if (this->top57_sensor_) this->top57_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[140]));
      if (this->top62_sensor_) this->top62_sensor_->publish_state(PanasonicDecode::getByteMinus1Times10(data[173]));
      if (this->top63_sensor_) this->top63_sensor_->publish_state(PanasonicDecode::getByteMinus1Times10(data[174]));
      if (this->top64_sensor_) this->top64_sensor_->publish_state(PanasonicDecode::getByteMinus1Div5(data[163]));
      if (this->top65_sensor_) this->top65_sensor_->publish_state(PanasonicDecode::getByteMinus1Times50(data[171]));
      if (this->top66_sensor_) this->top66_sensor_->publish_state(PanasonicDecode::getByteMinus1Times50(data[164]));
      if (this->top67_sensor_) this->top67_sensor_->publish_state(PanasonicDecode::getByteMinus1Div5(data[165]));
      if (this->top70_sensor_) this->top70_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[100]));
      if (this->top71_sensor_) this->top71_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[101]));
      if (this->top72_sensor_) this->top72_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[86]));
      if (this->top73_sensor_) this->top73_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[87]));
      if (this->top74_sensor_) this->top74_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[89]));
      if (this->top75_sensor_) this->top75_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[88]));
      if (this->top77_sensor_) this->top77_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[83]));
      if (this->top78_sensor_) this->top78_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[85]));
      if (this->top79_sensor_) this->top79_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[95]));
      if (this->top80_sensor_) this->top80_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[96]));
      if (this->top82_sensor_) this->top82_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[79]));
      if (this->top83_sensor_) this->top83_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[80]));
      if (this->top84_sensor_) this->top84_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[82]));
      if (this->top85_sensor_) this->top85_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[81]));
      if (this->top86_sensor_) this->top86_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[90]));
      if (this->top87_sensor_) this->top87_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[91]));
      if (this->top88_sensor_) this->top88_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[93]));
      if (this->top89_sensor_) this->top89_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[92]));
      if (this->top90_sensor_) this->top90_sensor_->publish_state(PanasonicDecode::getWordMinus1(data[185], data[186]));
      if (this->top91_sensor_) this->top91_sensor_->publish_state(PanasonicDecode::getWordMinus1(data[188], data[189]));
      if (this->top93_sensor_) this->top93_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[172]));
      if (this->top95_sensor_) this->top95_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[45]));
      if (this->top96_sensor_) this->top96_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[104]));
      if (this->top97_sensor_) this->top97_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[105]));
      if (this->top98_sensor_) this->top98_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[106]));
      if (this->top102_sensor_) this->top102_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[61]));
      if (this->top103_sensor_) this->top103_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[62]));
      if (this->top104_sensor_) this->top104_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[63]));
      if (this->top105_sensor_) this->top105_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[64]));
      if (this->top113_sensor_) this->top113_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[59]));
      if (this->top115_sensor_) this->top115_sensor_->publish_state(PanasonicDecode::getByteMinus1Div50(data[125]));
      if (this->top116_sensor_) this->top116_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[126]));
      if (this->top117_sensor_) this->top117_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[127]));
      if (this->top118_sensor_) this->top118_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[128]));
      if (this->top127_sensor_) this->top127_sensor_->publish_state(PanasonicDecode::getByteMinus1Div2(data[177]));
      if (this->top128_sensor_) this->top128_sensor_->publish_state(PanasonicDecode::getByteMinus1Div2(data[178]));
      if (this->top131_sensor_) this->top131_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[65]));
      if (this->top134_sensor_) this->top134_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[66]));
      if (this->top135_sensor_) this->top135_sensor_->publish_state(PanasonicDecode::getByteMinus128(data[68]));
      if (this->top136_sensor_) this->top136_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[67]));
      if (this->top137_sensor_) this->top137_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[69]));
      if (this->top138_sensor_) this->top138_sensor_->publish_state(PanasonicDecode::getByteMinus1(data[70]));
#endif
    }

    void PanasonicHeatpumpComponent::publish_binary_sensor(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;
#ifdef USE_BINARY_SENSOR
      if (this->top0_binary_sensor_) this->top0_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[4])));
      if (this->top2_binary_sensor_) this->top2_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[4])));
      if (this->top3_binary_sensor_) this->top3_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[7])));
      if (this->top13_binary_sensor_) this->top13_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[5])));
      if (this->top26_binary_sensor_) this->top26_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[111])));
      if (this->top60_binary_sensor_) this->top60_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[112])));
      if (this->top61_binary_sensor_) this->top61_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[112])));
      if (this->top68_binary_sensor_) this->top68_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[5])));
      if (this->top69_binary_sensor_) this->top69_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[117])));
      if (this->top99_binary_sensor_) this->top99_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[24])));
      if (this->top100_binary_sensor_) this->top100_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[24])));
      if (this->top108_binary_sensor_) this->top108_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[20])));
      if (this->top109_binary_sensor_) this->top109_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[20])));
      if (this->top110_binary_sensor_) this->top110_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[20])));
      if (this->top119_binary_sensor_) this->top119_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[23])));
      if (this->top120_binary_sensor_) this->top120_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[23])));
      if (this->top121_binary_sensor_) this->top121_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[23])));
      if (this->top122_binary_sensor_) this->top122_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[23])));
      if (this->top123_binary_sensor_) this->top123_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[116])));
      if (this->top124_binary_sensor_) this->top124_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[116])));
      if (this->top129_binary_sensor_) this->top129_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[26])));
      if (this->top132_binary_sensor_) this->top132_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[26])));
      if (this->top133_binary_sensor_) this->top133_binary_sensor_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[26])));
#endif
    }

    void PanasonicHeatpumpComponent::publish_text_sensor(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;
#ifdef USE_TEXT_SENSOR
      if (this->top4_text_sensor_) this->top4_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::OpModeDesc, PanasonicDecode::getOpMode(data[6])));
      if (this->top17_text_sensor_) this->top17_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Powerfulmode, PanasonicDecode::getBit6and7and8(data[7])));
      if (this->top18_text_sensor_) this->top18_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Quietmode, PanasonicDecode::getBit3and4and5(data[7])));
      if (this->top19_text_sensor_) this->top19_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::HolidayState, PanasonicDecode::getBit3and4(data[5])));
      if (this->top20_text_sensor_) this->top20_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Valve, PanasonicDecode::getBit7and8(data[111])));
      if (this->top44_text_sensor_) this->top44_text_sensor_->publish_state(PanasonicDecode::getErrorInfo(data[113], data[114]));
      if (this->top58_text_sensor_) this->top58_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::BlockedFree, PanasonicDecode::getBit5and6(data[9])));
      if (this->top59_text_sensor_) this->top59_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::BlockedFree, PanasonicDecode::getBit7and8(data[9])));
      if (this->top76_text_sensor_) this->top76_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::HeatCoolModeDesc, PanasonicDecode::getBit7and8(data[28])));
      if (this->top81_text_sensor_) this->top81_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::HeatCoolModeDesc, PanasonicDecode::getBit5and6(data[28])));
      if (this->top92_text_sensor_) this->top92_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ModelNames, PanasonicDecode::getModel(data, 129)));
      if (this->top94_text_sensor_) this->top94_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZonesState, PanasonicDecode::getBit1and2(data[6])));
      if (this->top101_text_sensor_) this->top101_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::SolarModeDesc, PanasonicDecode::getBit3and4(data[24])));
      if (this->top106_text_sensor_) this->top106_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::PumpFlowRateMode, PanasonicDecode::getBit3and4(data[29])));
      if (this->top107_text_sensor_) this->top107_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::LiquidType, PanasonicDecode::getBit1(data[20])));
      if (this->top111_text_sensor_) this->top111_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZonesSensorType, PanasonicDecode::getLowNibbleMinus1(data[22])));
      if (this->top112_text_sensor_) this->top112_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZonesSensorType, PanasonicDecode::getHighNibbleMinus1(data[22])));
      if (this->top114_text_sensor_) this->top114_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ExtPadHeaterType, PanasonicDecode::getBit3and4(data[25])));
      if (this->top125_text_sensor_) this->top125_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Valve2, PanasonicDecode::getBit5and6(data[116])));
      if (this->top126_text_sensor_) this->top126_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Valve, PanasonicDecode::getBit7and8(data[116])));
      if (this->top130_text_sensor_) this->top130_text_sensor_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Bivalent, PanasonicDecode::getBit5and6(data[26])));
#endif
    }

    void PanasonicHeatpumpComponent::publish_number(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;
#ifdef USE_NUMBER
      if (this->set11_number_) this->set11_number_->publish_state(PanasonicDecode::getByteMinus128(data[42]));
      if (this->set20_number_) this->set20_number_->publish_state(PanasonicDecode::getByteMinus128(data[99]));
      if (this->set18_number_) this->set18_number_->publish_state(PanasonicDecode::getByteMinus128(data[84]));
      if (this->set19_number_) this->set19_number_->publish_state(PanasonicDecode::getByteMinus128(data[94]));
      if (this->set5_number_) this->set5_number_->publish_state(PanasonicDecode::getByteMinus128(data[38]));
      if (this->set6_number_) this->set6_number_->publish_state(PanasonicDecode::getByteMinus128(data[39]));
      if (this->set16_1_number_) this->set16_1_number_->publish_state(PanasonicDecode::getByteMinus128(data[75]));
      if (this->set16_2_number_) this->set16_2_number_->publish_state(PanasonicDecode::getByteMinus128(data[76]));
      if (this->set16_4_number_) this->set16_4_number_->publish_state(PanasonicDecode::getByteMinus128(data[78]));
      if (this->set16_3_number_) this->set16_3_number_->publish_state(PanasonicDecode::getByteMinus128(data[77]));
      if (this->set7_number_) this->set7_number_->publish_state(PanasonicDecode::getByteMinus128(data[40]));
      if (this->set8_number_) this->set8_number_->publish_state(PanasonicDecode::getByteMinus128(data[41]));
      if (this->set16_9_number_) this->set16_9_number_->publish_state(PanasonicDecode::getByteMinus128(data[86]));
      if (this->set16_10_number_) this->set16_10_number_->publish_state(PanasonicDecode::getByteMinus128(data[87]));
      if (this->set16_12_number_) this->set16_12_number_->publish_state(PanasonicDecode::getByteMinus128(data[89]));
      if (this->set16_11_number_) this->set16_11_number_->publish_state(PanasonicDecode::getByteMinus128(data[88]));
      if (this->set29_number_) this->set29_number_->publish_state(PanasonicDecode::getByteMinus128(data[83]));
      if (this->set16_5_number_) this->set16_5_number_->publish_state(PanasonicDecode::getByteMinus128(data[79]));
      if (this->set16_6_number_) this->set16_6_number_->publish_state(PanasonicDecode::getByteMinus128(data[80]));
      if (this->set16_8_number_) this->set16_8_number_->publish_state(PanasonicDecode::getByteMinus128(data[82]));
      if (this->set16_7_number_) this->set16_7_number_->publish_state(PanasonicDecode::getByteMinus128(data[81]));
      if (this->set16_13_number_) this->set16_13_number_->publish_state(PanasonicDecode::getByteMinus128(data[90]));
      if (this->set16_14_number_) this->set16_14_number_->publish_state(PanasonicDecode::getByteMinus128(data[91]));
      if (this->set16_16_number_) this->set16_16_number_->publish_state(PanasonicDecode::getByteMinus128(data[93]));
      if (this->set16_15_number_) this->set16_15_number_->publish_state(PanasonicDecode::getByteMinus128(data[92]));
      if (this->set15_number_) this->set15_number_->publish_state(PanasonicDecode::getByteMinus1(data[45]));
      if (this->set21_number_) this->set21_number_->publish_state(PanasonicDecode::getByteMinus1(data[104]));
      if (this->set22_number_) this->set22_number_->publish_state(PanasonicDecode::getByteMinus128(data[105]));
      if (this->set23_number_) this->set23_number_->publish_state(PanasonicDecode::getByteMinus128(data[106]));
      if (this->set27_number_) this->set27_number_->publish_state(PanasonicDecode::getByteMinus128(data[59]));
      if (this->set36_number_) this->set36_number_->publish_state(PanasonicDecode::getByteMinus128(data[65]));
      if (this->set37_number_) this->set37_number_->publish_state(PanasonicDecode::getByteMinus128(data[66]));
      if (this->set38_number_) this->set38_number_->publish_state(PanasonicDecode::getByteMinus128(data[68]));
#endif
    }

    void PanasonicHeatpumpComponent::publish_select(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;
#ifdef USE_SELECT
      if (this->set9_select_) this->set9_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::OpModeDesc, PanasonicDecode::getOpMode(data[6])));
      if (this->set4_select_) this->set4_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Powerfulmode, PanasonicDecode::getBit6and7and8(data[7])));
      if (this->set3_select_) this->set3_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Quietmode, PanasonicDecode::getBit3and4and5(data[7])));
      if (this->set2_select_) this->set2_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::HolidayState, PanasonicDecode::getBit3and4(data[5])));
      if (this->set17_select_) this->set17_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ZonesState, PanasonicDecode::getBit1and2(data[6])));
      if (this->set26_select_) this->set26_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::ExtPadHeaterType, PanasonicDecode::getBit3and4(data[25])));
      if (this->set35_select_) this->set35_select_->publish_state(PanasonicDecode::getTextState(PanasonicDecode::Bivalent, PanasonicDecode::getBit5and6(data[26])));
#endif
    }

    void PanasonicHeatpumpComponent::publish_switch(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;
#ifdef USE_SWITCH
      if (this->set1_switch_) this->set1_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[4])));
      if (this->set10_switch_) this->set10_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[4])));
      if (this->set24_switch_) this->set24_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[5])));
      if (this->set12_switch_) this->set12_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[111])));
      if (this->set13_switch_) this->set13_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[117])));
      if (this->set28_switch_) this->set28_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[24])));
      if (this->set30_switch_) this->set30_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[23])));
      if (this->set33_switch_) this->set33_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[23])));
      if (this->set31_switch_) this->set31_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[23])));
      if (this->set32_switch_) this->set32_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[23])));
      if (this->set34_switch_) this->set34_switch_->publish_state(PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[26])));
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
      if (object == this->set16_1_number_ || object == this->set16_2_number_
        || object == this->set16_3_number_ || object == this->set16_4_number_
        || object == this->set16_9_number_ || object == this->set16_10_number_
        || object == this->set16_11_number_ || object == this->set16_12_number_)
      {
        std::vector<std::tuple<uint8_t, uint8_t>> curve;
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_1_number_->state), 75));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_2_number_->state), 76));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_3_number_->state), 77));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_4_number_->state), 78));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_9_number_->state), 86));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_10_number_->state), 87));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_11_number_->state), 88));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_12_number_->state), 89));
        this->set_command_bytes(curve);
      }
      // Set zone 2 curve
      if (object == this->set16_5_number_ || object == this->set16_6_number_
        || object == this->set16_7_number_ || object == this->set16_8_number_
        || object == this->set16_13_number_ || object == this->set16_14_number_
        || object == this->set16_15_number_ || object == this->set16_16_number_)
      {
        std::vector<std::tuple<uint8_t, uint8_t>> curve;
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_5_number_->state), 79));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_6_number_->state), 80));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_7_number_->state), 81));
        curve.push_back(std::make_tuple(PanasonicCommand::setPlus128(this->set16_8_number_->state), 82));
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
