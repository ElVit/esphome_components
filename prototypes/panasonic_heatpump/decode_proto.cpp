#include "decode_proto.h"

int getBit1(uint8_t input)
{
  return (input >> 7);
}

int getBit1and2(uint8_t input)
{
  return ((input >> 6) - 1);
}

int getBit3and4(uint8_t input)
{
  return (((input >> 4) & 0b11) - 1);
}

int getBit5and6(uint8_t input)
{
  return (((input >> 2) & 0b11) - 1);
}

int getBit7and8(uint8_t input)
{
  return ((input & 0b11) - 1);
}

int getBit3and4and5(uint8_t input)
{
  return (((input >> 3) & 0b111) - 1);
}

int getLeft5bits(uint8_t input)
{
  return ((input >> 3) - 1);
}

int getRight3bits(uint8_t input)
{
  return ((input & 0b111) - 1);
}

int getIntMinus1(uint8_t input)
{
  int value = (int)input - 1;
  return value;
}

int getIntMinus128(uint8_t input)
{
  int value = (int)input - 128;
  return value;
}

float getIntMinus1Div5(uint8_t input)
{
  return ((((float)input - 1) / 5), 1);
}

float getIntMinus1Div50(uint8_t input)
{
  return ((((float)input - 1) / 50), 2);
}

int getIntMinus1Times10(uint8_t input)
{
  int value = (int)input - 1;
  return (value * 10);
}

int getIntMinus1Times50(uint8_t input)
{
  int value = (int)input - 1;
  return (value * 50);
}

int getFirstByte(uint8_t input)
{
  return ((input >> 4) - 1);
}

int getSecondByte(uint8_t input)
{
  return ((input & 0b1111) - 1);
}

int getWord(uint8_t low, uint8_t hi)
{
  return ((hi << 8) + low) - 1;
}

int getUintt16(uint8_t input1, uint8_t input2)
{
  uint16_t value = static_cast<uint16_t>((input2 << 8) | input1);
  return (value - 1);
}

// TOP15, TOP16, TOP38, TOP39, TOP40, TOP41 //
int getPower(uint8_t input)
{
  int value = ((int)input - 1) * 200;
  return value;
}

// TOP4 //
int getOpMode(uint8_t input)
{
  switch ((int)(input & 0b111111))
  {
  case 18:
    return 0;
  case 19:
    return 1;
  case 25:
    return 2;
  case 33:
    return 3;
  case 34:
    return 4;
  case 35:
    return 5;
  case 41:
    return 6;
  case 26:
    return 7;
  case 42:
    return 8;
  default:
    return -1;
  }
}

// TOP124 //
int getBivalent(uint8_t input)
{
  switch ((int)input)
  {
  case 0x55:
    return 0;
  case 0x56:
    return 1;
  case 0x57:
    return 2;
  case 0x58:
    return 3;
  case 0x59:
    return 4;
  case 0x5A:
    return 5;
  default:
    return -1;
  }
}

// TOP92 //
int getModel(std::vector<uint8_t> &data, uint8_t index)
{
  uint8_t model[10] = { data[index], data[index+1], data[index+2], data[index+3], data[index+4], data[index+5], data[index+6], data[index+7], data[index+8], data[index+9] };
  int result = -1;
  int size = sizeof(KnownModels) / sizeof(KnownModels[0]);

  for (unsigned int i = 0 ; i < size; i++)
  {
    if (memcmp(model, KnownModels[i], 10) == 0)
    {
      result = i;
      break;
    }
  }

  return result;
}

// TOP1 //
// input1 = data[169]
// input2 = data[170]
float getPumpFlow(uint8_t input1, uint8_t input2)
{
  int PumpFlow2 = (int)input2;
  float PumpFlow1 = (((float)input1 - 1) / 256);
  float PumpFlow = PumpFlow2 + PumpFlow1;

  return PumpFlow;
}

// TOP5, TOP6 //
float getFractional(uint8_t input, uint8_t shift)
{
  float result;
  int fractional = (int)((input >> shift) & 0b111) - 1;
  result = fractional * 0.25;

  return result;
}

// TOP44 //
// errorType = data[113]
// errorNumber = data[114]
std::string getErrorInfo(uint8_t errorType, uint8_t errorNumber)
{
  int error_type = (int)(errorType);
  int error_number = ((int)(errorNumber)) - 17;
  char error_string[10];

  switch (error_type)
  {
  case 177: // B1=F type error
    sprintf(error_string, "F%02X", error_number);
    break;
  case 161: // A1=H type error
    sprintf(error_string, "H%02X", error_number);
    break;
  default:
    sprintf(error_string, "No error");
    break;
  }

  return error_string;
}

bool getBoolState(uint8_t input)
{
  return input != 0;
}

std::string getTextState(const char **array, int index)
{
  int size = atoi(array[0]);

  if ((index < 0) || (index >= size))
  {
    return "UNKNOWN";
  }

  return array[index + 1];
}
