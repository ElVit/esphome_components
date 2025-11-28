#include "panasonic_heatpump_sensor.h"

namespace esphome {
namespace panasonic_heatpump {
static const char* const TAG = "panasonic_heatpump.sensor";

void PanasonicHeatpumpSensor::dump_config() {
  LOG_SENSOR("", "Panasonic Heatpump Sensor", this);
  delay(10);
}

void PanasonicHeatpumpSensor::publish_new_state(const std::vector<uint8_t>& data) {
  if (data.empty())
    return;

  float new_state;
  switch (this->id_) {
  case SensorIds::CONF_TOP1:
    new_state = PanasonicDecode::getPumpFlow(data, 169);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP5:
    new_state = PanasonicDecode::getByteMinus128(data[143]) + PanasonicDecode::getFractional(data[118], 0);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP6:
    new_state = PanasonicDecode::getByteMinus128(data[144]) + PanasonicDecode::getFractional(data[118], 3);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP7:
    new_state = PanasonicDecode::getByteMinus128(data[153]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP8:
    new_state = PanasonicDecode::getByteMinus1(data[166]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP9:
    new_state = PanasonicDecode::getByteMinus128(data[42]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP10:
    new_state = PanasonicDecode::getByteMinus128(data[141]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP11:
    new_state = PanasonicDecode::getWordMinus1(data, 182);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP12:
    new_state = PanasonicDecode::getWordMinus1(data, 179);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP14:
    new_state = PanasonicDecode::getByteMinus128(data[142]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP15:
    new_state = PanasonicDecode::getByteMinus1Times200(data[194]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP16:
    new_state = PanasonicDecode::getByteMinus1Times200(data[193]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP21:
    new_state = PanasonicDecode::getByteMinus128(data[158]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP22:
    new_state = PanasonicDecode::getByteMinus128(data[99]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP23:
    new_state = PanasonicDecode::getByteMinus128(data[84]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP24:
    new_state = PanasonicDecode::getByteMinus128(data[94]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP25:
    new_state = PanasonicDecode::getByteMinus128(data[44]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP27:
    new_state = PanasonicDecode::getByteMinus128(data[38]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP28:
    new_state = PanasonicDecode::getByteMinus128(data[39]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP29:
    new_state = PanasonicDecode::getByteMinus128(data[75]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP30:
    new_state = PanasonicDecode::getByteMinus128(data[76]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP31:
    new_state = PanasonicDecode::getByteMinus128(data[78]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP32:
    new_state = PanasonicDecode::getByteMinus128(data[77]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP33:
    new_state = PanasonicDecode::getByteMinus128(data[156]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP34:
    new_state = PanasonicDecode::getByteMinus128(data[40]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP35:
    new_state = PanasonicDecode::getByteMinus128(data[41]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP36:
    new_state = PanasonicDecode::getByteMinus128(data[145]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP37:
    new_state = PanasonicDecode::getByteMinus128(data[146]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP38:
    new_state = PanasonicDecode::getByteMinus1Times200(data[196]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP39:
    new_state = PanasonicDecode::getByteMinus1Times200(data[195]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP40:
    new_state = PanasonicDecode::getByteMinus1Times200(data[198]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP41:
    new_state = PanasonicDecode::getByteMinus1Times200(data[197]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP42:
    new_state = PanasonicDecode::getByteMinus128(data[147]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP43:
    new_state = PanasonicDecode::getByteMinus128(data[148]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP45:
    new_state = PanasonicDecode::getByteMinus128(data[43]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP46:
    new_state = PanasonicDecode::getByteMinus128(data[149]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP47:
    new_state = PanasonicDecode::getByteMinus128(data[150]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP48:
    new_state = PanasonicDecode::getByteMinus128(data[151]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP49:
    new_state = PanasonicDecode::getByteMinus128(data[154]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP50:
    new_state = PanasonicDecode::getByteMinus128(data[155]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP51:
    new_state = PanasonicDecode::getByteMinus128(data[157]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP52:
    new_state = PanasonicDecode::getByteMinus128(data[159]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP53:
    new_state = PanasonicDecode::getByteMinus128(data[160]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP54:
    new_state = PanasonicDecode::getByteMinus128(data[161]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP55:
    new_state = PanasonicDecode::getByteMinus128(data[162]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP56:
    new_state = PanasonicDecode::getByteMinus128(data[139]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP57:
    new_state = PanasonicDecode::getByteMinus128(data[140]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP62:
    new_state = PanasonicDecode::getByteMinus1Times10(data[173]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP63:
    new_state = PanasonicDecode::getByteMinus1Times10(data[174]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP64:
    new_state = PanasonicDecode::getByteMinus1Div5(data[163]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP65:
    new_state = PanasonicDecode::getByteMinus1Times50(data[171]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP66:
    new_state = PanasonicDecode::getByteMinus1Times50(data[164]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP67:
    new_state = PanasonicDecode::getByteMinus1Div5(data[165]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP70:
    new_state = PanasonicDecode::getByteMinus128(data[100]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP71:
    new_state = PanasonicDecode::getByteMinus1(data[101]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP72:
    new_state = PanasonicDecode::getByteMinus128(data[86]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP73:
    new_state = PanasonicDecode::getByteMinus128(data[87]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP74:
    new_state = PanasonicDecode::getByteMinus128(data[89]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP75:
    new_state = PanasonicDecode::getByteMinus128(data[88]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP77:
    new_state = PanasonicDecode::getByteMinus128(data[83]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP78:
    new_state = PanasonicDecode::getByteMinus128(data[85]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP79:
    new_state = PanasonicDecode::getByteMinus128(data[95]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP80:
    new_state = PanasonicDecode::getByteMinus128(data[96]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP82:
    new_state = PanasonicDecode::getByteMinus128(data[79]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP83:
    new_state = PanasonicDecode::getByteMinus128(data[80]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP84:
    new_state = PanasonicDecode::getByteMinus128(data[82]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP85:
    new_state = PanasonicDecode::getByteMinus128(data[81]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP86:
    new_state = PanasonicDecode::getByteMinus128(data[90]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP87:
    new_state = PanasonicDecode::getByteMinus128(data[91]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP88:
    new_state = PanasonicDecode::getByteMinus128(data[93]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP89:
    new_state = PanasonicDecode::getByteMinus128(data[92]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP90:
    new_state = PanasonicDecode::getWordMinus1(data, 185);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP91:
    new_state = PanasonicDecode::getWordMinus1(data, 188);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP93:
    new_state = PanasonicDecode::getByteMinus1(data[172]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP95:
    new_state = PanasonicDecode::getByteMinus1(data[45]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP96:
    new_state = PanasonicDecode::getByteMinus1(data[104]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP97:
    new_state = PanasonicDecode::getByteMinus128(data[105]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP98:
    new_state = PanasonicDecode::getByteMinus128(data[106]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP102:
    new_state = PanasonicDecode::getByteMinus128(data[61]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP103:
    new_state = PanasonicDecode::getByteMinus128(data[62]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP104:
    new_state = PanasonicDecode::getByteMinus128(data[63]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP105:
    new_state = PanasonicDecode::getByteMinus128(data[64]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP113:
    new_state = PanasonicDecode::getByteMinus128(data[59]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP115:
    new_state = PanasonicDecode::getByteMinus1Div50(data[125]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP116:
    new_state = PanasonicDecode::getByteMinus128(data[126]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP117:
    new_state = PanasonicDecode::getByteMinus128(data[127]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP118:
    new_state = PanasonicDecode::getByteMinus128(data[128]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP127:
    new_state = PanasonicDecode::getByteMinus1Div2(data[177]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP128:
    new_state = PanasonicDecode::getByteMinus1Div2(data[178]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP131:
    new_state = PanasonicDecode::getByteMinus128(data[65]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP134:
    new_state = PanasonicDecode::getByteMinus128(data[66]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP135:
    new_state = PanasonicDecode::getByteMinus128(data[68]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP136:
    new_state = PanasonicDecode::getByteMinus1(data[67]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP137:
    new_state = PanasonicDecode::getByteMinus1(data[69]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_TOP138:
    new_state = PanasonicDecode::getByteMinus1(data[70]);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;

  case SensorIds::CONF_XTOP0:
    new_state = PanasonicDecode::getWordMinus1(data, 14);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_XTOP1:
    new_state = PanasonicDecode::getWordMinus1(data, 16);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_XTOP2:
    new_state = PanasonicDecode::getWordMinus1(data, 18);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_XTOP3:
    new_state = PanasonicDecode::getWordMinus1(data, 20);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_XTOP4:
    new_state = PanasonicDecode::getWordMinus1(data, 22);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  case SensorIds::CONF_XTOP5:
    new_state = PanasonicDecode::getWordMinus1(data, 24);
    if (this->has_state() && this->get_state() == new_state)
      return;
    break;
  default:
    return;
  };

  this->publish_state(new_state);
}
}  // namespace panasonic_heatpump
}  // namespace esphome
