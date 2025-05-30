#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#ifdef USE_SENSOR
#include "esphome/components/sensor/sensor.h"
#endif
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#ifdef USE_TEXT_SENSOR
#include "esphome/components/text_sensor/text_sensor.h"
#endif
#ifdef USE_NUMBER
#include "esphome/components/number/number.h"
#endif
#ifdef USE_SELECT
#include "esphome/components/select/select.h"
#endif
#ifdef USE_SWITCH
#include "esphome/components/switch/switch.h"
#endif
#include "decode.h"
#include "commands.h"
#include "helpers.h"
#include <vector>
#include <tuple>
#include <string>


namespace esphome
{
  namespace panasonic_heatpump
  {
    class PanasonicHeatpumpComponent : public PollingComponent, public uart::UARTDevice
    {
    public:
      // ToDo: Add template for custom sensor classes similar to SUB_SENSOR (see Pipsolar)
#ifdef USE_SENSOR
      SUB_SENSOR(top1);
      SUB_SENSOR(top5);
      SUB_SENSOR(top6);
      SUB_SENSOR(top7);
      SUB_SENSOR(top8);
      SUB_SENSOR(top9);
      SUB_SENSOR(top10);
      SUB_SENSOR(top11);
      SUB_SENSOR(top12);
      SUB_SENSOR(top14);
      SUB_SENSOR(top15);
      SUB_SENSOR(top16);
      SUB_SENSOR(top21);
      SUB_SENSOR(top22);
      SUB_SENSOR(top23);
      SUB_SENSOR(top24);
      SUB_SENSOR(top25);
      SUB_SENSOR(top27);
      SUB_SENSOR(top28);
      SUB_SENSOR(top29);
      SUB_SENSOR(top30);
      SUB_SENSOR(top31);
      SUB_SENSOR(top32);
      SUB_SENSOR(top33);
      SUB_SENSOR(top34);
      SUB_SENSOR(top35);
      SUB_SENSOR(top36);
      SUB_SENSOR(top37);
      SUB_SENSOR(top38);
      SUB_SENSOR(top39);
      SUB_SENSOR(top40);
      SUB_SENSOR(top41);
      SUB_SENSOR(top42);
      SUB_SENSOR(top43);
      SUB_SENSOR(top45);
      SUB_SENSOR(top46);
      SUB_SENSOR(top47);
      SUB_SENSOR(top48);
      SUB_SENSOR(top49);
      SUB_SENSOR(top50);
      SUB_SENSOR(top51);
      SUB_SENSOR(top52);
      SUB_SENSOR(top53);
      SUB_SENSOR(top54);
      SUB_SENSOR(top55);
      SUB_SENSOR(top56);
      SUB_SENSOR(top57);
      SUB_SENSOR(top62);
      SUB_SENSOR(top63);
      SUB_SENSOR(top64);
      SUB_SENSOR(top65);
      SUB_SENSOR(top66);
      SUB_SENSOR(top67);
      SUB_SENSOR(top70);
      SUB_SENSOR(top71);
      SUB_SENSOR(top72);
      SUB_SENSOR(top73);
      SUB_SENSOR(top74);
      SUB_SENSOR(top75);
      SUB_SENSOR(top77);
      SUB_SENSOR(top78);
      SUB_SENSOR(top79);
      SUB_SENSOR(top80);
      SUB_SENSOR(top82);
      SUB_SENSOR(top83);
      SUB_SENSOR(top84);
      SUB_SENSOR(top85);
      SUB_SENSOR(top86);
      SUB_SENSOR(top87);
      SUB_SENSOR(top88);
      SUB_SENSOR(top89);
      SUB_SENSOR(top90);
      SUB_SENSOR(top91);
      SUB_SENSOR(top93);
      SUB_SENSOR(top95);
      SUB_SENSOR(top96);
      SUB_SENSOR(top97);
      SUB_SENSOR(top98);
      SUB_SENSOR(top102);
      SUB_SENSOR(top103);
      SUB_SENSOR(top104);
      SUB_SENSOR(top105);
      SUB_SENSOR(top113);
      SUB_SENSOR(top115);
      SUB_SENSOR(top116);
      SUB_SENSOR(top117);
      SUB_SENSOR(top118);
      SUB_SENSOR(top127);
      SUB_SENSOR(top128);
      SUB_SENSOR(top131);
      SUB_SENSOR(top134);
      SUB_SENSOR(top135);
      SUB_SENSOR(top136);
      SUB_SENSOR(top137);
      SUB_SENSOR(top138);
#endif
#ifdef USE_BINARY_SENSOR
      SUB_BINARY_SENSOR(top0);
      SUB_BINARY_SENSOR(top2);
      SUB_BINARY_SENSOR(top3);
      SUB_BINARY_SENSOR(top13);
      SUB_BINARY_SENSOR(top26);
      SUB_BINARY_SENSOR(top60);
      SUB_BINARY_SENSOR(top61);
      SUB_BINARY_SENSOR(top68);
      SUB_BINARY_SENSOR(top69);
      SUB_BINARY_SENSOR(top99);
      SUB_BINARY_SENSOR(top100);
      SUB_BINARY_SENSOR(top108);
      SUB_BINARY_SENSOR(top109);
      SUB_BINARY_SENSOR(top110);
      SUB_BINARY_SENSOR(top119);
      SUB_BINARY_SENSOR(top120);
      SUB_BINARY_SENSOR(top121);
      SUB_BINARY_SENSOR(top122);
      SUB_BINARY_SENSOR(top123);
      SUB_BINARY_SENSOR(top124);
      SUB_BINARY_SENSOR(top129);
      SUB_BINARY_SENSOR(top132);
      SUB_BINARY_SENSOR(top133);
#endif
#ifdef USE_TEXT_SENSOR
      SUB_TEXT_SENSOR(top4);
      SUB_TEXT_SENSOR(top17);
      SUB_TEXT_SENSOR(top18);
      SUB_TEXT_SENSOR(top19);
      SUB_TEXT_SENSOR(top20);
      SUB_TEXT_SENSOR(top44);
      SUB_TEXT_SENSOR(top58);
      SUB_TEXT_SENSOR(top59);
      SUB_TEXT_SENSOR(top76);
      SUB_TEXT_SENSOR(top81);
      SUB_TEXT_SENSOR(top92);
      SUB_TEXT_SENSOR(top94);
      SUB_TEXT_SENSOR(top101);
      SUB_TEXT_SENSOR(top106);
      SUB_TEXT_SENSOR(top107);
      SUB_TEXT_SENSOR(top111);
      SUB_TEXT_SENSOR(top112);
      SUB_TEXT_SENSOR(top114);
      SUB_TEXT_SENSOR(top125);
      SUB_TEXT_SENSOR(top126);
      SUB_TEXT_SENSOR(top130);
#endif
#ifdef USE_NUMBER
      SUB_NUMBER(set5);
      SUB_NUMBER(set6);
      SUB_NUMBER(set7);
      SUB_NUMBER(set8);
      SUB_NUMBER(set11);
      SUB_NUMBER(set15);
      SUB_NUMBER(set16_1);
      SUB_NUMBER(set16_2);
      SUB_NUMBER(set16_3);
      SUB_NUMBER(set16_4);
      SUB_NUMBER(set16_5);
      SUB_NUMBER(set16_6);
      SUB_NUMBER(set16_7);
      SUB_NUMBER(set16_8);
      SUB_NUMBER(set16_9);
      SUB_NUMBER(set16_10);
      SUB_NUMBER(set16_11);
      SUB_NUMBER(set16_12);
      SUB_NUMBER(set16_13);
      SUB_NUMBER(set16_14);
      SUB_NUMBER(set16_15);
      SUB_NUMBER(set16_16);
      SUB_NUMBER(set18);
      SUB_NUMBER(set19);
      SUB_NUMBER(set20);
      SUB_NUMBER(set21);
      SUB_NUMBER(set22);
      SUB_NUMBER(set23);
      SUB_NUMBER(set27);
      SUB_NUMBER(set29);
      SUB_NUMBER(set36);
      SUB_NUMBER(set37);
      SUB_NUMBER(set38);

