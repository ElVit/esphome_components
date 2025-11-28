import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import number
from esphome.const import (
    UNIT_CELSIUS,
    UNIT_KELVIN,
    UNIT_MINUTE,
    ENTITY_CATEGORY_CONFIG,
    CONF_MIN_VALUE,
    CONF_MAX_VALUE,
    CONF_STEP,
)
from .. import (
    CONF_PANASONIC_HEATPUMP_ID,
    PanasonicHeatpumpComponent,
    panasonic_heatpump_ns,
)

CONF_SET5 = "set5"  # Set Z1 Heat Request Temperature
CONF_SET6 = "set6"  # Set Z1 Cool Request Temperature
CONF_SET7 = "set7"  # Set Z2 Heat Request Temperature
CONF_SET8 = "set8"  # Set Z2 Cool Request Temperature
CONF_SET11 = "set11"  # Set DHW Temp
CONF_SET15 = "set15"  # Set Max Pump Duty
CONF_SET16_01 = "set16_01"  # Set Zone1 Heat Target High
CONF_SET16_02 = "set16_02"  # Set Zone1 Heat Target Low
CONF_SET16_03 = "set16_03"  # Set Zone1 Heat Outside Low
CONF_SET16_04 = "set16_04"  # Set Zone1 Heat Outside High
CONF_SET16_05 = "set16_05"  # Set Zone2 Heat Target High
CONF_SET16_06 = "set16_06"  # Set Zone2 Heat Target Low
CONF_SET16_07 = "set16_07"  # Set Zone2 Heat Outside Low
CONF_SET16_08 = "set16_08"  # Set Zone2 Heat Outside High
CONF_SET16_09 = "set16_09"  # Set Zone1 Cool Target High
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
    CONF_SET16_01,
    CONF_SET16_02,
    CONF_SET16_03,
    CONF_SET16_04,
    CONF_SET16_05,
    CONF_SET16_06,
    CONF_SET16_07,
    CONF_SET16_08,
    CONF_SET16_09,
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


def number_options(min_val, max_val, step) -> cv.Schema:
    schema = cv.Schema(
        {
            cv.Optional(CONF_MIN_VALUE, default=min_val): cv.float_,
            cv.Optional(CONF_MAX_VALUE, default=max_val): cv.float_,
            cv.Optional(CONF_STEP, default=step): cv.float_range(min=1.0, max=10.0),
        }
    )
    return schema


PanasonicHeatpumpNumber = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpNumber", number.Number, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
            PanasonicHeatpumpComponent
        ),
        cv.Optional(CONF_SET5): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(-5.0, 5.0, 1.0)),
        cv.Optional(CONF_SET6): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(-5.0, 5.0, 1.0)),
        cv.Optional(CONF_SET7): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(-5.0, 5.0, 1.0)),
        cv.Optional(CONF_SET8): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(-5.0, 5.0, 1.0)),
        cv.Optional(CONF_SET11): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(40.0, 75.0, 1.0)),
        cv.Optional(CONF_SET15): number.number_schema(
            PanasonicHeatpumpNumber,
        ).extend(number_options(64.0, 254.0, 1.0)),
        cv.Optional(CONF_SET16_01): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(20.0, 60.0, 1.0)),
        cv.Optional(CONF_SET16_02): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(20.0, 60.0, 1.0)),
        cv.Optional(CONF_SET16_03): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(-20.0, 15.0, 1.0)),
        cv.Optional(CONF_SET16_04): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(-20.0, 15.0, 1.0)),
        cv.Optional(CONF_SET16_05): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(20.0, 60.0, 1.0)),
        cv.Optional(CONF_SET16_06): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(20.0, 60.0, 1.0)),
        cv.Optional(CONF_SET16_07): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(-20.0, 15.0, 1.0)),
        cv.Optional(CONF_SET16_08): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(-20.0, 15.0, 1.0)),
        cv.Optional(CONF_SET16_09): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(5.0, 20.0, 1.0)),
        cv.Optional(CONF_SET16_10): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(5.0, 20.0, 1.0)),
        cv.Optional(CONF_SET16_11): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(15.0, 30.0, 1.0)),
        cv.Optional(CONF_SET16_12): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(15.0, 30.0, 1.0)),
        cv.Optional(CONF_SET16_13): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(5.0, 20.0, 1.0)),
        cv.Optional(CONF_SET16_14): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(5.0, 20.0, 1.0)),
        cv.Optional(CONF_SET16_15): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(15.0, 30.0, 1.0)),
        cv.Optional(CONF_SET16_16): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ).extend(number_options(15.0, 30.0, 1.0)),
        cv.Optional(CONF_SET18): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_KELVIN,
        ).extend(number_options(1.0, 15.0, 1.0)),
        cv.Optional(CONF_SET19): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_KELVIN,
        ).extend(number_options(1.0, 15.0, 1.0)),
        cv.Optional(CONF_SET20): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_KELVIN,
        ).extend(number_options(112.0, -2.0, 1.0)),
        cv.Optional(CONF_SET21): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_MINUTE,
        ).extend(number_options(0.0, 250.0, 1.0)),
        cv.Optional(CONF_SET22): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_KELVIN,
        ).extend(number_options(1.0, 15.0, 1.0)),
        cv.Optional(CONF_SET23): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_KELVIN,
        ).extend(number_options(1.0, 15.0, 1.0)),
        cv.Optional(CONF_SET27): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_KELVIN,
        ).extend(number_options(0.0, 10.0, 1.0)),
        cv.Optional(CONF_SET29): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(5.0, 35.0, 1.0)),
        cv.Optional(CONF_SET36): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(-15.0, 35.0, 1.0)),
        cv.Optional(CONF_SET37): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(-10.0, 0.0, 1.0)),
        cv.Optional(CONF_SET38): number.number_schema(
            PanasonicHeatpumpNumber,
            unit_of_measurement=UNIT_CELSIUS,
        ).extend(number_options(-10.0, 0.0, 1.0)),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
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
