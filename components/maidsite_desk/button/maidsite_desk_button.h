#pragma once
#include "esphome/components/button/button.h"
#include "esphome/core/component.h"
#include "../maidsite_desk.h"


namespace esphome
{
  namespace maidsite_desk
  {
    class MaidsiteDeskButton : public button::Button, public Component, public Parented<MaidsiteDeskComponent>
    {
    public:
      MaidsiteDeskButton() = default;
      void dump_config() override;
      void set_id(int id) { this->id_ = id; }

    protected:
      void press_action() override;
      int id_;
    };
  } // namespace maidsite_desk
} // namespace esphome
