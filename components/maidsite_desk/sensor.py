import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
  UNIT_CENTIMETER,
  UNIT_PERCENT,
)
from . import MaidsiteDeskComponent, CONF_MAIDSITE_DESK_ID

CONF_UNIT = "unit"
CONF_HEIGHT_ABS = "height_abs"
CONF_HEIGHT_PCT = "height_pct"
CONF_HEIGHT_MIN = "height_min"
CONF_HEIGHT_MAX = "height_max"
CONF_POSITION_M1 = "position_m1"
CONF_POSITION_M2 = "position_m2"
CONF_POSITION_M3 = "position_m3"
CONF_POSITION_M4 = "position_m4"

TYPES = [
  CONF_UNIT,
  CONF_HEIGHT_ABS,
  CONF_HEIGHT_PCT,
  CONF_HEIGHT_MIN,
  CONF_HEIGHT_MAX,
  CONF_POSITION_M1,
  CONF_POSITION_M2,
  CONF_POSITION_M3,
  CONF_POSITION_M4,
]

CONFIG_SCHEMA = cv.All(
  cv.Schema(
    {
      cv.GenerateID(CONF_MAIDSITE_DESK_ID): cv.use_id(MaidsiteDeskComponent),

      cv.Optional(CONF_UNIT): sensor.sensor_schema(),
      cv.Optional(CONF_HEIGHT_ABS): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_CENTIMETER,
      ),
      cv.Optional(CONF_HEIGHT_PCT): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_PERCENT,
      ),
      cv.Optional(CONF_HEIGHT_MIN): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_CENTIMETER,
      ),
      cv.Optional(CONF_HEIGHT_MAX): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_CENTIMETER,
      ),
      cv.Optional(CONF_POSITION_M1): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_CENTIMETER,
      ),
      cv.Optional(CONF_POSITION_M2): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_CENTIMETER,
      ),
      cv.Optional(CONF_POSITION_M3): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_CENTIMETER,
      ),
      cv.Optional(CONF_POSITION_M4): sensor.sensor_schema(
        accuracy_decimals=2,
        unit_of_measurement=UNIT_CENTIMETER,
      ),
    }
  ).extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
  hub = await cg.get_variable(config[CONF_MAIDSITE_DESK_ID])
  for key in TYPES:
    await setup_conf(config, key, hub)

async def setup_conf(parent_config, key, hub):
  if child_config := parent_config.get(key):
    var = await sensor.new_sensor(child_config)
    cg.add(getattr(hub, f"set_{key}_sensor")(var))
