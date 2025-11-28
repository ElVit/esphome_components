import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import (
    UNIT_AMPERE,
    UNIT_CELSIUS,
    UNIT_HERTZ,
    UNIT_HOUR,
    UNIT_KELVIN,
    UNIT_MINUTE,
    UNIT_WATT,
    UNIT_EMPTY,
    UNIT_PERCENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_FREQUENCY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_PRESSURE,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_VOLUME_FLOW_RATE,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_TOTAL_INCREASING,
    ICON_COUNTER,
)
from .. import (
    CONF_PANASONIC_HEATPUMP_ID,
    PanasonicHeatpumpComponent,
    panasonic_heatpump_ns,
)

UNIT_LITRE_PER_MINUTE = "L/min"
UNIT_ROTATIONS_PER_MINUTE = "r/min"
UNIT_PRESSURE_KGFCM2 = "kgf/cm²"
UNIT_BAR = "bar"
ICON_FAN_SPEED = "mdi:fan"

CONF_TOP1 = "top1"  # Pump Flow
CONF_TOP5 = "top5"  # Main Inlet Temp
CONF_TOP6 = "top6"  # Main Outlet Temp
CONF_TOP7 = "top7"  # Main Target Temp
CONF_TOP8 = "top8"  # Compressor Freq
CONF_TOP9 = "top9"  # DHW Target Temp
CONF_TOP10 = "top10"  # DHW Temp
CONF_TOP11 = "top11"  # Operations Hours
CONF_TOP12 = "top12"  # Operations Counter
CONF_TOP14 = "top14"  # Outside Temp
CONF_TOP15 = "top15"  # Heat Power Production
CONF_TOP16 = "top16"  # Heat Power Consumption
CONF_TOP21 = "top21"  # Outside Pipe Temp
CONF_TOP22 = "top22"  # DHW Heat Delta
CONF_TOP23 = "top23"  # Heat Delta
CONF_TOP24 = "top24"  # Cool Delta
CONF_TOP25 = "top25"  # DHW Holiday Shift Temp
CONF_TOP27 = "top27"  # Z1 Heat Request Temp
CONF_TOP28 = "top28"  # Z1 Cool Request Temp
CONF_TOP29 = "top29"  # Z1 Heat Curve Target High Temp
CONF_TOP30 = "top30"  # Z1 Heat Curve Target Low Temp
CONF_TOP31 = "top31"  # Z1 Heat Curve Outside High Temp
CONF_TOP32 = "top32"  # Z1 Heat Curve Outside Low Temp
CONF_TOP33 = "top33"  # Room Thermostat Temp
CONF_TOP34 = "top34"  # Z2 Heat Request Temp
CONF_TOP35 = "top35"  # Z2 Cool Request Temp
CONF_TOP36 = "top36"  # Z1 Water Temp
CONF_TOP37 = "top37"  # Z2 Water Temp
CONF_TOP38 = "top38"  # Cool Power Production
CONF_TOP39 = "top39"  # Cool Power Consumption
CONF_TOP40 = "top40"  # DHW Power Production
CONF_TOP41 = "top41"  # DHW Power Consumption
CONF_TOP42 = "top42"  # Z1 Water Target Temp
CONF_TOP43 = "top43"  # Z2 Water Target Temp
CONF_TOP45 = "top45"  # Room Holiday Shift Temp
CONF_TOP46 = "top46"  # Buffer Temp
CONF_TOP47 = "top47"  # Solar Temp
CONF_TOP48 = "top48"  # Pool Temp
CONF_TOP49 = "top49"  # Main Hex Outlet Temp
CONF_TOP50 = "top50"  # Discharge Temp
CONF_TOP51 = "top51"  # Inside Pipe Temp
CONF_TOP52 = "top52"  # Defrost Temp
CONF_TOP53 = "top53"  # Eva Outlet Temp
CONF_TOP54 = "top54"  # Bypass Outlet Temp
CONF_TOP55 = "top55"  # Ipm Temp
CONF_TOP56 = "top56"  # Z1 Temp
CONF_TOP57 = "top57"  # Z2 Temp
CONF_TOP62 = "top62"  # Fan1 Motor Speed
CONF_TOP63 = "top63"  # Fan2 Motor Speed
CONF_TOP64 = "top64"  # High Pressure
CONF_TOP65 = "top65"  # Pump Speed
CONF_TOP66 = "top66"  # Low Pressure
CONF_TOP67 = "top67"  # Compressor Current
CONF_TOP70 = "top70"  # Sterilization Temp
CONF_TOP71 = "top71"  # Sterilization Max Time
CONF_TOP72 = "top72"  # Z1 Cool Curve Target High Temp
CONF_TOP73 = "top73"  # Z1 Cool Curve Target Low Temp
CONF_TOP74 = "top74"  # Z1 Cool Curve Outside High Temp
CONF_TOP75 = "top75"  # Z1 Cool Curve Outside Low Temp
CONF_TOP77 = "top77"  # Heating Off Outdoor Temp
CONF_TOP78 = "top78"  # Heater On Outdoor Temp
CONF_TOP79 = "top79"  # Heat To Cool Temp
CONF_TOP80 = "top80"  # Cool To Heat Temp
CONF_TOP82 = "top82"  # Z2 Heat Curve Target High Temp
CONF_TOP83 = "top83"  # Z2 Heat Curve Target Low Temp
CONF_TOP84 = "top84"  # Z2 Heat Curve Outside High Temp
CONF_TOP85 = "top85"  # Z2 Heat Curve Outside Low Temp
CONF_TOP86 = "top86"  # Z2 Cool Curve Target High Temp
CONF_TOP87 = "top87"  # Z2 Cool Curve Target Low Temp
CONF_TOP88 = "top88"  # Z2 Cool Curve Outside High Temp
CONF_TOP89 = "top89"  # Z2 Cool Curve Outside Low Temp
CONF_TOP90 = "top90"  # Room Heater Operations Hours
CONF_TOP91 = "top91"  # DHW Heater Operations Hours
CONF_TOP93 = "top93"  # Pump Duty
CONF_TOP95 = "top95"  # Max Pump Duty
CONF_TOP96 = "top96"  # Heater Delay Time
CONF_TOP97 = "top97"  # Heater Start Delta
CONF_TOP98 = "top98"  # Heater Stop Delta
CONF_TOP102 = "top102"  # Solar On Delta
CONF_TOP103 = "top103"  # Solar Off Delta
CONF_TOP104 = "top104"  # Solar Frost Protection
CONF_TOP105 = "top105"  # Solar High Limit
CONF_TOP113 = "top113"  # Buffer Tank Delta
CONF_TOP115 = "top115"  # Water Pressure
CONF_TOP116 = "top116"  # Second Inlet Temp
CONF_TOP117 = "top117"  # Economizer Outlet Temp
CONF_TOP118 = "top118"  # Second Room Thermostat Temp
CONF_TOP127 = "top127"  # Z1 Valve PID
CONF_TOP128 = "top128"  # Z2 Valve PID
CONF_TOP131 = "top131"  # Bivalent Start Temperature
CONF_TOP134 = "top134"  # Bivalent Advanced Start Temp
CONF_TOP135 = "top135"  # Bivalent Advanced Stop Temp
CONF_TOP136 = "top136"  # Bivalent Advanced Start Delay
CONF_TOP137 = "top137"  # Bivalent Advanced Stop Delay
CONF_TOP138 = "top138"  # Bivalent Advanced DHW Delay

