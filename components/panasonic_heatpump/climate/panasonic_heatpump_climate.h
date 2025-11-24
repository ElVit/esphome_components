#pragma once
#include "esphome/core/component.h"
#include "esphome/components/climate/climate.h"
#include "../panasonic_heatpump.h"
#include "../decode.h"
#include "../commands.h"
#include <set>


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum ClimateIds : uint8_t
    {
      CONF_CLIMATE_TANK,
      CONF_CLIMATE_ZONE1,
      CONF_CLIMATE_ZONE2,
    };

    class PanasonicHeatpumpClimate : public climate::Climate, public Component,
          public Parented<PanasonicHeatpumpComponent>, public PanasonicHeatpumpEntity
    {
    public:
      PanasonicHeatpumpClimate() = default;
      void dump_config() override;
      climate::ClimateTraits traits() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;

      void set_cool_mode(bool value) { this->cool_mode_ = value; }
      void set_min_temperature(float value) { this->min_temperature_ = value; }
      void set_max_temperature(float value) { this->max_temperature_ = value; }
      void set_temperature_step(float value) { this->temperature_step_ = value; }

    protected:
      void control(const climate::ClimateCall &call) override;
      uint8_t getClimateMode(const uint8_t input);
      uint8_t setClimateMode(const climate::ClimateMode mode, const uint8_t byte);

      bool cool_mode_ { false };
      float min_temperature_ { -5.0 };
      float max_temperature_ { 5.0 };
      float temperature_step_ { 0.5 };
    };
  } // namespace panasonic_heatpump
} // namespace esphome
