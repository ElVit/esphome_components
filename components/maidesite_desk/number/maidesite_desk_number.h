#pragma once
#include "esphome/components/number/number.h"
#include "esphome/core/component.h"
#include "../maidesite_desk.h"

namespace esphome {
namespace maidesite_desk {
class MaidesiteDeskNumber : public number::Number, public Component, public Parented<MaidesiteDeskComponent> {
 public:
  MaidesiteDeskNumber() = default;
  void dump_config() override;
  void set_id(int id) {
    this->id_ = id;
  }

 protected:
  void control(float value) override;
  int id_;
};
}  // namespace maidesite_desk
}  // namespace esphome
