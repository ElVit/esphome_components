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

#ifndef PANASONIC_HEATPUMP_VERSION
#define PANASONIC_HEATPUMP_VERSION "0.0.5-beta.1"
#endif

namespace esphome {
namespace panasonic_heatpump {
enum LoopState : uint8_t {
  READ_RESPONSE,
  CHECK_RESPONSE,
  PUBLISH_SENSOR,
  PUBLISH_BINARY_SENSOR,
  PUBLISH_TEXT_SENSOR,
  PUBLISH_NUMBER,
  PUBLISH_SELECT,
  PUBLISH_SWITCH,
  PUBLISH_CLIMATE,
  PUBLISH_EXTRA_SENSOR,
  SEND_REQUEST,
  READ_REQUEST,
  RESTART_LOOP,
};

enum RequestType : uint8_t {
  NONE,
  COMMAND,
  INITIAL,
  POLLING,
  POLLING_EXTRA,
};

enum ResponseType : uint8_t {
  UNKNOWN,
  STANDARD,
  EXTRA,
};

class PanasonicHeatpumpEntity {
 public:
  virtual void set_id(const int id) {
    id_ = id;
  }
  virtual void publish_new_state(const std::vector<uint8_t>& data) = 0;

 protected:
  int id_{-1};
  int keep_state_{0};
};

class PanasonicHeatpumpComponent : public PollingComponent, public uart::UARTDevice {
 public:
  PanasonicHeatpumpComponent() = default;
  // base class functions
  float get_setup_priority() const override {
    return setup_priority::DATA;
  }
  void dump_config() override;
  void setup() override;
  void update() override;
  void loop() override;
  // option functions
  void set_uart_client(uart::UARTComponent* uart) {
    this->uart_client_ = uart;
  }
  void set_uart_client_timeout(uint32_t timeout_ms) {
    this->uart_client_timeout_ = timeout_ms;
  }
  void set_log_uart_msg(bool active) {
    this->log_uart_msg_ = active;
  }
  // uart message variables to use in lambda functions
  int getResponseByte(const int index);
  int getExtraResponseByte(const int index);
  // command functions
  void set_command_high_nibble(const uint8_t value, const uint8_t index);
  void set_command_low_nibble(const uint8_t value, const uint8_t index);
  void set_command_byte(const uint8_t value, const uint8_t index);
  void set_command_curve(const uint8_t value, const uint8_t index);
  // entity functions
  void add_binary_sensor(PanasonicHeatpumpEntity* binary_sensor) {
    binary_sensors_.push_back(binary_sensor);
  }
  void add_climate(PanasonicHeatpumpEntity* climate) {
    climates_.push_back(climate);
  }
  void add_number(PanasonicHeatpumpEntity* number) {
    numbers_.push_back(number);
  }
  void add_select(PanasonicHeatpumpEntity* select) {
    selects_.push_back(select);
  }
  void add_sensor(PanasonicHeatpumpEntity* sensor) {
    sensors_.push_back(sensor);
  }
  void add_switch(PanasonicHeatpumpEntity* switch_) {
    switches_.push_back(switch_);
  }
  void add_text_sensor(PanasonicHeatpumpEntity* text_sensor) {
    text_sensors_.push_back(text_sensor);
  }
  void add_extra_sensor(PanasonicHeatpumpEntity* sensor) {
    extra_sensors_.push_back(sensor);
  }

 protected:
  // options variables
  uart::UARTComponent* uart_client_{nullptr};
  bool log_uart_msg_{false};
  uint32_t last_request_time_{0};
  uint32_t uart_client_timeout_{10000};
  // uart message variables
  std::vector<uint8_t> heatpump_default_message_;
  std::vector<uint8_t> heatpump_extra_message_;
  std::vector<uint8_t> response_message_;
  std::vector<uint8_t> request_message_;
  std::vector<uint8_t> command_message_;
  uint8_t payload_length_;
  uint8_t byte_;
  uint8_t current_response_count_{0};
  uint8_t last_response_count_{0};
  bool response_receiving_{false};
  bool request_receiving_{false};
  bool send_extra_request_{false};
  LoopState loop_state_{LoopState::RESTART_LOOP};
  RequestType next_request_{RequestType::INITIAL};
  ResponseType current_response_{ResponseType::UNKNOWN};
  // entity vectors
  std::vector<PanasonicHeatpumpEntity*> binary_sensors_;
  std::vector<PanasonicHeatpumpEntity*> climates_;
  std::vector<PanasonicHeatpumpEntity*> numbers_;
  std::vector<PanasonicHeatpumpEntity*> selects_;
  std::vector<PanasonicHeatpumpEntity*> sensors_;
  std::vector<PanasonicHeatpumpEntity*> switches_;
  std::vector<PanasonicHeatpumpEntity*> text_sensors_;
  std::vector<PanasonicHeatpumpEntity*> extra_sensors_;

  // uart message functions
  void read_response();
  void send_request(RequestType requestType);
  void read_request();
  ResponseType check_response(const std::vector<uint8_t>& data);
};
}  // namespace panasonic_heatpump
}  // namespace esphome
