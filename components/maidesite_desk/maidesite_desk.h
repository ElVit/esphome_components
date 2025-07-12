#pragma once
#include <vector>
#include <string>
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"

#define UART_LOG_CHUNK_SIZE 153


namespace esphome
{
  namespace maidesite_desk
  {
    class MaidesiteDeskEntity
    {
      public:
        virtual void set_id(const int id) { id_ = id; }
        virtual void publish_new_state(const std::vector<uint8_t>& data) = 0;

      protected:
        int id_ { -1 };
        float byte2float(int high, int low) { return static_cast<float>((high << 8) + low) / 10; }
    };

    class MaidesiteDeskComponent : public Component, public uart::UARTDevice
    {
    public:
      MaidesiteDeskComponent() = default;
      // base class functions
      float get_setup_priority() const override { return setup_priority::DATA; }
      void dump_config() override;
      void setup() override;
      void loop() override;
      
      // option functions
      void set_log_uart_msg(bool enable) { this->log_uart_msg_ = enable; }
      
      void send_1byte_command(unsigned char cmd);
      void send_2byte_command(unsigned char cmd, unsigned char high_byte, unsigned char low_byte);
      
      // functions to use in lambdas
      void request_physical_limits();
      void request_limits();
      void request_settings();
      void request_move_to();
      void goto_min_position();
      void goto_max_position();
      void goto_height_abs(float height);
      void goto_height_pct(float height);
      
      // uart message variables to use in lambda functions
      int getResponseByte(const int index);

      void add_button(MaidesiteDeskEntity *button) { buttons_.push_back(button); }
      void add_number(MaidesiteDeskEntity *number) { numbers_.push_back(number); }
      void add_sensor(MaidesiteDeskEntity *sensor) { sensors_.push_back(sensor); }
      
      // sensor variables
      float limit_min_ = 0;
      float limit_max_ = 0;
      float physical_min_ = 0;
      float physical_max_ = 0;

    protected:
      // uart message functions
      void read_response();
      void decode_response(std::vector<uint8_t> message);
      void log_uart_hex(std::string prefix, std::vector<uint8_t> bytes, uint8_t separator);

      // options variables
      bool log_uart_msg_ { false };

      // uart message variables
      bool checksum_passed_ { false };
      bool response_receiving_ { false };
      bool request_receiving_ { false };
      uint8_t byte_;
      uint8_t checksum_;
      uint8_t response_payload_length_;
      uint8_t request_payload_length_;
      std::vector<uint8_t> request_message_;
      std::vector<uint8_t> response_message_;
      std::vector<uint8_t> desk_message_;

      std::vector<MaidesiteDeskEntity *> buttons_;
      std::vector<MaidesiteDeskEntity *> numbers_;
      std::vector<MaidesiteDeskEntity *> sensors_;
    };
  } // namespace maidesite_desk
} // namespace esphome
