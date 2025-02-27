#pragma once
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "../maidsite_desk.h"


namespace esphome
{
  namespace maidsite_desk
  {
    class MaidsiteDeskSensor : public sensor::Sensor, public Component, public Parented<MaidsiteDeskComponent>
    {
    public:
      MaidsiteDeskSensor() = default;
      void dump_config() override;
    };
  } // namespace maidsite_desk
} // namespace esphome
