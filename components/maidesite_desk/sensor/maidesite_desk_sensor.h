#pragma once
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "../maidesite_desk.h"

namespace esphome {
namespace maidesite_desk {
class MaidesiteDeskSensor : public sensor::Sensor, public Component, public Parented<MaidesiteDeskComponent> {
 public:
  MaidesiteDeskSensor() = default;
  void dump_config() override;
};
}  // namespace maidesite_desk
}  // namespace esphome
