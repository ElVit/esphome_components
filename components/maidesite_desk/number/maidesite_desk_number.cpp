#include "maidesite_desk_number.h"
#include "esphome/core/log.h"


namespace esphome
{
  namespace maidesite_desk
  {
    static const char *const TAG = "maidesite_desk.number";

    void MaidesiteDeskNumber::dump_config()
    {
      LOG_NUMBER("", "Maidesite Desk Number", this);
      delay(10);
    }

    void MaidesiteDeskNumber::control(float value)
    {
      this->publish_state(value);
      this->parent_->number_control(this, value);
    }
  } // namespace maidesite_desk
} // namespace esphome