CONF_XTOP0 = "xtop0"  # Heat Power Consumption Extra
CONF_XTOP1 = "xtop1"  # Cool Power Consumption Extra
CONF_XTOP2 = "xtop2"  # DHW Power Consumption Extra
CONF_XTOP3 = "xtop3"  # Heat Power Production Extra
CONF_XTOP4 = "xtop4"  # Cool Power Production Extra
CONF_XTOP5 = "xtop5"  # DHW Power Production Extra

TYPES = [
    CONF_TOP1,
    CONF_TOP5,
    CONF_TOP6,
    CONF_TOP7,
    CONF_TOP8,
    CONF_TOP9,
    CONF_TOP10,
    CONF_TOP11,
    CONF_TOP12,
    CONF_TOP14,
    CONF_TOP15,
    CONF_TOP16,
    CONF_TOP21,
    CONF_TOP22,
    CONF_TOP23,
    CONF_TOP24,
    CONF_TOP25,
    CONF_TOP27,
    CONF_TOP28,
    CONF_TOP29,
    CONF_TOP30,
    CONF_TOP31,
    CONF_TOP32,
    CONF_TOP33,
    CONF_TOP34,
    CONF_TOP35,
    CONF_TOP36,
    CONF_TOP37,
    CONF_TOP38,
    CONF_TOP39,
    CONF_TOP40,
    CONF_TOP41,
    CONF_TOP42,
    CONF_TOP43,
    CONF_TOP45,
    CONF_TOP46,
    CONF_TOP47,
    CONF_TOP48,
    CONF_TOP49,
    CONF_TOP50,
    CONF_TOP51,
    CONF_TOP52,
    CONF_TOP53,
    CONF_TOP54,
    CONF_TOP55,
    CONF_TOP56,
    CONF_TOP57,
    CONF_TOP62,
    CONF_TOP63,
    CONF_TOP64,
    CONF_TOP65,
    CONF_TOP66,
    CONF_TOP67,
    CONF_TOP70,
    CONF_TOP71,
    CONF_TOP72,
    CONF_TOP73,
    CONF_TOP74,
    CONF_TOP75,
    CONF_TOP77,
    CONF_TOP78,
    CONF_TOP79,
    CONF_TOP80,
    CONF_TOP82,
    CONF_TOP83,
    CONF_TOP84,
    CONF_TOP85,
    CONF_TOP86,
    CONF_TOP87,
    CONF_TOP88,
    CONF_TOP89,
    CONF_TOP90,
    CONF_TOP91,
    CONF_TOP93,
    CONF_TOP95,
    CONF_TOP96,
    CONF_TOP97,
    CONF_TOP98,
    CONF_TOP102,
    CONF_TOP103,
    CONF_TOP104,
    CONF_TOP105,
    CONF_TOP113,
    CONF_TOP115,
    CONF_TOP116,
    CONF_TOP117,
    CONF_TOP118,
    CONF_TOP127,
    CONF_TOP128,
    CONF_TOP131,
    CONF_TOP134,
    CONF_TOP135,
    CONF_TOP136,
    CONF_TOP137,
    CONF_TOP138,
    CONF_XTOP0,
    CONF_XTOP1,
    CONF_XTOP2,
    CONF_XTOP3,
    CONF_XTOP4,
    CONF_XTOP5,
]

