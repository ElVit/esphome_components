#include "commands.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    const uint8_t PanasonicCommand::InitialMessage[REQUEST_INIT_MSG_SIZE] = {
      0x31, 0x05, 0x10, 0x01, 0x00, 0x00, 0x00, 0xB9
    };
    const uint8_t PanasonicCommand::PeriodicalMessage[REQUEST_DATA_MSG_SIZE] = {
      0x71, 0x6C, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x12
    };
    const uint8_t PanasonicCommand::CommandMessage[REQUEST_DATA_MSG_SIZE] = {
      0xF1, 0x6C, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x92
    };

    uint8_t PanasonicCommand::calcChecksum(std::vector<uint8_t>& data, int length)
    {
      uint8_t checksum = 0;
      for (int i = 0; i < length; i++)
      {
        checksum += data[i];
      }
      checksum = (checksum ^ 0xFF) + 01;
      return checksum;
    }

    uint8_t PanasonicCommand::setMultiply2(size_t input)
    {
      return input * 2;
    }

    uint8_t PanasonicCommand::setMultiply4(size_t input)
    {
      return input * 4;
    }

    uint8_t PanasonicCommand::setPlus1Multiply4(size_t input)
    {
      return (input + 1) * 4;
    }

    uint8_t PanasonicCommand::setPlus1Multiply8(size_t input)
    {
      return (input + 1) * 8;
    }

    uint8_t PanasonicCommand::setPlus1Multiply16(size_t input)
    {
      return (input + 1) * 16;
    }

    uint8_t PanasonicCommand::setPlus1Multiply64(size_t input)
    {
      return (input + 1) * 64;
    }

    uint8_t PanasonicCommand::setPlus1(size_t input)
    {
      return input + 1;
    }

    uint8_t PanasonicCommand::setPlus73(size_t input)
    {
      return input + 73;
    }

    uint8_t PanasonicCommand::setPlus128(size_t input)
    {
      return input + 128;
    }

    uint8_t PanasonicCommand::setOperationMode(size_t input)
    {
      switch (input)
      {
        case 0: return 18;
        case 1: return 19;
        case 2: return 24;
        case 3: return 33;
        case 4: return 34;
        case 5: return 35;
        case 6: return 40;
      }
      return 0;
    }

    //start of optional pcb commands

    uint8_t PanasonicCommand::temp2hex(float temp)
    {
      int hextemp = 0;
      if (temp > 120)
      {
        hextemp = 0;
      }
      else if (temp < -78)
      {
        hextemp = 255;
      }
      else
      {
        uint8_t Uref = 255;
        int constant = 3695;
        int R25 = 6340;
        uint8_t T25 = 25;
        int Rf = 6480;
        float K = 273.15;
        float RT = R25 * exp(constant * (1 / (temp + K) - 1 / (T25 + K)));
        hextemp = Uref * (RT / (Rf + RT));
      }

      return hextemp;
    }

    uint8_t PanasonicCommand::setByte6(uint8_t byte6, int val, int base, int bit)
    {
      return (byte6 & ~(base << bit)) | (val << bit);
    }

    uint8_t PanasonicCommand::setDemandControl(size_t input)
    {
      switch (input)
      {
        case 0: return 0xEB;
        case 1: return 0xB8;
        case 2: return 0x85;
        case 3: return 0x52;
        case 4: return 0x2B;
      }
      return 0;
    }
  }  // namespace panasonic_heatpump
}  // namespace esphome