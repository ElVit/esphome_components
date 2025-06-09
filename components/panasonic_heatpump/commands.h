#pragma once
#include <cmath>
#include <vector>

#define INIT_REQUEST_SIZE 8
#define INIT_RESPONSE_SIZE 51
#define DATA_MESSAGE_SIZE 111
#define OPTIONAL_MESSAGE_SIZE 20


namespace esphome
{
  namespace panasonic_heatpump
  {
    class PanasonicCommand
    {
    public:
      PanasonicCommand() = delete;

      static uint8_t calcChecksum(std::vector<uint8_t>& data, int length);
      static uint8_t setMultiply2(size_t input);
      static uint8_t setMultiply4(size_t input);
      static uint8_t setPlus1Multiply4(size_t input);
      static uint8_t setPlus1Multiply8(size_t input);
      static uint8_t setPlus1Multiply16(size_t input);
      static uint8_t setPlus1Multiply64(size_t input);
      static uint8_t setPlus1(size_t input);
      static uint8_t setPlus128(size_t input);
      static uint8_t setOperationMode(size_t input);
      static uint8_t temp2hex(float temp);
      static uint8_t setByte6(uint8_t byte6, int val, int base, int bit);
      static uint8_t setDemandControl(size_t input);

      static const uint8_t InitialRequest[INIT_REQUEST_SIZE];
      static const uint8_t InitialResponse[INIT_RESPONSE_SIZE];
      static const uint8_t PollingMessage[DATA_MESSAGE_SIZE];
      static const uint8_t CommandMessage[DATA_MESSAGE_SIZE];
    };
  }  // namespace panasonic_heatpump
}  // namespace esphome
