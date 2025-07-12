#include "maidesite_desk.h"


namespace esphome
{
  namespace maidesite_desk
  {
    static const char *const TAG = "maidesite_desk";

    void MaidesiteDeskComponent::dump_config()
    {
      ESP_LOGCONFIG(TAG, "Maidesite Desk");
      delay(10);  // NOLINT
    }

    void MaidesiteDeskComponent::setup()
    {
      ESP_LOGCONFIG(TAG, "Setting up Maidesite Desk ...");
      delay(10);  // NOLINT
      this->check_uart_settings(9600);
      this->request_physical_limits();
      this->request_limits();
      this->request_settings();
    }
    
    void MaidesiteDeskComponent::loop()
    {
      this->read_response();
      this->decode_response(this->desk_message_);
    }

    void MaidesiteDeskComponent::read_response()
    {
      // Read message:
      // format: 0xF2 0xF2 [command] [param_count] [[param] ...] [checksum] 0x7E
      // checksum: sum of [command], [param_count] and all [param]s

      while (this->available())
      {
        this->read_byte(&byte_);

        // Message shall start with 0xF2, if not skip this byte
        if (!this->response_receiving_)
        {
          if (byte_ != 0xF2)
            continue;
          this->response_message_.clear();
          this->response_receiving_ = true;
          this->checksum_ = 0;
          this->checksum_passed_ = false;
        }

        // Add current byte to message buffer
        this->response_message_.push_back(byte_);

        // Discard message if 1.and 2. byte are not equal
        if (this->response_message_.size() == 2 && this->response_message_[1] != byte_)
        {
          ESP_LOGW(TAG, "Invalid response message: 2. byte is 0x%02X but expexted is 0x%02X", byte_, this->response_message_[1]);
          delay(10);  // NOLINT
          this->response_message_.clear();
          this->response_receiving_ = false;
          continue;
        }

        // Verify chechsum
        if (this->response_message_.size() > 3 && this->response_message_.size() == this->response_message_[3] + 5)
        {
          this->checksum_passed_ = true;
          if (byte_ != checksum_) 
          {
            ESP_LOGW(TAG, "Calculated checksum (0x%X) is not equal to received checksum (0x%X)", checksum_, byte_);
            delay(10);  // NOLINT
          }
        }

        // Calculate chechsum
        if (this->response_message_.size() > 2 && !this->checksum_passed_)
        {
          checksum_ += byte_;
        }

        // Discard message if it is too long
        if (this->response_message_.size() > 10)
        {
          ESP_LOGW(TAG, "Response message too long: expected not more than 10 bytes");
          delay(10);  // NOLINT
          this->response_message_.clear();
          this->response_receiving_ = false;
          continue;
        }

        // Message is complete if last byte is 0x7E
        if (byte_ == 0x7E && this->checksum_passed_)
        {
          this->desk_message_ = this->response_message_;
          this->response_receiving_ = false;
          this->log_uart_hex("<<<", this->response_message_, ',');
        }
      }
    }

    void MaidesiteDeskComponent::log_uart_hex(std::string prefix, std::vector<uint8_t> bytes, uint8_t separator)
    {
      if (this->log_uart_msg_ == false) return;

      std::string logStr;
      char buffer[5];

      for (size_t i = 0; i < bytes.size(); i++)
      {
        if (i > 0) logStr += separator;
        sprintf(buffer, "%02X", bytes[i]);
        logStr += buffer;
      }
      for (size_t i = 0; i < logStr.length(); i += UART_LOG_CHUNK_SIZE)
      {
        ESP_LOGI(TAG, "%s %s", prefix.c_str(), logStr.substr(i, UART_LOG_CHUNK_SIZE).c_str());
        delay(10);  // NOLINT
      }
    }

    void MaidesiteDeskComponent::decode_response(std::vector<uint8_t> message)
    {
      if (message.empty()) return;
      if (message.size() < 6)
      {
        ESP_LOGW(TAG, "Received message too short.");
        return;
      }

      for (auto *entity : this->sensors_)
      {
        entity->publish_new_state(message);
      }
      for (auto *entity : this->numbers_)
      {
        entity->publish_new_state(message);
      }

      // switch (message[2])
      // {
      // case 0x0E:
      //   ESP_LOGI(TAG, "units 0x%02X", message[4]);
      //   if (units != nullptr)
      //     units->publish_state(byte2float(message[4], message[5]));
      //   break;

      // default:
      //   ESP_LOGI(TAG, "Received unknown message");
      //   delay(10);  // NOLINT
      // }
    }

    // Write message:
    // format: 0xF1 0xF1 [command] [param_count] [[param] ...] [checksum] 0x7E
    // checksum: sum of [command], [param_count] and all [param]s
    void MaidesiteDeskComponent::send_1byte_command(unsigned char cmd)
    {
      this->request_message_.clear();
      this->request_message_.insert(this->request_message_.end(), { 0xF1, 0xF1, cmd, 0x00, cmd, 0x7E });
      this->write_array(this->request_message_);
      this->log_uart_hex(">>>", this->request_message_, ',');
      delay(100);  // NOLINT
    }

    void MaidesiteDeskComponent::send_2byte_command(unsigned char cmd, unsigned char high_byte, unsigned char low_byte)
    {
      // ToDo: make one function of send_1byte_command and send_2byte_command

      unsigned char checksum = cmd + 2 + high_byte + low_byte;
      this->request_message_.clear();
      this->request_message_.insert(this->request_message_.end(), { 0xF1, 0xF1, cmd, 0x02, high_byte, low_byte, checksum, 0x7E });
      this->write_array(this->request_message_);
      this->log_uart_hex(">>>", this->request_message_, ',');
      delay(100);  // NOLINT
    }
    
    void MaidesiteDeskComponent::request_physical_limits()
    {
      this->send_1byte_command(0x0C);
    }

    void MaidesiteDeskComponent::request_limits()
    {
      this->send_1byte_command(0x20);
    }

    void MaidesiteDeskComponent::request_settings()
    {
      this->send_1byte_command(0x07);
    }

    void MaidesiteDeskComponent::request_move_to()
    {
      this->send_1byte_command(0x1B);
    }

    void MaidesiteDeskComponent::goto_max_position()
    {
      this->goto_height_abs(limit_max_);
    }

    void MaidesiteDeskComponent::goto_min_position()
    {
      this->goto_height_abs(limit_min_);
    }

    void MaidesiteDeskComponent::goto_height_abs(float height)
    {
      unsigned char high_byte = ((int)height * 10) >> 8;
      unsigned char low_byte = ((int)height * 10) & 0xFF;

      this->send_2byte_command(0x80, high_byte, low_byte);
      this->send_1byte_command(0x1B);
    }

    void MaidesiteDeskComponent::goto_height_pct(float height)
    {
      if (limit_max_ <= 0 || limit_max_ <= limit_min_) return;
      float height_abs = (limit_max_ - limit_min_) * height / 100 + limit_min_;
      this->goto_height_abs(height_abs);
    }
  } // namespace maidesite_desk
} // namespace esphome
