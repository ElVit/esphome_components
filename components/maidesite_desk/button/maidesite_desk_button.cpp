#include "maidesite_desk_button.h"
#include "esphome/core/log.h"


namespace esphome
{
  namespace maidesite_desk
  {
    static const char *const TAG = "maidesite_desk.button";

    void MaidesiteDeskButton::dump_config()
    {
      LOG_BUTTON("", "Maidesite Desk Button", this);
      delay(10);
    }

    void MaidesiteDeskButton::press_action()
    {
      switch (this->id_)
      {
        case ButtonIds::CONF_STEP_UP:
        {
          this->parent_->send_1byte_command(0x01);
          break;
        }
        case ButtonIds::CONF_STEP_DOWN:
        {
          this->parent_->send_1byte_command(0x02);
          break;
        }
        case ButtonIds::CONF_STOP:
        {
          this->parent_->send_1byte_command(0x2B);
          break;
        }
        case ButtonIds::CONF_GOTO_MAX:
        {
          this->parent_->goto_max_position();
          break;
        }
        case ButtonIds::CONF_GOTO_MIN:
        {
          this->parent_->goto_min_position();
          break;
        }
        case ButtonIds::CONF_GOTO_M1:
        {
          this->parent_->send_1byte_command(0x05);
          break;
        }
        case ButtonIds::CONF_GOTO_M2:
        {
          this->parent_->send_1byte_command(0x06);
          break;
        }
        case ButtonIds::CONF_GOTO_M3:
        {
          this->parent_->send_1byte_command(0x27);
          break;
        }
        case ButtonIds::CONF_GOTO_M4:
        {
          this->parent_->send_1byte_command(0x28);
          break;
        }
        case ButtonIds::CONF_SAVE_M1:
        {
          // set memory position
          this->parent_->send_1byte_command(0x03);
          // request settings
          this->parent_->send_1byte_command(0x07);
          break;
        }
        case ButtonIds::CONF_SAVE_M2:
        {
          // set memory position
          this->parent_->send_1byte_command(0x04);
          // request settings
          this->parent_->send_1byte_command(0x07);
          break;
        }
        case ButtonIds::CONF_SAVE_M3:
        {
          // set memory position
          this->parent_->send_1byte_command(0x25);
          // request settings
          this->parent_->send_1byte_command(0x07);
          break;
        }
        case ButtonIds::CONF_SAVE_M4:
        {
          // set memory position
          this->parent_->send_1byte_command(0x26);
          // request settings
          this->parent_->send_1byte_command(0x07);
          break;
        }
        default: return;
      };
    }
  } // namespace maidesite_desk
} // namespace esphome
