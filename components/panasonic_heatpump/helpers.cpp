#include "helpers.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    static const char *const TAG = "panasonic_heatpump";

    void PanasonicHelpers::log_uart_hex(UartLogDirection direction, const std::vector<uint8_t>& data, const char separator)
    {
      PanasonicHelpers::log_uart_hex(direction, &data[0], data.size(), separator);
    }

    void PanasonicHelpers::log_uart_hex(UartLogDirection direction, const uint8_t* data, const size_t length, const char separator)
    {
      std::string logStr = "";
      std::string msgDir = direction == UART_LOG_TX ? ">>>" : "<<<";
      std::string msgType = direction == UART_LOG_TX ? "request" : "response";
      switch(data[0])
      {
        case 0x31:
          msgType = "initial_" + msgType;
          break;
        case 0x71:
          msgType = "polling_" + msgType;
          if (data[3] == 0x21) msgType = "extra_" + msgType;
          break;
        case 0xF1:
          msgType = "command_" + msgType;
          break;
      };

      ESP_LOGI(TAG, "%s %s[%i]", msgDir.c_str(), msgType.c_str(), length);
      delay(10);

      char buffer[5];
      for (size_t i = 0; i < length; i++)
      {
        if (i > 0) logStr += separator;
        sprintf(buffer, "%02X", data[i]);
        logStr += buffer;
      }

      for (size_t i = 0; i < logStr.length(); i += UART_LOG_CHUNK_SIZE)
      {
        ESP_LOGI(TAG, "%s %s", msgDir.c_str(), logStr.substr(i, UART_LOG_CHUNK_SIZE).c_str());
        delay(10);
      }
    }
  }  // namespace panasonic_heatpump
}  // namespace esphome
