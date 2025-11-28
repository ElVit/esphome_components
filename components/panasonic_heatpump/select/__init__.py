import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
)
from .. import (
    CONF_PANASONIC_HEATPUMP_ID,
    PanasonicHeatpumpComponent,
    panasonic_heatpump_ns,
)

CONF_COOL_MODE = "cool_mode"
CONF_SET2 = "set2"  # Set Holiday Mode
CONF_SET3 = "set3"  # Set Quiet Mode
CONF_SET4 = "set4"  # Set Powerful Mode
CONF_SET9 = "set9"  # Set Operation Mode
# ToDo: Split up set9 into set9_1 (Heating Mode) and set9_2 (DHW Mode)
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
    [
        "Off",
        "Scheduled",
        "Active",
    ],
    [
        "Off",
        "Level 1",
        "Level 2",
        "Level 3",
    ],
    [
        "Off",
        "30min",
        "60min",
        "90min",
    ],
    [
        "TANK",
        "HEAT",
        "HEAT+TANK",
    ],
    [
        "Zone 1",
        "Zone 2",
        "Zone 1 & 2",
    ],
    ["Disabled", "Type-A", "Type-B"],
    ["Alternative", "Parallel", "Advanced Parallel"],
]

CONF_SELECT_COOL_MODE = [
    "TANK",
    "HEAT",
    "HEAT+TANK",
    "COOL",
    "COOL+TANK",
    "AUTO",
    "AUTO+TANK",
]

PanasonicHeatpumpSelect = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpSelect", select.Select, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
            PanasonicHeatpumpComponent
        ),
        cv.Optional(CONF_COOL_MODE, default=False): cv.boolean,
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
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
    for index, key in enumerate(TYPES):
        if child_config := config.get(key):
            child_options = (
                CONF_SELECT_COOL_MODE
                if config[CONF_COOL_MODE] and key == CONF_SET9
                else CONF_SELECTS[index]
            )

            var = await select.new_select(child_config, options=child_options)
            await cg.register_component(var, child_config)
            cg.add(var.set_parent(parent))
            cg.add(var.set_id(index))
            cg.add(parent.add_select(var))
