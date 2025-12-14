#include "panasonic_heatpump_switch.h"

namespace esphome {
namespace panasonic_heatpump {
static const char* const TAG = "panasonic_heatpump.switch";

void PanasonicHeatpumpSwitch::dump_config() {
  LOG_SWITCH("", "Panasonic Heatpump Switch", this);
  delay(10);
}

void PanasonicHeatpumpSwitch::write_state(bool state) {
  size_t value = state ? 1 : 0;

  switch (this->id_) {
  case SwitchIds::CONF_SET1:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1(value), 4);
    break;
  case SwitchIds::CONF_SET10:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply64(value), 4);
    break;
  case SwitchIds::CONF_SET12:
    this->parent_->set_command_byte(PanasonicCommand::setMultiply2(value), 8);
    break;
  case SwitchIds::CONF_SET13:
    this->parent_->set_command_byte(PanasonicCommand::setMultiply4(value), 8);
    break;
  case SwitchIds::CONF_SET14:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 4);
    break;
  case SwitchIds::CONF_SET24:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply64(value), 5);
    break;
  case SwitchIds::CONF_SET25:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 20);
    break;
  case SwitchIds::CONF_SET28:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply4(value), 24);
    break;
  case SwitchIds::CONF_SET30:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1(value), 23);
    break;
  case SwitchIds::CONF_SET31:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply16(value), 23);
    break;
  case SwitchIds::CONF_SET32:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply64(value), 23);
    break;
  case SwitchIds::CONF_SET33:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1Multiply4(value), 23);
    break;
  case SwitchIds::CONF_SET34:
    this->parent_->set_command_byte(PanasonicCommand::setPlus1(value), 26);
    break;
  default:
    return;
  };

  this->publish_state(state);
  delay(10);  // NOLINT
  this->keep_state_ = 2;
}

void PanasonicHeatpumpSwitch::publish_new_state(const std::vector<uint8_t>& data) {
  if (this->keep_state_ > 0) {
    this->keep_state_--;
    return;
  }
  if (data.empty())
    return;

  bool new_state;
  switch (this->id_) {
  case SwitchIds::CONF_SET1:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[4]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET10:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[4]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET24:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[5]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET12:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[111]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET13:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[117]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET28:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[24]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET25:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[20]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET30:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[23]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET33:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[23]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET31:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[23]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET32:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[23]));
    if (this->state == new_state)
      return;
    break;
  case SwitchIds::CONF_SET34:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[26]));
    if (this->state == new_state)
      return;
    break;
  default:
    return;
  };

  this->publish_state(new_state);
  delay(10);  // NOLINT
}
}  // namespace panasonic_heatpump
}  // namespace esphome
