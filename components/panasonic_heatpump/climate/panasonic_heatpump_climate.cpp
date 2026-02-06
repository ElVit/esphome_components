#include "panasonic_heatpump_climate.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_heatpump {
static const char* const TAG = "panasonic_heatpump.climate";

void PanasonicHeatpumpClimate::dump_config() {
  LOG_CLIMATE("", "Panasonic Heatpump Climate", this);
}

climate::ClimateTraits PanasonicHeatpumpClimate::traits() {
  auto traits = climate::ClimateTraits();

  // traits.set_supports_action(true);
  traits.add_feature_flags(climate::CLIMATE_SUPPORTS_CURRENT_TEMPERATURE);
  traits.set_supported_modes({climate::CLIMATE_MODE_OFF, climate::CLIMATE_MODE_HEAT});
  if (this->cool_mode_ &&
      (this->id_ == ClimateIds::CONF_CLIMATE_ZONE1 || this->id_ == ClimateIds::CONF_CLIMATE_ZONE2)) {
    traits.add_feature_flags(climate::CLIMATE_REQUIRES_TWO_POINT_TARGET_TEMPERATURE);
    traits.add_supported_mode(climate::CLIMATE_MODE_COOL);
    traits.add_supported_mode(climate::CLIMATE_MODE_AUTO);
  }

  traits.set_visual_min_temperature(this->min_temperature_);
  traits.set_visual_max_temperature(this->max_temperature_);
  traits.set_visual_temperature_step(this->temperature_step_);

  return traits;
}

void PanasonicHeatpumpClimate::control(const climate::ClimateCall& call) {
  if (call.get_mode().has_value()) {
    int byte6 = this->parent_->getResponseByte(6);
    if (byte6 >= 0) {
      climate::ClimateMode new_mode = *call.get_mode();
      uint8_t newByte6 = this->setClimateMode(new_mode, (uint8_t)byte6);
      this->parent_->set_command_byte(newByte6, 6);
    }
  }

  if (call.get_target_temperature().has_value()) {
    float new_temp = *call.get_target_temperature();
    switch (this->id_) {
    case ClimateIds::CONF_CLIMATE_ZONE1:
      this->parent_->set_command_byte(PanasonicCommand::setPlus128(new_temp), 38);  // set5
      break;
    case ClimateIds::CONF_CLIMATE_ZONE2:
      this->parent_->set_command_byte(PanasonicCommand::setPlus128(new_temp), 40);  // set7
      break;
    };
  }

  if (call.get_target_temperature_high().has_value()) {
    float new_temp = *call.get_target_temperature_high();
    switch (this->id_) {
    case ClimateIds::CONF_CLIMATE_ZONE1:
      this->parent_->set_command_byte(PanasonicCommand::setPlus128(new_temp), 38);  // set5
      break;
    case ClimateIds::CONF_CLIMATE_ZONE2:
      this->parent_->set_command_byte(PanasonicCommand::setPlus128(new_temp), 40);  // set7
      break;
    };
  }

  if (call.get_target_temperature_low().has_value()) {
    float new_temp = *call.get_target_temperature_low();
    switch (this->id_) {
    case ClimateIds::CONF_CLIMATE_ZONE1:
      this->parent_->set_command_byte(PanasonicCommand::setPlus128(new_temp), 39);  // set6
      break;
    case ClimateIds::CONF_CLIMATE_ZONE2:
      this->parent_->set_command_byte(PanasonicCommand::setPlus128(new_temp), 41);  // set8
      break;
    };
  }

  this->publish_state();
  this->keep_state_ = 2;
}

void PanasonicHeatpumpClimate::publish_new_state(const std::vector<uint8_t>& data) {
  if (this->keep_state_ > 0) {
    this->keep_state_--;
    return;
  }
  if (data.empty())
    return;

  uint8_t new_mode;
  float new_target_temp_heat;
  float new_target_temp_cool;
  float new_current_temp;

  new_mode = this->getClimateMode(data[6]);  // set9
  switch (this->id_) {
  case ClimateIds::CONF_CLIMATE_ZONE1:
    new_target_temp_heat = PanasonicDecode::getByteMinus128(data[38]);  // set5
    new_target_temp_cool = PanasonicDecode::getByteMinus128(data[39]);  // set6
    new_current_temp = PanasonicDecode::getByteMinus128(data[139]);     // top56
    break;
  case ClimateIds::CONF_CLIMATE_ZONE2:
    new_target_temp_heat = PanasonicDecode::getByteMinus128(data[40]);  // set7
    new_target_temp_cool = PanasonicDecode::getByteMinus128(data[41]);  // set8
    new_current_temp = PanasonicDecode::getByteMinus128(data[140]);     // top57
    break;
  default:
    return;
  };

  if (!this->get_traits().has_feature_flags(climate::CLIMATE_REQUIRES_TWO_POINT_TARGET_TEMPERATURE) &&
      this->mode == new_mode && this->target_temperature == new_target_temp_heat &&
      this->current_temperature == new_current_temp)
    return;
  if (this->get_traits().has_feature_flags(climate::CLIMATE_REQUIRES_TWO_POINT_TARGET_TEMPERATURE) &&
      this->mode == new_mode && this->target_temperature_high == new_target_temp_heat &&
      this->target_temperature_low == new_target_temp_cool && this->current_temperature == new_current_temp)
    return;

  if (new_mode != 0xFF)
    this->mode = (climate::ClimateMode)new_mode;
  if (this->get_traits().has_feature_flags(climate::CLIMATE_REQUIRES_TWO_POINT_TARGET_TEMPERATURE)) {
    this->target_temperature_high = new_target_temp_heat;
    this->target_temperature_low = new_target_temp_cool;
  } else {
    this->target_temperature = new_target_temp_heat;
  }
  this->current_temperature = new_current_temp;
  this->publish_state();
}

uint8_t PanasonicHeatpumpClimate::getClimateMode(const uint8_t input) {
  switch (this->id_) {
  case ClimateIds::CONF_CLIMATE_ZONE1:
  case ClimateIds::CONF_CLIMATE_ZONE2:
    switch ((uint8_t)(input & 0b1111)) {
    case 0b0001:
      return climate::CLIMATE_MODE_OFF;
    case 0b0010:
      return climate::CLIMATE_MODE_HEAT;
    case 0b0011:
      return climate::CLIMATE_MODE_COOL;
    case 0b1000:
      return climate::CLIMATE_MODE_AUTO;
    case 0b1001:
      return climate::CLIMATE_MODE_AUTO;  // 0x19 = auto-heat
    case 0b1010:
      return climate::CLIMATE_MODE_AUTO;  // 0x1A = auto-cool
    default:
      return 0xFF;
    };
  default:
    return 0xFF;
  };
}

uint8_t PanasonicHeatpumpClimate::setClimateMode(const climate::ClimateMode mode, uint8_t byte) {
  uint8_t newByte = byte;
  switch (this->id_) {
  case ClimateIds::CONF_CLIMATE_ZONE1:
  case ClimateIds::CONF_CLIMATE_ZONE2:
    newByte = newByte & 0b11110000;
    switch (mode) {
    case climate::CLIMATE_MODE_OFF:
      return newByte + 0b0001;
    case climate::CLIMATE_MODE_HEAT:
      return newByte + 0b0010;
    case climate::CLIMATE_MODE_COOL:
      return newByte + 0b0011;
    case climate::CLIMATE_MODE_AUTO:
      return newByte + 0b1000;
    default:
      return 0;
    };
  default:
    return 0;
  };
}
}  // namespace panasonic_heatpump
}  // namespace esphome
