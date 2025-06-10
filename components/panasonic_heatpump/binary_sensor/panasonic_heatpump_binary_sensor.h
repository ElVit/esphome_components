#pragma once
#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum BinarySensorIds : int
    {
      CONF_TOP0,
      CONF_TOP2,
      CONF_TOP3,
      CONF_TOP13,
      CONF_TOP26,
      CONF_TOP60,
      CONF_TOP61,
      CONF_TOP68,
      CONF_TOP69,
      CONF_TOP99,
      CONF_TOP100,
      CONF_TOP108,
      CONF_TOP109,
      CONF_TOP110,
      CONF_TOP119,
      CONF_TOP120,
      CONF_TOP121,
      CONF_TOP122,
      CONF_TOP123,
      CONF_TOP124,
      CONF_TOP129,
      CONF_TOP132,
      CONF_TOP133,
    };

    class PanasonicHeatpumpBinarySensor : public binary_sensor::BinarySensor, public Component, public Parented<PanasonicHeatpumpComponent>, public PanasonicHeatpumpEntity
    {
    public:
      PanasonicHeatpumpBinarySensor() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;
    };
  } // namespace panasonic_heatpump
} // namespace esphome
