#include "decode.h"


namespace esphome
{
  namespace panasonic_heatpump
  {
    constexpr const char* const PanasonicDecode::DisabledEnabled[];
    constexpr const char* const PanasonicDecode::BlockedFree[];
    constexpr const char* const PanasonicDecode::OffOn[];
    constexpr const char* const PanasonicDecode::InactiveActive[];
    constexpr const char* const PanasonicDecode::PumpFlowRateMode[];
    constexpr const char* const PanasonicDecode::HolidayState[];
    constexpr const char* const PanasonicDecode::OpModeDesc[];
    constexpr const char* const PanasonicDecode::Powerfulmode[];
    constexpr const char* const PanasonicDecode::Quietmode[];
    constexpr const char* const PanasonicDecode::Valve[];
    constexpr const char* const PanasonicDecode::Valve2[];
    constexpr const char* const PanasonicDecode::MixingValve[];
    constexpr const char* const PanasonicDecode::ZonesState[];
    constexpr const char* const PanasonicDecode::HeatCoolModeDesc[];
    constexpr const char* const PanasonicDecode::SolarModeDesc[];
    constexpr const char* const PanasonicDecode::ZonesSensorType[];
    constexpr const char* const PanasonicDecode::LiquidType[];
    constexpr const char* const PanasonicDecode::ExtPadHeaterType[];
    constexpr const char* const PanasonicDecode::Bivalent[];
    constexpr const char* const PanasonicDecode::ModelNames[];

