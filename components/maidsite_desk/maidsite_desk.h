#pragma once
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_BUTTON
#include "esphome/components/button/button.h"
#endif
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#include <vector>
#include <string>


namespace esphome
{
  namespace maidsite_desk
  {

    class MaidsiteDeskComponent : public Component, public uart::UARTDevice
    {
    public:
#ifdef USE_SENSOR
      SUB_SENSOR(unit);
      SUB_SENSOR(height_abs);
      SUB_SENSOR(height_pct);
      SUB_SENSOR(height_min);
      SUB_SENSOR(height_max);
      SUB_SENSOR(position_m1);
      SUB_SENSOR(position_m2);
      SUB_SENSOR(position_m3);
      SUB_SENSOR(position_m4);
#endif
#ifdef USE_BUTTON
      SUB_BUTTON(unit);
      SUB_BUTTON(step_up);
      SUB_BUTTON(step_down);
      SUB_BUTTON(stop);
      SUB_BUTTON(goto_max);
      SUB_BUTTON(goto_min);
      SUB_BUTTON(goto_m1);
      SUB_BUTTON(goto_m2);
      SUB_BUTTON(goto_m3);
      SUB_BUTTON(goto_m4);
      SUB_BUTTON(save_m1);
      SUB_BUTTON(save_m2);
      SUB_BUTTON(save_m3);
      SUB_BUTTON(save_m4);
#endif
#ifdef USE_NUMBER
      SUB_NUMBER(height_abs);
      SUB_NUMBER(height_pct);
#endif

      MaidsiteDeskComponent() = default;
      float get_setup_priority() const override { return setup_priority::LATE; }
      void dump_config() override;
      void setup() override;
      void loop() override;

      void button_press_action(button::Button* object);
      void number_control(number::Number* object, float value);

      void request_physical_limits();
      void request_limits();
      void request_settings();
      void request_move_to();

      void step_up();
      void step_down();
      void stop();
      void goto_height(float height);
      void goto_min_position();
      void goto_max_position();
      void goto_mem_position(int pos);
      void save_mem_position(int pos);

    protected:
      void decode_response(std::vector<uint8_t> message);
      void send_simple_command(unsigned char cmd);
      void send_2byte_command(unsigned char cmd, unsigned char high_byte, unsigned char low_byte);
      float byte2float(int high, int low);
      void log_uart_hex(std::string prefix, std::vector<uint8_t> bytes, uint8_t separator);

    private:
      std::vector<uint8_t> response_message_;
      std::vector<uint8_t> request_message_;
      uint8_t response_payload_length_;
      uint8_t request_payload_length_;
      bool response_receiving_{false};
      bool request_receiving_{false};

      float current_height_ = 0;
      float limit_min_ = 0;
      float limit_max_ = 0;
      float physical_min_ = 0;
      float physical_max_ = 0;
    };
  } // namespace maidsite_desk
} // namespace esphome
