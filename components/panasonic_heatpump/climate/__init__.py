import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate
from esphome.components.climate import (
  ClimateMode,
  ClimatePreset,
)
from esphome.const import (
  CONF_ID,
  CONF_SUPPORTED_MODES,
  CONF_SUPPORTED_PRESETS,
  CONF_CUSTOM_PRESETS,
  CONF_VISUAL,
  CONF_MIN_TEMPERATURE,
  CONF_MAX_TEMPERATURE,
  CONF_TEMPERATURE_STEP,
)
from .. import CONF_PANASONIC_HEATPUMP_ID, PanasonicHeatpumpComponent, panasonic_heatpump_ns

CONF_COOL_MODE = "cool_mode"
CONF_CLIMATE_TANK = "tank"
CONF_CLIMATE_ZONE1 = "zone1"
CONF_CLIMATE_ZONE2 = "zone2"

TYPES = [
  CONF_CLIMATE_TANK,
  CONF_CLIMATE_ZONE1,
  CONF_CLIMATE_ZONE2,
]

def climate_options(min_temp, max_temp, temp_step) -> cv.Schema:
  schema = cv.Schema({
    cv.Optional(CONF_MIN_TEMPERATURE, default=min_temp): cv.float_,
    cv.Optional(CONF_MAX_TEMPERATURE, default=max_temp): cv.float_,
    cv.Optional(CONF_TEMPERATURE_STEP, default=temp_step): cv.float_,
  })
  return schema

PanasonicHeatpumpClimate = panasonic_heatpump_ns.class_("PanasonicHeatpumpClimate", climate.Climate, cg.Component)

CONFIG_SCHEMA = cv.Schema(
  {
    cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
      PanasonicHeatpumpComponent
    ),
    cv.Optional(CONF_COOL_MODE, default=False): cv.boolean,

    cv.Optional(CONF_CLIMATE_TANK): climate.climate_schema(
      PanasonicHeatpumpClimate
    ).extend(
      climate_options(20.0, 65.0, 0.5)
    ),
    cv.Optional(CONF_CLIMATE_ZONE1): climate.climate_schema(
      PanasonicHeatpumpClimate
    ).extend(
      climate_options(-5.0, 5.0, 0.5)
    ),
    cv.Optional(CONF_CLIMATE_ZONE2): climate.climate_schema(
      PanasonicHeatpumpClimate
    ).extend(
      climate_options(-5.0, 5.0, 0.5)
    ),
  }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
  parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
  for index, key in enumerate(TYPES):
    if child_config := config.get(key):
      var = await climate.new_climate(child_config)
      await cg.register_component(var, child_config)
      cg.add(var.set_parent(parent))
      cg.add(var.set_id(index))
      cg.add(var.set_min_temperature(child_config[CONF_MIN_TEMPERATURE]))
      cg.add(var.set_max_temperature(child_config[CONF_MAX_TEMPERATURE]))
      cg.add(var.set_temperature_step(child_config[CONF_TEMPERATURE_STEP]))
      cg.add(var.set_cool_mode(config[CONF_COOL_MODE]))
      cg.add(parent.add_climate(var))
