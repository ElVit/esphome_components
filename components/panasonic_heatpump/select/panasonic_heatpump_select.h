#pragma once
#include "esphome/core/component.h"
#include "esphome/components/select/select.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"
#include "../commands.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum SelectIds : uint8_t
    {
      CONF_SET2,
      CONF_SET3,
      CONF_SET4,
      CONF_SET9,
      CONF_SET17,
      CONF_SET26,
      CONF_SET35,
    };

    class PanasonicHeatpumpSelect : public select::Select, public Component,
          public Parented<PanasonicHeatpumpComponent>, public PanasonicHeatpumpEntity
    {
    public:
      PanasonicHeatpumpSelect() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;

    protected:
      void control(const std::string &value) override;
    };
  } // namespace panasonic_heatpump
} // namespace esphome
