import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
  UNIT_CELSIUS,
  UNIT_KELVIN,
  UNIT_MINUTE,
  ENTITY_CATEGORY_CONFIG,
)
from .. import CONF_PANASONIC_HEATPUMP_ID, PanasonicHeatpumpComponent, panasonic_heatpump_ns


CONF_SET5 = "set5"  # Set Z1 Heat Request Temperature
CONF_SET6 = "set6"  # Set Z1 Cool Request Temperature
CONF_SET7 = "set7"  # Set Z2 Heat Request Temperature
CONF_SET8 = "set8"  # Set Z2 Cool Request Temperature
CONF_SET11 = "set11"  # Set DHW Temp
CONF_SET15 = "set15"  # Set Max Pump Duty
CONF_SET16_1 = "set16_1"  # Set Zone1 Heat Target High
CONF_SET16_2 = "set16_2"  # Set Zone1 Heat Target Low
CONF_SET16_3 = "set16_3"  # Set Zone1 Heat Outside Low
CONF_SET16_4 = "set16_4"  # Set Zone1 Heat Outside High
CONF_SET16_5 = "set16_5"  # Set Zone2 Heat Target High
CONF_SET16_6 = "set16_6"  # Set Zone2 Heat Target Low
CONF_SET16_7 = "set16_7"  # Set Zone2 Heat Outside Low
CONF_SET16_8 = "set16_8"  # Set Zone2 Heat Outside High
CONF_SET16_9 = "set16_9"  # Set Zone1 Cool Target High
CONF_SET16_10 = "set16_10"  # Set Zone1 Cool Target Low
CONF_SET16_11 = "set16_11"  # Set Zone1 Cool Outside Low
CONF_SET16_12 = "set16_12"  # Set Zone1 Cool Outside High
CONF_SET16_13 = "set16_13"  # Set Zone2 Cool Target High
CONF_SET16_14 = "set16_14"  # Set Zone2 Cool Target Low
CONF_SET16_15 = "set16_15"  # Set Zone2 Cool Outside Low
CONF_SET16_16 = "set16_16"  # Set Zone2 Cool Outside High
CONF_SET18 = "set18"  # Set Floor Heat Delta
CONF_SET19 = "set19"  # Set Floor Cool Delta
CONF_SET20 = "set20"  # Set DHW Heat Delta
CONF_SET21 = "set21"  # Set Heater Delay Time
CONF_SET22 = "set22"  # Set Heater Start Delta
CONF_SET23 = "set23"  # Set Heater Stop Delta
CONF_SET27 = "set27"  # Set Buffer Delta
CONF_SET29 = "set29"  # Set Heating Off Outdoor Temp
CONF_SET36 = "set36"  # Set Bivalent Start Temp
CONF_SET37 = "set37"  # Set Bivalent AP Start Temp
CONF_SET38 = "set38"  # Set Bivalent AP Stop Temp

TYPES = [
  CONF_SET5,
  CONF_SET6,
  CONF_SET7,
  CONF_SET8,
  CONF_SET11,
  CONF_SET15,
  CONF_SET16_1,
  CONF_SET16_2,
  CONF_SET16_3,
  CONF_SET16_4,
  CONF_SET16_5,
  CONF_SET16_6,
  CONF_SET16_7,
  CONF_SET16_8,
  CONF_SET16_9,
  CONF_SET16_10,
  CONF_SET16_11,
  CONF_SET16_12,
  CONF_SET16_13,
  CONF_SET16_14,
  CONF_SET16_15,
  CONF_SET16_16,
  CONF_SET18,
  CONF_SET19,
  CONF_SET20,
  CONF_SET21,
  CONF_SET22,
  CONF_SET23,
  CONF_SET27,
  CONF_SET29,
  CONF_SET36,
  CONF_SET37,
  CONF_SET38,
]

# min_value, max_value, step
CONF_NUMBERS = [
  [ -5, 5, 1, ],
  [ -5, 5, 1, ],
  [ -5, 5, 1, ],
  [ -5, 5, 1, ],
  [ 40, 75, 1, ],
  [ 64, 254, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ 1, 15, 1, ],
  [ 1, 15, 1, ],
  [ -12, -2, 1, ],
  [ 0, 1000, 1, ],
  [ 1, 15, 1, ],
  [ 1, 15, 1, ],
  [ 0, 10, 1, ],
  [ 5, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
  [ -15, 35, 1, ],
]

PanasonicHeatpumpNumber = panasonic_heatpump_ns.class_("PanasonicHeatpumpNumber", number.Number)

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(PanasonicHeatpumpComponent),

    cv.Optional(CONF_SET5): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET6): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET7): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET8): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET11): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET15): number.number_schema(
      PanasonicHeatpumpNumber,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_1): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_2): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_3): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_4): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_5): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_6): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_7): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_8): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_9): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_10): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_11): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_12): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_13): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_14): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_15): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET16_16): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET18): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_KELVIN,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET19): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_KELVIN,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET20): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_KELVIN,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET21): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_MINUTE,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET22): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_KELVIN,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET23): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_KELVIN,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET27): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_KELVIN,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET29): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET36): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET37): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
    cv.Optional(CONF_SET38): number.number_schema(
      PanasonicHeatpumpNumber,
      unit_of_measurement=UNIT_CELSIUS,
      entity_category=ENTITY_CATEGORY_CONFIG,
    ),
  }
)

async def to_code(config):
  hub = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
  for key in TYPES:
    await setup_conf(config, key, hub)

async def setup_conf(parent_config, key, hub):
  if child_config := parent_config.get(key):
    index = TYPES.index(key)
    var = await number.new_number(child_config, min_value=CONF_NUMBERS[index][0], max_value=CONF_NUMBERS[index][1], step=CONF_NUMBERS[index][2])
    await cg.register_parented(var, parent_config[CONF_PANASONIC_HEATPUMP_ID])
    cg.add(getattr(hub, f"set_{key}_number")(var))
