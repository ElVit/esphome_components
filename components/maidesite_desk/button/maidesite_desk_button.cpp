#include "maidesite_desk_button.h"
#include "esphome/core/log.h"

namespace esphome {
namespace maidesite_desk {
static const char* const TAG = "maidesite_desk.button";

void MaidesiteDeskButton::dump_config() {
  LOG_BUTTON("", "Maidesite Desk Button", this);
  delay(10);
}

void MaidesiteDeskButton::press_action() {
  this->parent_->button_press_action(this);
}
}  // namespace maidesite_desk
}  // namespace esphome
