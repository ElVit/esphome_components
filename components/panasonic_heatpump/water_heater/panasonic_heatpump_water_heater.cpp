#include "panasonic_heatpump_water_heater.h"
#include "esphome/core/log.h"

namespace esphome {
namespace panasonic_heatpump {
static const char* const TAG = "panasonic_heatpump.water_heater";

void PanasonicHeatpumpWaterHeater::dump_config() {
  LOG_WATER_HEATER("", "Panasonic Heatpump Water Heater", this);
}

water_heater::WaterHeaterTraits PanasonicHeatpumpWaterHeater::traits() {
  auto traits = water_heater::WaterHeaterTraits();

  traits.add_feature_flags(water_heater::WATER_HEATER_SUPPORTS_CURRENT_TEMPERATURE);
  traits.set_supported_modes({water_heater::WATER_HEATER_MODE_OFF, water_heater::WATER_HEATER_MODE_HEAT_PUMP});
  traits.set_min_temperature(this->min_temperature_);
  traits.set_max_temperature(this->max_temperature_);
  traits.set_target_temperature_step(this->temperature_step_);

  return traits;
}

void PanasonicHeatpumpWaterHeater::control(const water_heater::WaterHeaterCall& call) {
  if (call.get_mode().has_value()) {
    int byte6 = this->parent_->getResponseByte(6);
    if (byte6 >= 0) {
      water_heater::WaterHeaterMode new_mode = *call.get_mode();
      uint8_t newByte6 = this->setWaterHeaterMode(new_mode, (uint8_t)byte6);
      this->parent_->set_command_byte(newByte6, 6);
    }
  }

  float new_temp = call.get_target_temperature();
  switch (this->id_) {
  case WaterHeaterIds::CONF_HEATER_TANK:
    this->parent_->set_command_byte(PanasonicCommand::setPlus128(new_temp), 42);  // set11
    break;
  };

  this->publish_state();
  this->keep_state_ = 2;
}

void PanasonicHeatpumpWaterHeater::publish_new_state(const std::vector<uint8_t>& data) {
  if (this->keep_state_ > 0) {
    this->keep_state_--;
    return;
  }
  if (data.empty())
    return;

  uint8_t new_mode;
  float new_target_temp_heat;
  float new_current_temp;

  new_mode = this->getWaterHeaterMode(data[6]);  // set9
  switch (this->id_) {
  case WaterHeaterIds::CONF_HEATER_TANK:
    new_target_temp_heat = PanasonicDecode::getByteMinus128(data[42]);  // set11
    new_current_temp = PanasonicDecode::getByteMinus128(data[141]);     // top10
    break;
  default:
    return;
  };

  if (this->mode_ == new_mode && this->target_temperature_ == new_target_temp_heat &&
      this->current_temperature_ == new_current_temp)
    return;

  if (new_mode != 0xFF)
    this->mode_ = (water_heater::WaterHeaterMode)new_mode;
  this->target_temperature_ = new_target_temp_heat;
  this->current_temperature_ = new_current_temp;
  this->publish_state();
}

uint8_t PanasonicHeatpumpWaterHeater::getWaterHeaterMode(const uint8_t input) {
  switch (this->id_) {
  case WaterHeaterIds::CONF_HEATER_TANK:
    switch ((uint8_t)(input & 0b110000)) {
    case 0b010000:
      return water_heater::WATER_HEATER_MODE_OFF;
    case 0b100000:
      return water_heater::WATER_HEATER_MODE_HEAT_PUMP;
    default:
      return 0xFF;
    };
  default:
    return 0xFF;
  };
}

uint8_t PanasonicHeatpumpWaterHeater::setWaterHeaterMode(const water_heater::WaterHeaterMode mode, const uint8_t byte) {
  uint8_t newByte = byte;
  switch (this->id_) {
  case WaterHeaterIds::CONF_HEATER_TANK:
    newByte = newByte & 0b11001111;
    switch (mode) {
    case water_heater::WATER_HEATER_MODE_OFF:
      return newByte + 0b010000;
    case water_heater::WATER_HEATER_MODE_HEAT_PUMP:
      return newByte + 0b100000;
    default:
      return 0;
    };
  default:
    return 0;
  };
}

water_heater::WaterHeaterCallInternal PanasonicHeatpumpWaterHeater::make_call() {
  return water_heater::WaterHeaterCallInternal(this);
}
}  // namespace panasonic_heatpump
}  // namespace esphome
