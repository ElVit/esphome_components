import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ['@elvit']
MULTICONF = True
DEPENDENCIES = ['uart']

CONF_MAIDESITE_DESK_ID = "maidesite_desk"
CONF_LOG_UART_MSG = "log_uart_msg"

maidesite_desk_ns = cg.esphome_ns.namespace('maidesite_desk')
MaidesiteDeskComponent = maidesite_desk_ns.class_('MaidesiteDeskComponent', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = (
  cv.Schema(
    {
      cv.GenerateID(): cv.declare_id(MaidesiteDeskComponent),

      cv.Optional(CONF_LOG_UART_MSG, default=False): cv.boolean,
    }
  )
  .extend(uart.UART_DEVICE_SCHEMA)
  .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
  var = cg.new_Pvariable(config[CONF_ID])
  await cg.register_component(var, config)
  await uart.register_uart_device(var, config)
  cg.add(var.set_log_uart_msg(config[CONF_LOG_UART_MSG]))
