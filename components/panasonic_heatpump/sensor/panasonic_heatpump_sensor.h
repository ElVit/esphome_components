#pragma once
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum SensorIds : uint8_t
    {
      CONF_TOP1,
      CONF_TOP5,
      CONF_TOP6,
      CONF_TOP7,
      CONF_TOP8,
      CONF_TOP9,
      CONF_TOP10,
      CONF_TOP11,
      CONF_TOP12,
      CONF_TOP14,
      CONF_TOP15,
      CONF_TOP16,
      CONF_TOP21,
      CONF_TOP22,
      CONF_TOP23,
      CONF_TOP24,
      CONF_TOP25,
      CONF_TOP27,
      CONF_TOP28,
      CONF_TOP29,
      CONF_TOP30,
      CONF_TOP31,
      CONF_TOP32,
      CONF_TOP33,
      CONF_TOP34,
      CONF_TOP35,
      CONF_TOP36,
      CONF_TOP37,
      CONF_TOP38,
      CONF_TOP39,
      CONF_TOP40,
      CONF_TOP41,
      CONF_TOP42,
      CONF_TOP43,
      CONF_TOP45,
      CONF_TOP46,
      CONF_TOP47,
      CONF_TOP48,
      CONF_TOP49,
      CONF_TOP50,
      CONF_TOP51,
      CONF_TOP52,
      CONF_TOP53,
      CONF_TOP54,
      CONF_TOP55,
      CONF_TOP56,
      CONF_TOP57,
      CONF_TOP62,
      CONF_TOP63,
      CONF_TOP64,
      CONF_TOP65,
      CONF_TOP66,
      CONF_TOP67,
      CONF_TOP70,
      CONF_TOP71,
      CONF_TOP72,
      CONF_TOP73,
      CONF_TOP74,
      CONF_TOP75,
      CONF_TOP77,
      CONF_TOP78,
      CONF_TOP79,
      CONF_TOP80,
      CONF_TOP82,
      CONF_TOP83,
      CONF_TOP84,
      CONF_TOP85,
      CONF_TOP86,
      CONF_TOP87,
      CONF_TOP88,
      CONF_TOP89,
      CONF_TOP90,
      CONF_TOP91,
      CONF_TOP93,
      CONF_TOP95,
      CONF_TOP96,
      CONF_TOP97,
      CONF_TOP98,
      CONF_TOP102,
      CONF_TOP103,
      CONF_TOP104,
      CONF_TOP105,
      CONF_TOP113,
      CONF_TOP115,
      CONF_TOP116,
      CONF_TOP117,
      CONF_TOP118,
      CONF_TOP127,
      CONF_TOP128,
      CONF_TOP131,
      CONF_TOP134,
      CONF_TOP135,
      CONF_TOP136,
      CONF_TOP137,
      CONF_TOP138,

      CONF_XTOP0,
      CONF_XTOP1,
      CONF_XTOP2,
      CONF_XTOP3,
      CONF_XTOP4,
      CONF_XTOP5,
    };

    class PanasonicHeatpumpSensor : public sensor::Sensor, public Component,
          public Parented<PanasonicHeatpumpComponent>, public PanasonicHeatpumpEntity
    {
    public:
      PanasonicHeatpumpSensor() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;
    };
  } // namespace panasonic_heatpump
} // namespace esphome