    constexpr const uint8_t KnownModels[NUMBER_OF_MODELS][10] =
    {
      { 0xE2, 0xCF, 0x0B, 0x13, 0x33, 0x32, 0xD1, 0x0C, 0x16, 0x33 }, // 0
      { 0xE2, 0xCF, 0x0B, 0x14, 0x33, 0x42, 0xD1, 0x0B, 0x17, 0x33 }, // 1
      { 0xE2, 0xCF, 0x0D, 0x77, 0x09, 0x12, 0xD0, 0x0B, 0x05, 0x11 }, // 2
      { 0xE2, 0xCF, 0x0C, 0x88, 0x05, 0x12, 0xD0, 0x0B, 0x97, 0x05 }, // 3
      { 0xE2, 0xCF, 0x0D, 0x85, 0x05, 0x12, 0xD0, 0x0C, 0x94, 0x05 }, // 4
      { 0xE2, 0xCF, 0x0D, 0x86, 0x05, 0x12, 0xD0, 0x0C, 0x95, 0x05 }, // 5
      { 0xE2, 0xCF, 0x0D, 0x87, 0x05, 0x12, 0xD0, 0x0C, 0x96, 0x05 }, // 6
      { 0xE2, 0xCE, 0x0D, 0x71, 0x81, 0x72, 0xCE, 0x0C, 0x92, 0x81 }, // 7
      { 0x62, 0xD2, 0x0B, 0x43, 0x54, 0x42, 0xD2, 0x0B, 0x72, 0x66 }, // 8
      { 0xC2, 0xD3, 0x0B, 0x33, 0x65, 0xB2, 0xD3, 0x0B, 0x94, 0x65 }, // 9
      { 0xE2, 0xCF, 0x0B, 0x15, 0x33, 0x42, 0xD1, 0x0B, 0x18, 0x33 }, // 10
      { 0xE2, 0xCF, 0x0B, 0x41, 0x34, 0x82, 0xD1, 0x0B, 0x31, 0x35 }, // 11
      { 0x62, 0xD2, 0x0B, 0x45, 0x54, 0x42, 0xD2, 0x0B, 0x47, 0x55 }, // 12
      { 0xE2, 0xCF, 0x0C, 0x74, 0x09, 0x12, 0xD0, 0x0D, 0x95, 0x05 }, // 13
      { 0xE2, 0xCF, 0x0B, 0x82, 0x05, 0x12, 0xD0, 0x0C, 0x91, 0x05 }, // 14
      { 0xE2, 0xCF, 0x0C, 0x55, 0x14, 0x12, 0xD0, 0x0B, 0x15, 0x08 }, // 15
      { 0xE2, 0xCF, 0x0C, 0x43, 0x00, 0x12, 0xD0, 0x0B, 0x15, 0x08 }, // 16
      { 0x62, 0xD2, 0x0B, 0x45, 0x54, 0x32, 0xD2, 0x0C, 0x45, 0x55 }, // 17
      { 0x62, 0xD2, 0x0B, 0x43, 0x54, 0x42, 0xD2, 0x0C, 0x46, 0x55 }, // 18
      { 0xE2, 0xCF, 0x0C, 0x54, 0x14, 0x12, 0xD0, 0x0B, 0x14, 0x08 }, // 19
      { 0xC2, 0xD3, 0x0B, 0x34, 0x65, 0xB2, 0xD3, 0x0B, 0x95, 0x65 }, // 20
      { 0xC2, 0xD3, 0x0B, 0x35, 0x65, 0xB2, 0xD3, 0x0B, 0x96, 0x65 }, // 21
      { 0x62, 0xD2, 0x0B, 0x41, 0x54, 0x32, 0xD2, 0x0C, 0x45, 0x55 }, // 22
      { 0x32, 0xD4, 0x0B, 0x87, 0x84, 0x73, 0x90, 0x0C, 0x84, 0x84 }, // 23
      { 0x32, 0xD4, 0x0B, 0x88, 0x84, 0x73, 0x90, 0x0C, 0x85, 0x84 }, // 24
      { 0xE2, 0xCF, 0x0B, 0x75, 0x09, 0x12, 0xD0, 0x0C, 0x06, 0x11 }, // 25
      { 0x42, 0xD4, 0x0B, 0x83, 0x71, 0x42, 0xD2, 0x0C, 0x46, 0x55 }, // 26
      { 0xC2, 0xD3, 0x0C, 0x34, 0x65, 0xB2, 0xD3, 0x0B, 0x95, 0x65 }, // 27
      { 0xC2, 0xD3, 0x0C, 0x33, 0x65, 0xB2, 0xD3, 0x0B, 0x94, 0x65 }, // 28
      { 0xE2, 0xCF, 0x0B, 0x83, 0x05, 0x12, 0xD0, 0x0D, 0x92, 0x05 }, // 29
      { 0xE2, 0xCF, 0x0C, 0x78, 0x09, 0x12, 0xD0, 0x0B, 0x06, 0x11 }, // 30
      { 0xC2, 0xD3, 0x0C, 0x35, 0x65, 0xB2, 0xD3, 0x0B, 0x96, 0x65 }, // 31
      { 0x32, 0xD4, 0x0B, 0x99, 0x77, 0x62, 0x90, 0x0B, 0x01, 0x78 }, // 32
      { 0x42, 0xD4, 0x0B, 0x15, 0x76, 0x12, 0xD0, 0x0B, 0x10, 0x11 }, // 33
      { 0xE2, 0xD5, 0x0C, 0x29, 0x99, 0x83, 0x92, 0x0C, 0x28, 0x98 }, // 34
      { 0xE2, 0xCF, 0x0D, 0x85, 0x05, 0x12, 0xD0, 0x0E, 0x94, 0x05 }, // 35
      { 0xE2, 0xD5, 0x0D, 0x36, 0x99, 0x02, 0xD6, 0x0F, 0x67, 0x95 }, // 36
      { 0xE2, 0xD5, 0x0B, 0x08, 0x95, 0x02, 0xD6, 0x0E, 0x66, 0x95 }, // 37
      { 0xE2, 0xD5, 0x0B, 0x34, 0x99, 0x83, 0x92, 0x0C, 0x29, 0x98 }, // 38
      { 0xE2, 0xCF, 0x0C, 0x89, 0x05, 0x12, 0xD0, 0x0C, 0x98, 0x05 }, // 39
      { 0xE2, 0xD5, 0x0B, 0x08, 0x95, 0x02, 0xD6, 0x0E, 0x67, 0x95 }, // 40
      { 0xE2, 0xCF, 0x0C, 0x74, 0x09, 0x12, 0xD0, 0x0C, 0x96, 0x05 }, // 41
      { 0xE2, 0xCF, 0x0C, 0x74, 0x09, 0x12, 0xD0, 0x0E, 0x95, 0x05 }, // 42
      { 0x32, 0xD4, 0x0B, 0x89, 0x84, 0x73, 0x90, 0x0C, 0x86, 0x84 }, // 43
      { 0x32, 0xD4, 0x0B, 0x00, 0x78, 0x62, 0x90, 0x0B, 0x02, 0x78 }, // 44
      { 0xE2, 0xCF, 0x0B, 0x82, 0x05, 0x12, 0xD0, 0x0D, 0x91, 0x05 }, // 45
      { 0xE2, 0xD5, 0x0D, 0x99, 0x94, 0x02, 0xD6, 0x0D, 0x68, 0x95 }, // 46
      { 0xE2, 0xCF, 0x0C, 0x74, 0x09, 0x12, 0xD0, 0x0C, 0x95, 0x05 }, // 47
      { 0xE2, 0xD5, 0x0B, 0x34, 0x99, 0x83, 0x92, 0x0C, 0x28, 0x98 }, // 48
      { 0xE2, 0xCF, 0x0D, 0x77, 0x09, 0x12, 0xD0, 0x0C, 0x05, 0x11 }, // 49
      { 0xE2, 0xD5, 0x0C, 0x67, 0x00, 0x83, 0x92, 0x0C, 0x27, 0x98 }, // 51
      { 0xE2, 0xD5, 0x0B, 0x34, 0x99, 0x83, 0x92, 0x0C, 0x27, 0x98 }, // 52
    };

