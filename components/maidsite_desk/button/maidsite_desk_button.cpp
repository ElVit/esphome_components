#include "maidsite_desk_button.h"
#include "esphome/core/log.h"


namespace esphome
{
  namespace maidsite_desk
  {
    static const char *const TAG = "maidsite_desk.button";

    void MaidsiteDeskButton::dump_config()
    {
      LOG_BUTTON("", "Maidsite Desk Button", this);
      delay(10);
    }

    void MaidsiteDeskButton::press_action()
    {
      this->parent_->button_press_action(this);
    }
  } // namespace maidsite_desk
} // namespace esphome
