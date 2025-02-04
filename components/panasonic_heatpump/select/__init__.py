import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import (
  ENTITY_CATEGORY_CONFIG,
)
from .. import CONF_PANASONIC_HEATPUMP_ID, PanasonicHeatpumpComponent, panasonic_heatpump_ns


CONF_SET2 = "set2"  # Set Holiday Mode
CONF_SET3 = "set3"  # Set Quiet Mode
CONF_SET4 = "set4"  # Set Powerful Mode
CONF_SET9 = "set9"  # Set Operation Mode
CONF_SET17 = "set17"  # Set Zones
CONF_SET26 = "set26"  # Set External PadHeater
CONF_SET35 = "set35"  # Set Bivalent Mode

TYPES = [
  CONF_SET2,
  CONF_SET3,
  CONF_SET4,
  CONF_SET9,
  CONF_SET17,
  CONF_SET26,
  CONF_SET35,
]

CONF_SELECTS = [
  [ "Off", "Scheduled", "Active", ],
  [ "Off", "Level 1", "Level 2", "Level 3", ],
  [ "Off", "30min", "60min", "90min", ],
  [ "Heat only", "Cool only", "Auto", "Auto(heat)", "Auto(cool)", "DHW only", "Heat+DHW", "Cool+DHW", "Auto+DHW", "Auto(heat)+DHW", "Auto(cool)+DHW", ],
  [ "Zone 1", "Zone 2", "Zone 1 & 2", ],
  [ "Disabled", "Type-A", "Type-B" ],
  [ "Alternative", "Parallel", "Advanced Parallel" ],
]

PanasonicHeatpumpSelect = panasonic_heatpump_ns.class_("PanasonicHeatpumpSelect", select.Select)

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(PanasonicHeatpumpComponent),

    cv.Optional(CONF_SET2): select.select_schema(
      PanasonicHeatpumpSelect,
    ),
    cv.Optional(CONF_SET3): select.select_schema(
      PanasonicHeatpumpSelect,
    ),
    cv.Optional(CONF_SET4): select.select_schema(
      PanasonicHeatpumpSelect,
    ),
    cv.Optional(CONF_SET9): select.select_schema(
      PanasonicHeatpumpSelect,
    ),
    cv.Optional(CONF_SET17): select.select_schema(
      PanasonicHeatpumpSelect,
    ),
    cv.Optional(CONF_SET26): select.select_schema(
      PanasonicHeatpumpSelect,
    ),
    cv.Optional(CONF_SET35): select.select_schema(
      PanasonicHeatpumpSelect,
    ),
  }
)

async def to_code(config):
  hub = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
  for index, key in enumerate(TYPES):
    await setup_conf(config, key, index, hub)

async def setup_conf(parent_config, key, index, hub):
  if child_config := parent_config.get(key):
    var = await select.new_select(child_config, options=CONF_SELECTS[index])
    await cg.register_parented(var, parent_config[CONF_PANASONIC_HEATPUMP_ID])
    cg.add(getattr(hub, f"set_{key}_select")(var))
