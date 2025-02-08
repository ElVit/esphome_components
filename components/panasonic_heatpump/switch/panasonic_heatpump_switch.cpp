#include "panasonic_heatpump_switch.h"
#include "esphome/core/log.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    static const char *const TAG = "panasonic_heatpump.switch";

    void PanasonicHeatpumpSwitch::dump_config()
    {
      LOG_SWITCH("", "Panasonic Heatpump Switch", this);
    }

    void PanasonicHeatpumpSwitch::write_state(bool state)
    {
      this->publish_state(state);
      size_t value = state ? 1 : 0;
      this->parent_->switch_control(this, value);
    }
  } // namespace panasonic_heatpump
} // namespace esphome
