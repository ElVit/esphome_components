import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import switch
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
)
from .. import (
    CONF_PANASONIC_HEATPUMP_ID,
    PanasonicHeatpumpComponent,
    panasonic_heatpump_ns,
)

CONF_SET1 = "set1"  # Set Heatpump
CONF_SET10 = "set10"  # Set Force DHW
CONF_SET12 = "set12"  # Set Force Defrost
CONF_SET13 = "set13"  # Set Force Sterilization
CONF_SET14 = "set14"  # Set Pump
CONF_SET24 = "set24"  # Set Main Schedule
CONF_SET25 = "set25"  # Set Alt External Sensor
CONF_SET28 = "set28"  # Set Buffer
CONF_SET30 = "set30"  # Set External Control
CONF_SET31 = "set31"  # Set External Error
CONF_SET32 = "set32"  # Set External Compressor Control
CONF_SET33 = "set33"  # Set External Heat Cool Control
CONF_SET34 = "set34"  # Set Bivalent Control

TYPES = [
    CONF_SET1,
    CONF_SET10,
    CONF_SET12,
    CONF_SET13,
    CONF_SET14,
    CONF_SET24,
    CONF_SET25,
    CONF_SET28,
    CONF_SET30,
    CONF_SET31,
    CONF_SET32,
    CONF_SET33,
    CONF_SET34,
]

PanasonicHeatpumpSwitch = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpSwitch", switch.Switch, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
            PanasonicHeatpumpComponent
        ),
        cv.Optional(CONF_SET1): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET10): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET12): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET13): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET14): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET24): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET25): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET28): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET30): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET31): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET32): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET33): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
        cv.Optional(CONF_SET34): switch.switch_schema(
            PanasonicHeatpumpSwitch,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
    for index, key in enumerate(TYPES):
        if child_config := config.get(key):
            var = await switch.new_switch(child_config)
            await cg.register_component(var, child_config)
            cg.add(var.set_parent(parent))
            cg.add(var.set_id(index))
            cg.add(parent.add_switch(var))
