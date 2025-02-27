import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
  UNIT_CENTIMETER,
  UNIT_PERCENT,
)
from .. import CONF_MAIDSITE_DESK_ID, MaidsiteDeskComponent, maidsite_desk_ns


CONF_HEIGHT_ABS = "height_abs"
CONF_HEIGHT_PCT = "height_pct"

TYPES = [
  CONF_HEIGHT_ABS,
  CONF_HEIGHT_PCT,
]

CONF_NUMBERS = [
  [ 0, 120, .2, ],
  [ 0, 100, .1, ],
]

MaidsiteDeskNumber = maidsite_desk_ns.class_("MaidsiteDeskNumber", number.Number, cg.Component)

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_MAIDSITE_DESK_ID): cv.use_id(MaidsiteDeskComponent),

    cv.Optional(CONF_HEIGHT_ABS): number.number_schema(
      MaidsiteDeskNumber,
      unit_of_measurement=UNIT_CENTIMETER,
    ),
    cv.Optional(CONF_HEIGHT_PCT): number.number_schema(
      MaidsiteDeskNumber,
      unit_of_measurement=UNIT_PERCENT,
    ),
  }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
  hub = await cg.get_variable(config[CONF_MAIDSITE_DESK_ID])
  for index, key in enumerate(TYPES):
    if child_config := config.get(key):
      var = await number.new_number(
        child_config,
        min_value=CONF_NUMBERS[index][0],
        max_value=CONF_NUMBERS[index][1],
        step=CONF_NUMBERS[index][2]
      )
      await cg.register_component(var, child_config)
      await cg.register_parented(var, config[CONF_MAIDSITE_DESK_ID])
      cg.add(getattr(hub, f"set_{key}_number")(var))
      cg.add(var.set_id(index))
