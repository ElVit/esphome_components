import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
)
from .. import CONF_MAIDESITE_DESK_ID, MaidesiteDeskComponent, maidesite_desk_ns

ICON_STEP_UP = "mdi:arrow-up-drop-circle-outline"
ICON_STEP_DOWN = "mdi:arrow-down-drop-circle-outline"
ICON_STOP = "mdi:stop-circle-outline"
ICON_GOTO_MIN = "mdi:format-vertical-align-bottom"
ICON_GOTO_MAX = "mdi:format-vertical-align-top"
ICON_GOTO_M1 = "mdi:numeric-1-circle-outline"
ICON_GOTO_M2 = "mdi:numeric-2-circle-outline"
ICON_GOTO_M3 = "mdi:numeric-3-circle-outline"
ICON_GOTO_M4 = "mdi:numeric-4-circle-outline"
ICON_SAFE_M1 = "mdi:numeric-1-circle"
ICON_SAFE_M2 = "mdi:numeric-2-circle"
ICON_SAFE_M3 = "mdi:numeric-3-circle"
ICON_SAFE_M4 = "mdi:numeric-4-circle"

CONF_STEP_UP = "step_up"
CONF_STEP_DOWN = "step_down"
CONF_STOP = "stop"
CONF_GOTO_MAX = "goto_max"
CONF_GOTO_MIN = "goto_min"
CONF_GOTO_M1 = "goto_m1"
CONF_GOTO_M2 = "goto_m2"
CONF_GOTO_M3 = "goto_m3"
CONF_GOTO_M4 = "goto_m4"
CONF_SAVE_M1 = "save_m1"
CONF_SAVE_M2 = "save_m2"
CONF_SAVE_M3 = "save_m3"
CONF_SAVE_M4 = "save_m4"

TYPES = [
    CONF_STEP_UP,
    CONF_STEP_DOWN,
    CONF_STOP,
    CONF_GOTO_MAX,
    CONF_GOTO_MIN,
    CONF_GOTO_M1,
    CONF_GOTO_M2,
    CONF_GOTO_M3,
    CONF_GOTO_M4,
    CONF_SAVE_M1,
    CONF_SAVE_M2,
    CONF_SAVE_M3,
    CONF_SAVE_M4,
]

MaidesiteDeskButton = maidesite_desk_ns.class_(
    "MaidesiteDeskButton", button.Button, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_MAIDESITE_DESK_ID): cv.use_id(MaidesiteDeskComponent),
        cv.Optional(CONF_STEP_UP): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_STEP_UP,
        ),
        cv.Optional(CONF_STEP_DOWN): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_STEP_DOWN,
        ),
        cv.Optional(CONF_STOP): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_STOP,
        ),
        cv.Optional(CONF_GOTO_MAX): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_GOTO_MAX,
        ),
        cv.Optional(CONF_GOTO_MIN): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_GOTO_MIN,
        ),
        cv.Optional(CONF_GOTO_M1): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_GOTO_M1,
        ),
        cv.Optional(CONF_GOTO_M2): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_GOTO_M2,
        ),
        cv.Optional(CONF_GOTO_M3): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_GOTO_M3,
        ),
        cv.Optional(CONF_GOTO_M4): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_GOTO_M4,
        ),
        cv.Optional(CONF_SAVE_M1): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_SAFE_M1,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_M2): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_SAFE_M2,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_M3): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_SAFE_M3,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_M4): button.button_schema(
            MaidesiteDeskButton,
            icon=ICON_SAFE_M4,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_MAIDESITE_DESK_ID])
    for index, key in enumerate(TYPES):
        if child_config := config.get(key):
            var = await button.new_button(child_config)
            await cg.register_component(var, child_config)
            await cg.register_parented(var, config[CONF_MAIDESITE_DESK_ID])
            cg.add(getattr(hub, f"set_{key}_button")(var))
            cg.add(var.set_id(index))
