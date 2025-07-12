#include "maidesite_desk_sensor.h"


namespace esphome
{
  namespace maidesite_desk
  {
    static const char *const TAG = "maidesite_desk.sensor";

    void MaidesiteDeskSensor::dump_config()
    {
      LOG_SENSOR("", "Maidesite Desk Sensor", this);
      delay(10);
    }

    void MaidesiteDeskSensor::publish_new_state(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;

      float new_state;
      switch (this->id_)
      {
        case SensorIds::CONF_HEIGHT_ABS:
        {
          if (data[2] != 0x01 && data[2] != 0x21 && data[2] != 0x22) return;
          new_state = this->byte2float(data[4], data[5]);
          if (this->has_state() && this->state == new_state) return;
          break;
        }
        case SensorIds::CONF_HEIGHT_PCT:
        {
          if (data[2] != 0x01 && data[2] != 0x21 && data[2] != 0x22) return;
          new_state = this->byte2float(data[4], data[5]);
          new_state = (new_state - this->parent_->limit_min_) / (this->parent_->limit_max_ - this->parent_->limit_min_) * 100;
          if (this->has_state() && this->state == new_state) return;
          break;
        }
        case SensorIds::CONF_HEIGHT_PHYS_MIN:
        {
          if (data[2] != 0x07) return;
          new_state = this->byte2float(data[6], data[7]);
          this->parent_->physical_min_ = new_state;
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        case SensorIds::CONF_HEIGHT_PHYS_MAX:
        {
          if (data[2] != 0x07) return;
          new_state = this->byte2float(data[4], data[5]);
          this->parent_->physical_max_ = new_state;
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        case SensorIds::CONF_HEIGHT_LIMIT_MIN:
        {
          if (data[2] == 0x20 && (data[4] >> 4) == 0)
          {
            // high nibble 0 -> no min limit, use physical_min_
            new_state = this->parent_->physical_min_;
          }
          if (data[2] == 0x22)
          {
            new_state = this->byte2float(data[4], data[5]);
          }
          this->parent_->limit_min_ = new_state;
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        case SensorIds::CONF_HEIGHT_LIMIT_MAX:
        {
          if (data[2] == 0x20 && (data[4] & 1) == 0)
          {
            // low nibble 0 -> no max limit, use physical_max_
            new_state = this->parent_->physical_max_;
          }
          if (data[2] == 0x21)
          {
            new_state = this->byte2float(data[4], data[5]);
          }
          this->parent_->limit_max_ = new_state;
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        case SensorIds::CONF_POSITION_M1:
        {
          if (data[2] != 0x25) return;
          new_state = this->byte2float(data[4], data[5]);
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        case SensorIds::CONF_POSITION_M2:
        {
          if (data[2] != 0x26) return;
          new_state = this->byte2float(data[4], data[5]);
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        case SensorIds::CONF_POSITION_M3:
        {
          if (data[2] != 0x27) return;
          new_state = this->byte2float(data[4], data[5]);
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        case SensorIds::CONF_POSITION_M4:
        {
          if (data[2] != 0x28) return;
          new_state = this->byte2float(data[4], data[5]);
          if (this->has_state() && this->get_state() == new_state) return;
          break;
        }
        default: return;
      };

      this->publish_state(new_state);
    }
  } // namespace maidesite_desk
} // namespace esphome
