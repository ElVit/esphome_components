#pragma once
#include "esphome/core/component.h"
#include "esphome/components/water_heater/water_heater.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"
#include "../commands.h"
#include <set>

namespace esphome {
namespace panasonic_heatpump {
enum WaterHeaterIds : uint8_t {
  CONF_TANK,
};

class PanasonicHeatpumpWaterHeater : public water_heater::WaterHeater,
                                      public Parented<PanasonicHeatpumpComponent>,
                                      public PanasonicHeatpumpEntity {
 public:
  PanasonicHeatpumpWaterHeater() = default;
  void dump_config() override;
  water_heater::WaterHeaterTraits traits() override;
  void publish_new_state(const std::vector<uint8_t>& data) override;

  void set_min_temperature(float value) {
    this->min_temperature_ = value;
  }
  void set_max_temperature(float value) {
    this->max_temperature_ = value;
  }
  void set_temperature_step(float value) {
    this->temperature_step_ = value;
  }

 protected:
  void control(const water_heater::WaterHeaterCall& call) override;
  water_heater::WaterHeaterCallInternal make_call() override;
  uint8_t getWaterHeaterMode(const uint8_t input);
  uint8_t setWaterHeaterMode(const water_heater::WaterHeaterMode mode, const uint8_t byte);

  float min_temperature_{20.0f};
  float max_temperature_{65.0f};
  float temperature_step_{0.5f};
};
}  // namespace panasonic_heatpump
}  // namespace esphome
