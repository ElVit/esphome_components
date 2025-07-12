import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
  STATE_CLASS_MEASUREMENT,
  UNIT_CENTIMETER,
  UNIT_PERCENT,
)
from .. import CONF_MAIDESITE_DESK_ID, MaidesiteDeskComponent, maidesite_desk_ns

CONF_HEIGHT_ABS = "height_abs"
CONF_HEIGHT_PCT = "height_pct"
CONF_HEIGHT_PHYS_MIN = "height_phys_min"
CONF_HEIGHT_PHYS_MAX = "height_phys_max"
CONF_HEIGHT_LIMIT_MIN = "height_limit_min"
CONF_HEIGHT_LIMIT_MAX = "height_limit_max"
CONF_POSITION_M1 = "position_m1"
CONF_POSITION_M2 = "position_m2"
CONF_POSITION_M3 = "position_m3"
CONF_POSITION_M4 = "position_m4"
CONF_UNIT = "unit"

TYPES = [
  CONF_HEIGHT_ABS,
  CONF_HEIGHT_PCT,
  CONF_HEIGHT_PHYS_MIN,
  CONF_HEIGHT_PHYS_MAX,
  CONF_HEIGHT_LIMIT_MIN,
  CONF_HEIGHT_LIMIT_MAX,
  CONF_POSITION_M1,
  CONF_POSITION_M2,
  CONF_POSITION_M3,
  CONF_POSITION_M4,
  CONF_UNIT,
]

MaidesiteDeskSensor = maidesite_desk_ns.class_("MaidesiteDeskSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_MAIDESITE_DESK_ID): cv.use_id(
      MaidesiteDeskComponent
    ),

    cv.Optional(CONF_HEIGHT_ABS): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_HEIGHT_PCT): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_PERCENT,
    ),
    cv.Optional(CONF_HEIGHT_PHYS_MIN): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_HEIGHT_PHYS_MAX): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_HEIGHT_LIMIT_MIN): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_HEIGHT_LIMIT_MAX): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_POSITION_M1): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_POSITION_M2): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_POSITION_M3): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_POSITION_M4): sensor.sensor_schema(
      MaidesiteDeskSensor,
      accuracy_decimals=2,
      state_class=STATE_CLASS_MEASUREMENT,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
  }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
  parent = await cg.get_variable(config[CONF_MAIDESITE_DESK_ID])
  for index, key in enumerate(TYPES):
    if child_config := config.get(key):
      var = await sensor.new_sensor(child_config)
      await cg.register_component(var, child_config)
      cg.add(var.set_parent(parent))
      cg.add(var.set_id(index))
      cg.add(parent.add_sensor(var))