    int PanasonicDecode::getBit1(uint8_t input)
    {
      return input >> 7;
    }

    int PanasonicDecode::getBit1and2(uint8_t input)
    {
      return (input >> 6) - 1;
    }

    int PanasonicDecode::getBit3and4(uint8_t input)
    {
      return ((input >> 4) & 0b11) - 1;
    }

    int PanasonicDecode::getBit5and6(uint8_t input)
    {
      return ((input >> 2) & 0b11) - 1;
    }

    int PanasonicDecode::getBit7and8(uint8_t input)
    {
      return (input & 0b11) - 1;
    }

    int PanasonicDecode::getBit3and4and5(uint8_t input)
    {
      return ((input >> 3) & 0b111) - 1;
    }

    int PanasonicDecode::getBit6and7and8(uint8_t input)
    {
      return (input & 0b111) - 1;
    }

    int PanasonicDecode::getByteMinus1(uint8_t input)
    {
      return (int)input - 1;
    }

    int PanasonicDecode::getByteMinus128(uint8_t input)
    {
      return (int)input - 128;
    }

    float PanasonicDecode::getByteMinus1Div2(uint8_t input)
    {
      return ((float)input - 1) / 2;
    }

    float PanasonicDecode::getByteMinus1Div5(uint8_t input)
    {
      return ((float)input - 1) / 5;
    }

    float PanasonicDecode::getByteMinus1Div50(uint8_t input)
    {
      return ((float)input - 1) / 50;
    }

    int PanasonicDecode::getByteMinus1Times10(uint8_t input)
    {
      return ((int)input - 1) * 10;
    }

    int PanasonicDecode::getByteMinus1Times50(uint8_t input)
    {
      return ((int)input - 1) * 50;
    }

    int PanasonicDecode::getByteMinus1Times200(uint8_t input)
    {
      return ((int)input - 1) * 200;
    }

    int PanasonicDecode::getHighNibbleMinus1(uint8_t input)
    {
      return (input >> 4) - 1;
    }

    int PanasonicDecode::getLowNibbleMinus1(uint8_t input)
    {
      return (input & 0b1111) - 1;
    }

    int PanasonicDecode::getWordMinus1(uint8_t low, uint8_t hi)
    {
      return ((hi << 8) + low) - 1;
    }

    int PanasonicDecode::getUintt16(uint8_t input1, uint8_t input2)
    {
      return (static_cast<uint16_t>((input2 << 8) | input1)) - 1;
    }

    // TOP4 //
    int PanasonicDecode::getOpMode(uint8_t input)
    {
      switch ((int)(input & 0b111111))
      {
        case 18: return 0;  // heat-only
        case 19: return 1;  // cool-only
        case 24: return 2;  // auto
        case 25: return 3;  // auto-heat
        case 26: return 4;  // auto-cool
        case 33: return 5;  // dhw-only
        case 34: return 6;  // heat+dhw
        case 35: return 7;  // cool+dhw
        case 40: return 8;  // auto-dhw
        case 41: return 9;  // auto-heat+dhw
        case 42: return 10; // auto-cool+dhw
        default: return -1; // unknown
      }
    }

    // TOP92 //
    int PanasonicDecode::getModel(const std::vector<uint8_t>& data, uint8_t index)
    {
      // Ensure there are at least 10 bytes in data message after the start index
      if (index + 10 > data.size()) return -1;
      // Get model bytes from data message
      auto model = std::vector<uint8_t>(data.begin() + index, data.begin() + index + 10);
      // Search model bytes in the list of known models and return the index
      for (size_t i = 0; i < NUMBER_OF_MODELS; i++)
      {
        if (std::equal(KnownModels[i], KnownModels[i] + 10, model.begin()))
        {
          return static_cast<int>(i);
        }
      }

      return -1;
    }

    // TOP1 //
    // input1 = data[169]
    // input2 = data[170]
    float PanasonicDecode::getPumpFlow(uint8_t input1, uint8_t input2)
    {
      return (((float)input1 - 1) / 256) + ((int)input2);
    }

    // TOP5, TOP6 //
    float PanasonicDecode::getFractional(uint8_t input, uint8_t shift)
    {
      return ((int)((input >> shift) & 0b111) - 1) * 0.25;
    }

    // TOP44 //
    // errorType = data[113]
    // errorNumber = data[114]
    std::string PanasonicDecode::getErrorInfo(uint8_t errorType, uint8_t errorNumber)
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

    bool PanasonicDecode::getBinaryState(uint8_t input)
    {
      return input != 0;
    }

    std::string PanasonicDecode::getTextState(const char* const array[], int index)
    {
      int size = atoi(array[0]);

      if ((index < 0) || (index >= size))
      {
        return "UNKNOWN";
      }

      return array[index + 1];
    }
  }  // namespace panasonic_heatpump
}  // namespace esphome
