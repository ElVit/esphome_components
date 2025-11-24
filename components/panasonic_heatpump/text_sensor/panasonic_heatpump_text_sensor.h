#pragma once
#include "esphome/core/component.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"

namespace esphome {
namespace panasonic_heatpump {
enum TextSensorIds : uint8_t {
  CONF_TOP4,
  CONF_TOP17,
  CONF_TOP18,
  CONF_TOP19,
  CONF_TOP20,
  CONF_TOP44,
  CONF_TOP58,
  CONF_TOP59,
  CONF_TOP76,
  CONF_TOP81,
  CONF_TOP92,
  CONF_TOP94,
  CONF_TOP101,
  CONF_TOP106,
  CONF_TOP107,
  CONF_TOP111,
  CONF_TOP112,
  CONF_TOP114,
  CONF_TOP125,
  CONF_TOP126,
  CONF_TOP130,
};

class PanasonicHeatpumpTextSensor : public text_sensor::TextSensor,
                                    public Component,
                                    public Parented<PanasonicHeatpumpComponent>,
                                    public PanasonicHeatpumpEntity {
 public:
  PanasonicHeatpumpTextSensor() = default;
  void dump_config() override;
  void publish_new_state(const std::vector<uint8_t>& data) override;
};
}  // namespace panasonic_heatpump
}  // namespace esphome
