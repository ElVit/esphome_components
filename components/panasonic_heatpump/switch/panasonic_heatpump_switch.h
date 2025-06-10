#pragma once
#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"
#include "../commands.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum SwitchIds : int
    {
      CONF_SET1,
      CONF_SET10,
      CONF_SET12,
      CONF_SET13,
      CONF_SET14,
      CONF_SET24,
      CONF_SET25,
      CONF_SET28,
      CONF_SET30,
      CONF_SET31,
      CONF_SET32,
      CONF_SET33,
      CONF_SET34,
    };

    class PanasonicHeatpumpSwitch : public switch_::Switch, public Component, public Parented<PanasonicHeatpumpComponent>, public PanasonicHeatpumpEntity
    {
    public:
      PanasonicHeatpumpSwitch() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;

    protected:
      void write_state(bool state) override;
    };
  } // namespace panasonic_heatpump
} // namespace esphome
