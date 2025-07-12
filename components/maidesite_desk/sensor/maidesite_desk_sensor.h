#pragma once
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "../maidesite_desk.h"


namespace esphome
{
  enum SensorIds : uint8_t
  {
    CONF_HEIGHT_ABS,
    CONF_HEIGHT_PCT,
    CONF_HEIGHT_PHYS_MIN,
    CONF_HEIGHT_PHYS_MAX,
    CONF_HEIGHT_LIMIT_MIN,
    CONF_HEIGHT_LIMIT_MAX,
    CONF_POSITION_M1,
    CONF_POSITION_M2,
    CONF_POSITION_M3,
    CONF_POSITION_M4,
  };

  namespace maidesite_desk
  {
    class MaidesiteDeskSensor : public sensor::Sensor, public Component,
          public Parented<MaidesiteDeskComponent>, public MaidesiteDeskEntity
    {
    public:
      MaidesiteDeskSensor() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;
    };
  } // namespace maidesite_desk
} // namespace esphome
