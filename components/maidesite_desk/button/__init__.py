import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import button
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
)
from .. import CONF_MAIDESITE_DESK_ID, MaidesiteDeskComponent, maidesite_desk_ns

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
        ),
        cv.Optional(CONF_STEP_DOWN): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_STOP): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_GOTO_MAX): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_GOTO_MIN): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_GOTO_M1): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_GOTO_M2): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_GOTO_M3): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_GOTO_M4): button.button_schema(
            MaidesiteDeskButton,
        ),
        cv.Optional(CONF_SAVE_M1): button.button_schema(
            MaidesiteDeskButton,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_M2): button.button_schema(
            MaidesiteDeskButton,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_M3): button.button_schema(
            MaidesiteDeskButton,
            entity_category=ENTITY_CATEGORY_CONFIG,
        ),
        cv.Optional(CONF_SAVE_M4): button.button_schema(
            MaidesiteDeskButton,
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
