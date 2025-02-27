#include "maidsite_desk_sensor.h"
#include "esphome/core/log.h"


namespace esphome
{
  namespace maidsite_desk
  {
    static const char *const TAG = "maidsite_desk.sensor";

    void MaidsiteDeskSensor::dump_config()
    {
      LOG_SENSOR("", "Maidsite Desk Sensor", this);
      delay(10);
    }
  } // namespace maidsite_desk
} // namespace esphome
