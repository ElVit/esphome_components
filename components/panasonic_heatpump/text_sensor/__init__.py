import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import text_sensor
from esphome.const import (
  DEVICE_CLASS_HEAT,
)
from .. import CONF_PANASONIC_HEATPUMP_ID, PanasonicHeatpumpComponent, panasonic_heatpump_ns


ICON_OPERATION_MODE = "mdi:thermostat"
ICON_POWERFUL_MODE = "mdi:arm-flex"
ICON_QUIET_MODE = "mdi:sleep"
ICON_HOLIDAY_MODE = "mdi:palm-tree"
ICON_ERROR = "mdi:close-circle"
ICON_BLOCKED = "mdi:cancel"
ICON_HEATING_MODE = "mdi:fire"
ICON_COOLING_MODE = "mdi:snowflake"
ICON_ZONE = "mdi:texture-box"
ICON_MODEL = "mdi:heat-pump"
ICON_SOLAR_MODE = "mdi:solar-power-variant"
ICON_LIQUID = "mdi:water"
ICON_VALVE = "mdi:pipe-valve"
ICON_PUMP = "mdi:pump"
ICON_EXTERNAL_PAD_HEATER = "mdi:radiator"


CONF_TOP4 = "top4"  # Operation Mode
# ToDo: Split up top4 into top4_1 (Heating Mode State) and top4_2 (DHW Mode State)
CONF_TOP17 = "top17"  # Powerful Mode Time
CONF_TOP18 = "top18"  # Quiet Mode Level
CONF_TOP19 = "top19"  # Holiday Mode State
CONF_TOP20 = "top20"  # ThreeWay Valve State
CONF_TOP44 = "top44"  # Error
CONF_TOP58 = "top58"  # DHW Heater State
CONF_TOP59 = "top59"  # Room Heater State
CONF_TOP76 = "top76"  # Heating Mode
CONF_TOP81 = "top81"  # Cooling Mode
CONF_TOP92 = "top92"  # Heat Pump Model
CONF_TOP94 = "top94"  # Zones State
CONF_TOP101 = "top101"  # Solar Mode
CONF_TOP106 = "top106"  # Pump Flowrate Mode
CONF_TOP107 = "top107"  # Liquid Type
CONF_TOP111 = "top111"  # Z1 Sensor Settings
CONF_TOP112 = "top112"  # Z2 Sensor Settings
CONF_TOP114 = "top114"  # External Pad Heater
CONF_TOP125 = "top125"  # TwoWay Valve State
CONF_TOP126 = "top126"  # ThreeWay Valve State2
CONF_TOP130 = "top130"  # Bivalent Mode

TYPES = [
  CONF_TOP4,
  CONF_TOP17,
  CONF_TOP18,
  CONF_TOP19,
  CONF_TOP20,
  CONF_TOP44,
  CONF_TOP58,
  CONF_TOP59,
  CONF_TOP76,
  CONF_TOP81,
  CONF_TOP92,
  CONF_TOP94,
  CONF_TOP101,
  CONF_TOP106,
  CONF_TOP107,
  CONF_TOP111,
  CONF_TOP112,
  CONF_TOP114,
  CONF_TOP125,
  CONF_TOP126,
  CONF_TOP130,
]

PanasonicHeatpumpTextSensor = panasonic_heatpump_ns.class_("PanasonicHeatpumpTextSensor", text_sensor.TextSensor, cg.Component)

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
      PanasonicHeatpumpComponent
    ),

    cv.Optional(CONF_TOP4): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_OPERATION_MODE,
    ),
    cv.Optional(CONF_TOP17): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_POWERFUL_MODE,
    ),
    cv.Optional(CONF_TOP18): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_QUIET_MODE,
    ),
    cv.Optional(CONF_TOP19): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_HOLIDAY_MODE,
    ),
    cv.Optional(CONF_TOP20): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_VALVE,
    ),
    cv.Optional(CONF_TOP44): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_ERROR,
    ),
    cv.Optional(CONF_TOP58): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_BLOCKED,
    ),
    cv.Optional(CONF_TOP59): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_BLOCKED,
    ),
    cv.Optional(CONF_TOP76): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_HEATING_MODE,
    ),
    cv.Optional(CONF_TOP81): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_COOLING_MODE,
    ),
    cv.Optional(CONF_TOP92): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_MODEL,
    ),
    cv.Optional(CONF_TOP94): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_ZONE,
    ),
    cv.Optional(CONF_TOP101): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_SOLAR_MODE,
    ),
    cv.Optional(CONF_TOP106): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_PUMP,
    ),
    cv.Optional(CONF_TOP107): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_LIQUID,
    ),
    cv.Optional(CONF_TOP111): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
    ),
    cv.Optional(CONF_TOP112): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
    ),
    cv.Optional(CONF_TOP114): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_EXTERNAL_PAD_HEATER,
    ),
    cv.Optional(CONF_TOP125): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_VALVE,
    ),
    cv.Optional(CONF_TOP126): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,
      icon=ICON_VALVE,
    ),
    cv.Optional(CONF_TOP130): text_sensor.text_sensor_schema(
      PanasonicHeatpumpTextSensor,),
  }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
  parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
  for index, key in enumerate(TYPES):
    if child_config := config.get(key):
      var = await text_sensor.new_text_sensor(child_config)
      await cg.register_component(var, child_config)
      cg.add(var.set_parent(parent))
      cg.add(var.set_id(index))
      cg.add(parent.add_text_sensor(var))
