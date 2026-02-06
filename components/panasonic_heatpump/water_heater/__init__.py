import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import water_heater
from esphome.const import (
    CONF_MIN_TEMPERATURE,
    CONF_MAX_TEMPERATURE,
)
from .. import (
    CONF_PANASONIC_HEATPUMP_ID,
    PanasonicHeatpumpComponent,
    panasonic_heatpump_ns,
)

CONF_TARGET_TEMPERATURE_STEP = "target_temperature_step"

CONF_TANK = "tank"

TYPES = [
    CONF_TANK,
]


def water_heater_options(min_temp, max_temp, temp_step) -> cv.Schema:
    schema = cv.Schema(
        {
            cv.Optional(CONF_MIN_TEMPERATURE, default=min_temp): cv.float_,
            cv.Optional(CONF_MAX_TEMPERATURE, default=max_temp): cv.float_,
            cv.Optional(CONF_TARGET_TEMPERATURE_STEP, default=temp_step): cv.float_,
        }
    )
    return schema


PanasonicHeatpumpWaterHeater = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpWaterHeater", water_heater.WaterHeater, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
            PanasonicHeatpumpComponent
        ),
        cv.Optional(CONF_TANK): water_heater.water_heater_schema(
            PanasonicHeatpumpWaterHeater
        ).extend(water_heater_options(20.0, 65.0, 0.5)),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
    for index, key in enumerate(TYPES):
        if child_config := config.get(key):
            var = await water_heater.new_water_heater(child_config)
            cg.add(var.set_parent(parent))
            cg.add(var.set_id(index))
            cg.add(var.set_min_temperature(child_config[CONF_MIN_TEMPERATURE]))
            cg.add(var.set_max_temperature(child_config[CONF_MAX_TEMPERATURE]))
            cg.add(var.set_temperature_step(child_config[CONF_TARGET_TEMPERATURE_STEP]))
            cg.add(parent.add_water_heater(var))
