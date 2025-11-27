import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import (
    DEVICE_CLASS_RUNNING,
)
from .. import (
    CONF_PANASONIC_HEATPUMP_ID,
    PanasonicHeatpumpComponent,
    panasonic_heatpump_ns,
)

CONF_TOP0 = "top0"  # Heatpump State
CONF_TOP2 = "top2"  # Force DHW State
CONF_TOP3 = "top3"  # Quiet Mode Schedule
CONF_TOP13 = "top13"  # Main Schedule State
CONF_TOP26 = "top26"  # Defrosting State
CONF_TOP60 = "top60"  # Internal Heater State
CONF_TOP61 = "top61"  # External Heater State
CONF_TOP68 = "top68"  # Force Heater State
CONF_TOP69 = "top69"  # Sterilization State
CONF_TOP99 = "top99"  # Buffer Installed
CONF_TOP100 = "top100"  # DHW Installed
CONF_TOP108 = "top108"  # Alt External Sensor
CONF_TOP109 = "top109"  # Anti Freeze Mode
CONF_TOP110 = "top110"  # Optional PCB
CONF_TOP119 = "top119"  # External Control
CONF_TOP120 = "top120"  # External Heat Cool Control
CONF_TOP121 = "top121"  # External Error Signal
CONF_TOP122 = "top122"  # External Compressor Control
CONF_TOP123 = "top123"  # Z2 Pump State
CONF_TOP124 = "top124"  # Z1 Pump State
CONF_TOP129 = "top129"  # Bivalent Control
CONF_TOP132 = "top132"  # Bivalent Advanced Heat
CONF_TOP133 = "top133"  # Bivalent Advanced DHW

TYPES = [
    CONF_TOP0,
    CONF_TOP2,
    CONF_TOP3,
    CONF_TOP13,
    CONF_TOP26,
    CONF_TOP60,
    CONF_TOP61,
    CONF_TOP68,
    CONF_TOP69,
    CONF_TOP99,
    CONF_TOP100,
    CONF_TOP108,
    CONF_TOP109,
    CONF_TOP110,
    CONF_TOP119,
    CONF_TOP120,
    CONF_TOP121,
    CONF_TOP122,
    CONF_TOP123,
    CONF_TOP124,
    CONF_TOP129,
    CONF_TOP132,
    CONF_TOP133,
]

PanasonicHeatpumpBinarySensor = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpBinarySensor", binary_sensor.BinarySensor, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
            PanasonicHeatpumpComponent
        ),
        cv.Optional(CONF_TOP0): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP2): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP3): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP13): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP26): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP60): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP61): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP68): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP69): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP99): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP100): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP108): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP109): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
            device_class=DEVICE_CLASS_RUNNING,
        ),
        cv.Optional(CONF_TOP110): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP119): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP120): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP121): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP122): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP123): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP124): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP129): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP132): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
        cv.Optional(CONF_TOP133): binary_sensor.binary_sensor_schema(
            PanasonicHeatpumpBinarySensor,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
    for index, key in enumerate(TYPES):
        if child_config := config.get(key):
            var = await binary_sensor.new_binary_sensor(child_config)
            await cg.register_component(var, child_config)
            cg.add(var.set_parent(parent))
            cg.add(var.set_id(index))
            cg.add(parent.add_binary_sensor(var))
