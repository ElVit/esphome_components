#include "panasonic_heatpump_binary_sensor.h"

namespace esphome {
namespace panasonic_heatpump {
static const char* const TAG = "panasonic_heatpump.binary_sensor";

void PanasonicHeatpumpBinarySensor::dump_config() {
  LOG_BINARY_SENSOR("", "Panasonic Heatpump Binary Sensor", this);
  delay(10);
}

void PanasonicHeatpumpBinarySensor::publish_new_state(const std::vector<uint8_t>& data) {
  if (data.empty())
    return;

  bool new_state;
  switch (this->id_) {
  case BinarySensorIds::CONF_TOP0:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[4]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP2:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[4]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP3:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[7]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP13:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[5]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP26:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[111]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP60:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[112]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP61:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[112]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP68:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[5]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP69:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[117]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP99:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[24]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP100:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[24]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP108:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[20]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP109:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[20]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP110:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[20]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP119:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[23]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP120:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit5and6(data[23]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP121:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[23]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP122:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[23]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP123:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[116]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP124:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[116]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP129:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit7and8(data[26]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP132:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit3and4(data[26]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  case BinarySensorIds::CONF_TOP133:
    new_state = PanasonicDecode::getBinaryState(PanasonicDecode::getBit1and2(data[26]));
    if (this->has_state() && this->state == new_state)
      return;
    break;
  default:
    return;
  };

  this->publish_state(new_state);
}
}  // namespace panasonic_heatpump
}  // namespace esphome
