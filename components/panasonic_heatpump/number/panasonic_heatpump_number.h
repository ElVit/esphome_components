#pragma once
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"
#include "../commands.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum NumberIds : int
    {
      CONF_SET5,
      CONF_SET6,
      CONF_SET7,
      CONF_SET8,
      CONF_SET11,
      CONF_SET15,
      CONF_SET16_01,
      CONF_SET16_02,
      CONF_SET16_03,
      CONF_SET16_04,
      CONF_SET16_05,
      CONF_SET16_06,
      CONF_SET16_07,
      CONF_SET16_08,
      CONF_SET16_09,
      CONF_SET16_10,
      CONF_SET16_11,
      CONF_SET16_12,
      CONF_SET16_13,
      CONF_SET16_14,
      CONF_SET16_15,
      CONF_SET16_16,
      CONF_SET18,
      CONF_SET19,
      CONF_SET20,
      CONF_SET21,
      CONF_SET22,
      CONF_SET23,
      CONF_SET27,
      CONF_SET29,
      CONF_SET36,
      CONF_SET37,
      CONF_SET38,
    };

    class PanasonicHeatpumpNumber : public number::Number, public Component, public Parented<PanasonicHeatpumpComponent>, public PanasonicHeatpumpEntity
    {
    public:
      PanasonicHeatpumpNumber() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;
      bool set_traits(std::map<std::string, int>& raw_topics) override;

    protected:
      void control(float value) override;
    };
  } // namespace panasonic_heatpump
} // namespace esphome
