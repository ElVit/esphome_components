#include "maidesite_desk_number.h"


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
      switch (this->id_)
      {
        case NumberIds::CONF_HEIGHT_ABS:
        {
          this->parent_->goto_height_abs(value);
          break;
        }
        case NumberIds::CONF_HEIGHT_PCT:
        {
          this->parent_->goto_height_pct(value);
          break;
        }
        default: return;
      };
      this->publish_state(state);

    }

    void MaidesiteDeskNumber::publish_new_state(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return;

      float new_state;
      switch (this->id_)
      {
        case NumberIds::CONF_HEIGHT_ABS:
        {
          if (data[2] != 0x01 && data[2] != 0x21 && data[2] != 0x22) return;
          new_state = this->byte2float(data[4], data[5]);
          if (this->has_state() && this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_HEIGHT_PCT:
        {
          if (data[2] != 0x01 && data[2] != 0x21 && data[2] != 0x22) return;
          new_state = this->byte2float(data[4], data[5]);
          new_state = roundf((new_state - this->parent_->limit_min_) / (this->parent_->limit_max_ - this->parent_->limit_min_) * 1000) / 10;
          if (this->has_state() && this->state == new_state) return;
          break;
        }
        default: return;
      };

      this->publish_state(new_state);
    }
  } // namespace maidesite_desk
} // namespace esphome
