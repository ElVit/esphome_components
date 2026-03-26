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
#define PANASONIC_HEATPUMP_VERSION "0.0.8"
#endif

#ifndef KEEP_STATE
#define KEEP_STATE 1
#endif

namespace esphome {
namespace panasonic_heatpump {
enum LoopState : uint8_t {
  READ_RESPONSE,
  PUBLISH_SENSOR,
  PUBLISH_BINARY_SENSOR,
  PUBLISH_TEXT_SENSOR,
  PUBLISH_NUMBER,
  PUBLISH_SELECT,
  PUBLISH_SWITCH,
  PUBLISH_CLIMATE,
  PUBLISH_WATER_HEATER,
  PUBLISH_EXTRA_SENSOR,
  SEND_REQUEST,
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
  // functions to use in esphome lambda
  int get_response_byte(const int index);
  int get_extra_response_byte(const int index);
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
  void add_water_heater(PanasonicHeatpumpEntity* water_heater) {
    water_heaters_.push_back(water_heater);
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
  bool get_uart_client_timeout_exceeded() {
    return this->uart_client_timeout_exceeded_;
  }

 protected:
  // options variables
  bool log_uart_msg_{false};
  uint32_t last_client_request_time_{0};
  uint32_t uart_client_timeout_{10000};  // 10 sec
  uint32_t request_send_time_{5000};  // 5 sec --> default is 5 sec so first request is not sent too fast after startup
  static const uint32_t REQUEST_SEND_INTERVAL{250};  // 250 ms
  static const size_t HEADER_SIZE = 4;

  // uart message variables, process in main loop
  TaskHandle_t uart_task_handle_{nullptr};
  TaskHandle_t uart_client_task_handle_{nullptr};
  QueueHandle_t response_queue_handle_{nullptr};
  QueueHandle_t request_queue_handle_{nullptr};
  uart::UARTComponent* uart_client_{nullptr};
  std::vector<uint8_t> heatpump_default_message_;
  std::vector<uint8_t> heatpump_extra_message_;
  std::vector<uint8_t> command_message_;
  bool uart_client_timeout_exceeded_{false};
  LoopState loop_state_{LoopState::RESTART_LOOP};

  // entity vectors
  std::vector<PanasonicHeatpumpEntity*> binary_sensors_;
  std::vector<PanasonicHeatpumpEntity*> climates_;
  std::vector<PanasonicHeatpumpEntity*> water_heaters_;
  std::vector<PanasonicHeatpumpEntity*> numbers_;
  std::vector<PanasonicHeatpumpEntity*> selects_;
  std::vector<PanasonicHeatpumpEntity*> sensors_;
  std::vector<PanasonicHeatpumpEntity*> switches_;
  std::vector<PanasonicHeatpumpEntity*> text_sensors_;
  std::vector<PanasonicHeatpumpEntity*> extra_sensors_;

  // uart message functions
  static void uart_task(void* pvParameters);
  static void uart_client_task(void* pvParameters);
  bool receive_from_uart(uart::UARTComponent* src, std::vector<uint8_t>& buffer);

  void send_request();
  void queue_request(const std::vector<uint8_t>& message);
  ResponseType read_response();
  static bool check_response_length(const std::vector<uint8_t>& message);
  static bool verify_message_header(const std::vector<uint8_t>& message, bool reading_succeeded);
  static bool verify_message_checksum(const std::vector<uint8_t>& message);

  template <size_t N>
  static std::vector<uint8_t> build_message(const uint8_t (&msg)[N]) {
    return std::vector<uint8_t>(msg, msg + N);
  }
};
}  // namespace panasonic_heatpump
}  // namespace esphome
