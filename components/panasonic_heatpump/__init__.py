import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@elvit"]
DEPENDENCIES = ["uart"]
MULTICONF = True

CONF_PANASONIC_HEATPUMP_ID = "panasonic_heatpump"
CONF_UART_CLIENT = "uart_client_id"
CONF_LOG_UART_MSG = "log_uart_msg"

panasonic_heatpump_ns = cg.esphome_ns.namespace("panasonic_heatpump")
PanasonicHeatpumpComponent = panasonic_heatpump_ns.class_(
    "PanasonicHeatpumpComponent", cg.PollingComponent, uart.UARTDevice
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(PanasonicHeatpumpComponent),
            cv.Optional(CONF_UART_CLIENT): cv.use_id(uart.UARTComponent),
            cv.Optional(CONF_LOG_UART_MSG, default=False): cv.boolean,
        }
    )
    .extend(cv.polling_component_schema("3s"))
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)

    if CONF_UART_CLIENT in config:
        uart_client = await cg.get_variable(config[CONF_UART_CLIENT])
        cg.add(var.set_uart_client(uart_client))

    cg.add(var.set_log_uart_msg(config[CONF_LOG_UART_MSG]))
