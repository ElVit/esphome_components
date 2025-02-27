#include "maidsite_desk_number.h"
#include "esphome/core/log.h"


namespace esphome
{
  namespace maidsite_desk
  {
    static const char *const TAG = "maidsite_desk.number";

    void MaidsiteDeskNumber::dump_config()
    {
      LOG_NUMBER("", "Maidsite Desk Number", this);
      delay(10);
    }

    void MaidsiteDeskNumber::control(float value)
    {
      this->publish_state(value);
      this->parent_->number_control(this, value);
    }
  } // namespace maidsite_desk
} // namespace esphome
