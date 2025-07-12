#pragma once
#include "esphome/components/number/number.h"
#include "esphome/core/component.h"
#include "../maidesite_desk.h"


namespace esphome
{
  namespace maidesite_desk
  {
    enum NumberIds : uint8_t
    {
      CONF_HEIGHT_ABS,
      CONF_HEIGHT_PCT,
    };

    class MaidesiteDeskNumber : public number::Number, public Component,
          public Parented<MaidesiteDeskComponent>, public MaidesiteDeskEntity
    {
    public:
      MaidesiteDeskNumber() = default;
      void dump_config() override;
      void publish_new_state(const std::vector<uint8_t>& data) override;

    protected:
      void control(float value) override;
    };
  } // namespace maidesite_desk
} // namespace esphome
