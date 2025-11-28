#include "maidesite_desk_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace maidesite_desk {
static const char* const TAG = "maidesite_desk.sensor";

void MaidesiteDeskSensor::dump_config() {
  LOG_SENSOR("", "Maidesite Desk Sensor", this);
  delay(10);
}
}  // namespace maidesite_desk
}  // namespace esphome
