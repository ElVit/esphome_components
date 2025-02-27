#pragma once
#include "esphome/components/number/number.h"
#include "esphome/core/component.h"
#include "../maidsite_desk.h"


namespace esphome
{
  namespace maidsite_desk
  {
    class MaidsiteDeskNumber : public number::Number, public Component, public Parented<MaidsiteDeskComponent>
    {
    public:
      MaidsiteDeskNumber() = default;
      void dump_config() override;
      void set_id(int id) { this->id_ = id; }

    protected:
      void control(float value) override;
      int id_;
    };
  } // namespace maidsite_desk
} // namespace esphome
