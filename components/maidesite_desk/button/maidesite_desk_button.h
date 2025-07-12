#pragma once
#include "esphome/components/button/button.h"
#include "esphome/core/component.h"
#include "../maidesite_desk.h"


namespace esphome
{
  namespace maidesite_desk
  {
    enum ButtonIds : uint8_t
    {
      CONF_STEP_UP,
      CONF_STEP_DOWN,
      CONF_STOP,
      CONF_GOTO_MAX,
      CONF_GOTO_MIN,
      CONF_GOTO_M1,
      CONF_GOTO_M2,
      CONF_GOTO_M3,
      CONF_GOTO_M4,
      CONF_SAVE_M1,
      CONF_SAVE_M2,
      CONF_SAVE_M3,
      CONF_SAVE_M4,
    };

    class MaidesiteDeskButton : public button::Button, public Component,
          public Parented<MaidesiteDeskComponent>, public MaidesiteDeskEntity
    {
    public:
      MaidesiteDeskButton() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override {}

    protected:
      void press_action() override;
    };
  } // namespace maidesite_desk
} // namespace esphome