      void control_number(number::Number* object, float value);
#endif
#ifdef USE_SELECT
      SUB_SELECT(set2);
      SUB_SELECT(set3);
      SUB_SELECT(set4);
      SUB_SELECT(set9);
      SUB_SELECT(set17);
      SUB_SELECT(set26);
      SUB_SELECT(set35);

      void control_select(select::Select* object, size_t value);
#endif
#ifdef USE_SWITCH
      SUB_SWITCH(set1);
      SUB_SWITCH(set10);
      SUB_SWITCH(set12);
      SUB_SWITCH(set13);
      SUB_SWITCH(set14);
      SUB_SWITCH(set24);
      SUB_SWITCH(set25);
      SUB_SWITCH(set28);
      SUB_SWITCH(set30);
      SUB_SWITCH(set31);
      SUB_SWITCH(set32);
      SUB_SWITCH(set33);
      SUB_SWITCH(set34);

      void control_switch(switch_::Switch* object, bool state);
#endif

      PanasonicHeatpumpComponent() = default;
      // base class functions
      float get_setup_priority() const override { return setup_priority::LATE; }
      void dump_config() override;
      void setup() override;
      void update() override;
      void loop() override;
      // option functions
      void set_uart_client(uart::UARTComponent* uart) { this->uart_client_ = uart; }
      void set_log_uart_msg(bool enable) { this->log_uart_msg_ = enable; }
      // uart message variables to use in lambda functions
      int getResponseByte(const int index);

    protected:
      // options variables
      uart::UARTComponent* uart_client_ { nullptr };
      bool log_uart_msg_ { false };
      // uart message variables
      std::vector<uint8_t> heatpump_message_;
      std::vector<uint8_t> response_message_;
      std::vector<uint8_t> request_message_;
      std::vector<uint8_t> command_message_;
      uint8_t payload_length_;
      uint8_t byte_;
      uint8_t current_response_count_ { 0 };
      uint8_t last_response_count_ { 0 };
      bool response_receiving_ { false };
      bool request_receiving_ { false };
      bool trigger_request_ { false };
      uint8_t next_request_ { 0 };  // 0 = initial, 1 = polling, 2 = command
      uint8_t loop_state_ { 0 };

      // uart message functions
      void read_response();
      void send_request();
      void read_request();
      bool check_response(const std::vector<uint8_t>& data);
      void set_command_byte(const uint8_t value, const uint8_t index);
      void set_command_bytes(const std::vector<std::tuple<uint8_t, uint8_t>>& data);
      // sensor and control publish functions
      void set_number_min_max_value(const std::vector<uint8_t>& data);
      void publish_sensor(const std::vector<uint8_t>& data);
      void publish_binary_sensor(const std::vector<uint8_t>& data);
      void publish_text_sensor(const std::vector<uint8_t>& data);
      void publish_number(const std::vector<uint8_t>& data);
      void publish_select(const std::vector<uint8_t>& data);
      void publish_switch(const std::vector<uint8_t>& data);
    };
  }  // namespace panasonic_heatpump
}  // namespace esphome
