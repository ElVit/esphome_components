#include "panasonic_heatpump_number.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    static const char *const TAG = "panasonic_heatpump.number";

    void PanasonicHeatpumpNumber::dump_config()
    {
      LOG_NUMBER("", "Panasonic Heatpump Number", this);
      delay(10);
    }

    void PanasonicHeatpumpNumber::control(float value)
    {
      switch (this->id_)
      {
        case NumberIds::CONF_SET5:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 38);
          break;
        }
        case NumberIds::CONF_SET6:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 39);
          break;
        }
        case NumberIds::CONF_SET7:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 40);
          break;
        }
        case NumberIds::CONF_SET8:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 41);
          break;
        }
        case NumberIds::CONF_SET11:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 42);
          break;
        }
        case NumberIds::CONF_SET15:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1(value), 45);
          break;
        }
        case NumberIds::CONF_SET16_01:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 75);
          break;
        }
        case NumberIds::CONF_SET16_02:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 76);
          break;
        }
        case NumberIds::CONF_SET16_03:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 77);
          break;
        }
        case NumberIds::CONF_SET16_04:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 78);
          break;
        }
        case NumberIds::CONF_SET16_05:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 79);
          break;
        }
        case NumberIds::CONF_SET16_06:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 80);
          break;
        }
        case NumberIds::CONF_SET16_07:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 81);
          break;
        }
        case NumberIds::CONF_SET16_08:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 82);
          break;
        }
        case NumberIds::CONF_SET16_09:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 86);
          break;
        }
        case NumberIds::CONF_SET16_10:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 87);
          break;
        }
        case NumberIds::CONF_SET16_11:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 88);
          break;
        }
        case NumberIds::CONF_SET16_12:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 89);
          break;
        }
        case NumberIds::CONF_SET16_13:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 90);
          break;
        }
        case NumberIds::CONF_SET16_14:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 91);
          break;
        }
        case NumberIds::CONF_SET16_15:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 92);
          break;
        }
        case NumberIds::CONF_SET16_16:
        {
          this->parent_->set_command_curve(PanasonicCommand::setPlus128(value), 93);
          break;
        }
        case NumberIds::CONF_SET18:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 84);
          break;
        }
        case NumberIds::CONF_SET19:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 94);
          break;
        }
        case NumberIds::CONF_SET20:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 99);
          break;
        }
        case NumberIds::CONF_SET21:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1(value), 104);
          break;
        }
        case NumberIds::CONF_SET22:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 105);
          break;
        }
        case NumberIds::CONF_SET23:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 106);
          break;
        }
        case NumberIds::CONF_SET27:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 59);
          break;
        }
        case NumberIds::CONF_SET29:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 83);
          break;
        }
        case NumberIds::CONF_SET36:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 65);
          break;
        }
        case NumberIds::CONF_SET37:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 66);
          break;
        }
        case NumberIds::CONF_SET38:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus128(value), 68);
          break;
        }
        default: return;
      };
      this->publish_state(state);
      this->keep_state_ = 2;
    }

    void PanasonicHeatpumpNumber::publish_new_state(const std::vector<uint8_t>& data)
    {
      if (this->keep_state_ > 0)
      {
        this->keep_state_--;
        return;
      }

      float new_state;
      switch (this->id_)
      {
        case NumberIds::CONF_SET11:
        {
          new_state = PanasonicDecode::getByteMinus128(data[42]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET20:
        {
          new_state = PanasonicDecode::getByteMinus128(data[99]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET18:
        {
          new_state = PanasonicDecode::getByteMinus128(data[84]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET19:
        {
          new_state = PanasonicDecode::getByteMinus128(data[94]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET5:
        {
          new_state = PanasonicDecode::getByteMinus128(data[38]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET6:
        {
          new_state = PanasonicDecode::getByteMinus128(data[39]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_01:
        {
          new_state = PanasonicDecode::getByteMinus128(data[75]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_02:
        {
          new_state = PanasonicDecode::getByteMinus128(data[76]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_04:
        {
          new_state = PanasonicDecode::getByteMinus128(data[78]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_03:
        {
          new_state = PanasonicDecode::getByteMinus128(data[77]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET7:
        {
          new_state = PanasonicDecode::getByteMinus128(data[40]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET8:
        {
          new_state = PanasonicDecode::getByteMinus128(data[41]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_09:
        {
          new_state = PanasonicDecode::getByteMinus128(data[86]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_10:
        {
          new_state = PanasonicDecode::getByteMinus128(data[87]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_12:
        {
          new_state = PanasonicDecode::getByteMinus128(data[89]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_11:
        {
          new_state = PanasonicDecode::getByteMinus128(data[88]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET29:
        {
          new_state = PanasonicDecode::getByteMinus128(data[83]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_05:
        {
          new_state = PanasonicDecode::getByteMinus128(data[79]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_06:
        {
          new_state = PanasonicDecode::getByteMinus128(data[80]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_08:
        {
          new_state = PanasonicDecode::getByteMinus128(data[82]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_07:
        {
          new_state = PanasonicDecode::getByteMinus128(data[81]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_13:
        {
          new_state = PanasonicDecode::getByteMinus128(data[90]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_14:
        {
          new_state = PanasonicDecode::getByteMinus128(data[91]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_16:
        {
          new_state = PanasonicDecode::getByteMinus128(data[93]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET16_15:
        {
          new_state = PanasonicDecode::getByteMinus128(data[92]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET15:
        {
          new_state = PanasonicDecode::getByteMinus1(data[45]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET21:
        {
          new_state = PanasonicDecode::getByteMinus1(data[104]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET22:
        {
          new_state = PanasonicDecode::getByteMinus128(data[105]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET23:
        {
          new_state = PanasonicDecode::getByteMinus128(data[106]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET27:
        {
          new_state = PanasonicDecode::getByteMinus128(data[59]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET36:
        {
          new_state = PanasonicDecode::getByteMinus128(data[65]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET37:
        {
          new_state = PanasonicDecode::getByteMinus128(data[66]);
          if (this->state == new_state) return;
          break;
        }
        case NumberIds::CONF_SET38:
        {
          new_state = PanasonicDecode::getByteMinus128(data[68]);
          if (this->state == new_state) return;
          break;
        }
        default: return;
      };
      this->publish_state(new_state);
    }

    bool PanasonicHeatpumpNumber::set_traits(const std::vector<uint8_t>& data)
    {
      std::string top76 = PanasonicDecode::getTextState(
        PanasonicDecode::WaterTempControl, PanasonicDecode::getBit7and8(data[28]));   // Heating Mode
      std::string top81 = PanasonicDecode::getTextState(
        PanasonicDecode::WaterTempControl, PanasonicDecode::getBit5and6(data[28]));   // Cooling Mode

      switch (this->id_)
      {
        case NumberIds::CONF_SET5:
        {
          if (this->traits.get_min_value() != -5.0 && top76 == PanasonicDecode::WaterTempControl[1])
          {
            this->traits.set_min_value(-5.0);
            this->traits.set_max_value(5.0);
            return true;
          }
          if (this->traits.get_min_value() != 20.0 && top76 == PanasonicDecode::WaterTempControl[2])
          {
            this->traits.set_min_value(20.0);
            this->traits.set_max_value(60.0);
            return true;
          }
          break;
        }
        case NumberIds::CONF_SET6:
        {
          if (this->traits.get_min_value() != -5.0 && top81 == PanasonicDecode::WaterTempControl[1])
          {
            this->traits.set_min_value(-5.0);
            this->traits.set_max_value(5.0);
            return true;
          }
          if (this->traits.get_min_value() != 20.0 && top81 == PanasonicDecode::WaterTempControl[2])
          {
            this->traits.set_min_value(20.0);
            this->traits.set_max_value(60.0);
            return true;
          }
          break;
        }
        case NumberIds::CONF_SET7:
        {
          if (this->traits.get_min_value() != -5.0 && top76 == PanasonicDecode::WaterTempControl[1])
          {
            this->traits.set_min_value(-5.0);
            this->traits.set_max_value(5.0);
            return true;
          }
          if (this->traits.get_min_value() != 20.0 && top76 == PanasonicDecode::WaterTempControl[2])
          {
            this->traits.set_min_value(20.0);
            this->traits.set_max_value(60.0);
            return true;
          }
          break;
        }
        case NumberIds::CONF_SET8:
        {
          if (this->traits.get_min_value() != -5.0 && top81 == PanasonicDecode::WaterTempControl[1])
          {
            this->traits.set_min_value(-5.0);
            this->traits.set_max_value(5.0);
            return true;
          }
          if (this->traits.get_min_value() != 20.0 && top81 == PanasonicDecode::WaterTempControl[2])
          {
            this->traits.set_min_value(20.0);
            this->traits.set_max_value(60.0);
            return true;
          }
          break;
        }
      };
      return false;
    }
  } // namespace panasonic_heatpump
} // namespace esphome
