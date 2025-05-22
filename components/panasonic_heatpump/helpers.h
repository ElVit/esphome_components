#pragma once

#include "esphome/core/hal.h"
#include "esphome/core/log.h"
#include <vector>
#include <string>

#define UART_LOG_CHUNK_SIZE 153


namespace esphome
{
  namespace panasonic_heatpump
  {
    enum UartLogDirection
    {
      UART_LOG_RX,
      UART_LOG_TX,
    };

    class PanasonicHelpers
    {
    public:
      static void log_uart_hex(UartLogDirection direction, const std::vector<uint8_t>& data, const char separator);
      static void log_uart_hex(UartLogDirection direction, const uint8_t* data, const size_t length, const char separator);
    };
  }
}