#include "panasonic_heatpump_select.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    static const char *const TAG = "panasonic_heatpump.select";

    void PanasonicHeatpumpSelect::dump_config()
    {
      LOG_SELECT("", "Panasonic Heatpump Select", this);
      delay(10);
    }

    void PanasonicHeatpumpSelect::control(const std::string &value)
    {
      auto optIndex = this->index_of(value);
      if (!optIndex.has_value()) return;
      size_t index = optIndex.value();

      switch (this->id_)
      {
        case SelectIds::CONF_SET2:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply16(index), 5);
          break;
        }
        case SelectIds::CONF_SET3:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply8(index), 7);
          break;
        }
        case SelectIds::CONF_SET4:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1(index), 7);
          break;
        }
        case SelectIds::CONF_SET9:
        {
          this->parent_->set_command_byte(PanasonicCommand::setOperationMode(index), 6);
          break;
        }
        case SelectIds::CONF_SET17:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply64(index), 6);
          break;
        }
        case SelectIds::CONF_SET26:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply16(index), 25);
          break;
        }
        case SelectIds::CONF_SET35:
        {
          this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply4(index), 26);
          break;
        }
        default: return;
      };
      this->publish_state(value);
      this->keep_state_ = 2;
    }

    void PanasonicHeatpumpSelect::publish_new_state(const std::vector<uint8_t>& data)
    {
      if (this->keep_state_ > 0)
      {
        this->keep_state_--;
        return;
      }
      if (data.empty()) return;

      std::string new_state;
      switch (this->id_)
      {
        case SelectIds::CONF_SET9:
        {
          new_state = PanasonicDecode::getTextState(PanasonicDecode::OperationMode, PanasonicDecode::getOperationMode(data[6]));
          if (this->state == new_state) return;
          break;
        }
        case SelectIds::CONF_SET4:
        {
          new_state = PanasonicDecode::getTextState(PanasonicDecode::PowerfulMode, PanasonicDecode::getBit6and7and8(data[7]));
          if (this->state == new_state) return;
          break;
        }
        case SelectIds::CONF_SET3:
        {
          new_state = PanasonicDecode::getTextState(PanasonicDecode::QuietMode, PanasonicDecode::getBit3and4and5(data[7]));
          if (this->state == new_state) return;
          break;
        }
        case SelectIds::CONF_SET2:
        {
          new_state = PanasonicDecode::getTextState(PanasonicDecode::HolidayState, PanasonicDecode::getBit3and4(data[5]));
          if (this->state == new_state) return;
          break;
        }
        case SelectIds::CONF_SET17:
        {
          new_state = PanasonicDecode::getTextState(PanasonicDecode::ZoneState, PanasonicDecode::getBit1and2(data[6]));
          if (this->state == new_state) return;
          break;
        }
        case SelectIds::CONF_SET26:
        {
          new_state = PanasonicDecode::getTextState(PanasonicDecode::ExtPadHeaterType, PanasonicDecode::getBit3and4(data[25]));
          if (this->state == new_state) return;
          break;
        }
        case SelectIds::CONF_SET35:
        {
          new_state = PanasonicDecode::getTextState(PanasonicDecode::BivalentMode, PanasonicDecode::getBit5and6(data[26]));
          if (this->state == new_state) return;
          break;
        }
        default: return;
      };

      this->publish_state(new_state);
    }

    bool PanasonicHeatpumpSelect::set_traits(const std::vector<uint8_t>& data)
    {
      if (data.empty()) return false;

      bool top120 = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[23]));  // Heat Cool Control
      switch (this->id_)
      {
        case SelectIds::CONF_SET9:
        {
          if (this->traits.get_options().size() != 3 && !top120)
          {
            auto options = std::vector<std::string>(PanasonicDecode::OperationMode + 1, PanasonicDecode::OperationMode + 4);
            this->traits.set_options(options);
            return true;
          }
          if (this->traits.get_options().size() != 10 && top120)
          {
            auto options = std::vector<std::string>(PanasonicDecode::OperationMode + 1, std::end(PanasonicDecode::OperationMode));
            this->traits.set_options(options);
            return true;
          }
          break;
        }
      };
      return false;
    }
  } // namespace panasonic_heatpump
} // namespace esphome
