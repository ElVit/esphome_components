import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@elvit"]
DEPENDENCIES = ["uart"]
MULTICONF = True

CONF_PANASONIC_HEATPUMP_ID = "panasonic_heatpump"
CONF_UART_CLIENT = "uart_client_id"
CONF_UART_CLIENT_TIMEOUT = "uart_client_timeout"
CONF_REQUEST_SEND_INTERVAL = "request_send_interval"
CONF_LOG_UART_MSG = "log_uart_msg"

ARDUINO_BRANCH_URL = "https://github.com/ElVit/esphome_components/tree/heatpump/arduino"


def require_esp32_idf(config):
    """Reject targets unsupported by the threaded UART implementation."""
    try:
        cv.only_on_esp32(config)
        cv.only_with_framework("esp-idf")(config)
    except cv.Invalid as err:
        raise cv.Invalid(
            f"{err}\n"
            "The panasonic_heatpump component requires ESP32 with ESP-IDF. "
            "For ESP8266 or the Arduino framework, use the 'heatpump/arduino' "
            f"branch: {ARDUINO_BRANCH_URL}"
        ) from err
    return config


panasonic_heatpump_ns = cg.esphome_ns.namespace("panasonic_heatpump")
PanasonicHeatpumpComponent = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpComponent", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PanasonicHeatpumpComponent),
            cv.Optional(CONF_UART_CLIENT): cv.use_id(uart.UARTComponent),
            cv.Optional(
                CONF_UART_CLIENT_TIMEOUT, default="10000ms"
            ): cv.positive_time_period_milliseconds,
            cv.Optional(
                CONF_REQUEST_SEND_INTERVAL, default="250ms"
            ): cv.positive_time_period_milliseconds,
            cv.Optional(CONF_LOG_UART_MSG, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("3s"))
    .extend(uart.UART_DEVICE_SCHEMA),
    require_esp32_idf,
)

FINAL_VALIDATE_SCHEMA = uart.final_validate_device_schema(
    "panasonic_heatpump",
    baud_rate=9600,
    data_bits=8,
    parity="EVEN",
    stop_bits=1,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_UART_CLIENT in config:
        uart_client = await cg.get_variable(config[CONF_UART_CLIENT])
        cg.add(var.set_uart_client(uart_client))
        cg.add(var.set_uart_client_timeout(config[CONF_UART_CLIENT_TIMEOUT]))

    cg.add(var.set_request_send_interval(config[CONF_REQUEST_SEND_INTERVAL]))
    cg.add(var.set_log_uart_msg(config[CONF_LOG_UART_MSG]))
