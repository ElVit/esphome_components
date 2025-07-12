import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
  UNIT_CENTIMETER,
  UNIT_PERCENT,
  CONF_MIN_VALUE,
  CONF_MAX_VALUE,
  CONF_STEP,
)
from .. import CONF_MAIDESITE_DESK_ID, MaidesiteDeskComponent, maidesite_desk_ns


CONF_HEIGHT_ABS = "height_abs"
CONF_HEIGHT_PCT = "height_pct"

TYPES = [
  CONF_HEIGHT_ABS,
  CONF_HEIGHT_PCT,
]

def number_options(min_val, max_val, step) -> cv.Schema:
  schema = cv.Schema({
    cv.Optional(CONF_MIN_VALUE, default=min_val): cv.float_,
    cv.Optional(CONF_MAX_VALUE, default=max_val): cv.float_,
    cv.Optional(CONF_STEP, default=step): cv.float_range(min=0.1, max=10.0),
  })
  return schema

MaidesiteDeskNumber = maidesite_desk_ns.class_("MaidesiteDeskNumber", number.Number, cg.Component)

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_MAIDESITE_DESK_ID): cv.use_id(
      MaidesiteDeskComponent
    ),

    cv.Optional(CONF_HEIGHT_ABS): number.number_schema(
      MaidesiteDeskNumber,
      unit_of_measurement=UNIT_CENTIMETER,
    ).extend(
      number_options(0.0, 120.0, 0.2)
    ),
    cv.Optional(CONF_HEIGHT_PCT): number.number_schema(
      MaidesiteDeskNumber,
      unit_of_measurement=UNIT_PERCENT,
    ).extend(
      number_options(0.0, 100.0, 0.1)
    ),
  }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
  parent = await cg.get_variable(config[CONF_MAIDESITE_DESK_ID])
  for index, key in enumerate(TYPES):
    if child_config := config.get(key):
      var = await number.new_number(
        child_config,
        min_value=child_config[CONF_MIN_VALUE],
        max_value=child_config[CONF_MAX_VALUE],
        step=child_config[CONF_STEP],
      )
      await cg.register_component(var, child_config)
      cg.add(var.set_parent(parent))
      cg.add(var.set_id(index))
      cg.add(parent.add_number(var))
