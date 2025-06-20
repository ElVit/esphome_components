#pragma once
#include <vector>
#include <tuple>
#include <string>
#include <map>
#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/log.h"
#include "esphome/components/uart/uart.h"
#include "helpers.h"
#include "decode.h"
#include "commands.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum LoopState : uint8_t
    {
      READ_RESPONSE,
      CHECK_RESPONSE,
      SET_NUMBER_TRAITS,
      SET_SELECT_TRAITS,
      PUBLISH_SENSOR,
      PUBLISH_BINARY_SENSOR,
      PUBLISH_TEXT_SENSOR,
      PUBLISH_NUMBER,
      PUBLISH_SELECT,
      PUBLISH_SWITCH,
      PUBLISH_CLIMATE,
      SEND_REQUEST,
      READ_REQUEST,
      RESTART_LOOP
    };

    enum RequestType : uint8_t
    {
      INITIAL,
      POLLING,
      COMMAND,
      NONE
    };

    class PanasonicHeatpumpEntity
    {
      public:
        virtual void set_id(const int id) { id_ = id; }
        virtual void publish_new_state(const std::vector<uint8_t>& data) = 0;
        virtual bool set_traits(std::map<std::string, int>& traits_settings) { return false; }

      protected:
        int id_ { -1 };
        int keep_state_ { 0 };
    };

    class PanasonicHeatpumpComponent : public PollingComponent, public uart::UARTDevice
    {
    public:
      bool traits_changed_ { false };

      PanasonicHeatpumpComponent() = default;
      // base class functions
      float get_setup_priority() const override { return setup_priority::DATA; }
      void dump_config() override;
      void setup() override;
      void update() override;
      void loop() override;
      // option functions
      void set_uart_client(uart::UARTComponent* uart) { this->uart_client_ = uart; }
      void set_log_uart_msg(bool active) { this->log_uart_msg_ = active; }
      void set_cool_mode(bool active) { this->cool_mode_ = active; }
      // uart message variables to use in lambda functions
      int getResponseByte(const int index);
      // command functions
      void set_command_high_nibble(const uint8_t value, const uint8_t index);
      void set_command_low_nibble(const uint8_t value, const uint8_t index);
      void set_command_byte(const uint8_t value, const uint8_t index);
      void set_command_curve(const uint8_t value, const uint8_t index);
      // entity functions
      void add_binary_sensor(PanasonicHeatpumpEntity *binary_sensor) { binary_sensors_.push_back(binary_sensor); }
      void add_climate(PanasonicHeatpumpEntity *climate) { climates_.push_back(climate); }
      void add_number(PanasonicHeatpumpEntity *number) { numbers_.push_back(number); }
      void add_select(PanasonicHeatpumpEntity *select) { selects_.push_back(select); }
      void add_sensor(PanasonicHeatpumpEntity *sensor) { sensors_.push_back(sensor); }
      void add_switch(PanasonicHeatpumpEntity *switch_) { switches_.push_back(switch_); }
      void add_text_sensor(PanasonicHeatpumpEntity *text_sensor) { text_sensors_.push_back(text_sensor); }

    protected:
      // options variables
      uart::UARTComponent* uart_client_ { nullptr };
      bool log_uart_msg_ { false };
      bool cool_mode_ { false };
      // uart message variables
      std::vector<uint8_t> heatpump_message_;
      std::vector<uint8_t> response_message_;
      std::vector<uint8_t> request_message_;
      std::vector<uint8_t> command_message_;
      std::map<std::string, int> traits_settings_;
      uint8_t payload_length_;
      uint8_t byte_;
      uint8_t current_response_count_ { 0 };
      uint8_t last_response_count_ { 0 };
      bool response_receiving_ { false };
      bool request_receiving_ { false };
      RequestType next_request_ { RequestType::INITIAL };
      LoopState loop_state_ { LoopState::RESTART_LOOP };
      uint8_t traits_update_counter_ { 0 };
      // entity vectors
      std::vector<PanasonicHeatpumpEntity *> binary_sensors_;
      std::vector<PanasonicHeatpumpEntity *> climates_;
      std::vector<PanasonicHeatpumpEntity *> numbers_;
      std::vector<PanasonicHeatpumpEntity *> selects_;
      std::vector<PanasonicHeatpumpEntity *> sensors_;
      std::vector<PanasonicHeatpumpEntity *> switches_;
      std::vector<PanasonicHeatpumpEntity *> text_sensors_;

      // uart message functions
      void read_response();
      void send_request(RequestType requestType);
      void read_request();
      bool check_response(const std::vector<uint8_t>& data);
    };
  }  // namespace panasonic_heatpump
}  // namespace esphome
