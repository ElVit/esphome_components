#pragma once
#include <cstring>
#include <string>
#include <vector>

#ifndef RESPONSE_MSG_SIZE
#define RESPONSE_MSG_SIZE 203
#endif
#ifndef NUMBER_OF_MODELS
#define NUMBER_OF_MODELS 55
#endif


namespace esphome
{
  namespace panasonic_heatpump
  {
    class PanasonicDecode
    {
    public:
      PanasonicDecode() = delete;

      static int getBit1(uint8_t input);
      static int getBit1and2(uint8_t input);
      static int getBit3and4(uint8_t input);
      static int getBit5and6(uint8_t input);
      static int getBit7and8(uint8_t input);
      static int getBit3and4and5(uint8_t input);
      static int getBit6and7and8(uint8_t input);
      static int getByteMinus1(uint8_t input);
      static int getByteMinus128(uint8_t input);
      static float getByteMinus1Div2(uint8_t input);
      static float getByteMinus1Div5(uint8_t input);
      static float getByteMinus1Div50(uint8_t input);
      static int getByteMinus1Times10(uint8_t input);
      static int getByteMinus1Times50(uint8_t input);
      static int getByteMinus1Times200(uint8_t input);
      static int getHighNibbleMinus1(uint8_t input);
      static int getLowNibbleMinus1(uint8_t input);
      static int getWordMinus1(const std::vector<uint8_t>& data, uint8_t index);
      static int getUintt16(const std::vector<uint8_t>& data, uint8_t index);

      static int getOperationMode(uint8_t input);
      static int getModel(const std::vector<uint8_t>& data, uint8_t index);
      static float getPumpFlow(const std::vector<uint8_t>& data, uint8_t index);
      static float getFractional(uint8_t input, uint8_t shift);
      static std::string getErrorInfo(uint8_t errorType, uint8_t errorNumber);

      static bool getBinaryState(uint8_t input);
      static std::string getTextState(const char* const array[], int index);

