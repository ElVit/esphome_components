#pragma once
#include "esphome/components/button/button.h"
#include "esphome/core/component.h"
#include "../maidesite_desk.h"


namespace esphome
{
  namespace maidesite_desk
  {
    class MaidesiteDeskButton : public button::Button, public Component, public Parented<MaidesiteDeskComponent>
    {
    public:
      MaidesiteDeskButton() = default;
      void dump_config() override;
      void set_id(int id) { this->id_ = id; }

    protected:
      void press_action() override;
      int id_;
    };
  } // namespace maidesite_desk
} // namespace esphome