PanasonicHeatpumpSensor = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpSensor", sensor.Sensor, cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_PANASONIC_HEATPUMP_ID): cv.use_id(
            PanasonicHeatpumpComponent
        ),
        cv.Optional(CONF_TOP1): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_VOLUME_FLOW_RATE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_LITRE_PER_MINUTE,
        ),
        cv.Optional(CONF_TOP5): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP6): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP7): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP8): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_FREQUENCY,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_HERTZ,
        ),
        cv.Optional(CONF_TOP9): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP10): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP11): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_TOTAL_INCREASING,
            unit_of_measurement=UNIT_HOUR,
        ),
        cv.Optional(CONF_TOP12): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_EMPTY,
            icon=ICON_COUNTER,
        ),
        cv.Optional(CONF_TOP14): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP15): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_TOP16): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_TOP21): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP22): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP23): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP24): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP25): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP27): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP28): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP29): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP30): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP31): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP32): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP33): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP34): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP35): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP36): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP37): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP38): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_TOP39): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_TOP40): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_TOP41): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_TOP42): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP43): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP45): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP46): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP47): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP48): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP49): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP50): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP51): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP52): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP53): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP54): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP55): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP56): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP57): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP62): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            icon=ICON_FAN_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_ROTATIONS_PER_MINUTE,
        ),
        cv.Optional(CONF_TOP63): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            icon=ICON_FAN_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_ROTATIONS_PER_MINUTE,
        ),
        cv.Optional(CONF_TOP64): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_PRESSURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_PRESSURE_KGFCM2,
        ),
        cv.Optional(CONF_TOP65): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_ROTATIONS_PER_MINUTE,
        ),
        cv.Optional(CONF_TOP66): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_PRESSURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_PRESSURE_KGFCM2,
        ),
        cv.Optional(CONF_TOP67): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_CURRENT,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_AMPERE,
        ),
        cv.Optional(CONF_TOP70): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP71): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_MINUTE,
        ),
        cv.Optional(CONF_TOP72): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP73): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP74): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP75): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP77): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP78): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP79): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP80): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP82): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP83): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP84): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP85): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP86): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP87): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP88): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP89): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP90): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_TOTAL_INCREASING,
            unit_of_measurement=UNIT_HOUR,
        ),
        cv.Optional(CONF_TOP91): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_TOTAL_INCREASING,
            unit_of_measurement=UNIT_HOUR,
        ),
        cv.Optional(CONF_TOP93): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_EMPTY,
        ),
        cv.Optional(CONF_TOP95): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_EMPTY,
        ),
        cv.Optional(CONF_TOP96): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_MINUTE,
        ),
        cv.Optional(CONF_TOP97): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP98): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP102): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP103): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP104): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP105): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP113): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_KELVIN,
        ),
        cv.Optional(CONF_TOP115): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_PRESSURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_BAR,
        ),
        cv.Optional(CONF_TOP116): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP117): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP118): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP127): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_PERCENT,
        ),
        cv.Optional(CONF_TOP128): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_PERCENT,
        ),
        cv.Optional(CONF_TOP131): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP134): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP135): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_TEMPERATURE,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_CELSIUS,
        ),
        cv.Optional(CONF_TOP136): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_MINUTE,
        ),
        cv.Optional(CONF_TOP137): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_MINUTE,
        ),
        cv.Optional(CONF_TOP138): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_MINUTE,
        ),
        cv.Optional(CONF_XTOP0): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_XTOP1): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_XTOP2): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_XTOP3): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_XTOP4): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
        cv.Optional(CONF_XTOP5): sensor.sensor_schema(
            PanasonicHeatpumpSensor,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_POWER,
            state_class=STATE_CLASS_MEASUREMENT,
            unit_of_measurement=UNIT_WATT,
        ),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PANASONIC_HEATPUMP_ID])
    for index, key in enumerate(TYPES):
        if child_config := config.get(key):
            var = await sensor.new_sensor(child_config)
            await cg.register_component(var, child_config)
            cg.add(var.set_parent(parent))
            cg.add(var.set_id(index))
            if key.startswith("xtop"):
                cg.add(parent.add_extra_sensor(var))
            else:
                cg.add(parent.add_sensor(var))