      static const constexpr char* const DisabledEnabled[] = { "2", "Disabled", "Enabled" };
      static const constexpr char* const BlockedFree[] = { "2", "Blocked", "Free" };
      static const constexpr char* const OffOn[] = { "2", "Off", "On" };
      static const constexpr char* const InactiveActive[] = { "2", "Inactive", "Active" };
      static const constexpr char* const PumpFlowRateMode[] = { "2", "DeltaT", "Max flow" };
      static const constexpr char* const ThreeWayValve[] = { "2", "Buffer Tank", "DHW Tank" };
      static const constexpr char* const TwoWayValve[] = { "2", "Cool", "Heat" };
      static const constexpr char* const WaterTempControl[] = { "2", "Comp. Curve", "Direct" };
      static const constexpr char* const LiquidType[] = { "2", "Water", "Glycol" };
      static const constexpr char* const HolidayState[] = { "3", "Off", "Scheduled", "Active" };
      static const constexpr char* const SolarMode[] = { "3", "Disabled", "Buffer Tank", "DHW Tank" };
      static const constexpr char* const MixingValve[] = { "4", "Off", "Decrease","Increase", "Invalid" };
      static const constexpr char* const ZoneSensorType[] = { "4", "Water Temperature", "External Thermostat", "Internal Thermostat", "Thermistor" };
      static const constexpr char* const QuietMode[] = { "4", "Off", "Level 1", "Level 2", "Level 3" };
      static const constexpr char* const PowerfulMode[] = { "4", "Off", "30min", "60min", "90min" };
      static const constexpr char* const OperationMode[] =
      {
        "11", "TANK", "HEAT", "HEAT+TANK", "COOL", "COOL+TANK", "AUTO", "AUTO+TANK",
        "AUTO(HEAT)", "AUTO(HEAT)+TANK", "AUTO(COOL)", "AUTO(COOL)+TANK"
      };
      static const constexpr char* const ZoneState[] = { "3", "Zone 1", "Zone 2", "Zone 1 & 2" };
      static const constexpr char* const ExtPadHeaterType[] = { "3", "Disabled", "Type-A", "Type-B" };
      static const constexpr char* const BivalentMode[] = { "3", "Alternative", "Parallel", "Advanced Parallel" };
      static const constexpr char* const ModelNames[] =
      {
        "55", // string representation of number of known models (last model number + 1)
        "WH-MDC05H3E5", // 0
        "WH-MDC07H3E5", // 1
        "IDU:WH-SXC09H3E5, ODU:WH-UX09HE5", // 2
        "IDU:WH-SDC09H3E8, ODU:WH-UD09HE8", // 3
        "IDU:WH-SXC09H3E8, ODU:WH-UX09HE8", // 4
        "IDU:WH-SXC12H9E8, ODU:WH-UX12HE8", // 5
        "IDU:WH-SXC16H9E8, ODU:WH-UX16HE8", // 6
        "IDU:WH-SDC05H3E5, ODU:WH-UD05HE5", // 7
        "IDU:WH-SDC0709J3E5, ODU:WH-UD09JE5", // 8
        "WH-MDC05J3E5", // 9
        "WH-MDC09H3E5", // 10
        "WH-MXC09H3E5", // 11
        "IDU:WH-ADC0309J3E5, ODU:WH-UD09JE5", // 12
        "IDU:WH-ADC0916H9E8, ODU:WH-UX12HE8", // 13
        "IDU:WH-SQC09H3E8, ODU:WH-UQ09HE8", // 14
        "IDU:WH-SDC09H3E5, ODU:WH-UD09HE5", // 15
        "IDU:WH-ADC0309H3E5, ODU:WH-UD09HE5", // 16
        "IDU:WH-ADC0309J3E5, ODU:WH-UD05JE5", // 17
        "IDU:WH-SDC0709J3E5, ODU:WH-UD07JE5", // 18
        "IDU:WH-SDC07H3E5-1, ODU:WH-UD07HE5-1", // 19
        "WH-MDC07J3E5", // 20
        "WH-MDC09J3E5", // 21
        "IDU:WH-SDC0305J3E5, ODU:WH-UD05JE5", // 22
        "WH-MXC09J3E8", // 23
        "WH-MXC12J9E8", // 24
        "IDU:WH-ADC1216H6E5, ODU:WH-UD12HE5", // 25
        "IDU:WH-ADC0309J3E5C, ODU:WH-UD07JE5", // 26
        "WH-MDC07J3E5", // 27
        "WH-MDC05J3E5", // 28
        "IDU:WH-SQC12H9E8, ODU:WH-UQ12HE8", // 29
        "IDU:WH-SXC12H6E5, ODU:WH-UX12HE5", // 30
        "WH-MDC09J3E5", // 31
        "WH-MXC09J3E5", // 32
        "IDU:WH-ADC1216H6E5C, ODU:WH-UD12HE5", // 33
        "IDU:WH-ADC0509L3E5, ODU:WH-WDG07LE5", // 34
        "IDU:WH-SXC09H3E8, ODU:WH-UX09HE8", // 35
        "IDU:WH-ADC0309K3E5AN, ODU:WH-UDZ07KE5", // 36
        "IDU:WH-SDC0309K3E5, ODU:WH-UDZ05KE5", // 37
        "IDU:WH-SDC0509L3E5, ODU:WH-WDG09LE5", // 38
        "IDU:WH-SDC12H9E8, ODU:WH-UD12HE8", // 39
        "IDU:WH-SDC0309K3E5, ODU:WH-UDZ07KE5", // 40
        "IDU:WH-ADC0916H9E8, ODU:WH-UX16HE8", // 41
        "IDU:WH-ADC0912H9E8, ODU:WH-UX12HE8", // 42
        "WH-MXC16J9E8", // 43
        "WH-MXC12J6E5", // 44
        "IDU:WH-SQC09H3E8, ODU:WH-UQ09HE8", // 45
        "IDU:WH-ADC0309K3E5, ODU:WH-UDZ09KE5", // 46
        "IDU:WH-ADC0916H9E8, ODU:WH-UX12HE8", // 47
        "IDU:WH-SDC0509L3E5, ODU:WH-WDG07LE5", // 48
        "IDU:WH-SXC09H3E5, ODU:WH-UX09HE5", // 49
        "WH-MXC12H9E8", // 50
        "IDU:WH-ADC0509L3E5AN, ODU:WH-WDG05LE5", // 51
        "IDU:WH-SDC0509L3E5, ODU:WH-WDG05LE5", // 52
        "IDU:WH-ADC0309J3E5C, ODU:WH-UD03JE5", // 53
        "IDU:WH-ADC0916H9E8, ODU:WH-UX09HE8", // 54
      };
    };

    extern const uint8_t KnownModels[NUMBER_OF_MODELS][10];
  }  // namespace panasonic_heatpump
}  // namespace esphome
