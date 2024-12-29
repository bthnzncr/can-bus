// Copyright (c) Sandeep Mistry. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
// Updated by Batuhan Zencir.
#include <math.h>
#include <esp32_can.h>
#include "esp32_obd2.h"

#define PROGMEM

// Standard addressing CAN IDs
#define OBD2_STD_ID_REQ 0x7DF
#define OBD2_STD_ID_RES_MIN 0x7E8
#define OBD2_STD_ID_RES_MAX 0x7EF

// Extended addressing CAN IDs (29-bit)
#define OBD2_EXT_ID_REQ 0x18DB33F1
#define OBD2_EXT_ID_RES_MIN 0x18DAF100
#define OBD2_EXT_ID_RES_MAX 0x18DAF1FF

const char PID_NAME_0x00[] PROGMEM = "PIDs supported [01 - 20]";
const char PID_NAME_0x01[] PROGMEM = "Monitor status since DTCs cleared";
const char PID_NAME_0x02[] PROGMEM = "Freeze DTC";
const char PID_NAME_0x03[] PROGMEM = "Fuel system status";
const char PID_NAME_0x04[] PROGMEM = "Calculated engine load";
const char PID_NAME_0x05[] PROGMEM = "Engine coolant temperature";
const char PID_NAME_0x06[] PROGMEM = "Short term fuel trim — Bank 1";
const char PID_NAME_0x07[] PROGMEM = "Long term fuel trim — Bank 1";
const char PID_NAME_0x08[] PROGMEM = "Short term fuel trim — Bank 2";
const char PID_NAME_0x09[] PROGMEM = "Long term fuel trim — Bank 2";
const char PID_NAME_0x0a[] PROGMEM = "Fuel pressure";
const char PID_NAME_0x0b[] PROGMEM = "Intake manifold absolute pressure";
const char PID_NAME_0x0c[] PROGMEM = "Engine RPM";
const char PID_NAME_0x0d[] PROGMEM = "Vehicle speed";
const char PID_NAME_0x0e[] PROGMEM = "Timing advance";
const char PID_NAME_0x0f[] PROGMEM = "Intake air temperature";
const char PID_NAME_0x10[] PROGMEM = "MAF air flow rate";
const char PID_NAME_0x11[] PROGMEM = "Throttle position";
const char PID_NAME_0x12[] PROGMEM = "Commanded secondary air status";
const char PID_NAME_0x13[] PROGMEM = "Oxygen sensors present (in 2 banks)";
const char PID_NAME_0x14[] PROGMEM = "Oxygen Sensor 1 - Short term fuel trim";
const char PID_NAME_0x15[] PROGMEM = "Oxygen Sensor 2 - Short term fuel trim";
const char PID_NAME_0x16[] PROGMEM = "Oxygen Sensor 3 - Short term fuel trim";
const char PID_NAME_0x17[] PROGMEM = "Oxygen Sensor 4 - Short term fuel trim";
const char PID_NAME_0x18[] PROGMEM = "Oxygen Sensor 5 - Short term fuel trim";
const char PID_NAME_0x19[] PROGMEM = "Oxygen Sensor 6 - Short term fuel trim";
const char PID_NAME_0x1a[] PROGMEM = "Oxygen Sensor 7 - Short term fuel trim";
const char PID_NAME_0x1b[] PROGMEM = "Oxygen Sensor 8 - Short term fuel trim";
const char PID_NAME_0x1c[] PROGMEM = "OBD standards this vehicle conforms to";
const char PID_NAME_0x1d[] PROGMEM = "Oxygen sensors present (in 4 banks)";
const char PID_NAME_0x1e[] PROGMEM = "Auxiliary input status";
const char PID_NAME_0x1f[] PROGMEM = "Run time since engine start";
const char PID_NAME_0x20[] PROGMEM = "PIDs supported [21 - 40]";
const char PID_NAME_0x21[] PROGMEM = "Distance traveled with malfunction indicator lamp (MIL) on";
const char PID_NAME_0x22[] PROGMEM = "Fuel Rail Pressure (relative to manifold vacuum)";
const char PID_NAME_0x23[] PROGMEM = "Fuel Rail Gauge Pressure (diesel, or gasoline direct injection)";
const char PID_NAME_0x24[] PROGMEM = "Oxygen Sensor 1 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x25[] PROGMEM = "Oxygen Sensor 2 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x26[] PROGMEM = "Oxygen Sensor 3 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x27[] PROGMEM = "Oxygen Sensor 4 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x28[] PROGMEM = "Oxygen Sensor 5 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x29[] PROGMEM = "Oxygen Sensor 6 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x2a[] PROGMEM = "Oxygen Sensor 7 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x2b[] PROGMEM = "Oxygen Sensor 8 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x2c[] PROGMEM = "Commanded EGR";
const char PID_NAME_0x2d[] PROGMEM = "EGR Error";
const char PID_NAME_0x2e[] PROGMEM = "Commanded evaporative purge";
const char PID_NAME_0x2f[] PROGMEM = "Fuel Tank Level Input";
const char PID_NAME_0x30[] PROGMEM = "Warm-ups since codes cleared";
const char PID_NAME_0x31[] PROGMEM = "Distance traveled since codes cleared";
const char PID_NAME_0x32[] PROGMEM = "Evap. System Vapor Pressure";
const char PID_NAME_0x33[] PROGMEM = "Absolute Barometric Pressure";
const char PID_NAME_0x34[] PROGMEM = "Oxygen Sensor 1 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x35[] PROGMEM = "Oxygen Sensor 2 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x36[] PROGMEM = "Oxygen Sensor 3 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x37[] PROGMEM = "Oxygen Sensor 4 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x38[] PROGMEM = "Oxygen Sensor 5 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x39[] PROGMEM = "Oxygen Sensor 6 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x3a[] PROGMEM = "Oxygen Sensor 7 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x3b[] PROGMEM = "Oxygen Sensor 8 - Fuel–Air Equivalence Ratio";
const char PID_NAME_0x3c[] PROGMEM = "Catalyst Temperature: Bank 1, Sensor 1";
const char PID_NAME_0x3d[] PROGMEM = "Catalyst Temperature: Bank 2, Sensor 1";
const char PID_NAME_0x3e[] PROGMEM = "Catalyst Temperature: Bank 1, Sensor 2";
const char PID_NAME_0x3f[] PROGMEM = "Catalyst Temperature: Bank 2, Sensor 2";
const char PID_NAME_0x40[] PROGMEM = "PIDs supported [41 - 60]";
const char PID_NAME_0x41[] PROGMEM = "Monitor status this drive cycle";
const char PID_NAME_0x42[] PROGMEM = "Control module voltage";
const char PID_NAME_0x43[] PROGMEM = "Absolute load value";
const char PID_NAME_0x44[] PROGMEM = "Fuel–Air commanded equivalence ratio";
const char PID_NAME_0x45[] PROGMEM = "Relative throttle position";
const char PID_NAME_0x46[] PROGMEM = "Ambient air temperature";
const char PID_NAME_0x47[] PROGMEM = "Absolute throttle position B";
const char PID_NAME_0x48[] PROGMEM = "Absolute throttle position C";
const char PID_NAME_0x49[] PROGMEM = "Absolute throttle position D";
const char PID_NAME_0x4a[] PROGMEM = "Absolute throttle position E";
const char PID_NAME_0x4b[] PROGMEM = "Absolute throttle position F";
const char PID_NAME_0x4c[] PROGMEM = "Commanded throttle actuator";
const char PID_NAME_0x4d[] PROGMEM = "Time run with MIL on";
const char PID_NAME_0x4e[] PROGMEM = "Time since trouble codes cleared";
const char PID_NAME_0x4f[] PROGMEM = "Maximum value for Fuel–Air equivalence ratio, oxygen sensor voltage, oxygen sensor current, and intake manifold absolute pressure";
const char PID_NAME_0x50[] PROGMEM = "Maximum value for air flow rate from mass air flow sensor";
const char PID_NAME_0x51[] PROGMEM = "Fuel Type";
const char PID_NAME_0x52[] PROGMEM = "Ethanol fuel percentage";
const char PID_NAME_0x53[] PROGMEM = "Absolute Evap system Vapor Pressure";
const char PID_NAME_0x54[] PROGMEM = "Evap system vapor pressure";
const char PID_NAME_0x55[] PROGMEM = "Short term secondary oxygen sensor trim";
const char PID_NAME_0x56[] PROGMEM = "Long term secondary oxygen sensor trim";
const char PID_NAME_0x57[] PROGMEM = "Short term secondary oxygen sensor trim";
const char PID_NAME_0x58[] PROGMEM = "Long term secondary oxygen sensor trim";
const char PID_NAME_0x59[] PROGMEM = "Fuel rail absolute pressure";
const char PID_NAME_0x5a[] PROGMEM = "Relative accelerator pedal position";
const char PID_NAME_0x5b[] PROGMEM = "Hybrid battery pack remaining life";
const char PID_NAME_0x5c[] PROGMEM = "Engine oil temperature";
const char PID_NAME_0x5d[] PROGMEM = "Fuel injection timing";
const char PID_NAME_0x5e[] PROGMEM = "Engine fuel rate";
const char PID_NAME_0x5f[] PROGMEM = "Emission requirements to which vehicle is designed";

const char *const PID_NAME_MAPPER[] PROGMEM = {
    PID_NAME_0x00,
    PID_NAME_0x01,
    PID_NAME_0x02,
    PID_NAME_0x03,
    PID_NAME_0x04,
    PID_NAME_0x05,
    PID_NAME_0x06,
    PID_NAME_0x07,
    PID_NAME_0x08,
    PID_NAME_0x09,
    PID_NAME_0x0a,
    PID_NAME_0x0b,
    PID_NAME_0x0c,
    PID_NAME_0x0d,
    PID_NAME_0x0e,
    PID_NAME_0x0f,
    PID_NAME_0x10,
    PID_NAME_0x11,
    PID_NAME_0x12,
    PID_NAME_0x13,
    PID_NAME_0x14,
    PID_NAME_0x15,
    PID_NAME_0x16,
    PID_NAME_0x17,
    PID_NAME_0x18,
    PID_NAME_0x19,
    PID_NAME_0x1a,
    PID_NAME_0x1b,
    PID_NAME_0x1c,
    PID_NAME_0x1d,
    PID_NAME_0x1e,
    PID_NAME_0x1f,
    PID_NAME_0x20,
    PID_NAME_0x21,
    PID_NAME_0x22,
    PID_NAME_0x23,
    PID_NAME_0x24,
    PID_NAME_0x25,
    PID_NAME_0x26,
    PID_NAME_0x27,
    PID_NAME_0x28,
    PID_NAME_0x29,
    PID_NAME_0x2a,
    PID_NAME_0x2b,
    PID_NAME_0x2c,
    PID_NAME_0x2d,
    PID_NAME_0x2e,
    PID_NAME_0x2f,
    PID_NAME_0x30,
    PID_NAME_0x31,
    PID_NAME_0x32,
    PID_NAME_0x33,
    PID_NAME_0x34,
    PID_NAME_0x35,
    PID_NAME_0x36,
    PID_NAME_0x37,
    PID_NAME_0x38,
    PID_NAME_0x39,
    PID_NAME_0x3a,
    PID_NAME_0x3b,
    PID_NAME_0x3c,
    PID_NAME_0x3d,
    PID_NAME_0x3e,
    PID_NAME_0x3f,
    PID_NAME_0x40,
    PID_NAME_0x41,
    PID_NAME_0x42,
    PID_NAME_0x43,
    PID_NAME_0x44,
    PID_NAME_0x45,
    PID_NAME_0x46,
    PID_NAME_0x47,
    PID_NAME_0x48,
    PID_NAME_0x49,
    PID_NAME_0x4a,
    PID_NAME_0x4b,
    PID_NAME_0x4c,
    PID_NAME_0x4d,
    PID_NAME_0x4e,
    PID_NAME_0x4f,
    PID_NAME_0x50,
    PID_NAME_0x51,
    PID_NAME_0x52,
    PID_NAME_0x53,
    PID_NAME_0x54,
    PID_NAME_0x55,
    PID_NAME_0x56,
    PID_NAME_0x57,
    PID_NAME_0x58,
    PID_NAME_0x59,
    PID_NAME_0x5a,
    PID_NAME_0x5b,
    PID_NAME_0x5c,
    PID_NAME_0x5d,
    PID_NAME_0x5e,
    PID_NAME_0x5f,
};

const char PERCENTAGE[] PROGMEM = "%";
const char KPA[] PROGMEM = "kPa";
const char PA[] PROGMEM = "Pa";
const char RPM[] PROGMEM = "rpm";
const char KPH[] PROGMEM = "km/h";
const char DEGREES_BEFORE_TDC[] PROGMEM = "° before TDC";
const char GRAMS_PER_SECOND[] PROGMEM = "grams/sec";
const char SECONDS[] PROGMEM = "seconds";
const char RATIO[] PROGMEM = "ratio";
const char COUNT[] PROGMEM = "count";
const char KM[] PROGMEM = "km";
const char VOLTS[] PROGMEM = "V";
const char MINUTES[] PROGMEM = "minutes";
const char GPS[] PROGMEM = "g/s";
const char DEGREES[] PROGMEM = "°";
const char DEGREES_CELCIUS[] PROGMEM = "°C";
const char LPH[] PROGMEM = "L/h";

const char *const PID_UNIT_MAPPER[] PROGMEM = {
    NULL,
    NULL,
    NULL,
    NULL,
    PERCENTAGE,
    DEGREES_CELCIUS,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    KPA,
    KPA,
    RPM,
    KPH,
    DEGREES_BEFORE_TDC,
    DEGREES_CELCIUS,
    GRAMS_PER_SECOND,
    PERCENTAGE,
    NULL,
    NULL,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    NULL,
    NULL,
    NULL,
    SECONDS,
    NULL,
    KM,
    KPA,
    KPA,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    COUNT,
    KM,
    PA,
    KPA,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    RATIO,
    DEGREES_CELCIUS,
    DEGREES_CELCIUS,
    DEGREES_CELCIUS,
    DEGREES_CELCIUS,
    NULL,
    NULL,
    VOLTS,
    PERCENTAGE,
    RATIO,
    PERCENTAGE,
    DEGREES_CELCIUS,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    MINUTES,
    MINUTES,
    NULL,
    GPS,
    NULL,
    PERCENTAGE,
    KPA,
    PA,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    PERCENTAGE,
    KPA,
    PERCENTAGE,
    PERCENTAGE,
    DEGREES_CELCIUS,
    DEGREES,
    LPH,
    NULL,
};

OBD2Class::OBD2Class() : _responseTimeout(OBD2_DEFAULT_TIMEOUT),
                         _lastPidResponseMillis(0),
                         _useExtendedAddressing(false)
{
  memset(_supportedPids, 0x00, sizeof(_supportedPids));
}

OBD2Class::~OBD2Class()
{
}

int OBD2Class::begin()
{
  if (!CAN0.begin(CAN_BPS_500K))
  {
    return 0;
  }

  memset(_supportedPids, 0x00, sizeof(_supportedPids));

  // First try standard addressing
  _useExtendedAddressing = false;
  CAN0.watchFor(OBD2_STD_ID_RES_MIN, OBD2_STD_ID_RES_MAX);

  if (!supportedPidsRead())
  {
    // Next try extended addressing
    _useExtendedAddressing = true;
    CAN0.watchFor(OBD2_EXT_ID_RES_MIN, OBD2_EXT_ID_RES_MAX);

    if (!supportedPidsRead())
    {
      return 0;
    }
  }

  return 1;
}

void OBD2Class::end()
{
}

bool OBD2Class::pidSupported(uint8_t pid)
{
  if (pid == 0)
  {
    return true;
  }

  return (_supportedPids[(pid - 1) / 0x20] & ((1UL << (31 - ((pid - 1) % 32))))) ? true : false;
}

bool OBD2Class::pidValueRaw(uint8_t pid)
{
  switch (pid)
  {
  case PIDS_SUPPORT_01_20:                                // raw
  case MONITOR_STATUS_SINCE_DTCS_CLEARED:                 // raw
  case FREEZE_DTC:                                        // raw
  case PIDS_SUPPORT_21_40:                                // raw
  case PIDS_SUPPORT_41_60:                                // raw
  case MONITOR_STATUS_THIS_DRIVE_CYCLE:                   // raw
  case FUEL_SYSTEM_STATUS:                                // raw
  case COMMANDED_SECONDARY_AIR_STATUS:                    // raw
  case OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO:            // raw
  case OXYGEN_SENSORS_PRESENT_IN_2_BANKS:                 // raw
  case OXYGEN_SENSORS_PRESENT_IN_4_BANKS:                 // raw
  case AUXILIARY_INPUT_STATUS:                            // raw
  case FUEL_TYPE:                                         // raw
  case EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED: // raw
    return true;

  default:
    return (pid > 0x5f);
  }
}

String OBD2Class::pidName(uint8_t pid)
{
  if (pid > 0x5f)
  {
    return "Unknown";
  }
  return PID_NAME_MAPPER[pid];
}

String OBD2Class::pidUnits(uint8_t pid)
{
  if (pid > 0x5f)
  {
    return "";
  }

  return PID_UNIT_MAPPER[pid];
}

float OBD2Class::pidRead(uint8_t pid)
{
  if (!pidSupported(pid))
  {
    return NAN;
  }

#define A value[0]
#define B value[1]
#define C value[2]
#define D value[3]
  uint8_t value[4];
  CAN0.watchFor(0x7e8);
  if (!pidRead(0x01, pid, &value, sizeof(value)))
  {
    return NAN;
  }

  switch (pid)
  {
  default:
  case PIDS_SUPPORT_01_20:                // raw
  case MONITOR_STATUS_SINCE_DTCS_CLEARED: // raw
  case FREEZE_DTC:                        // raw
  case PIDS_SUPPORT_21_40:                // raw
  case PIDS_SUPPORT_41_60:                // raw
  case MONITOR_STATUS_THIS_DRIVE_CYCLE:   // raw
    // NOTE: return value can lose precision!
    return ((uint32_t)A << 24 | (uint32_t)B << 16 | (uint32_t)C << 8 | (uint32_t)D);

  case FUEL_SYSTEM_STATUS: // raw
  case RUN_TIME_SINCE_ENGINE_START:
  case DISTANCE_TRAVELED_WITH_MIL_ON:
  case DISTANCE_TRAVELED_SINCE_CODES_CLEARED:
  case TIME_RUN_WITH_MIL_ON:
  case TIME_SINCE_TROUBLE_CODES_CLEARED:
    return (A * 256.0 + B);

  case CALCULATED_ENGINE_LOAD:
  case THROTTLE_POSITION:
  case COMMANDED_EGR:
  case COMMANDED_EVAPORATIVE_PURGE:
  case FUEL_TANK_LEVEL_INPUT:
  case RELATIVE_THROTTLE_POSITION:
  case ABSOLUTE_THROTTLE_POSITION_B:
  case ABSOLUTE_THROTTLE_POSITION_C:
  case ABSOLUTE_THROTTLE_POSITION_D:
  case ABSOLUTE_THROTTLE_POSITION_E:
  case ABSOLUTE_THROTTLE_POSITION_F:
  case COMMANDED_THROTTLE_ACTUATOR:
  case ETHANOL_FUEL_PERCENTAGE:
  case RELATIVE_ACCELERATOR_PEDAL_POSITTION:
  case HYBRID_BATTERY_PACK_REMAINING_LIFE:
    return (A / 2.55);

  case COMMANDED_SECONDARY_AIR_STATUS:                    // raw
  case OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO:            // raw
  case OXYGEN_SENSORS_PRESENT_IN_2_BANKS:                 // raw
  case OXYGEN_SENSORS_PRESENT_IN_4_BANKS:                 // raw
  case AUXILIARY_INPUT_STATUS:                            // raw
  case FUEL_TYPE:                                         // raw
  case EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED: // raw
    return (A);

  case OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM:
  case OXYGEN_SENSOR_2_SHORT_TERM_FUEL_TRIM:
  case OXYGEN_SENSOR_3_SHORT_TERM_FUEL_TRIM:
  case OXYGEN_SENSOR_4_SHORT_TERM_FUEL_TRIM:
  case OXYGEN_SENSOR_5_SHORT_TERM_FUEL_TRIM:
  case OXYGEN_SENSOR_6_SHORT_TERM_FUEL_TRIM:
  case OXYGEN_SENSOR_7_SHORT_TERM_FUEL_TRIM:
  case OXYGEN_SENSOR_8_SHORT_TERM_FUEL_TRIM:
    return ((B / 1.28) - 100.0);
    break;

  case ENGINE_COOLANT_TEMPERATURE:
  case AIR_INTAKE_TEMPERATURE:
  case AMBIENT_AIR_TEMPERATURE:
  case ENGINE_OIL_TEMPERATURE:
    return (A - 40.0);

  case SHORT_TERM_FUEL_TRIM_BANK_1:
  case LONG_TERM_FUEL_TRIM_BANK_1:
  case SHORT_TERM_FUEL_TRIM_BANK_2:
  case LONG_TERM_FUEL_TRIM_BANK_2:
  case EGR_ERROR:
    return ((A / 1.28) - 100.0);

  case FUEL_PRESSURE:
    return (A * 3.0);

  case INTAKE_MANIFOLD_ABSOLUTE_PRESSURE:
  case VEHICLE_SPEED:
  case WARM_UPS_SINCE_CODES_CLEARED:
  case ABSOLUTE_BAROMETRIC_PRESSURE:
    return (A);

  case ENGINE_RPM:
    return ((A * 256.0 + B) / 4.0);

  case TIMING_ADVANCE:
    return ((A / 2.0) - 64.0);

  case MAF_AIR_FLOW_RATE:
    return ((A * 256.0 + B) / 100.0);

  case FUEL_RAIL_PRESSURE:
    return ((A * 256.0 + B) * 0.079);

  case FUEL_RAIL_GAUGE_PRESSURE:
  case FUEL_RAIL_ABSOLUTE_PRESSURE:
    return ((A * 256.0 + B) * 10.0);

  case OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO:
  case OXYGEN_SENSOR_2_FUEL_AIR_EQUIVALENCE_RATIO:
  case OXYGEN_SENSOR_3_FUEL_AIR_EQUIVALENCE_RATIO:
  case OXYGEN_SENSOR_4_FUEL_AIR_EQUIVALENCE_RATIO:
  case OXYGEN_SENSOR_5_FUEL_AIR_EQUIVALENCE_RATIO:
  case OXYGEN_SENSOR_6_FUEL_AIR_EQUIVALENCE_RATIO:
  case OXYGEN_SENSOR_7_FUEL_AIR_EQUIVALENCE_RATIO:
  case OXYGEN_SENSOR_8_FUEL_AIR_EQUIVALENCE_RATIO:
  case 0x34:
  case 0x35:
  case 0x36:
  case 0x37:
  case 0x38:
  case 0x39:
  case 0x3a:
  case 0x3b:
    return (((A * 256.0 + B) * 2.0) / 65536.0);

  case EVAP_SYSTEM_VAPOR_PRESSURE:
    return (((int16_t)(A * 256.0 + B)) / 4.0);

  case CATALYST_TEMPERATURE_BANK_1_SENSOR_1:
  case CATALYST_TEMPERATURE_BANK_2_SENSOR_1:
  case CATALYST_TEMPERATURE_BANK_1_SENSOR_2:
  case CATALYST_TEMPERATURE_BANK_2_SENSOR_2:
    return (((A * 256.0 + B) / 10.0) - 40.0);

  case CONTROL_MODULE_VOLTAGE:
    return ((A * 256.0 + B) / 1000.0);

  case ABSOLUTE_LOAD_VALUE:
    return ((A * 256.0 + B) / 2.55);

  case FUEL_AIR_COMMANDED_EQUIVALENCE_RATE:
    return (2.0 * (A * 256.0 + B) / 65536.0);

  case ABSOLUTE_EVAP_SYSTEM_VAPOR_PRESSURE:
    return ((A * 256.0 + B) / 200.0);

  case 0x54:
    return ((A * 256.0 + B) - 32767.0);

  case FUEL_INJECTION_TIMING:
    return (((A * 256.0 + B) / 128.0) - 210.0);

  case ENGINE_FUEL_RATE:
    return ((A * 256.0 + B) / 20.0);
  }
}

String OBD2Class::vinRead()
{
  char vin[18];

  memset(vin, 0x00, sizeof(vin));

  if (!pidRead(0x09, 0x02, vin, 17))
  {
    // failed
    return "";
  }

  return vin;
}

uint32_t OBD2Class::pidReadRaw(uint8_t pid)
{
  if (!pidSupported(pid))
  {
    return 0;
  }

#define A value[0]
#define B value[1]
#define C value[2]
#define D value[3]
  uint8_t value[4];

  if (!pidRead(0x01, pid, &value, sizeof(value)))
  {
    return 0;
  }

  switch (pid)
  {
  case COMMANDED_SECONDARY_AIR_STATUS:
  case OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO:
  case OXYGEN_SENSORS_PRESENT_IN_2_BANKS:
  case OXYGEN_SENSORS_PRESENT_IN_4_BANKS:
  case AUXILIARY_INPUT_STATUS:
  case FUEL_TYPE:
  case EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED:
    return (A);

  case FUEL_SYSTEM_STATUS:
    return ((uint32_t)A << 8 | (uint32_t)B);

  default:
    return ((uint32_t)A << 24 | (uint32_t)B << 16 | (uint32_t)C << 8 | (uint32_t)D);
  }
}

String OBD2Class::ecuNameRead()
{
  CAN0.watchFor(0x7e8);
  char ecuName[21];

  memset(ecuName, 0x00, sizeof(ecuName));

  if (!pidRead(0x09, 0x0a, ecuName, 20))
  {
    // failed
    return "";
  }

  return ecuName;
}
String OBD2Class::dtcRead()
{
  CAN0.watchFor(0x7e8, 0x7f8);
  uint8_t data[512];
  memset(data, 0, sizeof(data));

  // Attempt to read Mode 03 data. This should place only the DTC bytes into 'data'.
  int length = pidReadForDtc(0x03, data, sizeof(data));

  // Each DTC is represented by 2 bytes, so find how many DTCs are present.
  int dtcCount = length / 2;

  if (dtcCount == 0)
  {
    // No DTCs
    return "No DTC Found";
  }

  // Parse each pair of bytes to form a DTC code
  String dtcList;
  for (int i = 0; i < dtcCount; i++)
  {
    uint8_t firstByte = data[i * 2];
    uint8_t secondByte = data[i * 2 + 1];

    // The top two bits of the first byte determine the code category:
    // 00 = P (Powertrain), 01 = C (Chassis), 10 = B (Body), 11 = U (Network)
    uint8_t codeType = firstByte >> 6;
    char codeLetter;
    switch (codeType)
    {
    case 0:
      codeLetter = 'P';
      break;
    case 1:
      codeLetter = 'C';
      break;
    case 2:
      codeLetter = 'B';
      break;
    case 3:
      codeLetter = 'U';
      break;
    }

    // The lower 14 bits form the numeric portion of the code.
    // Mask out the top two bits of the first byte and combine with the second byte.
    uint16_t codeNumber = ((firstByte & 0x3F) << 8) | secondByte;

    // Format the DTC code (e.g., P0300)
    char dtcCode[6]; // 5 chars plus null terminator
    sprintf(dtcCode, "%c%04u", codeLetter, codeNumber);

    // Append to the output list, separated by commas if there are multiple
    if (i > 0)
    {
      dtcList += ",";
    }
    dtcList += dtcCode;
  }

  return dtcList;
}

void OBD2Class::setTimeout(unsigned long timeout)
{
  _responseTimeout = timeout;
}

int OBD2Class::supportedPidsRead()
{
  for (int pid = 0x00; pid < 0xe0; pid += 0x20)
  {
    uint8_t value[4];

    if (pidRead(0x01, pid, value, sizeof(value)) != 4)
    {
      return 0;
    }

#define A value[0]
#define B value[1]
#define C value[2]
#define D value[3]
    _supportedPids[pid / 0x20] = ((uint32_t)A << 24 | (uint32_t)B << 16 | (uint32_t)C << 8 | (uint32_t)D << 0);

    if ((_supportedPids[pid / 0x20] & 0x00000001) == 0x00000000)
    {
      // no more
      break;
    }
  }

  return 1;
}
int OBD2Class::pidRead(uint8_t mode, uint8_t pid, void *data, int length) // except dtcs
{
  unsigned long lastResponseDelta = millis() - _lastPidResponseMillis;
  if (lastResponseDelta < 60)
  {
    delay(60 - lastResponseDelta);
  }

  CAN_FRAME outgoing;
  outgoing.id = _useExtendedAddressing ? OBD2_EXT_ID_REQ : OBD2_STD_ID_REQ;
  outgoing.length = 8;
  outgoing.extended = _useExtendedAddressing;
  outgoing.rtr = 0;
  outgoing.data.uint8[0] = 0x02;
  outgoing.data.uint8[1] = mode;
  outgoing.data.uint8[2] = pid;
  outgoing.data.uint8[3] = 0x00;
  outgoing.data.uint8[4] = 0x00;
  outgoing.data.uint8[5] = 0x00;
  outgoing.data.uint8[6] = 0x00;
  outgoing.data.uint8[7] = 0x00;

  for (int retries = 10; retries > 0; retries--)
  {
    if (CAN0.sendFrame(outgoing))
    {
      // send success
      break;
    }
    else if (retries <= 1)
    {
      return 0;
    }
  }

  CAN_FRAME incoming;
  for (unsigned long start = millis(); (millis() - start) < _responseTimeout;)
  {
    if (CAN0.read(incoming) != 0)
    {
      _lastPidResponseMillis = millis();

      // Check if the response ID is in valid range
      if (_useExtendedAddressing) {
        if (incoming.id < OBD2_EXT_ID_RES_MIN || incoming.id > OBD2_EXT_ID_RES_MAX) {
          continue;
        }
      } else {
        if (incoming.id < OBD2_STD_ID_RES_MIN || incoming.id > OBD2_STD_ID_RES_MAX) {
          continue;
        }
      }

      if (((incoming.data.uint8[0] & 0xF0) != 0x10) && incoming.data.uint8[1] == (mode | 0x40) && incoming.data.uint8[2] == pid)
      {
        for (uint8_t i = 0; i < 4; i++) // problematic
        {
          ((uint8_t *)data)[i] = incoming.data.uint8[i + 3];
        }
        return length;
      }

      // Is multiple packets
      if ((incoming.data.uint8[0] & 0xF0) == 0x10) // check for most significant nibble, if exist than it is first frame of multiple packets

      {

        int bytesRead = 0;
        int size = (((incoming.data.uint8[0] & 0x0F) << 8) | incoming.data.uint8[1]) - 3; // low nipple along with the [1] byte depicts length of the data. -3 for metadata including mode, pid requested and data info byte "1" for vin and ecuname

        while (bytesRead < 3)
        {
          ((uint8_t *)data)[bytesRead] = incoming.data.uint8[bytesRead + 5]; // read 3 byte from first frame
          bytesRead++;
        }
        // sent flow control packet
        incoming.data.uint8[0] = 0;
        incoming.data.uint8[1] = 0;
        incoming.data.uint8[2] = 0;
        incoming.data.uint8[3] = 0;
        incoming.data.uint8[4] = 0;
        incoming.data.uint8[5] = 0;
        incoming.data.uint8[6] = 0;
        incoming.data.uint8[7] = 0;
        CAN_FRAME outgoing;
        outgoing.id = _useExtendedAddressing ? (incoming.id - 8) : 0x7E0; // for ecu phsical???? what about others?
        outgoing.length = 8;
        outgoing.extended = _useExtendedAddressing;
        outgoing.rtr = 0;
        outgoing.data.uint8[0] = 0x30; // flow control byte
        outgoing.data.uint8[1] = 0x00;
        outgoing.data.uint8[2] = 0x00;
        outgoing.data.uint8[3] = 0x00;
        outgoing.data.uint8[4] = 0x00;
        outgoing.data.uint8[5] = 0x00;
        outgoing.data.uint8[6] = 0x00;
        outgoing.data.uint8[7] = 0x00;

        for (int retries = 10; retries > 0; retries--) // sent 10 more times if not successful
        {
          if (CAN0.sendFrame(outgoing))
          {
            // send success
            break;
          }
          else if (retries <= 1)
          {

            return 0;
          }
        }

        for (int pck = 0; bytesRead < size; pck++) // if we read the enough bytes (which is size) exit the loop, pck is for each consecutive frame which is intended to set low nipple of first byte of consecutive frame and wrap around 16
        {

          // Loop through rest of multiple packets

          // CAN_FRAME incoming; // do we need to shading here? if we do not will previos incoming variable pass the check below since it is set before?
          //  wait for (proper) response
          unsigned long start_time = millis();
          while (CAN0.read(incoming) == 0 || (incoming.data.uint8[0] != (0x20 | ((pck + 1) & 0x0F)))) // wait until packet with the true sequence number is received may be too strict check? || (incoming.data.uint8[0] != (0x20 | ((pck + 1) & 0x0F)))
          {

            if ((millis() - start_time) > _responseTimeout)
            { // exit if timeout occurred

              return 0;
            }
          }

          // Something recieved
          int remaining = size - bytesRead;             // how many bytes are still needed
          int toRead = (remaining < 7) ? remaining : 7; // read only what's left, max 7

          for (int i = 0; i < toRead; i++)
          {
            ((uint8_t *)data)[bytesRead++] = incoming.data.uint8[i + 1];
          }

          _lastPidResponseMillis = millis(); // set the last response timestamp
        }
        return bytesRead;
      }
    }
  }
  return 0;
}
const OBD2Class::DTCEntry OBD2Class::DTC[]  PROGMEM =  {
    {"B0001", "PCM Discrete Input Speed Signal Error"},
    {"B0004", "PCM Discrete Input Speed Signal Not Present"},
    {"B0005", "In Park Switch Circuit Malfunction"},
    {"B0012", "Right Front/Passenger Frontal Deployment Loop Circuit"},
    {"B0013", "Right Front/Passenger Frontal Deployment Loop Circuit"},
    {"B0014", "Right Front/Passenger Frontal Deployment Loop Circuit"},
    {"B0016", "Right Front/Passenger Frontal Deployment Loop (Single Stage or Stage 1) Resistance Low"},
    {"B0017", "Right Front/Passenger Frontal Deployment Loop (Single Stage or Stage 1) Open"},
    {"B0018", "Right Front/Passenger Frontal Deployment Loop (Single Stage or Stage 1) Short to Ground/Voltage Out of Range"},
    {"B0022", "Left Front/Driver Frontal Deployment Loop (Single Stage or Stage 1) Resistance Low"},
    {"B0024", "Left Front/Driver Frontal Deployment Loop (Single Stage or Stage 1) Short to Ground/Voltage Out of Range"},
    {"B0026", "Left Front/Driver Frontal Deployment Loop (Single Stage or Stage 1) Open"},
    {"B0028", "Right Front/Passenger Side Deployment Loop Resistance Low"},
    {"B0029", "Right Front/Passenger Side Deployment Loop Open"},
    {"B0030", "Right Front/Passenger Side Deployment Loop Short to Ground/Voltage Out of Range"},
    {"B0035", "ADS Closed/Shorted to Ground"},
    {"B0036", "ADS Open/Missing/Shorted to Battery"},
    {"B0037", "AUX switch closed/shorted to ground"},
    {"B0038", "AUX switch open/shorted to battery"},
    {"B0040", "Left Front/Driver Side Deployment Loop Resistance Low"},
    {"B0041", "Left Front/Driver Side Deployment Loop Open"},
    {"B0042", "Left Front/Driver Frontal Deployment Loop Circuit"},
    {"B0043", "Left Front/Driver Frontal Deployment Loop Circuit"},
    {"B0044", "Left Front/Driver Frontal Deployment Loop Circuit"},
    {"B0045", "Left Front Side Deploy Loop Short to Ground/Voltage Out of Range"},
    {"B0051", "Deployment Commanded"},
    {"B0053", "Deployment Commanded with Loop Malfunctions Present"},
    {"B0057", "Right Front/Passenger Pretensioner Deployment Loop Resistance Low"},
    {"B0058", "Right Front/Passenger Pretensioner Deployment Loop Open"},
    {"B0059", "Right Front/Passenger Pretensioner Deployment Loop Short to Ground/Voltage Out of Range"},
    {"B0061", "Roof Rail ModuleLeft Deployment Loop Circuit"},
    {"B0062", "Roof Rail ModuleLeft Deployment Loop Circuit"},
    {"B0064", "Left Front/Driver Pretensioner Deployment Loop Resistance Low"},
    {"B0065", "Left Front/Driver Pretensioner Deployment Loop Open"},
    {"B0066", "Left Front/Driver Pretensioner Deployment Loop Short to Ground/Voltage Out of Range"},
    {"B0068", "Roof Rail ModuleLeft Deployment Loop Circuit"},
    {"B0069", "Roof Rail ModuleRight Deployment Loop Circuit"},
    {"B0070", "Roof Rail ModuleRight Deployment Loop Circuit"},
    {"B0071", "Roof Rail ModuleRight Deployment Loop Circuit"},
    {"B0073", "Supplemental Deployment Loop #1 Resistance Low"},
    {"B0074", "Supplemental Deployment Loop #1 Open"},
    {"B0075", "Supplemental Deployment Loop #1 Short to Ground/Voltage Out of Range"},
    {"B0077", "Left Front/Driver Side Impact Sensor (SIS) Malfunction"},
    {"B0078", "Right Front/Passenger SIS Malfunction"},
    {"B0079", "Incorrect Left Front/Driver SIS Installed"},
    {"B0080", "Discard Left Front/Driver SIS"},
    {"B0081", "Incorrect Right Front/Passenger SIS Installed"},
    {"B0082", "Discard Right Front/Passenger SIS"},
    {"B0086", "Supplemental Deployment Loop #2 Resistance Low"},
    {"B0087", "Supplemental Deployment Loop #2 Open"},
    {"B0088", "Supplemental Deployment Loop #2 Short to Ground/Voltage Out of Range"},
    {"B0090", "Active switch voltage out of range"},
    {"B0091", "Active switch: wrong state"},
    {"B0092", "PPS passenger detection error"},
    {"B0093", "PPS/CPS selftest malfunction"},
    {"B0094", "CPS childseat detection error"},
    {"B0095", "SDMPPS/CPS mismatch malfunction"},
    {"B0100", "Electronic Front End Sensor 1 Performance"},
    {"B0101", "Electronic Front End Sensor 1 Performance"},
    {"B0102", "Electronic Front End Sensor 1 Performance"},
    {"B0103", "Electronic Frontal Sensor Circuit"},
    {"B0104", "Electronic Frontal Sensor Circuit"},
    {"B0105", "Electronic Frontal Sensor Circuit"},
    {"B0109", "Side impact moduleLeft Rear deployment loop resistance is less than 1.3 ohms"},
    {"B0110", "Side impact moduleLeft Rear high circuit is less than 2.4 volts"},
    {"B0111", "Side impact moduleLeft Rear high and/or low circuits is short to ground or short to voltage"},
    {"B0112", "Side impact moduleRight Rear deployment loop resistance is less than 1.3 ohms"},
    {"B0113", "Side impact moduleRight Rear high circuit is less than 2.4 volts"},
    {"B0114", "Side impact moduleRight Rear high and/or low circuits is short to ground or short to voltage"},
    {"B0126", "Right Panel Discharge Temperature Fault"},
    {"B0130", "Air Temperature/Mode Door Actuator Malfunction"},
    {"B0131", "Right Heater Discharge Temperature Fault"},
    {"B0145", "Auxiliary HAVC Actuator Circuit"},
    {"B0159", "Outside Air Temperature Sensor Circuit Range/Performance"},
    {"B0160", "Ambient Air Temperature Sensor Circuit"},
    {"B0162", "Ambient Air Temperature Sensor Circuit"},
    {"B0164", "Passenger Compartment Temperature Sensor #1 (Single Sensor or LH) Circuit Range/Performance"},
    {"B0169", "Incar Temp Sensor Failure (passenger not used)"},
    {"B0174", "Output Air Temperature Sensor #1 (Upper; Single or LH) Circuit Range/Performance"},
    {"B0179", "Output Air Temperature Sensor #2 (Lower; Single or LH) Circuit Range/Performance"},
    {"B0183", "Sunload Sensor Circuit"},
    {"B0184", "Solar Load Sensor #1 Circuit Range (sunload)"},
    {"B0188", "Sunload Sensor Circuit"},
    {"B0189", "Solar Load Sensor #2 Circuit Range (sunload)"},
    {"B0229", "HVAC Actuator Circuit"},
    {"B0248", "Mode Door Inoperative Error"},
    {"B0249", "Heater/Defrost/AC Door Range Error"},
    {"B0263", "HVAC Actuator Circuit"},
    {"B0268", "A/I Door Inoperative Error"},
    {"B0269", "Air Inlet Door Range Error"},
    {"B0283", "Electric Rear Defrost Circuit"},
    {"B0285", "Electric Rear Defrost Circuit Low (BCM)"},
    {"B0286", "Electric Rear Defrost Circuit High (BCM)"},
    {"B0408", "Temperature Control #1 (Main/Front) Circuit Malfunction"},
    {"B0409", "Air Mix Door #1 Range Error"},
    {"B0414", "Air Temperature/Mode Door Actuator Malfunction"},
    {"B0418", "HVAC Actuator Circuit"},
    {"B0419", "Air Mix Door #2 Range Error"},
    {"B0423", "Air Mix Door #2 Inoperative Error"},
    {"B0424", "Air Temperature/Mode Door Actuator Malfunction"},
    {"B0428", "Air Mix Door #3 Inoperative Error"},
    {"B0429", "Temperature Control #3 Rear Circuit Range/Performance"},
    {"B0510", "RH Panel Discharge Temp Sensor Failure"},
    {"B0515", "RH Heater Discharge Temp Sensor Failure"},
    {"B0520", "Rear Discharge Temp Sensor Failure"},
    {"B0530", "Fuel Level Sensor Stuck"},
    {"B0532", "Fuel Sensor Shorted To Ground"},
    {"B0533", "Fuel Sensor Open/Shorted To B+"},
    {"B0540", "Speedometer Circuit"},
    {"B0560", "Tachometer Circuit"},
    {"B0688", "Security System Indicator Circuit High"},
    {"B0689", "Security System Indicator Circuit Open (BCM)"},
    {"B0728", "P (park) Indicator Circuit High (BCM)"},
    {"B0729", "P (park) Indicator Circuit Open (BCM)"},
    {"B0733", "R (reverse) Indicator Circuit High (BCM)"},
    {"B0734", "R (reverse) Indicator Circuit Open (BCM)"},
    {"B0738", "N (neutral) Indicator Circuit High (BCM)"},
    {"B0739", "N (neutral) Indicator Circuit Open (BCM)"},
    {"B0748", "D (drive) Indicator Circuit High (BCM)"},
    {"B0749", "D (drive) Indicator Circuit Open (BCM)"},
    {"B0768", "Service Indicator Circuit High"},
    {"B0770", "All Wheel Drive (AWD) Indicator Circuit"},
    {"B0775", "Four Wheel Drive High Range (4HI) Indicator Circuit"},
    {"B0780", "Four Wheel Drive Low Range (4LO) Indicator Circuit"},
    {"B0785", "Two Wheel Drive High Range (2HI) Indicator Circuit"},
    {"B0790", "Transfer Case Neutral Indicator Circuit"},
    {"B0846", "+5 Volt Reference Out of Range"},
    {"B0856", "Battery 2 Out of Range"},
    {"B0951", "Dimming Circuit"},
    {"B1000", "ECU Malfunction"},
    {"B1001", "Option Configuration Error"},
    {"B1004", "Keep Alive Memory (KAM)"},
    {"B1007", "EEPROM Write Error"},
    {"B1008", "Calibration Data Not Programmed (BCM)"},
    {"B1009", "EEPROM Checksum Error"},
    {"B1013", "ROM Checksum Error"},
    {"B1014", "Program ROM Checksum Error"},
    {"B1015", "Passenger Deploy. Loop Resistance High"},
    {"B1016", "Passenger Deployment Loop Resistance Low"},
    {"B1017", "Passenger Deployment Loop Open"},
    {"B1018", "Passenger Deployment Loop Short to GND"},
    {"B1019", "Passenger Deploy. Loop Short to Voltage"},
    {"B1021", "Driver Deployment Loop Resistance High"},
    {"B1022", "Driver Deployment Loop Resistance Low"},
    {"B1023", "Integral Switch Performance"},
    {"B1024", "Driver Deployment Loop Short to Ground"},
    {"B1025", "Driver Deployment Loop Short to Voltage"},
    {"B1026", "Driver Deployment Loop Open"},
    {"B1035", "Discr. Sensor Closed or Short to GND"},
    {"B1036", "Discr. Sensor Open or Short to Voltage"},
    {"B1045", "Audio Output 3 Circuit (Radio)"},
    {"B1051", "Frontal Air Bag and Preten Deployment"},
    {"B1053", "Deployment Commanded w/Loop Malfunction"},
    {"B1054", "Infl Rst IP Module Switch Circuit Failure"},
    {"B1055", "Audio Output 4 Circuit (Radio)"},
    {"B1061", "Lamp Circuit Failure"},
    {"B1071", "Internal SDM Failure"},
    {"B1115", "Passenger Deploy. Loop Resistance High"},
    {"B1116", "Passenger Deploy. Loop Resistance Low"},
    {"B1117", "Passenger Deployment Loop Open"},
    {"B1121", "Driver Deployment Loop Resistance High"},
    {"B1122", "Driver Deployment Loop Resistance Low"},
    {"B1124", "Driver/Passenger Deploy. Loop Short to GND"},
    {"B1125", "Driver/Pass Deploy Loop Short to Voltage"},
    {"B1126", "Driver Deployment Loop Open"},
    {"B1131", "Passenger Pretensioner Loop Resistance High"},
    {"B1132", "Passenger Pretensioner Loop Resistance Low"},
    {"B1134", "Passenger Pretensioner Loop Open"},
    {"B1135", "Discr. Sensor Closed or Short to GND"},
    {"B1136", "Discr. Sensor Open or Short to Voltage"},
    {"B1141", "Driver Pretensioner Loop Resistance High"},
    {"B1142", "Driver Pretens. Deploy. Loop Res. Low"},
    {"B1144", "Driver/Pass Pretens. Loop Short to GND"},
    {"B1145", "Driver/Pass Pretens. Loop Short to Volt"},
    {"B1146", "Driver Pretensioner Loop Open"},
    {"B1147", "Driver Side Air Bag System Malfunction"},
    {"B1148", "Passenger Side Air Bag System Malfunction"},
    {"B1151", "Deployment Commanded Frontal"},
    {"B1152", "Data Area Full"},
    {"B1153", "Deployment Commanded w/Loop Malfunction"},
    {"B1155", "SDM Calibration Mismatch"},
    {"B1159", "Loss of Serial DataKey Not Received"},
    {"B1160", "Loss of Serial Data VIN Not Received"},
    {"B1161", "Lamp Circuit Failure"},
    {"B1163", "Loss of Serial DataNo Lamp Response"},
    {"B1171", "Internal SDM Failure"},
    {"B1200", "Climate Control Pushbutton Circuit Failure"},
    {"B1201", "Fuel Sender Circuit Failure"},
    {"B1202", "Fuel Sender Circuit Open"},
    {"B1203", "Fuel Sender Circuit Short To Battery"},
    {"B1204", "Fuel Sender Circuit Short To Ground"},
    {"B1205", "EIC Switch1 Assembly Circuit Failure"},
    {"B1206", "EIC Switch1 Assembly Circuit Open"},
    {"B1207", "EIC Switch1 Assembly Circuit Short To Battery"},
    {"B1208", "EIC Switch1 Assembly Circuit Short To Ground"},
    {"B1209", "EIC Switch2 Assembly Circuit Failure"},
    {"B1210", "EIC Switch2 Assembly Circuit Open"},
    {"B1211", "EIC Switch2 Assembly Circuit Short To Battery"},
    {"B1212", "EIC Switch2 Assembly Circuit Short To Ground"},
    {"B1213", "AntiTheft Number of Programmed Keys Is Below Minimum"},
    {"B1214", "Running Board Lamp Circuit Failure"},
    {"B1215", "Running Board Lamp Circuit Short to Battery"},
    {"B1216", "Emergency & Road Side Assistance Switch Circuit Short to Ground"},
    {"B1217", "Horn Relay Coil Circuit Failure"},
    {"B1218", "Horn Relay Coil Circuit Short to V batt"},
    {"B1219", "Fuel Tank Pressure Sensor Circuit Failure"},
    {"B1220", "Fuel Tank Pressure Sensor Circuit Open"},
    {"B1222", "Fuel Temperature Sensor #1 Circuit Failure"},
    {"B1223", "Fuel Temperature Sensor #1 Circuit Open"},
    {"B1224", "Fuel Temperature Sensor #1 Circuit Short to Battery"},
    {"B1225", "Fuel Temperature Sensor #1 Circuit Short to Ground"},
    {"B1226", "Fuel Temperature Sensor #2 Circuit Failure"},
    {"B1227", "Fuel Temperature Sensor #2 Circuit Open"},
    {"B1228", "Fuel Temperature Sensor #2 Circuit Short to Battery"},
    {"B1229", "Fuel Temperature Sensor #2 Circuit Short to Ground"},
    {"B1231", "Longitudinal Acceleration Threshold Exceeded"},
    {"B1232", "See Manufacturer"},
    {"B1233", "Glass Break Sensor Failure"},
    {"B1234", "Mirror Switch Invalid Code"},
    {"B1235", "Window Feedback Failure"},
    {"B1236", "Window Feedback Loss of Signal"},
    {"B1237", "Window Feedback Out of Range"},
    {"B1238", "over Temperature Fault"},
    {"B1239", "Air Flow Blend Door Driver Circuit Failure"},
    {"B1240", "Wiper Washer Rear Pump Relay Circuit Failure"},
    {"B1241", "Wiper Washer Rear Pump Relay Circuit Short to Battery"},
    {"B1242", "Air Flow Recirculation Door Driver Circuit Failure"},
    {"B1243", "Express Window down Switch Circuit Short to Battery"},
    {"B1244", "Wiper Rear Motor Run Relay Circuit Failure"},
    {"B1245", "Wiper Rear Motor Run Relay Circuit Short to Battery"},
    {"B1246", "Dim Panel Potentiometer Switch Circuit Failure"},
    {"B1247", "Panel Dim Switch Circuit Open"},
    {"B1248", "Passenger's Seatback Auto glides Rearward Switch Circuit Short to Ground"},
    {"B1249", "Blend Door Failure"},
    {"B1250", "Air Temperature Internal Sensor Circuit Failure"},
    {"B1251", "Air Temperature Internal Sensor Circuit Open"},
    {"B1252", "Air Temperature Internal Sensor Circuit Short To Battery"},
    {"B1253", "Air Temperature Internal Sensor Circuit Short To Ground"},
    {"B1254", "Air Temperature External Sensor Circuit Failure"},
    {"B1255", "Air Temperature External Sensor Circuit Open"},
    {"B1256", "Air Temperature External Sensor Circuit Short To Battery"},
    {"B1257", "Air Temperature External Sensor Circuit Short To Ground"},
    {"B1258", "Solar Radiation Sensor Circuit Failure"},
    {"B1259", "Solar Radiation Sensor Circuit Open"},
    {"B1260", "Solar Radiation Sensor Circuit Short To Battery"},
    {"B1261", "Solar Radiation Sensor Circuit Short To Ground"},
    {"B1262", "Servo Motor Defrost Circuit Failure"},
    {"B1263", "Servo Motor Vent Circuit Failure"},
    {"B1264", "Servo Motor Foot Circuit Failure"},
    {"B1265", "Servo Motor Cool air Bypass Circuit Failure"},
    {"B1266", "Servo Motor Air intake Left Circuit Failure"},
    {"B1267", "Servo Motor Air intake Right Circuit Failure"},
    {"B1268", "Servo Motor Potentiometer Defrost Circuit Failure"},
    {"B1269", "Servo Motor Potentiometer Defrost Circuit Open"},
    {"B1270", "Servo Motor Potentiometer Defrost Circuit Short To Battery"},
    {"B1271", "Servo Motor Potentiometer Defrost Circuit Short To Ground"},
    {"B1272", "Servo Motor Potentiometer Vent Circuit Failure"},
    {"B1273", "Servo Motor Potentiometer Vent Circuit Open"},
    {"B1274", "Servo Motor Potentiometer Vent Circuit Short To Battery"},
    {"B1275", "Servo Motor Potentiometer Vent Circuit Short To Ground"},
    {"B1276", "Servo Motor Potentiometer Foot Circuit Failure"},
    {"B1277", "Servo Motor Potentiometer Foot Circuit Open"},
    {"B1278", "Servo Motor Potentiometer Foot Circuit Short To Battery"},
    {"B1279", "Servo Motor Potentiometer Foot Circuit Short To Ground"},
    {"B1280", "Servo Motor Potentiometer Cool air Circuit Failure"},
    {"B1281", "Servo Motor Potentiometer Cool air Circuit Open"},
    {"B1282", "Servo Motor Potentiometer Cool air Circuit Short to Battery"},
    {"B1283", "Servo Motor Potentiometer Cool air Circuit Short to Ground"},
    {"B1284", "Servo Motor Potentiometer Air intake Left Circuit Failure"},
    {"B1285", "Servo Motor Potentiometer Air intake Left Circuit Open"},
    {"B1286", "Servo Motor Potentiometer Air intake Left Circuit Short to Battery"},
    {"B1287", "Servo Motor Potentiometer Air intake Left Circuit Short to Ground"},
    {"B1288", "Servo Motor Potentiometer Air intake Right Circuit Failure"},
    {"B1289", "Servo Motor Potentiometer Air intake Right Circuit Open"},
    {"B1290", "Servo Motor Potentiometer Air intake Right Circuit Short to Battery"},
    {"B1291", "Servo Motor Potentiometer Air intake Right Circuit Short to Ground"},
    {"B1292", "Battery Power Relay Circuit Failure"},
    {"B1293", "Battery Power Relay Circuit Open"},
    {"B1294", "Battery Power Relay Circuit Short To Battery"},
    {"B1295", "Battery Power Relay Circuit Short To Ground"},
    {"B1296", "Power Supply Sensor Circuit Failure"},
    {"B1297", "Power Supply Sensor Circuit Open"},
    {"B1298", "Power Supply Sensor Circuit Short To Battery"},
    {"B1299", "Power Supply Sensor Circuit Short To Ground"},
    {"B1300", "Power Door Lock Circuit Failure"},
    {"B1301", "Power Door Lock Circuit Open"},
    {"B1302", "Accessory Delay Relay Coil Circuit Failure"},
    {"B1303", "Accessory Delay Relay Coil Circuit Open"},
    {"B1304", "Accessory Delay Relay Coil Circuit Short To Battery"},
    {"B1305", "Accessory Delay Relay Coil Circuit Short To Ground"},
    {"B1306", "Oil Level Switch Circuit Open"},
    {"B1307", "Oil Level Switch Circuit Short To Battery"},
    {"B1308", "Oil Level Switch Circuit Short To Ground"},
    {"B1309", "Power Door Lock Circuit Short To Ground"},
    {"B1310", "Power Door Unlock Circuit Failure"},
    {"B1311", "Power Door Unlock Circuit Open"},
    {"B1312", "Lamp Headlamp Input Circuit Short To Battery"},
    {"B1313", "Battery Saver Relay Coil Circuit Failure"},
    {"B1314", "Battery Saver Relay Coil Circuit Open"},
    {"B1315", "Battery Saver Relay Coil Circuit Short To Battery"},
    {"B1316", "Battery Saver Relay Coil Circuit Short To Ground"},
    {"B1317", "Battery Voltage High"},
    {"B1318", "Battery Voltage Low"},
    {"B1319", "Driver Door Ajar Circuit Failure"},
    {"B1320", "Driver Door Ajar Circuit Open"},
    {"B1321", "Driver Door Ajar Circuit Short To Battery"},
    {"B1322", "Driver Door Ajar Circuit Short To Ground"},
    {"B1323", "Door Ajar Lamp Circuit Failure"},
    {"B1324", "Door Ajar Lamp Circuit Open"},
    {"B1325", "Door Ajar Lamp Circuit Short To Battery"},
    {"B1326", "Door Ajar Lamp Circuit Short To Ground"},
    {"B1327", "Passenger Door Ajar Circuit Failure"},
    {"B1328", "Passenger Door Ajar Circuit Open"},
    {"B1329", "Passenger Door Ajar Circuit Short To Battery"},
    {"B1330", "Passenger Door Ajar Circuit Short To Ground"},
    {"B1331", "Deck lid Ajar Rear Door Circuit Failure"},
    {"B1332", "Deck lid Ajar Rear Door Circuit Open"},
    {"B1333", "Deck lid Ajar Rear Door Circuit Short To Battery"},
    {"B1334", "Deck lid Ajar Rear Door Circuit Short To Ground"},
    {"B1335", "Door Ajar RR Circuit Failure"},
    {"B1336", "Door Ajar RR Circuit Open"},
    {"B1337", "Door Ajar RR Circuit Short To Battery"},
    {"B1338", "Door Ajar RR Circuit Short To Ground"},
    {"B1339", "Chime Input Request Circuit Short To Battery"},
    {"B1340", "Chime Input Request Circuit Short To Ground"},
    {"B1341", "Power Door Unlock Circuit Short To Ground"},
    {"B1342", "ECU Is Defective"},
    {"B1343", "Heated Backlight Input Circuit Failure"},
    {"B1344", "Heated Backlight Input Circuit Open"},
    {"B1345", "Heated Backlight Input Circuit Short To Ground"},
    {"B1346", "Heated Backlight Input Circuit Short To Battery"},
    {"B1347", "Heated Backlight Relay Circuit Failure"},
    {"B1348", "Heated Backlight Relay Circuit Open"},
    {"B1349", "Heated Backlight Relay Short to Battery"},
    {"B1350", "Heated Backlight Relay Short To Ground"},
    {"B1351", "Ignition KeyIn Circuit Short To Battery"},
    {"B1352", "Ignition KeyIn Circuit Failure"},
    {"B1353", "Ignition KeyIn Circuit Open"},
    {"B1354", "Ignition KeyIn Circuit Short To Ground"},
    {"B1355", "Ignition Run Circuit Failure"},
    {"B1356", "Ignition Run Circuit Open"},
    {"B1357", "Ignition Run Circuit Short To Battery"},
    {"B1358", "Ignition Run Circuit Short To Ground"},
    {"B1359", "Ignition Run/Acc Circuit Failure"},
    {"B1360", "Ignition Run/Acc Circuit Open"},
    {"B1361", "Ignition Run/Acc Circuit Short To Battery"},
    {"B1362", "Ignition Run/Acc Circuit Short To Ground"},
    {"B1363", "Ignition Start Circuit Failure"},
    {"B1364", "Ignition Start Circuit Open"},
    {"B1365", "Ignition Start Circuit Short To Battery"},
    {"B1366", "Ignition Start Circuit Short To Ground"},
    {"B1367", "Ignition Tach Circuit Failure"},
    {"B1368", "Ignition Tach Circuit Open"},
    {"B1369", "Ignition Tach Circuit Short To Battery"},
    {"B1370", "Ignition Tach Circuit Short To Ground"},
    {"B1371", "Illuminated Entry Relay Circuit Failure"},
    {"B1372", "Illuminated Entry Relay Circuit Open"},
    {"B1373", "Illuminated Entry Relay Short To Battery"},
    {"B1374", "Illuminated Entry Relay Short To Ground"},
    {"B1375", "Oil Change Lamp Circuit Open"},
    {"B1376", "Oil Change Lamp Circuit Short To Battery"},
    {"B1377", "Oil Change Lamp Circuit Failure"},
    {"B1378", "Oil Change Lamp Circuit Short To Ground"},
    {"B1379", "Oil Change Reset Button Circuit Short To Ground"},
    {"B1380", "Oil Change Reset Button Circuit Failure"},
    {"B1381", "Oil Change Reset Button Circuit Open"},
    {"B1382", "Oil Change Reset Button Circuit Short To Battery"},
    {"B1383", "Oil Level Lamp Circuit Short To Battery"},
    {"B1384", "Oil Level Lamp Circuit Failure"},
    {"B1385", "Oil Level Lamp Circuit Open"},
    {"B1386", "Oil Level Lamp Circuit Short To Ground"},
    {"B1387", "Oil Temperature Sensor Circuit Open"},
    {"B1388", "Oil Temperature Sensor Circuit Short To Battery"},
    {"B1389", "Oil Temperature Sensor Circuit Failure"},
    {"B1390", "Oil Temperature Sensor Circuit Short To Ground"},
    {"B1391", "Oil Level Switch Circuit Failure"},
    {"B1392", "Power Door Memory Lock Relay Circuit Failure"},
    {"B1393", "Power Door Memory Lock Relay Circuit Open"},
    {"B1394", "Power Door Memory Lock Relay Circuit Short To Battery"},
    {"B1395", "Power Door Memory Lock Relay Circuit Short To Ground"},
    {"B1396", "Power Door Lock Circuit Short To Battery"},
    {"B1397", "Power Door Unlock Circuit Short To Battery"},
    {"B1398", "Driver Power Window One Touch Window Relay Circuit Failure"},
    {"B1399", "Driver Power Window One Touch Window Relay Circuit Open"},
    {"B1400", "Driver Power Window One Touch Window Relay Circuit Short To Battery"},
    {"B1401", "Driver Power Window One Touch Window Relay Circuit Short To Ground"},
    {"B1402", "Driver Power Window down Switch Circuit Failure"},
    {"B1403", "Driver Power Window up Switch Circuit Failure"},
    {"B1404", "Driver Power Window down Circuit Open"},
    {"B1405", "Driver Power Window down Circuit Short to Battery"},
    {"B1406", "Driver Power Window Down Circuit Short To Ground"},
    {"B1407", "Driver Power Window up Circuit Open"},
    {"B1408", "Driver Power Window up Circuit Short to Battery"},
    {"B1409", "Driver Power Window Up Circuit Short To Ground"},
    {"B1410", "Driver Power Window Motor Circuit Failure"},
    {"B1411", "Driver Power Window Motor Circuit Open"},
    {"B1412", "Driver Power Window Motor Circuit Short To Battery"},
    {"B1413", "Driver Power Window Motor Circuit Short To Ground"},
    {"B1414", "Power Window LR Motor Circuit Failure"},
    {"B1415", "Power Window LR Motor Circuit Open"},
    {"B1416", "Power Window LR Motor Circuit Short To Battery"},
    {"B1417", "Power Window LR Motor Circuit Short To Ground"},
    {"B1418", "Passenger Power Window Motor Circuit Failure"},
    {"B1419", "Passenger Power Window Motor Circuit Open"},
    {"B1420", "Passenger Power Window Motor Circuit Short To Battery"},
    {"B1421", "Passenger Power Window Motor Circuit Short To Ground"},
    {"B1422", "Power Window RR Motor Circuit Failure"},
    {"B1423", "Power Window RR Motor Circuit Open"},
    {"B1424", "Power Window RR Motor Circuit Short To Battery"},
    {"B1425", "Power Window RR Motor Circuit Short To Ground"},
    {"B1426", "Lamp Seat Belt Circuit Short To Battery"},
    {"B1427", "Lamp Seat Belt Circuit Open"},
    {"B1428", "Lamp Seat Belt Circuit Failure"},
    {"B1429", "Lamp Seat Belt Circuit Short To Ground"},
    {"B1430", "Seat Belt Switch Circuit Short To Ground"},
    {"B1431", "Wiper Brake/Run Relay Circuit Failure"},
    {"B1432", "Wiper Brake/Run Relay Circuit Short To Battery"},
    {"B1433", "Wiper Brake/Run Relay Circuit Short To Ground"},
    {"B1434", "Wiper Hi/Low Speed Relay Coil Circuit Failure"},
    {"B1435", "Wiper Hi/Low Speed Relay Coil Circuit Open"},
    {"B1436", "Wiper Hi/Low Speed Relay Coil Circuit Short To Battery"},
    {"B1437", "Wiper Hi/Low Speed Relay Coil Circuit Short To Ground"},
    {"B1438", "Wiper Mode Select Switch Circuit Failure"},
    {"B1439", "Wiper Mode Select Switch Circuit Open"},
    {"B1440", "Wiper Mode Select Switch Circuit Short To Battery"},
    {"B1441", "Wiper Mode Select Switch Circuit Short To Ground"},
    {"B1442", "Door Handle Switch Circuit Failure"},
    {"B1443", "Door Handle Switch Circuit Open"},
    {"B1444", "Door Handle Switch Circuit Short To Battery"},
    {"B1445", "Door Handle Switch Circuit Short To Ground"},
    {"B1446", "Wiper Park Sense Circuit Failure"},
    {"B1447", "Wiper Park Sense Circuit Open"},
    {"B1448", "Wiper Park Sense Circuit Short To Battery"},
    {"B1449", "Wiper Park Sense Circuit Short To Ground"},
    {"B1450", "Wiper Wash/Delay Switch Circuit Failure"},
    {"B1451", "Wiper Wash/Delay Switch Circuit Open"},
    {"B1452", "Wiper Wash/Delay Switch Circuit Short To Battery"},
    {"B1453", "Wiper Wash/Delay Switch Circuit Short To Ground"},
    {"B1454", "Wiper Washer Fluid Lamp Circuit Failure"},
    {"B1455", "Wiper Washer Fluid Lamp Circuit Open"},
    {"B1456", "Wiper Washer Fluid Lamp Circuit Short To Battery"},
    {"B1457", "Wiper Washer Fluid Lamp Circuit Short To Ground"},
    {"B1458", "Wiper Washer Pump Motor Relay Circuit Failure"},
    {"B1459", "Wiper Washer Pump Motor Relay Coil Circuit Open"},
    {"B1460", "Wiper Washer Pump Motor Relay Coil Circuit Short To Battery"},
    {"B1461", "Wiper Washer Pump Motor Relay Coil Circuit Short To Ground"},
    {"B1462", "Seat Belt Switch Circuit Failure"},
    {"B1463", "Seat Belt Switch Circuit Open"},
    {"B1464", "Seat Belt Switch Circuit Short To Battery"},
    {"B1465", "Wiper Brake/Run Relay Circuit Open"},
    {"B1466", "Wiper Hi/Low Speed Not Switching"},
    {"B1467", "Wiper Hi/Low Speed Circuit Motor Short to Battery"},
    {"B1468", "Chime Input Request Circuit Failure"},
    {"B1469", "Chime Input Request Circuit Open"},
    {"B1470", "Lamp Headlamp Input Circuit Failure"},
    {"B1471", "Lamp Headlamp Input Circuit Open"},
    {"B1472", "Lamp Headlamp Input Circuit Short To Ground"},
    {"B1473", "Wiper Low Speed Circuit Motor Failure"},
    {"B1474", "Battery Saver Power Relay Circuit Short To Battery"},
    {"B1475", "Accessory Delay Relay Contact Short To Battery"},
    {"B1476", "Wiper High Speed Circuit Motor Failure"},
    {"B1477", "Wiper Hi/Low Circuit Motor Short to Ground"},
    {"B1478", "Power Window One Touch Up/Down Activated Simultaneously"},
    {"B1479", "Wiper Washer Fluid Level Sensor Circuit Failure"},
    {"B1480", "Wiper Washer Fluid Level Sensor Circuit Open"},
    {"B1481", "Wiper Washer Fluid Level Sensor Circuit Short To Battery"},
    {"B1482", "Wiper Washer Fluid Level Sensor Circuit Short To Ground"},
    {"B1483", "Brake Pedal Input Circuit Failure"},
    {"B1484", "Brake Pedal Input Open Circuit"},
    {"B1485", "Brake Pedal Input Circuit Battery Short"},
    {"B1486", "Brake Pedal Input Circuit Ground Short"},
    {"B1487", "Door Handle Right Front Circuit Failure"},
    {"B1488", "Door Handle Right Front Circuit Open"},
    {"B1489", "Door Handle Right Front Short to Battery"},
    {"B1490", "Door Handle Right Front Short to Ground"},
    {"B1491", "Ignition Cylinder Sensor Circuit Failure"},
    {"B1492", "Ignition Cylinder Sensor Open Circuit"},
    {"B1493", "Ignition Cylinder Sensor Battery Short"},
    {"B1494", "Ignition Cylinder Sensor Ground Short"},
    {"B1495", "Deck lid PunchOut Sensor Circuit Failure"},
    {"B1496", "Deck lid PunchOut Sensor Open Circuit"},
    {"B1497", "Deck lid PunchOut Sensor Battery Short"},
    {"B1498", "Deck lid PunchOut Sensor Ground Short"},
    {"B1499", "Lamp Turn Signal Left Circuit Failure"},
    {"B1500", "Lamp Turn Signal Left Circuit Open"},
    {"B1501", "Lamp Turn Signal Left Circuit Short To Battery"},
    {"B1502", "Lamp Turn Signal Left Circuit Short To Ground"},
    {"B1503", "Lamp Turn Signal Right Circuit Failure"},
    {"B1504", "Lamp Turn Signal Right Circuit Open"},
    {"B1505", "Lamp Turn Signal Right Circuit Short To Battery"},
    {"B1506", "Lamp Turn Signal Right Circuit Short To Ground"},
    {"B1507", "Flash to Pass Switch Circuit Failure"},
    {"B1508", "Flash to Pass Switch Circuit Open"},
    {"B1509", "Flash to Pass Switch Circuit Short To Battery"},
    {"B1510", "Flash to Pass Switch Circuit Short To Ground"},
    {"B1511", "Driver Door Handle Circuit Failure"},
    {"B1512", "Driver Door Handle Circuit Open"},
    {"B1513", "Driver Door Handle Circuit Short To Battery"},
    {"B1514", "Driver Door Handle Circuit Short To Ground"},
    {"B1515", "Seat Driver Occupied Switch Circuit Failure"},
    {"B1516", "Seat Driver Occupied Switch Circuit Open"},
    {"B1517", "Seat Driver Occupied Switch Circuit Short To Battery"},
    {"B1518", "Seat Driver Occupied Switch Circuit Short To Ground"},
    {"B1519", "Hood Switch Circuit Failure"},
    {"B1520", "Hood Switch Circuit Open"},
    {"B1521", "Hood Switch Circuit Short To Battery"},
    {"B1522", "Hood Switch Circuit Short To Ground"},
    {"B1523", "Keyless Entry Circuit Failure"},
    {"B1524", "Keyless Entry Circuit Open"},
    {"B1525", "Keyless Entry Circuit Short To Battery"},
    {"B1526", "Keyless Entry Circuit Short To Ground"},
    {"B1527", "Memory Set Switch Circuit Failure"},
    {"B1528", "Memory Set Switch Circuit Open"},
    {"B1529", "Memory Set Switch Circuit Short To Battery"},
    {"B1530", "Memory Set Switch Circuit Short To Ground"},
    {"B1531", "Memory 1 Switch Circuit Failure"},
    {"B1532", "Memory 1 Switch Circuit Open"},
    {"B1533", "Memory 1 Switch Circuit Short To Battery"},
    {"B1534", "Memory 1 Switch Circuit Short To Ground"},
    {"B1535", "Memory 2 Switch Circuit Failure"},
    {"B1536", "Memory 2 Switch Circuit Open"},
    {"B1537", "Memory 2 Switch Circuit Short To Battery"},
    {"B1538", "Memory 2 Switch Circuit Short To Ground"},
    {"B1539", "Mirror Driver Switch Assembly Circuit Failure"},
    {"B1540", "Mirror Driver Switch Assembly Circuit Open"},
    {"B1541", "Mirror Driver Switch Assembly Circuit Short To Battery"},
    {"B1542", "Mirror Driver Switch Assembly Circuit Short To Ground"},
    {"B1543", "Seat Direction Switch Assembly Circuit Failure"},
    {"B1544", "Seat Direction Switch Assembly Circuit Open"},
    {"B1545", "Seat Direction Switch Assembly Circuit Short To Battery"},
    {"B1546", "Seat Direction Switch Assembly Circuit Short To Ground"},
    {"B1547", "Power Window Master Circuit Failure"},
    {"B1548", "Power Window Master Circuit Open"},
    {"B1549", "Power Window Master Circuit Short to Battery"},
    {"B1550", "Power Window Master Circuit Short to Ground"},
    {"B1551", "Deck lid Release Circuit Failure"},
    {"B1552", "Deck lid Release Circuit Open"},
    {"B1553", "Deck lid Release Circuit Short to Battery"},
    {"B1554", "Deck lid Release Circuit Short to Ground"},
    {"B1555", "Ignition Run/Start Circuit Failure"},
    {"B1556", "Ignition Run/Start Circuit Open"},
    {"B1557", "Ignition Run/Start Circuit Short To Battery"},
    {"B1558", "Ignition Run/Start Circuit Short To Ground"},
    {"B1559", "Door Lock Cylinder Circuit Failure"},
    {"B1560", "Door Lock Cylinder Circuit Open"},
    {"B1561", "Door Lock Cylinder Circuit Short To Battery"},
    {"B1562", "Door Lock Cylinder Circuit Short To Ground"},
    {"B1563", "Door Ajar Circuit Failure"},
    {"B1564", "Door Ajar Circuit Open"},
    {"B1565", "Door Ajar Circuit Short To Battery"},
    {"B1566", "Door Ajar Circuit Short To Ground"},
    {"B1567", "Lamp Headlamp HighBeam Circuit Failure"},
    {"B1568", "Lamp Headlamp HighBeam Circuit Open"},
    {"B1569", "Lamp Headlamp HighBeam Circuit Short To Battery"},
    {"B1570", "Lamp Headlamp HighBeam Circuit Short To Ground"},
    {"B1571", "Door Ajar LR Circuit Failure"},
    {"B1572", "Door Ajar LR Circuit Open"},
    {"B1573", "Door Ajar LR Circuit Short To Battery"},
    {"B1574", "Door Ajar LR Circuit Short To Ground"},
    {"B1575", "Lamp Park Input Circuit Failure"},
    {"B1576", "Lamp Park Input Circuit Open"},
    {"B1577", "Lamp Park Input Circuit Short To Battery"},
    {"B1578", "Lamp Park Input Circuit Short To Ground"},
    {"B1579", "Dim Panel Increase Input Circuit Failure"},
    {"B1580", "Dim Panel Increase Input Circuit Open"},
    {"B1581", "Dim Panel Increase Input Circuit Short To Battery"},
    {"B1582", "Dim Panel Increase Input Circuit Short To Ground"},
    {"B1583", "Dim Panel Decrease Input Circuit Failure"},
    {"B1584", "Dim Panel Decrease Input Circuit Open"},
    {"B1585", "Dim Panel Decrease Input Circuit Short To Battery"},
    {"B1586", "Dim Panel Decrease Input Circuit Short To Ground"},
    {"B1587", "Auto lamp Delay Increase Circuit Failure"},
    {"B1588", "Auto lamp Delay Increase Circuit Open"},
    {"B1589", "Auto lamp Delay Increase Circuit Short to Battery"},
    {"B1590", "Auto lamp Delay Increase Circuit Short to Ground"},
    {"B1591", "Auto lamp Delay Decrease Circuit Failure"},
    {"B1592", "Auto lamp Delay Decrease Circuit Open"},
    {"B1593", "Auto lamp Delay Decrease Circuit Short to Battery"},
    {"B1594", "Auto lamp Delay Decrease Circuit Short to Ground"},
    {"B1595", "Ignition Switch Illegal Input Code"},
    {"B1596", "Service Continuous Codes"},
    {"B1597", "Driver's Seat Seatback Auto glides Forward Switch Circuit Short to Ground"},
    {"B1598", "Driver's Seat Seatback Auto glides Rearward Switch Circuit Short to Ground"},
    {"B1599", "Passenger's Seatback Auto glides Forward Switch Circuit Short to Ground"},
    {"B1600", "PATS Ignition Key Transponder Signal Is Not Received"},
    {"B1601", "PATS Received Incorrect KeyCode from Ignition Key Transponder"},
    {"B1602", "PATS Received Invalid Format of KeyCode from Ignition Key Transponder"},
    {"B1603", "Lamp AntiTheft Indicator Circuit Failure"},
    {"B1604", "Lamp AntiTheft Indicator Circuit Open"},
    {"B1605", "Lamp AntiTheft Indicator Circuit Short To Battery"},
    {"B1606", "Lamp AntiTheft Indicator Circuit Short To Ground"},
    {"B1607", "Illuminated Entry Input Circuit Failure"},
    {"B1608", "Illuminated Entry Input Open Circuit"},
    {"B1609", "Illuminated Entry Input Short Circuit to Battery"},
    {"B1610", "Illuminated Entry Input Short Circuit to Ground"},
    {"B1611", "Wiper Rear Mode Select Switch Circuit Failure"},
    {"B1612", "Wiper Rear Mode Select Switch Circuit Open"},
    {"B1613", "Wiper Rear Mode Select Switch Circuit Short To Battery"},
    {"B1614", "Wiper Rear Mode Select Switch Circuit Short To Ground"},
    {"B1615", "Wiper Rear Disable Switch Circuit Failure"},
    {"B1616", "Wiper Rear Disable Switch Circuit Open"},
    {"B1617", "Wiper Rear Disable Switch Circuit Short To Battery"},
    {"B1618", "Wiper Rear Disable Switch Circuit Short To Ground"},
    {"B1619", "Wiper Rear Low Limit Input Circuit Failure"},
    {"B1620", "Wiper Rear Low Limit Input Circuit Open"},
    {"B1621", "Wiper Rear Low Limit Input Circuit Short To Battery"},
    {"B1622", "Wiper Rear Low Limit Input Circuit Short To Ground"},
    {"B1623", "Lamp Keypad Output Circuit Failure"},
    {"B1624", "Lamp Keypad Output Open Circuit"},
    {"B1625", "Lamp Keypad Output Short Circuit to Battery"},
    {"B1626", "Lamp Keypad Output Short Circuit to Ground"},
    {"B1627", "PRNDL Reverse Input Circuit Failure"},
    {"B1628", "PRNDL Reverse Input Open Circuit"},
    {"B1629", "PRNDL Reverse Input Short to Battery"},
    {"B1630", "PRNDL Reverse Input Short Circuit to Ground"},
    {"B1631", "Mirror Driver Left Circuit Failure"},
    {"B1632", "Mirror Driver Left Circuit Open"},
    {"B1633", "Mirror Driver Left Circuit Short To Battery"},
    {"B1634", "Mirror Driver Left Circuit Short To Ground"},
    {"B1635", "Mirror Driver Right Circuit Failure"},
    {"B1636", "Mirror Driver Right Circuit Open"},
    {"B1637", "Mirror Driver Right Circuit Short To Battery"},
    {"B1638", "Mirror Driver Right Short To Ground"},
    {"B1639", "Mirror Passenger Left Circuit Failure"},
    {"B1640", "Mirror Passenger Left Circuit Open"},
    {"B1641", "Mirror Passenger Left Circuit Short To Battery"},
    {"B1642", "Mirror Passenger Left Circuit Short To Ground"},
    {"B1643", "Mirror Passenger Right Circuit Failure"},
    {"B1644", "Mirror Passenger Right Circuit Open"},
    {"B1645", "Mirror Passenger Right Circuit Short To Battery"},
    {"B1646", "Mirror Passenger Right Circuit Short To Ground"},
    {"B1647", "Seat Driver Recline Forward Circuit Failure"},
    {"B1648", "Seat Driver Recline Forward Circuit Open"},
    {"B1649", "Seat Driver Recline Forward Circuit Short To Battery"},
    {"B1650", "Seat Driver Recline Forward Circuit Short To Ground"},
    {"B1651", "Seat Driver Recline Backward Circuit Failure"},
    {"B1652", "Seat Driver Recline Backward Circuit Open"},
    {"B1653", "Seat Driver Recline Backward Circuit Short To Battery"},
    {"B1654", "Seat Driver Recline Backward Circuit Short To Ground"},
    {"B1655", "Seat Driver Rear up Circuit Failure"},
    {"B1656", "Seat Driver Rear up Circuit Open"},
    {"B1657", "Seat Driver Rear up Circuit Short to Battery"},
    {"B1658", "Seat Driver Rear Up Circuit Short To Ground"},
    {"B1659", "Seat Driver Front up Circuit Failure"},
    {"B1660", "Seat Driver Front up Circuit Open"},
    {"B1661", "Seat Driver Front up Circuit Short to Battery"},
    {"B1662", "Seat Driver Front Up Circuit Short To Ground"},
    {"B1663", "Seat Driver Front up/Down Motor Stalled"},
    {"B1664", "Seat Driver Rear Up/Down Motor Stalled"},
    {"B1665", "Seat Driver Forward/Backward Motor Stalled"},
    {"B1666", "Seat Driver Recline Motor Stalled"},
    {"B1667", "Mirror Driver Up/Down Motor Stalled"},
    {"B1668", "Mirror Driver Right/Left Motor Stalled"},
    {"B1669", "Mirror Passenger Up/Down Motor Stalled"},
    {"B1670", "Mirror Passenger Right/Left Motor Stalled"},
    {"B1671", "Battery Module Voltage Out Of Range"},
    {"B1672", "Seat Driver Occupied Input Circuit Failure"},
    {"B1673", "Seat Driver Occupied Input Circuit Open"},
    {"B1674", "Seat Driver Occupied Input Circuit Short To Battery"},
    {"B1675", "Seat Driver Occupied Input Circuit Short To Ground"},
    {"B1676", "Battery Pack Voltage Out Of Range"},
    {"B1677", "Alarm Panic Input Circuit Failure"},
    {"B1678", "Alarm Panic Input Circuit Open"},
    {"B1679", "Alarm Panic Input Circuit Short To Battery"},
    {"B1680", "Alarm Panic Input Circuit Short To Ground"},
    {"B1681", "PATS Transceiver Module Signal Is Not Received"},
    {"B1682", "PATS Is Disabled (Check Link between PATS and Transponder)"},
    {"B1683", "Mirror Driver/Passenger Switch Circuit Failure"},
    {"B1684", "Mirror Driver/Passenger Switch Circuit Open"},
    {"B1685", "Lamp Dome Input Circuit Failure"},
    {"B1686", "Lamp Dome Input Circuit Open"},
    {"B1687", "Lamp Dome Input Circuit Short To Battery"},
    {"B1688", "Lamp Dome Input Circuit Short To Ground"},
    {"B1689", "Auto lamp Delay Circuit Failure"},
    {"B1690", "Auto lamp Delay Circuit Open"},
    {"B1691", "Auto lamp Delay Circuit Short to Battery"},
    {"B1692", "Auto lamp Delay Circuit Short to Ground"},
    {"B1693", "Auto lamp On Circuit Failure"},
    {"B1694", "Auto lamp On Circuit Open"},
    {"B1695", "Auto lamp On Circuit Short to Battery"},
    {"B1696", "Auto lamp On Circuit Short to Ground"},
    {"B1697", "Mirror Driver/Passenger Switch Circuit Short To Battery"},
    {"B1698", "Mirror Driver/Passenger Switch Circuit Short To Ground"},
    {"B1699", "Passenger's Seat Occupied Switch Circuit Short to Ground"},
    {"B1700", "Passenger's Seatbelt Tension Reducer Circuit Short to Ground"},
    {"B1701", "Seat Driver Recline Forward Switch Circuit Failure"},
    {"B1702", "Seat Driver Recline Forward Switch Circuit Open"},
    {"B1703", "Seat Driver Recline Forward Switch Circuit Short To Battery"},
    {"B1704", "Seat Driver Recline Forward Switch Circuit Short To Ground"},
    {"B1705", "Seat Driver Recline Rearward Switch Circuit Failure"},
    {"B1706", "Seat Driver Recline Rearward Switch Circuit Open"},
    {"B1707", "Seat Driver Recline Rearward Switch Circuit Short To Battery"},
    {"B1708", "Seat Driver Recline Rearward Switch Circuit Short To Ground"},
    {"B1709", "Seat Driver Front up Switch Circuit Failure"},
    {"B1710", "Seat Driver Front up Switch Circuit Open"},
    {"B1711", "Seat Driver Front up Switch Circuit Short to Battery"},
    {"B1712", "Seat Driver Front up Switch Circuit Short to Ground"},
    {"B1713", "Seat Driver Front down Switch Circuit Failure"},
    {"B1714", "Seat Driver Front down Switch Circuit Open"},
    {"B1715", "Seat Driver Front down Switch Circuit Short to Battery"},
    {"B1716", "Seat Driver Front down Switch Circuit Short to Ground"},
    {"B1717", "Seat Driver Forward Switch Circuit Failure"},
    {"B1718", "Seat Driver Forward Switch Circuit Open"},
    {"B1719", "Seat Driver Forward Switch Circuit Short To Battery"},
    {"B1720", "Seat Driver Forward Switch Circuit Short To Ground"},
    {"B1721", "Seat Driver Rearward Switch Circuit Failure"},
    {"B1722", "Seat Driver Rearward Switch Circuit Open"},
    {"B1723", "Seat Driver Rearward Switch Circuit Short To Battery"},
    {"B1724", "Seat Driver Rearward Switch Circuit Short To Ground"},
    {"B1725", "Seat Driver Rear up Switch Circuit Failure"},
    {"B1726", "Seat Driver Rear up Switch Circuit Open"},
    {"B1727", "Seat Driver Rear up Switch Circuit Short to Battery"},
    {"B1728", "Seat Driver Rear up Switch Circuit Short to Ground"},
    {"B1729", "Seat Driver Rear down Switch Circuit Failure"},
    {"B1730", "Seat Driver Rear down Switch Circuit Open"},
    {"B1731", "Seat Driver Rear down Switch Circuit Short to Battery"},
    {"B1732", "Seat Driver Rear down Switch Circuit Short to Ground"},
    {"B1733", "Mirror Driver Vertical Switch Circuit Failure"},
    {"B1734", "Mirror Driver Vertical Switch Circuit Open"},
    {"B1735", "Mirror Driver Vertical Switch Circuit Short To Battery"},
    {"B1736", "Mirror Driver Vertical Switch Circuit Short To Ground"},
    {"B1737", "Mirror Driver Horizontal Switch Circuit Failure"},
    {"B1738", "Mirror Driver Horizontal Switch Circuit Open"},
    {"B1739", "Mirror Driver Horizontal Switch Circuit Short To Battery"},
    {"B1740", "Mirror Driver Horizontal Switch Circuit Short To Ground"},
    {"B1741", "Mirror Passenger Vertical Switch Circuit Failure"},
    {"B1742", "Mirror Passenger Vertical Switch Circuit Open"},
    {"B1743", "Mirror Passenger Vertical Switch Circuit Short To Battery"},
    {"B1744", "Mirror Passenger Vertical Switch Circuit Short To Ground"},
    {"B1745", "Mirror Passenger Horizontal Switch Circuit Failure"},
    {"B1746", "Mirror Passenger Horizontal Switch Circuit Open"},
    {"B1747", "Mirror Passenger Horizontal Switch Circuit Short To Battery"},
    {"B1748", "Mirror Passenger Horizontal Switch Circuit Short To Ground"},
    {"B1749", "Park/Neutral Switch Circuit Failure"},
    {"B1750", "Park/Neutral Switch Circuit Open"},
    {"B1751", "Park/Neutral Switch Circuit Short To Battery"},
    {"B1752", "Park/Neutral Switch Circuit Short To Ground"},
    {"B1753", "Hazard Flash Output Circuit Failure"},
    {"B1754", "Hazard Flash Output Circuit Open"},
    {"B1755", "Hazard Flash Output Circuit Short Battery"},
    {"B1756", "Hazard Flash Output Circuit Short To Ground"},
    {"B1757", "Seat Driver Rear down Circuit Failure"},
    {"B1758", "Seat Driver Rear down Circuit Open"},
    {"B1759", "Seat Driver Rear down Circuit Short to Battery"},
    {"B1760", "Seat Driver Rear Down Circuit Short To Ground"},
    {"B1761", "Seat Driver Front down Circuit Failure"},
    {"B1762", "Seat Driver Front down Circuit Open"},
    {"B1763", "Seat Driver Front down Circuit Short to Battery"},
    {"B1764", "Seat Driver Front Down Circuit Short To Ground"},
    {"B1765", "Seat Driver Forward Circuit Failure"},
    {"B1766", "Seat Driver Forward Circuit Open"},
    {"B1767", "Seat Driver Forward Circuit Short To Battery"},
    {"B1768", "Seat Driver Forward Circuit Short To Ground"},
    {"B1769", "Seat Driver Backward Circuit Failure"},
    {"B1770", "Seat Driver Backward Circuit Open"},
    {"B1771", "Seat Driver Backward Circuit Short To Battery"},
    {"B1772", "Seat Driver Backward Circuit Short To Ground"},
    {"B1773", "Mirror Driver up Circuit Failure"},
    {"B1774", "Mirror Driver up Circuit Open"},
    {"B1775", "Mirror Driver up Circuit Short to Battery"},
    {"B1776", "Mirror Driver Up Circuit Short To Ground"},
    {"B1777", "Driver's Seatbelt Tension Reducer Circuit Short to Ground"},
    {"B1778", "Mirror Driver down Circuit Failure"},
    {"B1779", "Mirror Driver down Circuit Open"},
    {"B1780", "Mirror Driver down Circuit Short to Battery"},
    {"B1781", "Mirror Driver down Short to Ground"},
    {"B1782", "Mirror Passenger up Circuit Failure"},
    {"B1783", "Mirror Passenger up Circuit Open"},
    {"B1784", "Mirror Passenger up Circuit Short to Battery"},
    {"B1785", "Mirror Passenger Up Circuit Short To Ground"},
    {"B1786", "Mirror Passenger down Circuit Failure"},
    {"B1787", "Mirror Passenger down Circuit Open"},
    {"B1788", "Mirror Passenger down Circuit Short to Battery"},
    {"B1789", "Mirror Passenger Down Circuit Short To Ground"},
    {"B1790", "Auto lamp Sensor Input Circuit Failure"},
    {"B1791", "Auto lamp Sensor Input Circuit Open"},
    {"B1792", "Auto lamp Sensor Input Circuit Short to Battery"},
    {"B1793", "Auto lamp Sensor Input Circuit Short to Ground"},
    {"B1794", "Lamp Headlamp LowBeam Circuit Failure"},
    {"B1795", "Lamp Headlamp LowBeam Circuit Open"},
    {"B1796", "Lamp Headlamp LowBeam Circuit Short To Battery"},
    {"B1797", "Lamp Headlamp LowBeam Circuit Short To Ground"},
    {"B1798", "Lamp Turn Signal Front Output Circuit Failure"},
    {"B1799", "Lamp Turn Signal Front Output Circuit Open"},
    {"B1800", "Lamp Turn Signal Front Output Circuit Short To Battery"},
    {"B1801", "Lamp Turn Signal Front Output Circuit Short To Ground"},
    {"B1802", "Lamp Turn Signal Rear Output Circuit Failure"},
    {"B1803", "Lamp Turn Signal Rear Output Circuit Open"},
    {"B1804", "Lamp Turn Signal Rear Output Circuit Short To Battery"},
    {"B1805", "Lamp Turn Signal Rear Output Circuit Short To Ground"},
    {"B1806", "Lamp Tail Output Circuit Failure"},
    {"B1807", "Lamp Tail Output Circuit Open"},
    {"B1808", "Lamp Tail Output Circuit Short To Battery"},
    {"B1809", "Lamp Tail Output Circuit Short To Ground"},
    {"B1810", "Lamp Backup Switch Input Circuit Failure"},
    {"B1811", "Lamp Backup Switch Input Circuit Open"},
    {"B1812", "Lamp Backup Switch Input Circuit Short To Battery"},
    {"B1813", "Lamp Backup Switch Input Circuit Short To Ground"},
    {"B1814", "Wiper Rear Motor down Relay Coil Circuit Failure"},
    {"B1815", "Wiper Rear Motor down Relay Coil Circuit Open"},
    {"B1816", "Wiper Rear Motor down Relay Coil Circuit Short to Battery"},
    {"B1817", "Wiper Rear Motor down Relay Coil Circuit Short to Ground"},
    {"B1818", "Wiper Rear Motor up Relay Coil Circuit Failure"},
    {"B1819", "Wiper Rear Motor up Relay Coil Circuit Open"},
    {"B1820", "Wiper Rear Motor up Relay Coil Circuit Short to Battery"},
    {"B1821", "Wiper Rear Motor up Relay Coil Circuit Short to Ground"},
    {"B1822", "Wiper Rear Park Sense Input Circuit Failure"},
    {"B1823", "Wiper Rear Park Sense Input Circuit Open"},
    {"B1824", "Wiper Rear Park Sense Input Circuit Short To Battery"},
    {"B1825", "Wiper Rear Park Sense Input Circuit Short To Ground"},
    {"B1826", "Wiper Rear High Limit Input Circuit Failure"},
    {"B1827", "Wiper Rear High Limit Input Circuit Open"},
    {"B1828", "Wiper Rear High Limit Input Circuit Short To Battery"},
    {"B1829", "Wiper Rear High Limit Input Circuit Short To Ground"},
    {"B1830", "Door Unlock Disarm Switch Circuit Failure"},
    {"B1831", "Door Unlock Disarm Switch Circuit Open"},
    {"B1832", "Door Unlock Disarm Switch Circuit Short To Battery"},
    {"B1833", "Door Unlock Disarm Switch Circuit Short To Ground"},
    {"B1834", "Door Unlock Disarm Output Circuit Failure"},
    {"B1835", "Door Unlock Disarm Output Circuit Open"},
    {"B1836", "Door Unlock Disarm Output Circuit Short To Battery"},
    {"B1837", "Door Unlock Disarm Output Circuit Short To Ground"},
    {"B1838", "Battery Saver Power Relay Circuit Failure"},
    {"B1839", "Wiper Rear Motor Circuit Failure"},
    {"B1840", "Wiper Front Power Circuit Failure"},
    {"B1841", "Wiper Front Power Circuit Open"},
    {"B1842", "Wiper Front Power Circuit Short To Battery"},
    {"B1843", "Wiper Front Power Circuit Short To Ground"},
    {"B1844", "Phone Handset Circuit Failure"},
    {"B1845", "Ignition Tamper Circuit Failure"},
    {"B1846", "Ignition Tamper Circuit Open"},
    {"B1847", "Ignition Tamper Circuit Short To Battery"},
    {"B1848", "Ignition Tamper Circuit Short To Ground"},
    {"B1849", "Climate Control Temperature Differential Circuit Failure"},
    {"B1850", "Climate Control Temperature Differential Circuit Open"},
    {"B1851", "Climate Control Temperature Differential Circuit Short To Battery"},
    {"B1852", "Climate Control Temperature Differential Circuit Short To Ground"},
    {"B1853", "Climate Control Air Temperature Internal Sensor Motor Circuit Failure"},
    {"B1854", "Climate Control Air Temperature Internal Sensor Motor Circuit Open"},
    {"B1855", "Climate Control Air Temperature Internal Sensor Motor Circuit Short To Battery"},
    {"B1856", "Climate Control Air Temperature Internal Sensor Motor Circuit Short To Ground"},
    {"B1857", "Climate Control On/Off Switch Circuit Failure"},
    {"B1858", "Climate Control A/C Pressure Switch Circuit Failure"},
    {"B1859", "Climate Control A/C Pressure Switch Circuit Open"},
    {"B1860", "Climate Control A/C Pressure Switch Circuit Short To Battery"},
    {"B1861", "Climate Control A/C Pressure Switch Circuit Short To Ground"},
    {"B1862", "Climate Control A/C Lock Sensor Failure"},
    {"B1863", "Ground ECU Circuit Open"},
    {"B1864", "Battery Power Supply ECU Circuit Failure"},
    {"B1865", "Battery Power Supply ECU Circuit Open"},
    {"B1866", "Battery Power Supply ECU Circuit Short To Battery"},
    {"B1867", "Battery Power Supply ECU Circuit Short To Ground"},
    {"B1868", "Lamp Air Bag Warning Indicator Circuit Failure"},
    {"B1869", "Lamp Air Bag Warning Indicator Circuit Open"},
    {"B1870", "Lamp Air Bag Warning Indicator Circuit Short To Battery"},
    {"B1871", "Passenger Air Bag Disable Module Fault"},
    {"B1872", "Turn Signal / Hazard Power Feed Circuit Short To Battery"},
    {"B1873", "Turn Signal / Hazard Power Feed Circuit Short To Ground"},
    {"B1874", "Cellular Phone Handset Not Present"},
    {"B1875", "Turn Signal / Hazard Switch Signal Circuit Failure"},
    {"B1876", "Seatbelt Driver Pretension Circuit Failure"},
    {"B1877", "Seatbelt Driver Pretensioner Circuit Open"},
    {"B1878", "Seatbelt Driver Pretensioner Circuit Short to Battery"},
    {"B1879", "Seatbelt Driver Pretensioner Circuit Short to Ground"},
    {"B1880", "Seatbelt Passenger Pretensioner Circuit Failure"},
    {"B1881", "Seatbelt Passenger Pretensioner Circuit Open"},
    {"B1882", "Seatbelt Passenger Pretensioner Circuit Short to Battery"},
    {"B1883", "Seatbelt Passenger Pretensioner Circuit Short to Ground"},
    {"B1884", "PAD Warning Lamp Inoperative"},
    {"B1885", "Seatbelt Driver Pretensioner Circuit Resistance Low on Squib"},
    {"B1886", "Seatbelt Passenger Pretensioner Circuit Resistance Low on Squib"},
    {"B1887", "Air Bag Driver Circuit Resistance Low or Shorted Together"},
    {"B1888", "Air Bag Passenger Circuit Resistance Low or Shorted Together"},
    {"B1889", "Passenger Airbag Disable Module Sensor Obstructed"},
    {"B1890", "PAD Warning Lamp Circuit Short to Battery"},
    {"B1891", "Air Bag Tone Warning Indicator Circuit Short to Battery"},
    {"B1892", "Air Bag Tone Warning Indicator Circuit Failure"},
    {"B1893", "GPS Antenna Open Circuit"},
    {"B1894", "Wiper Rear Motor Speed Sense Circuit Failure"},
    {"B1895", "Driver's / Passenger's Door Ajar Output Circuit Failure"},
    {"B1896", "Driver's / Passenger's Door Ajar Output Circuit Short to Battery"},
    {"B1897", "Horn Switch Circuit Failure"},
    {"B1898", "Chime Input #2 Circuit Short to Ground"},
    {"B1899", "Microphone Input Signal Circuit Open"},
    {"B1900", "Driver Side Airbag Fault"},
    {"B1901", "Air Bag Crash Sensor #1 Feed/Return Circuit Short To Ground"},
    {"B1902", "Air Bag Crash Sensor #1 Ground Circuit Failure"},
    {"B1903", "Air Bag Crash Sensor #1 Ground Circuit Short To Battery"},
    {"B1904", "Air Bag Crash Sensor #2 Feed/Return Circuit Failure"},
    {"B1905", "Air Bag Crash Sensor #2 Feed/Return Circuit Short To Battery"},
    {"B1906", "Air Bag Crash Sensor #2 Feed/Return Circuit Short To Ground"},
    {"B1907", "Air Bag Crash Sensor #2 Ground Circuit Failure"},
    {"B1908", "Air Bag Crash Sensor #2 Ground Circuit Short To Battery"},
    {"B1909", "Air Bag Crash Sensor #2 Ground Circuit Short To Ground"},
    {"B1910", "Air Bag Diagnostic Monitor Ground Circuit Failure"},
    {"B1911", "Air Bag Diagnostic Monitor Ground Circuit Short To Battery"},
    {"B1912", "Air Bag Diagnostic Monitor Ground Circuit Short To Ground"},
    {"B1913", "Air Bag Driver/Passenger Circuit Short To Ground"},
    {"B1914", "Air Bag Crash Sensors #1 / #2 Circuit Short To Ground"},
    {"B1915", "Air Bag Driver Circuit Failure"},
    {"B1916", "Air Bag Driver Circuit Short To Battery"},
    {"B1917", "Air Bag Memory Clear Circuit Failure"},
    {"B1918", "Air Bag Memory Clear Circuit Open"},
    {"B1919", "Air Bag Memory Clear Circuit Short To Battery"},
    {"B1920", "Air Bag Passenger Circuit Failure"},
    {"B1921", "Air Bag Diagnostic Monitor Ground Circuit Open"},
    {"B1922", "Air Bag Seeing Sensor Output Circuit Short To Battery"},
    {"B1923", "Air Bag Memory Clear Circuit Short To Ground"},
    {"B1924", "Air Bag Internal Diagnostic Monitor Fault or System Disarm Fault"},
    {"B1925", "Air Bag Passenger Circuit Short To Battery"},
    {"B1926", "Air Bag Passenger Pressure Switch Circuit Failure"},
    {"B1927", "Passenger Side Airbag Fault"},
    {"B1928", "Air Bag Safing Sensor Output Circuit Failure"},
    {"B1929", "Air Bag Safing Sensor Output Circuit Open"},
    {"B1930", "Air Bag Safing Sensor Output Circuit Short To Ground"},
    {"B1931", "Air Bag Crash Sensor #1 Feed/Return Circuit Failure"},
    {"B1932", "Air Bag Driver Circuit Open"},
    {"B1933", "Air Bag Passenger Circuit Open"},
    {"B1934", "Air Bag Driver Inflator Circuit Resistance Low on Squib"},
    {"B1935", "Air Bag Passenger Inflator Circuit Resistance Low on Squib"},
    {"B1936", "Air Bag Driver Circuit Short To Ground"},
    {"B1937", "Air Bag Passenger Pressure Switch Circuit Open"},
    {"B1938", "Air Bag Passenger Circuit Short To Ground"},
    {"B1939", "Air Bag Passenger Pressure Switch Circuit Short To Ground"},
    {"B1941", "Air Bag Crash Sensor #1 Feed/Return Circuit Open"},
    {"B1942", "Air Bag Crash Sensor #2 Feed/Return Circuit Open"},
    {"B1943", "Air Bag Crash Sensor #1 Ground Circuit Short To Ground"},
    {"B1944", "Air Bag Crash Sensor #1 Ground Circuit Open"},
    {"B1945", "Air Bag Crash Sensor #2 Ground Circuit Open"},
    {"B1946", "Climate Control A/C Post Evaporator Sensor Circuit Failure"},
    {"B1947", "Climate Control A/C Post Evaporator Sensor Circuit Short To Ground"},
    {"B1948", "Climate Control Water Temperature Sensor Circuit Failure"},
    {"B1949", "Climate Control Water Temperature Sensor Circuit Short To Ground"},
    {"B1950", "Seat Rear Up/Down Potentiometer Feedback Circuit Failure"},
    {"B1951", "Seat Rear Up/Down Potentiometer Feedback Circuit Open"},
    {"B1952", "Seat Rear Up/Down Potentiometer Feedback Circuit Short To Battery"},
    {"B1953", "Seat Rear Up/Down Potentiometer Feedback Circuit Short To Ground"},
    {"B1954", "Seat Front Up/Down Potentiometer Feedback Circuit Failure"},
    {"B1955", "Seat Front Up/Down Potentiometer Feedback Circuit Open"},
    {"B1956", "Seat Front Up/Down Potentiometer Feedback Circuit Short To Battery"},
    {"B1957", "Seat Front Up/Down Potentiometer Feedback Circuit Short To Ground"},
    {"B1958", "Seat Recline Forward/Backward Potentiometer Feedback Circuit Failure"},
    {"B1959", "Seat Recline Forward/Backward Potentiometer Feedback Circuit Open"},
    {"B1960", "Seat Recline Forward/Backward Potentiometer Feedback Circuit Short To Battery"},
    {"B1961", "Seat Recline Forward/Backward Potentiometer Feedback Circuit Short To Ground"},
    {"B1962", "Seat Horizontal Forward/Rearward Potentiometer Feedback Circuit Failure"},
    {"B1963", "Seat Horizontal Forward/Rearward Potentiometer Feedback Circuit Open"},
    {"B1964", "Seat Horizontal Forward/Rearward Potentiometer Feedback Circuit Short To Battery"},
    {"B1965", "Seat Horizontal Forward/Rearward Potentiometer Feedback Circuit Short To Ground"},
    {"B1966", "A/C Post Heater Sensor Circuit Failure"},
    {"B1967", "A/C Post Heater Sensor Circuit Short To Ground"},
    {"B1968", "A/C Water Pump Detection Circuit Failure"},
    {"B1969", "A/C Clutch Magnetic Control Circuit Failure"},
    {"B1970", "Passenger Seatback Forward Switch Circuit Short to Ground"},
    {"B1971", "Passenger Seatback Rearward Switch Circuit Short to Ground"},
    {"B1972", "Passenger Rear Seat up Switch Circuit Short to Battery"},
    {"B1973", "Passenger Rear Seat down Switch Circuit Short to Battery"},
    {"B1974", "Passenger's Seat Recline Forward Switch Circuit Short to Battery"},
    {"B1975", "Passenger's Seat Recline Back Switch Circuit Short to Battery"},
    {"B1976", "Passenger's Seat Forward Switch Circuit Short to Battery"},
    {"B1977", "Passenger's Front Seat up Switch Circuit Short to Battery"},
    {"B1978", "Passenger's Front Seat down Switch Circuit Short to Battery"},
    {"B1979", "Passenger Seat Rearward Switch Circuit Short to Battery"},
    {"B1980", "Bulb Outage Condition Detected"},
    {"B1981", "Memory off Switch Circuit Short to Battery"},
    {"B1982", "Driver's Door Unlock Relay Circuit Failure"},
    {"B1983", "Driver's Door Unlock Relay Circuit Short to Battery"},
    {"B1984", "Seat Switch Lumbar Inflate Circuit Failure"},
    {"B1985", "Seat Switch Lumbar Deflate Circuit Failure"},
    {"B1986", "Driver's Seat Seatback Auto glides Rearward Switch Circuit Failure"},
    {"B1987", "Pedal Forward / Rearward Motor Stalled"},
    {"B1988", "Pedal Position Forward Switch Circuit Short to Battery"},
    {"B1989", "Pedal Position Rearward Switch Circuit Short to Battery"},
    {"B1990", "Pedal Forward / Rearward Potentiometer Feedback Circuit Failure"},
    {"B1991", "Pedal Forward / Rearward Potentiometer Feedback Circuit Short to Battery"},
    {"B1992", "Driver Side, Side mount Airbag Circuit Short to V batt"},
    {"B1993", "Driver Side, Side mount Airbag Circuit Short to Ground"},
    {"B1994", "Driver Side, Side mount Airbag Circuit Open"},
    {"B1995", "Driver Side, Side mount Airbag Low resistance on Squib"},
    {"B1996", "Passenger Side, Side mount Airbag Circuit Short to V batt"},
    {"B1997", "Passenger Side, Side mount Airbag Circuit Short to Ground"},
    {"B1998", "Passenger Side, Side mount Airbag Circuit Open"},
    {"B1999", "Passenger Side, Side mount Airbag Low resistance on Squib"},
    {"B2075", "Lumbar Switch Control Circuit"},
    {"B2080", "Lumbar Switch Control Circuit"},
    {"B2085", "Lumbar Switch Control Circuit"},
    {"B2090", "Lumbar Switch Control Circuit"},
    {"B2100", "Door Driver Key Cylinder Switch Failure"},
    {"B2101", "Head Rest Switch Circuit Failure"},
    {"B2102", "Antenna Circuit Short to Ground"},
    {"B2103", "Antenna Not Connected"},
    {"B2104", "Door Passenger Key Cylinder Switch Failure"},
    {"B2105", "Throttle Position Input Out of Range Low"},
    {"B2106", "Throttle Position Input Out of Range High"},
    {"B2107", "Front Wiper Motor Relay Circuit Short to V batt"},
    {"B2108", "Trunk Key Cylinder Switch Failure"},
    {"B2109", "Heated Wind Shield Relay Short to V battÂ"},
    {"B2110", "Front Wiper Motor Relay Circuit Open (changed from Failure 2/6/97)"},
    {"B2111", "All Door Lock Input Short to Ground"},
    {"B2112", "Door Driver Set Switch Stuck Failure"},
    {"B2113", "Heated Windshield Input Short to Ground"},
    {"B2114", "Front Washer Input Short to Ground"},
    {"B2115", "Rear Washer Input Short to Ground"},
    {"B2116", "Door Driver Reset Switch Stuck Failure"},
    {"B2117", "Driver Side, Side mount Airbag Low capacitance on Squib"},
    {"B2118", "Passenger Side, Side mount Airbag Low capacitance on Squib"},
    {"B2119", "Compressor Failure"},
    {"B2120", "Door Passenger Set Switch Stuck Failure"},
    {"B2122", "Driver Side Satellite Communication Circuit Short to Ground"},
    {"B2123", "Passenger Side Satellite Communication Circuit Short to Ground"},
    {"B2124", "Door Passenger Reset Switch Stuck Failure"},
    {"B2128", "Central Lock Motor Failure"},
    {"B2129", "Central Lock Feedback Failure"},
    {"B2130", "Double Lock Timeout Failure"},
    {"B2131", "Double Lock Feedback Failure"},
    {"B2132", "Dimmer switch Circuit Short to Gnd"},
    {"B2133", "Brake Motor Warning lamp Circuit Failure"},
    {"B2134", "Brake Motor Warning lamp Circuit Short to V batt"},
    {"B2135", "Park Brake Applied Warning Lamp Circuit Failure"},
    {"B2136", "Park Brake Applied Warning Lamp Circuit Short To Battery"},
    {"B2139", "Data Mismatch (receive data does not match what was expected)"},
    {"B2140", "Mirror Up Control Switch Circuit Malfunction"},
    {"B2141", "NVM Configuration Failure"},
    {"B2142", "NVM TIC Failure"},
    {"B2143", "NVM Memory Failure"},
    {"B2144", "NVM Alarm Data Failure"},
    {"B2145", "NVM RF HR Failure"},
    {"B2146", "Seat Recline Motor Position Out of Range"},
    {"B2147", "Left Front Mirror Horizontal Position Sensor Circuit High"},
    {"B2148", "PWM Input Circuit Failure"},
    {"B2149", "Seat Front Vertical Motor Position Out of Range"},
    {"B2150", "Power Supply #1 Circuit Short to Ground"},
    {"B2151", "Power Supply #2 Circuit Short to Ground"},
    {"B2152", "Seat Rear Vertical Motor Position Out of Range"},
    {"B2153", "Rear Echo Sensor Circuit Failure"},
    {"B2154", "Front Echo Sensor Circuit Failure"},
    {"B2155", "Seat Horizontal Motor Position Out of Range"},
    {"B2156", "Rear Doppler Sensor Circuit Failure"},
    {"B2157", "Front Doppler Sensor Circuit Failure"},
    {"B2158", "Seat Recline Motor Memory Position Out of Range"},
    {"B2159", "Memory #1 output Short to Ground"},
    {"B2160", "Memory #1 output Short to V Batt"},
    {"B2161", "Seat Front Vertical Motor Memory Position Out of Range"},
    {"B2162", "Data Mismatch #2 (receive data does not match what was expected)"},
    {"B2163", "Clutch Position Fault"},
    {"B2164", "Seat Rear Vertical Motor Memory Position Out of Range"},
    {"B2165", "Gear shift position Fault"},
    {"B2166", "Gear select position Fault"},
    {"B2167", "Seat Horizontal Motor Memory Position Out of Range"},
    {"B2168", "Unable to confirm Unlock Condition"},
    {"B2169", "Unable to confirm locks Condition"},
    {"B2170", "Steering Column Lock Switch Circuit Failure"},
    {"B2172", "Inertia Switch input Circuit Open"},
    {"B2174", "Window Driver Rear Remote up Switch Short to Battery"},
    {"B2175", "A/C Request Signal Circuit Short to Ground"},
    {"B2176", "Overdrive switch circuit short to V batt"},
    {"B2177", "Interior Scanning Sensor Circuit Failure"},
    {"B2178", "Window Driver Rear Remote down Switch Short to Battery"},
    {"B2179", "Front Wiper Select Switch A Short to Ground"},
    {"B2180", "Front Wiper Select Switch B Short to Ground"},
    {"B2181", "Front Wiper Select Switch C Short to Ground"},
    {"B2182", "Window Passenger Front Remote Up Switch Short to Battery"},
    {"B2183", "Front Wiper Select Switch H Short to Ground"},
    {"B2184", "Front Wiper Select Switch W Short to Ground"},
    {"B2185", "Rear Wiper Select Switch D Short to Ground"},
    {"B2186", "Window Passenger Front Remote Down Switch Short to Battery"},
    {"B2187", "Rear Wiper Select Switch B Short to Ground"},
    {"B2188", "Rear Wiper Select Switch E Short to Ground"},
    {"B2190", "Window Passenger Rear Remote up Switch Short to Battery"},
    {"B2194", "Window Passenger Rear Remote down Switch Short to Battery"},
    {"B2195", "Driver Window Up / Down Power Circuit Short to Ground"},
    {"B2196", "Passenger Window Up / Down Power Circuit Short to Ground"},
    {"B2197", "TV Module Error"},
    {"B2198", "Traffic Master Module Error"},
    {"B2199", "VICS Module Error"},
    {"B2200", "No Communication to TV Module (No Fitting of TV)"},
    {"B2201", "No Communication with Traffic Master Module"},
    {"B2202", "No Communication to VICS Module (No Fitting of VICS)"},
    {"B2203", "CDROM Error"},
    {"B2204", "GPS Antenna Connection Open or Short"},
    {"B2205", "GPS Receiver Error"},
    {"B2206", "Gyroscope Error"},
    {"B2207", "ECU ROM Checksum Error"},
    {"B2208", "Communication Link to Display and Switch Module Error"},
    {"B2209", "Interior Lamp Override Switch Open Circuit"},
    {"B2210", "Interior Lamp Override Switch Short to Ground"},
    {"B2211", "Low Coolant Lamp Output Circuit Short to Battery"},
    {"B2212", "Right Front Switch on Left Front Door Stuck Up Position"},
    {"B2213", "Right Front Switch on Left Front Door Stuck Down Position"},
    {"B2214", "Window Passenger Front up Switch Short to Battery"},
    {"B2215", "Window Passenger Front down Switch Short to Battery"},
    {"B2216", "Right Rear Switch on Left Front Door Stuck Up Position"},
    {"B2217", "Right Rear Switch on Left Front Door Stuck Down Position"},
    {"B2219", "Window Driver Front Current Feedback Exceeded"},
    {"B2220", "Window Driver Rear Current Feedback Exceeded"},
    {"B2221", "Window Passenger Front Current Feedback Exceeded"},
    {"B2222", "Window Passenger Rear Current Feedback Exceeded"},
    {"B2223", "Mirror Driver Drive Circuit Failure"},
    {"B2224", "Mirror Passenger Drive Circuit Failure"},
    {"B2225", "Front Crash Sensor Mount Fault"},
    {"B2226", "Front Crash Sensor Internal Fault"},
    {"B2227", "Front Crash Sensor Driver Communications Fault"},
    {"B2228", "Air Bag Driver Circuit Short to Ground Loop #2"},
    {"B2229", "Air Bag Passenger Circuit Short to Ground Loop #2"},
    {"B2230", "Air Bag Driver Circuit Short to Battery Loop #2"},
    {"B2231", "Air Bag Passenger Circuit Short to Battery Loop #2"},
    {"B2232", "Air Bag Driver Circuit Open Loop #2"},
    {"B2233", "Air Bag passenger Circuit Open Loop #2"},
    {"B2234", "Air Bag Driver Inflator Circuit Resistance Low on Squib Loop #2"},
    {"B2235", "Air Bag Passenger Inflator Circuit"},
    {"B2236", "Weak or Defected Electric Vehicle Battery Module Fault"},
    {"B2237", "Vehicle Signal indicating Park While VSS Present"},
    {"B2238", "Power Cable for Power Sliding Door Broken"},
    {"B2239", "Rear Cargo Door Set Switch Stuck (Short to Ground)"},
    {"B2240", "Rear Cargo Door Reset Switch Stuck (Short to Ground)"},
    {"B2241", "Rear Cargo Door Lock Circuit Short to Ground"},
    {"B2242", "Rear Cargo Door Unlock Circuit Open"},
    {"B2243", "Driver Rear Door Ajar Circuit Open"},
    {"B2244", "Driver Sliding Door Ajar Circuit Short to GND"},
    {"B2245", "Passenger Rear Door Ajar Circuit Open"},
    {"B2246", "Passenger Sliding Door Ajar Circuit Short to GND"},
    {"B2247", "EV Battery Pack Temperature Fault"},
    {"B2248", "Heated Windshield Relay Coil Circuit Failure"},
    {"B2249", "Head Lamp Relay Coil Short to Battery"},
    {"B2250", "All Doors Unlock Relay Circuit Failure"},
    {"B2251", "Park lamp Output Relay Driver Circuit Failure"},
    {"B2252", "Park lamp Output Relay Driver Short to Battery"},
    {"B2300", "Seat Driver Memory Position Error"},
    {"B2301", "Seat Passenger Memory Position Error"},
    {"B2302", "Seat Headrest Feedback Potentiometer Circuit Failure"},
    {"B2303", "Seat Headrest Feedback Potentiometer Circuit Open"},
    {"B2304", "Seat Headrest Feedback Potentiometer Circuit Short to Battery"},
    {"B2305", "Seat Headrest Feedback Potentiometer Circuit Short to Ground"},
    {"B2306", "Seat Headrest Motor Stalled"},
    {"B2310", "Mirror Driver Memory Position Error"},
    {"B2311", "Mirror Passenger Memory Position Error"},
    {"B2312", "Mirror Passenger Horizontal Feedback Potentiometer Circuit Failure"},
    {"B2313", "Mirror Passenger Horizontal Feedback Potentiometer Circuit Open"},
    {"B2314", "Mirror Passenger Horizontal Feedback Potentiometer Circuit Short to Battery"},
    {"B2315", "Mirror Passenger Horizontal Feedback Potentiometer Circuit Short to Ground"},
    {"B2316", "Mirror Passenger Vertical Feedback Potentiometer Circuit Failure"},
    {"B2317", "Mirror Passenger Vertical Feedback Potentiometer Circuit Open"},
    {"B2318", "Mirror Passenger Vertical Feedback Potentiometer Circuit Short to Battery"},
    {"B2319", "Mirror Passenger Vertical Feedback Potentiometer Circuit Short to Ground"},
    {"B2320", "Mirror Driver Horizontal Feedback Potentiometer Circuit Failure"},
    {"B2321", "Mirror Driver Horizontal Feedback Potentiometer Circuit Open"},
    {"B2322", "Mirror Driver Horizontal Feedback Potentiometer Circuit Short to Battery"},
    {"B2323", "Mirror Driver Horizontal Feedback Potentiometer Circuit Short to Ground"},
    {"B2324", "Mirror Driver Vertical Feedback Potentiometer Circuit Failure"},
    {"B2325", "Mirror Driver Vertical Feedback Potentiometer Circuit Open"},
    {"B2326", "Mirror Driver Vertical Feedback Potentiometer Circuit Short to Battery"},
    {"B2327", "Mirror Driver Vertical Feedback Potentiometer Circuit Short to Ground"},
    {"B2328", "Column Reach Feedback Potentiometer Circuit Failure"},
    {"B2329", "Column Reach Feedback Potentiometer Circuit Open"},
    {"B2330", "Column Reach Feedback Potentiometer Circuit Short to Battery"},
    {"B2331", "Column Reach Feedback Potentiometer Circuit Short to Ground"},
    {"B2332", "Column Tilt Feedback Potentiometer Circuit Failure"},
    {"B2333", "Column Tilt Feedback Potentiometer Circuit Open"},
    {"B2334", "Column Tilt Feedback Potentiometer Circuit Short to Battery"},
    {"B2335", "Column Tilt Feedback Potentiometer Circuit Short to Ground"},
    {"B2336", "Mirror Switch Assembly Circuit Failure"},
    {"B2337", "Mirror Switch Assembly Circuit Open"},
    {"B2338", "Mirror Switch Assembly Circuit Short to Battery"},
    {"B2339", "Mirror Switch Assembly Circuit Short to Ground"},
    {"B2340", "Column Reach Motor Stalled"},
    {"B2341", "Column Tilt Motor Stalled"},
    {"B2342", "Seat Switch Reference Voltage Positive Common Open Circuit"},
    {"B2343", "Seat Switch Reference Voltage Positive Common Supply Low Voltage"},
    {"B2344", "Seat Switch Reference Voltage Positive Common Supply Voltage Fault"},
    {"B2345", "Seat Switch Reference Voltage Negative Common Open Circuit"},
    {"B2346", "Mirror Switch Reference Voltage Positive Common Open Circuit"},
    {"B2347", "Mirror Switch Reference Voltage Positive Common Supply Low Voltage"},
    {"B2348", "Mirror Switch Reference Voltage Positive Common Supply Voltage Fault"},
    {"B2349", "Mirror Switch Reference Voltage Negative Common Open Circuit"},
    {"B2350", "Steering Column Switch Circuit Short to Battery"},
    {"B2351", "Steering Column Switch Circuit Failure"},
    {"B2352", "Driver Memory Power Switch Indicator Circuit Short to Battery"},
    {"B2353", "Driver Mirror Power Driver Circuit Short Ground"},
    {"B2354", "Driver Mirror Horizontal / Vertical Feedback Potentiometer Circuit Open"},
    {"B2355", "Passenger Mirror Horizontal / Vertical Feedback Potentiometer Circuit Open"},
    {"B2357", "Driver Window Down Current Sense Low Circuit Failure"},
    {"B2362", "Remote Open/Close signal Circuit Short to Ground"},
    {"B2363", "Optical Sensor System Failure"},
    {"B2364", "Fuel Filler Door Circuit Open"},
    {"B2365", "Bpillar Power Sliding Door Open/Close Switch Input Ckt Short to Gnd"},
    {"B2366", "IP Power Sliding Door Open/Close switch Ckt Short to Gnd"},
    {"B2367", "Power Sliding Door Override Switch Input Ckt Short to Gnd"},
    {"B2368", "Steering Column Switch Circuit Out of Range"},
    {"B2369", "Chime OUTPUT Request Ckt Short to Ground"},
    {"B2373", "LED #1 Circuit Short to Battery"},
    {"B2374", "Power Sliding Detent (Latch) Circuit Failure"},
    {"B2375", "Driver Seat Asm Horiz Sensor Circuit Malfunction"},
    {"B2380", "Heater Coolant Temp sensor circuit Short to GND"},
    {"B2381", "Heater Coolant Temp sensor circuit Open"},
    {"B2384", "Audio Reverse Aid Mute Input Ckt Failure"},
    {"B2385", "Audio Navigation Mute Input Ckt Failure"},
    {"B2401", "Audio Tape Deck Mechanism Fault"},
    {"B2402", "Audio CD/DJ Thermal Shutdown Fault"},
    {"B2403", "Audio CD/DJ Internal Fault"},
    {"B2404", "Audio Steering Wheel Switch Circuit Fault"},
    {"B2405", "Audio SingleDisc CD player Thermal Shutdown Fault"},
    {"B2406", "Audio SingleDisc CD player Internal Fault"},
    {"B2416", "Climate Control Recirculation Actuator Out of Limits"},
    {"B2419", "Twilight Photocell Circuit Problem"},
    {"B2420", "Twilight Delay Pot Failure"},
    {"B2422", "Interior Lamp Rheostat Failure"},
    {"B2425", "Remote Keyless Entry Out of Synchronization"},
    {"B2426", "Passenger Solar Radiation Sensor Circuit Open"},
    {"B2427", "Passenger Solar Radiation Sensor Circuit Short to Ground"},
    {"B2428", "A/C Post Heater Sensor #2 Circuit Failure"},
    {"B2429", "A/C Post Heater Sensor #2 Circuit Short to Ground"},
    {"B2430", "Seat Heater Circuit"},
    {"B2431", "Transponder Programming Failed"},
    {"B2432", "Drivers Seat Belt Buckle Switch Circuit Open"},
    {"B2433", "Drivers Seat Belt Buckle Switch Circuit Short to Battery"},
    {"B2434", "Drivers Seat Belt Buckle Switch Circuit Short to Ground"},
    {"B2435", "Drivers Seat Belt Buckle Switch Resistance out of Range"},
    {"B2436", "Passengers Seat Belt Buckle Switch Circuit Open"},
    {"B2437", "Passengers Seat Belt Buckle Switch Circuit Short to Battery"},
    {"B2438", "Passengers Seat Belt Buckle Switch Circuit Short to Ground"},
    {"B2439", "Passengers Seat Belt Buckle Switch Resistance out of Range"},
    {"B2440", "Passenger Side, Side crash sensor mount fault"},
    {"B2441", "Driver Side, Side crash sensor mount fault"},
    {"B2442", "Intrusion Sensor Fault"},
    {"B2443", "Power train Performance Mode Switch Circuit failure"},
    {"B2444", "Driver Side Crash Sensor Internal Fault"},
    {"B2445", "Passenger Side Crash Sensor Internal Fault"},
    {"B2446", "RESCU/VEMS Input Circuit Open"},
    {"B2447", "RESCU/VEMS Input Circuit Short to Battery"},
    {"B2448", "RESCU/VEMS Input Circuit Short to Ground"},
    {"B2449", "Aux Heater Glow Plug Circuit Short to Ground"},
    {"B2450", "Aux Heater Glow Plug Circuit Open"},
    {"B2451", "Aux Heater Fuel Pump Circuit Short to Ground"},
    {"B2452", "Aux Heater Fuel Pump Circuit Open"},
    {"B2453", "Aux Heater Blower Fan Circuit Short to Ground"},
    {"B2454", "Aux Heater Blower Fan Circuit Open"},
    {"B2455", "Aux Heater Blower Faulted"},
    {"B2456", "Aux Heater Coolant Sensor Circuit Short to Ground"},
    {"B2457", "Aux Heater Coolant Sensor Circuit Open"},
    {"B2458", "Aux Heater Overheat Sensor Circuit Short to Ground"},
    {"B2459", "Aux Heater Overheat Sensor Circuit Open"},
    {"B2460", "Aux Heater Flame Sensor Circuit Short to Ground"},
    {"B2461", "Aux Heater Flame Sensor Circuit Open"},
    {"B2462", "Aux Heater Flame Out Fault"},
    {"B2463", "Aux Heater Overheat Fault"},
    {"B2464", "Aux Heater Start Time Exceeded"},
    {"B2465", "Aux Heater Start Counter Overrun/System Locked"},
    {"B2466", "Aux Heater Overheat Counter Overrun/System Locked"},
    {"B2467", "Aux Heater Cool down Time ExceededÂ"},
    {"B2468", "Aux Heater Coolant Pump Circuit Short to Ground"},
    {"B2469", "Aux Heater Coolant Pump Circuit Open"},
    {"B2470", "Interior Fan Control Circuit Short to Ground"},
    {"B2471", "Interior Fan Control Circuit Open"},
    {"B2472", "Fog Lamp Switch Failure"},
    {"B2473", "Passenger Door Disarm Switch Ckt Short to Ground"},
    {"B2474", "Passenger Door Lock Switch Circuit Shorted to Ground"},
    {"B2475", "Passenger Door Unlock Switch Circuit Shorted to Ground"},
    {"B2476", "RADIO PRESENT SWITCH Ckt Failure"},
    {"B2477", "Module Configuration Failure"},
    {"B2478", "ANTI THEFT INPUT SIGNAL SHORT TO GROUND"},
    {"B2479", "BRAKE PARK SWITCH CIRCUIT SHORT TO GROUND"},
    {"B2480", "LF CORNER LAMP OUTPUT Ckt Short to Battery"},
    {"B2481", "Convertible Top Up/Down switch Fault"},
    {"B2482", "RF CORNER LAMP OUTPUT Ckt Short to Battery"},
    {"B2483", "Enable Signal Open Circuit"},
    {"B2484", "Disable Signal Short to Ground"},
    {"B2485", "LF SIDE REPEATER LAMP OUTPUT Ckt Short to Battery"},
    {"B2487", "RF SIDE REPEATER LAMP OUTPUT Ckt Short to Battery"},
    {"B2489", "UNDERHOOD LAMP OUTPUT CIRCUIT Failure"},
    {"B2490", "UNDERHOOD LAMP OUTPUT CIRCUIT Short to Battery"},
    {"B2491", "RF PARK LAMP OUTPUT CIRCUIT Short to Battery"},
    {"B2492", "Already Programmed"},
    {"B2493", "LF PARK LAMP OUTPUT CIRCUIT Short to Battery"},
    {"B2494", "ANTI THEFT HORN OUTPUT CIRCUIT Short to Batt"},
    {"B2495", "ANTI THEFT HORN OUTPUT CIRCUIT Failure"},
    {"B2496", "ANTI THEFT HORN OUTPUT CIRCUIT Short to Gnd"},
    {"B2499", "COURTESY LAMP OUTPUT Failure"},
    {"B2500", "COURTESY LAMP OUTPUT Ckt Short to Battery"},
    {"B2501", "LF LAMP LOW BEAM CIRCUIT Failure"},
    {"B2502", "LF LAMP LOW BEAM CIRCUIT Short to Battery"},
    {"B2503", "RF LAMP LOW BEAM CIRCUIT Failure"},
    {"B2504", "RF LAMP LOW BEAM CIRCUIT Short to Battery"},
    {"B2505", "LF LAMP HIGH BEAM CIRCUIT Failure"},
    {"B2506", "LF LAMP HIGH BEAM CIRCUIT Short to Battery"},
    {"B2507", "RF LAMP HIGH BEAM CIRCUIT Failure"},
    {"B2508", "RF LAMP HIGH BEAM CIRCUIT Short to Battery"},
    {"B2509", "Rear Fog Lamp Switch Circuit Failure"},
    {"B2510", "Main Blower Motor Relay Circuit Short to Battery"},
    {"B2511", "Horn Output Relay Circuit Short to Battery"},
    {"B2512", "Front Fog Lamp Relay Ckt Short to Battery"},
    {"B2513", "Blower (Fan) Circuit Failure"},
    {"B2514", "Blower (Fan) Circuit Short to V batt"},
    {"B2515", "Heater Blower Relay Circuit Failure"},
    {"B2516", "Blower Control Circuit Failure"},
    {"B2517", "Emergency Power off System Faulted"},
    {"B2518", "Compressor Over temp Fault"},
    {"B2519", "High Mount Stop Lamp Circuit Failure"},
    {"B2520", "High Mount Stop Lamp Circuit Short to Battery"},
    {"B2523", "License Lamp Circuit Failure"},
    {"B2524", "License Lamp Circuit Short to Battery"},
    {"B2525", "Left Rear Backup Lamp Circuit Failure"},
    {"B2526", "Left Rear Backup Lamp Circuit Short to Battery"},
    {"B2527", "Left Rear Stop lamp Circuit Failure"},
    {"B2528", "Left Rear Stop lamp Circuit Short to Battery"},
    {"B2529", "Left Rear Turn Lamp Circuit Failure"},
    {"B2530", "Left Rear Turn Lamp Circuit Short to Battery"},
    {"B2531", "Right Rear Backup Lamp Circuit Failure"},
    {"B2532", "Right Rear Backup Lamp Circuit Short to Battery"},
    {"B2533", "Right Rear Stop lamp Circuit Failure"},
    {"B2534", "Right Rear Stop lamp Circuit Short to Battery"},
    {"B2535", "Right Rear Turn Lamp Circuit Failure"},
    {"B2536", "Right Rear Turn Lamp Circuit Short to Battery"},
    {"B2539", "Aux A/C Mode Position Reference Circuit Short to Ground"},
    {"B2540", "Aux A/C Mode Position Reference Circuit Short to Battery"},
    {"B2543", "Aux A/C Control Switch Reference Circuit Short to Ground"},
    {"B2544", "Aux A/C Control Switch Reference Circuit Short to Battery"},
    {"B2545", "System Power Relay Circuit Short to Battery"},
    {"B2546", "System Power Relay Circuit Failure"},
    {"B2550", "LAMP DOME OUTPUT Circuit Short to Ground"},
    {"B2553", "Disable Signal Output Circuit Short to Battery"},
    {"B2554", "LAMP DOME OUTPUT Circuit Failure"},
    {"B2555", "LAMP DOME OUTPUT Circuit Short to Battery"},
    {"B2556", "Enable Signal Circuit Short to Battery"},
    {"B2557", "Left Power Sliding Door Open/Close Output Circuit Short to Battery"},
    {"B2558", "Right Power Sliding Door Open/Close Output Circuit Short to Battery"},
    {"B2559", "Aux A/C Blower Motor Relay Circuit Short to Battery"},
    {"B2560", "Aux A/C Blower Motor Relay Circuit Short to Ground"},
    {"B2561", "Aux A/C Blower Speed 1 Circuit Failure"},
    {"B2562", "Aux A/C Blower Speed 1 Circuit Short to Ground"},
    {"B2563", "Aux A/C Blower Speed 2 Circuit Failure"},
    {"B2564", "Aux A/C Blower Speed 2 Circuit Short to Ground"},
    {"B2565", "Right Tail Lamp Circuit Failure"},
    {"B2566", "Right Tail Lamp Circuit Short to Ground"},
    {"B2567", "Reverse Mirror Output Circuit Failure"},
    {"B2568", "Reverse Mirror Output Circuit Short to Ground"},
    {"B2569", "Lift gate Disarm Switch Circuit Short to Ground"},
    {"B2570", "Right Lamp Outage Signal Circuit Short to Ground"},
    {"B2571", "Left Lamp Outage Signal Circuit Short to Ground"},
    {"B2575", "Headlamp Control Circuit"},
    {"B2577", "Headlamp Relay Output Circuit Low (BCM)"},
    {"B2578", "Headlamp Relay Output Circuit High (BCM)"},
    {"B2580", "Aux Blower Sense Switch Circuit Failure"},
    {"B2581", "Passenger Seat Occupant Detection Circuit Short To Ground"},
    {"B2582", "Passenger Seat Occupant Detection Circuit Open"},
    {"B2583", "Child Seat Detection Circuit Short to Ground"},
    {"B2584", "Child Seat Detection Circuit Open"},
    {"B2585", "Anti Theft Input Signal Circuit Short To Battery"},
    {"B2586", "Headlamp Mode Select Circuit Failure"},
    {"B2587", "Passenger Seat Occupant Detection Circuit Short To Battery"},
    {"B2588", "Child Seat Detection Circuit Short To Battery"},
    {"B2589", "Unexpected Door Reversal during Close"},
    {"B2590", "Vehicle Park/Speed Signal Circuit Failure"},
    {"B2591", "Detent Signal Missing During Unlatch"},
    {"B2592", "PSD Not Fully Closed (Module Commanded Successfully)"},
    {"B2593", "Power Sliding Door Opened During Module Close Command"},
    {"B2594", "No Movement Detected After an Unlatch During Power Open"},
    {"B2595", "Anti Theft Input Signal Circuit Failure"},
    {"B2596", "Headlamp Aim Output Relay Circuit Short To Battery"},
    {"B2597", "Headlamp Aim Output Relay Circuit Failure"},
    {"B2598", "Headlamp Relay Circuit Failure"},
    {"B2599", "Tailgate Release Open Circuit"},
    {"B2600", "Double Locking Door Motor Frozen"},
    {"B2601", "No Latch Signal Sensed on Closing and Door Reversed"},
    {"B2602", "Missing Latch Signal during Power Sliding Door Unlatch"},
    {"B2603", "PSD Not Fully Closed During SelfTest"},
    {"B2604", "Power Sliding Door On/Off Switch Open Circuit"},
    {"B2605", "Disable Signal Open Circuit"},
    {"B2606", "A/C Temperature Sensor Out of Range"},
    {"B2610", "Passenger Compartment Dimming 1 Circuit"},
    {"B2615", "Passenger Compartment Dimming 2 Circuit"},
    {"B2620", "Display Dimming Input Circuit"},
    {"B2622", "Display Dimming Pulse Width Modulation (PWM) Input Circuit Low (BCM)"},
    {"B2623", "Display Dimming Pulse Width Modulation (PWM) Input Circuit High (BCM)"},
    {"B2625", "Display Dimming Pulse Width Modulation (PWM) Output Circuit"},
    {"B2627", "Display Dimming Pulse Width Modulation (PWM) Output Circuit Low (BCM)"},
    {"B2628", "Display Dimming Pulse Width Modulation (PWM) Output Circuit High (BCM)"},
    {"B2641", "Delayed Exterior Lighting Ctrl Circuit Range"},
    {"B2645", "Ambient Light Sensor Circuit"},
    {"B2646", "Ambient Light Sensor Circuit Range"},
    {"B2647", "Ambient Light Sensor Circuit Low (BCM)"},
    {"B2662", "Park Lamp Relay Output Circuit Low (BCM)"},
    {"B2697", "Headlamp Washer Request Circuit"},
    {"B2700", "Ignition Shift Interlock Circuit (base number)"},
    {"B2701", "Ignition Shift Interlock Circuit Range/Performance"},
    {"B2702", "Ignition Shift Interlock Circuit Low"},
    {"B2703", "Ignition Shift Interlock Circuit High"},
    {"B2704", "Ignition Shift Interlock Circuit Open"},
    {"B2707", "Gearshift Unlock Circuit Low (BCM)"},
    {"B2708", "Gearshift Unlock Circuit High (BCM)"},
    {"B2710", "PASSKey Open/Shorted Pellet"},
    {"B2711", "PASSKey Open/Shorted Pellet After Good Key"},
    {"B2719", "CTDTrunk Tamper Switch Fault"},
    {"B2725", "ATC Mode Switch Circuit Malfunction"},
    {"B2732", "Trunk Release Switch Circuit Low"},
    {"B2734", "Coded Key Controller Memory Failure"},
    {"B2743", "Fog Lamp Switch Circuit High (BCM)"},
    {"B2750", "PASSKey Data Communication Failure"},
    {"B2752", "Horn Relay Coil Circuit Low (BCM)"},
    {"B2753", "Horn Relay Coil Circuit High (BCM)"},
    {"B2757", "Driver Memory 1 Select Switch Circuit Low"},
    {"B2762", "Driver Memory 2 Select Switch Circuit Low"},
    {"B2767", "Memory Set Switch Failed Low"},
    {"B2772", "Driver Memory Exit Switch Circuit Low"},
    {"B2780", "Wrong Resistor Before Good"},
    {"B2781", "Wrong Resistor After Good"},
    {"B2782", "PASSKey Pellet Resistance Not Programmed"},
    {"B2783", "Key Code/Password Corrupted in Memory"},
    {"B2805", "Steering Whl Controls HVAC Switch Circuit Malfunction"},
    {"B2810", "Steering wheel controls Entertainment Circuit Malfunction"},
    {"B2815", "Steering Wheel Controls Illegal Resistance Value"},
    {"B2853", "Telescope Forward Switch Circuit Malfunction"},
    {"B2858", "Telescope Rearward Switch Circuit Malfunction"},
    {"B2860", "Telescope Position Sensor Range"},
    {"B2862", "Telescope Position Sensor Low"},
    {"B2873", "Tilt Up Switch Circuit Malfunction"},
    {"B2878", "Tilt Down Switch Circuit Malfunction"},
    {"B2880", "Tilt Position Sensor Range"},
    {"B2882", "Tilt Position Sensor Low"},
    {"B2891", "Tilt/Telescope Motor Over Current"},
    {"B2904", "Tilt/Telescope 4way Position Switch Circuit Malfunction"},
    {"B2947", "Security System Sensor Power Circuit Low"},
    {"B2948", "Security System Sensor Power Circuit High"},
    {"B2955", "Security System Sensor Data Circuit Malfunction"},
    {"B2957", "Security System Sensor Data Circuit Low"},
    {"B2958", "Security System Sensor Data Circuit High"},
    {"B2960", "Security System Sensor Data Incorrect but Valid"},
    {"B2961", "Key In Ignition Circuit Malfunction"},
    {"B2965", "Key in Ignition Circuit Open (IPC)"},
    {"B3028", "Starter Relay Interface Short to Ground"},
    {"B3029", "Starter Relay Interface Open/Short to B+"},
    {"B3031", "Key Decoder Device in Assem Learn Mode"},
    {"B3033", "Security System Indicates Tamper"},
    {"B3055", "Key Not Present"},
    {"B3060", "Security System Sensor Data Incorrect but Valid"},
    {"B3064", "Driver Door Key Cylinder Circuit"},
    {"B3069", "Right Front/Left Front Door Key Unlock Circuit Low"},
    {"B3108", "Transmitter Synchronization Failure"},
    {"B3109", "3 consecutive low battery signals from the same programmed transmitter"},
    {"B3127", "LF Door Only, Unlock Circuit Low (BCM)"},
    {"B3128", "LF Door Only, Unlock Circuit High (BCM)"},
    {"B3132", "All Door Unlock Circuit Low (BCM)"},
    {"B3133", "All Door Unlock Circuit High (BCM)"},
    {"B3137", "All Door Lock Circuit Low (BCM)"},
    {"B3138", "All Door Lock Circuit High (BCM)"},
    {"B3142", "Left Front Unlock Switch Circuit Low"},
    {"B3147", "Passenger Unlock Switch Circuit Low"},
    {"B3152", "Left Front Lock Switch Circuit Low"},
    {"B3157", "Passenger Lock Switch Circuit Low"},
    {"B3172", "Window Up Switch Circuit Low"},
    {"B3177", "Window Down Switch Circuit Low"},
    {"B3182", "Window Switch Circuit"},
    {"B3187", "Window Switch Circuit"},
    {"B3192", "Window Switch Circuit"},
    {"B3197", "Window Switch Circuit"},
    {"B3203", "Rear Window Lockout Switch Input Circuit High"},
    {"B3282", "Window SwitchExpress Input Shorted to Ground"},
    {"B3287", "Window Switch Circuit"},
    {"B3292", "Window Switch Circuit"},
    {"B3377", "Left Front Window Up Switch Circuit Low"},
    {"B3382", "Left Front Window Down Switch Circuit Low"},
    {"B3387", "Right Front Window Up Switch Circuit Low"},
    {"B3392", "Right Front Window Down Switch Circuit Low"},
    {"B3397", "Left Rear Window Up Switch Circuit Low"},
    {"B3410", "AHLD Front Axle Sensor Signal Circuit"},
    {"B3420", "AHLD Rear Axle Sensor Signal Circuit"},
    {"B3452", "Left Rear Window Down Switch Circuit Low"},
    {"B3457", "Right Rear Window Up Switch Circuit Low"},
    {"B3462", "Right Rear Window Down Switch Circuit Low"},
    {"B3467", "Left Front Window Express Down Circuit Low"},
    {"B3472", "Right Front Window Express Down Circuit Low"},
    {"B3477", "Left Rear Window Express Down Circuit Low"},
    {"B3482", "MalfunctionWindow Express Down Circuit Low"},
    {"B3517", "Left Rear Door Switch Express Up/Down Window Contact Shorted to GND"},
    {"B3522", "Right Rear Door Switch Express Up/Down Window Contact Shorted to GND"},
    {"B3527", "Window Lockout Switch Circuit Low"},
    {"B3531", "Auxiliary HAVC Actuator Circuit"},
    {"B3642", "Seat cool temperature switch shorted"},
    {"B3702", "Intermittent Wiper Delay Input Circuit Low (BCM)"},
    {"B3703", "Intermittent Wiper Delay Input Circuit High/Open (BCM)"},
    {"B3708", "Front Washer Motor Input High (BCM)"},
    {"B3713", "Rear Washer Motor Input Circuit High (BCM)"},
    {"B3717", "Front Wiper Relay Drive Circuit Low (BCM)"},
    {"B3718", "Front Wiper Relay Drive Circuit High (BCM)"},
    {"B3722", "Rear Wiper Relay Drive Circuit Low (BCM)"},
    {"B3723", "Rear Wiper Relay Drive Circuit High (BCM)"},
    {"B3761", "HVAC Actuator Circuit"},
    {"B3770", "HVAC Actuator Circuit"},
    {"B3793", "Memory Seat Module Over Current"},
    {"B3801", "Passenger Compartment Lamp Request Circuit"},
    {"B3802", "Parklamps Request Circuit"},
    {"B3808", "Rear Door Lock Relay Circuit"},
    {"B3809", "Rear Door Unlock Relay Circuit"},
    {"B3810", "Washer Relay Circuit"},
    {"B3811", "Washer Relay Circuit"},
    {"B3819", "Left Rear Power Window Up Relay Circuit"},
    {"B3820", "Left Rear Power Window Down Relay Circuit"},
    {"B3821", "Window Lockout Circuit Low"},
    {"B3822", "Right Rear Power Window Up Relay Circuit"},
    {"B3823", "Right Rear Power Window Down Relay Circuit"},
    {"B3824", "Window Lockout Circuit Low"},
    {"B3832", "Window Position Sensor Circuit"},
    {"B3833", "Window Position Sensor Circuit"},
    {"B3905", "Lumbar Switch Control Circuit"},
    {"B3935", "Transponder Authentication Error"},
    {"C0000", "Vehicle Speed Information Circuit Malfunction"},
    {"C0035", "Left Front Wheel Speed Circuit Malfunction"},
    {"C0040", "Right Front Wheel Speed Circuit Malfunction"},
    {"C0041", "Right Front Wheel Speed Sensor Circuit Range/Performance (EBCM)"},
    {"C0045", "Left Rear Wheel Speed Circuit Malfunction"},
    {"C0046", "Left Rear Wheel Speed Sensor Circuit Range/Performance (EBCM)"},
    {"C0050", "Right Rear Wheel Speed Circuit Malfunction"},
    {"C0051", "LF Wheel Speed Sensor Circuit Range/Performance (EBCM)"},
    {"C0060", "Left Front ABS Solenoid #1 Circuit Malfunction"},
    {"C0065", "Left Front ABS Solenoid #2 Circuit Malfunction"},
    {"C0070", "Right Front ABS Solenoid #1 Circuit Malfunction"},
    {"C0075", "Right Front ABS Solenoid #2 Circuit Malfunction"},
    {"C0080", "Left Rear ABS Solenoid #1 Circuit Malfunction"},
    {"C0085", "Left Rear ABS Solenoid #2 Circuit Malfunction"},
    {"C0090", "Right Rear ABS Solenoid #1 Circuit Malfunction"},
    {"C0095", "Right Rear ABS Solenoid #2 Circuit Malfunction"},
    {"C0110", "Pump Motor Circuit Malfunction"},
    {"C0121", "Valve Relay Circuit Malfunction"},
    {"C0128", "Low Brake Fluid Circuit Low"},
    {"C0141", "Left TCS Solenoid #1 Circuit Malfunction"},
    {"C0146", "Left TCS Solenoid #2 Circuit Malfunction"},
    {"C0151", "Right TCS Solenoid #1 Circuit Malfunction"},
    {"C0156", "Right TCS Solenoid #2 Circuit Malfunction"},
    {"C0161", "ABS/TCS Brake Switch Circuit Malfunction"},
    {"C0221", "Right Front Wheel Speed Sensor Circuit Open"},
    {"C0222", "Right Front Wheel Speed Signal Missing"},
    {"C0223", "Right Front Wheel Speed Signal Erratic"},
    {"C0225", "Left Front Wheel Speed Sensor Circuit Open"},
    {"C0226", "Left Front Wheel Speed Signal Missing"},
    {"C0227", "Left Front Wheel Speed Signal Erratic"},
    {"C0229", "Drop Out of Front Wheel Speed Signals"},
    {"C0235", "Rear Wheel Speed Signal Circuit Open"},
    {"C0236", "Rear Wheel Speed Signal Circuit Missing"},
    {"C0237", "Rear Wheel Speed Signal Erratic"},
    {"C0238", "Wheel Speed Mismatch"},
    {"C0241", "EBCM Control Valve Circuit"},
    {"C0245", "Wheel Speed Sensor Frequency Error"},
    {"C0254", "EBCM Control Valve Circuit"},
    {"C0265", "EBCM Relay Circuit"},
    {"C0266", "EBCM Relay Circuit"},
    {"C0267", "Pump Motor Circuit Open/Shorted"},
    {"C0268", "Pump Motor Circuit Open/Shorted"},
    {"C0269", "Excessive Dump/Isolation Time"},
    {"C0271", "EBCM Malfunction"},
    {"C0272", "EBCM Malfunction"},
    {"C0273", "EBCM Malfunction"},
    {"C0274", "Excessive Dump/Isolation Time"},
    {"C0279", "Powertrain Configuration Not Valid"},
    {"C0281", "Brake Switch Circuit"},
    {"C0283", "Traction Switch Shorted to Ground"},
    {"C0284", "EBCM Malfunction"},
    {"C0286", "ABS Indicator Lamp Circuit Shorted to B+"},
    {"C0287", "Delivered Torque Circuit"},
    {"C0288", "Brake Warning Lamp Circuit Shorted to B+"},
    {"C0290", "Lost Communications With PCM"},
    {"C0291", "Lost Communications With BCM"},
    {"C0292", "Lost Communications With PCM"},
    {"C0297", "Powertrain Configuration Data Not Received"},
    {"C0298", "Powertrain Indicated Traction Control Malfunction"},
    {"C0300", "Rear Speed Sensor Malfunction"},
    {"C0305", "Front Speed Sensor Malfunction"},
    {"C0306", "Motor A or B Circuit"},
    {"C0308", "Motor A/B Circuit Low"},
    {"C0309", "Motor A/B Circuit High"},
    {"C0310", "Motor A/B Circuit Open"},
    {"C0315", "Motor Ground Circuit Open"},
    {"C0321", "Transfer Case Lock Circuit"},
    {"C0323", "TCase Lock Circuit Low"},
    {"C0324", "TCase Lock Circuit High"},
    {"C0327", "Encoder Circuit Malfunction"},
    {"C0357", "Park Switch Circuit High"},
    {"C0359", "Four Wheel Drive Low Range (4LO) Discrete Output Circuit"},
    {"C0362", "4LO Discrete Output Circuit High"},
    {"C0367", "Front Axle Control Circuit High"},
    {"C0374", "General System Malfunction"},
    {"C0376", "Front/Rear Shaft Speed Mismatch"},
    {"C0379", "Front Axle System"},
    {"C0387", "Unable to Perform Shift"},
    {"C0472", "Steering Handwheel Speed Sensor Signal V Low"},
    {"C0473", "Steering Handwheel Speed Sensor Signal V High"},
    {"C0495", "EVO Tracking Error"},
    {"C0498", "Steering Assist Control Actuator Feed Circuit Low"},
    {"C0499", "Steering Assist Control Solenoid Feed Circuit High"},
    {"C0503", "Steering Assist Control Solenoid Return Circuit Low"},
    {"C0504", "Steering Assist Control Solenoid Return Circuit High"},
    {"C0550", "ECU Malfunction internal write / checksum malfunction"},
    {"C0559", "EEPROM Checksum Error"},
    {"C0563", "Calibration ROM Checksum Error"},
    {"C0577", "Left Front Solenoid Circuit Low"},
    {"C0578", "Left Front Solenoid Circuit High"},
    {"C0579", "Left Front Solenoid Circuit Open"},
    {"C0582", "Right Front Solenoid Circuit Low"},
    {"C0583", "Right Front Solenoid Circuit High"},
    {"C0584", "Right Front Solenoid Circuit Open"},
    {"C0587", "Left Rear Solenoid Circuit Low"},
    {"C0588", "Left Rear Solenoid Circuit High"},
    {"C0589", "Left Rear Solenoid Circuit Open"},
    {"C0592", "Right Rear Solenoid Circuit Low"},
    {"C0593", "Right Rear Solenoid Circuit High"},
    {"C0594", "Right Rear Solenoid Circuit Open"},
    {"C0611", "VIN Information Error"},
    {"C0615", "Left Front Position Sensor Malfunction"},
    {"C0620", "Right Front Position Sensor Malfunction"},
    {"C0625", "Left Rear Position Sensor Malfunction"},
    {"C0628", "Level Control Position Sensor Circuit High"},
    {"C0630", "Right Rear Position Sensor Malfunction"},
    {"C0635", "Left Front Normal Force Circuit Malfunction"},
    {"C0638", "Left Front Normal Force Circuit High"},
    {"C0640", "Right Front Normal Force Circuit Malfunction"},
    {"C0643", "Right Front Normal Force Circuit High"},
    {"C0655", "Level Control Compressor Relay Malfunction"},
    {"C0657", "Level Control Compressor Circuit Low"},
    {"C0658", "Level Control Compressor Circuit High"},
    {"C0660", "Level Control Exhaust Valve Circuit Malfunction"},
    {"C0662", "Level Control Exhaust Valve Circuit Low"},
    {"C0663", "Level Control Exhaust Valve Circuit High"},
    {"C0665", "Chassis Pitch Signal Circuit"},
    {"C0690", "Damper Control Relay Circuit Malfunction"},
    {"C0691", "Damper Control Relay Circuit Range"},
    {"C0693", "Damper Control Relay Circuit High"},
    {"C0695", "Position Sensor Overcurrent (8 volt supply)"},
    {"C0696", "Position Sensor Overcurrent (5 volt supply)"},
    {"C0710", "Steering Position Signal Malfunction"},
    {"C0750", "Tire Pressure Monitor (TPM) system sensor not transmitting"},
    {"C0755", "Tire Pressure Monitor (TPM) system sensor not transmitting"},
    {"C0760", "Tire Pressure Monitor (TPM) system sensor not transmitting"},
    {"C0765", "Tire Pressure Monitor (TPM) system sensor not transmitting"},
    {"C0800", "Device Power #1 Circuit Malfunction"},
    {"C0896", "Electronic Suspension Control (ESC) voltage is outside the normal range of 9 to 15.5 volts"},
    {"P0010", "A camshaft position actuator circuit (bank 1)"},
    {"P0011", "A camshaft position timing over advanced or system performance (Bank 1)"},
    {"P0012", "A camshaft position timing over retarded (Bank 1)"},
    {"P0013", "B camshaft position actuator circuit (Bank 1)"},
    {"P0014", "B camshaft position timing over advanced or system performance (Bank 1)"},
    {"P0015", "B camshaft position timing over retarded (Bank 1)"},
    {"P0016", "Crankshaft Position Camshaft Position Correlation Bank 1 Sensor A"},
    {"P0020", "A camshaft position actuator circuit (Bank 2)"},
    {"P0021", "A camshaft postion timing overadvanced or system performance (Bank 2)"},
    {"P0022", "A camshaft position timing over retarded (Bank 2)"},
    {"P0023", "B camshaft position actuator circuit (Bank 2)"},
    {"P0024", "B camshaft position timing over advanced or system performance (Bank 2)"},
    {"P0025", "B camshaft position timing over retaded (Bank 2)"},
    {"P0030", "Heated Oxygen Sensor (H02S) Heater Control Circuit Bank 1 Sensor 1"},
    {"P0031", "Heated Oxygen Sensor (HO2S) Heater Circuit Low Voltage Bank 1 Sensor 1"},
    {"P0032", "Heated Oxygen Sensor (HO2S) Heater Circuit High Voltage Bank 1 Sensor 1"},
    {"P0033", "Turbo charger bypass valve control circuit"},
    {"P0034", "Turbo charger bypass valve control circuit low"},
    {"P0035", "Turbo charger bypass valve control circuit high"},
    {"P0036", "Heated Oxygen Sensor (HO2S) Heater Control Circuit Bank 1 Sensor 2"},
    {"P0037", "Heated Oxygen Sensor (HO2S) Heater Circuit Low Voltage Bank 1 Sensor 2"},
    {"P0038", "Heated Oxygen Sensor (HO2S) Heater Circuit High Voltage Bank 1 Sensor 2"},
    {"P0042", "H02S heater control circuit (Bank 1 sensor 3)"},
    {"P0043", "H02S heater control circuit Low (Bank 1 sensor 3)"},
    {"P0044", "H02S heater control circuit High (Bank 1 sensor 3)"},
    {"P0050", "H02S heater control circuit (Bank 2 sensor 1)"},
    {"P0051", "H02S heater control circuit Low (Bank 2 sensor 1)"},
    {"P0052", "H02S heater control circuit High (Bank 2 sensor 1)"},
    {"P0056", "Heated Oxygen Sensor (HO2S) Heater Circuit Bank 2 Sensor 2"},
    {"P0057", "Heated Oxygen Sensor (HO2S) Heater Circuit Low Voltage Bank 2 Sensor 2"},
    {"P0058", "Heated Oxygen Sensor (HO2S) Heater Circuit High Voltage Bank 2 Sensor 2"},
    {"P0062", "H02S heater control circuit (Bank 2 sensor 3)"},
    {"P0063", "H02S heater control circuit Low (Bank 2 sensor 3)"},
    {"P0064", "H02S heater control circuit High (Bank 2 sensor 3)"},
    {"P0065", "Air assisted injector control range / performance"},
    {"P0066", "Air assisted injector control circuit or circuit low"},
    {"P0067", "Air assisted injector control circuit high"},
    {"P0068", "Throttle Body Airflow Performance (PCM)"},
    {"P0070", "Ambient air temperature sensor circuit"},
    {"P0071", "Ambient air temperature sensor range/performance"},
    {"P0072", "Ambient air temperature sensor circuit low input"},
    {"P0073", "Ambient air temperature sensor circuit high input"},
    {"P0074", "Ambient air temperature sensor circuit intermittent"},
    {"P0075", "Intake valve control solenoid circuit (Bank 1)"},
    {"P0076", "Intake valve control solenoid circuit low (Bank 1)"},
    {"P0077", "Intake valve control solenoid circuit high (Bank 1)"},
    {"P0078", "Exhaust valve control solenoid circuit (Bank 1)"},
    {"P0079", "Exhaust valve control solenoid circuit low (Bank 1)"},
    {"P0080", "Exhaust valve control solenoid circuit high (Bank 1)"},
    {"P0081", "Intake valve control solenoid circuit (Bank 2)"},
    {"P0082", "Intake valve control solenoid circuit low (Bank 2)"},
    {"P0083", "Intake valve control solenoid circuit high (Bank 2)"},
    {"P0084", "Exhaust valve control solenoid circuit (Bank 2)"},
    {"P0085", "Exhaust valve control solenoid circuit low (Bank 2)"},
    {"P0086", "Exhaust valve control solenoid circuit high (Bank 2)"},
    {"P0100", "MAF Sensor Circuit Insufficient Activity"},
    {"P0101", "Mass Air Flow (MAF) Sensor Performance"},
    {"P0102", "Mass Air Flow (MAF) Sensor Circuit Low Frequency"},
    {"P0103", "Mass Air Flow (MAF) Sensor Circuit High Frequency"},
    {"P0104", "Mass Air Flow Circuit Intermittent"},
    {"P0105", "MAP Sensor Circuit Insufficient Activity"},
    {"P0106", "Manifold Absolute Pressure (MAP) System Performance"},
    {"P0107", "Manifold Absolute Pressure (MAP) Sensor Circuit Low Voltage"},
    {"P0108", "Manifold Absolute Pressure (MAP) Sensor Circuit High Voltage"},
    {"P0109", "Manifold Absolute Pressure Circuit Intermittent"},
    {"P0110", "Intake Air Temperature (IAT) Sensor Circuit"},
    {"P0111", "Intake Air Temperature (IAT) Sensor Performance"},
    {"P0112", "Intake Air Temperature (IAT) Sensor Circuit Low Voltage"},
    {"P0113", "Intake Air Temperature (IAT) Sensor Circuit High Voltage"},
    {"P0114", "Intake Air Temperature Circuit Intermittent"},
    {"P0115", "Engine Coolant Temperature (ECT) Sensor Circuit"},
    {"P0116", "Engine Coolant Temperature (ECT) Sensor Performance"},
    {"P0117", "Engine Coolant Temperature (ECT) Sensor Circuit Low Voltage"},
    {"P0118", "Engine Coolant Temperature (ECT) Sensor Circuit High Voltage"},
    {"P0119", "Engine Coolant Temperature Circuit Intermittent"},
    {"P0120", "TP System Performance"},
    {"P0121", "TP Sensor Circuit Insufficient Activity"},
    {"P0122", "Throttle Position (TP) Sensor Circuit Low Voltage"},
    {"P0123", "Throttle Position (TP) Sensor Circuit High Voltage"},
    {"P0124", "Throttle Position Sensor 1 Circuit Intermittent"},
    {"P0125", "Engine Coolant Temperature (ECT) Insufficient for Closed Loop Fuel Control"},
    {"P0126", "Insufficient ECT for Stable Operation"},
    {"P0128", "Coolant Thermostat Below Thermostat Regulating Temperature"},
    {"P0130", "Heated Oxygen Sensor (HO2S) Circuit Closed Loop (CL) Performance Bank 1 Sensor 1"},
    {"P0131", "Heated Oxygen Sensor (HO2S) Circuit Low Voltage Bank 1 Sensor 1"},
    {"P0132", "Heated Oxygen Sensor (HO2S) Circuit High Voltage Bank 1 Sensor 1"},
    {"P0133", "Heated Oxygen Sensor (HO2S) Slow Response Bank 1 Sensor 1"},
    {"P0134", "Heated Oxygen Sensor (HO2S) Circuit Insufficient Activity Bank 1 Sensor 1"},
    {"P0135", "Heated Oxygen Sensor (HO2S) Heater Performance Bank 1 Sensor 1"},
    {"P0136", "Heated Oxygen Sensor (HO2S) Circuit Bank 1 Sensor 2"},
    {"P0137", "Heated Oxygen Sensor (HO2S) Circuit Low Voltage Bank 1 Sensor 2"},
    {"P0138", "Heated Oxygen Sensor (HO2S) Circuit High Voltage Bank 1 Sensor 2"},
    {"P0139", "Heated Oxygen Sensor (HO2S) Slow Response Bank 1 Sensor 2"},
    {"P0140", "Heated Oxygen Sensor (HO2S) Circuit Insufficient Activity Bank 1 Sensor 2"},
    {"P0141", "Heated Oxygen Sensor (HO2S) Heater Performance Bank 1 Sensor 2"},
    {"P0142", "Heated Oxygen Sensor (HO2S) Circuit Bank 1 Sensor 3"},
    {"P0143", "Heated Oxygen Sensor (HO2S) Circuit Low Voltage Bank 1 Sensor 3"},
    {"P0144", "Heated Oxygen Sensor (HO2S) Circuit High Voltage Bank 1 Sensor 3"},
    {"P0145", "Heated Oxygen Sensor (HO2S) Circuit Bank 1 Sensor 2 Slow Response"},
    {"P0146", "Heated Oxygen Sensor (HO2S) Circuit Insufficient Activity Bank 1 Sensor 3"},
    {"P0147", "Heated Oxygen Sensor (HO2S) Heater Performance Bank 1 Sensor 3"},
    {"P0150", "Heated Oxygen Sensor (HO2S) Circuit Closed Loop (CL) Performance Bank 2 Sensor 1"},
    {"P0151", "Heated Oxygen Sensor (HO2S) Circuit Low Voltage Bank 2 Sensor 1"},
    {"P0152", "Heated Oxygen Sensor (HO2S) Circuit High Voltage Bank 2 Sensor 1"},
    {"P0153", "Heated Oxygen Sensor (HO2S) Slow Response Bank 2 Sensor 1"},
    {"P0154", "Heated Oxygen Sensor (HO2S) Circuit Insufficient Activity Bank 2 Sensor 1"},
    {"P0155", "Heated Oxygen Sensor (HO2S) Heater Performance Bank 2 Sensor 1"},
    {"P0156", "Heated Oxygen Sensor (HO2S) Circuit Bank 2 Sensor 2"},
    {"P0157", "Heated Oxygen Sensor (HO2S) Circuit Low Voltage Bank 2 Sensor 2"},
    {"P0158", "Heated Oxygen Sensor (HO2S) Circuit High Voltage Bank 2 Sensor 2"},
    {"P0159", "Heated Oxygen Sensor (HO2S) Slow Response Bank 2 Sensor 2"},
    {"P0160", "Heated Oxygen Sensor (HO2S) Circuit Insufficient Activity Bank 2 Sensor 2"},
    {"P0161", "Heated Oxygen Sensor (HO2S) Heater Performance Bank 2 Sensor 2"},
    {"P0162", "Heated Oxygen Sensor (HO2S) Circuit Bank 2 Sensor 3"},
    {"P0163", "Heated Oxygen Sensor (HO2S) Circuit Bank 2 Sensor 3 Low Voltage"},
    {"P0164", "Heated Oxygen Sensor (HO2S) Circuit Bank 2 Sensor 3 High Voltage"},
    {"P0165", "Heated Oxygen Sensor (HO2S) Circuit Bank 2 Sensor 3 Slow Response"},
    {"P0166", "Heated Oxygen Sensor (HO2S) Circuit Bank 2 Sensor 3 No Activity Detected"},
    {"P0167", "Heated Oxygen Sensor (HO2S) Heater Circuit Bank 2 Sensor 3"},
    {"P0169", "Fuel Composition Sensor"},
    {"P0170", "Fuel Trim Bank 1"},
    {"P0171", "Fuel Trim System Lean Bank 1"},
    {"P0172", "Fuel Trim System Rich Bank 1"},
    {"P0173", "Fuel Trim Bank 2"},
    {"P0174", "Fuel Trim System Lean Bank 2"},
    {"P0175", "Fuel Trim System Rich Bank 2"},
    {"P0176", "Fuel Composition Sensor Circuit"},
    {"P0177", "Fuel Composition Sensor Circuit Performance"},
    {"P0178", "Fuel Composition Sensor Circuit Low Voltage"},
    {"P0179", "Fuel Composition Sensor Circuit High Voltage"},
    {"P0180", "Fuel Temperature Sensor 1 Circuit"},
    {"P0181", "Fuel Temp. Sensor 1 Circuit Performance"},
    {"P0182", "Fuel Temperature Sensor Circuit Low Voltage"},
    {"P0183", "Fuel Temperature Sensor Circuit High Voltage"},
    {"P0184", "Fuel Temperature Sensor 1 Circuit Intermittent"},
    {"P0185", "Fuel Temperature Sensor 2 Circuit"},
    {"P0186", "Fuel Temp. Sensor 2 Circuit Performance"},
    {"P0187", "Fuel Temperature Sensor 2 Circuit Low Voltage"},
    {"P0188", "Fuel Temperature Sensor 2 Circuit High Voltage"},
    {"P0189", "Fuel Temperature Sensor 2 Circuit Intermittent"},
    {"P0190", "Fuel Rail Pressure Sensor Circuit"},
    {"P0191", "Fuel Rail Pressure Sensor Circuit Performance"},
    {"P0192", "Fuel Rail Pressure Sensor Circuit Low Voltage"},
    {"P0193", "Fuel Rail Pressure Sensor Circuit High Voltage"},
    {"P0194", "Fuel Rail Pressure Sensor Circuit Intermittent"},
    {"P0195", "Engine Oil Temperature Sensor"},
    {"P0196", "Engine Oil Temperature Sensor Performance"},
    {"P0197", "Engine Oil Temperature Sensor Low Voltage"},
    {"P0198", "Engine Oil Temperature Sensor High Voltage"},
    {"P0199", "Engine Oil Temperature Sensor Intermittent"},
    {"P0200", "Injector Control Circuit"},
    {"P0201", "Injector 1 Control Circuit"},
    {"P0202", "Injector 2 Control Circuit"},
    {"P0203", "Injector 3 Control Circuit"},
    {"P0204", "Injector 4 Control Circuit"},
    {"P0205", "Injector 5 Control Circuit"},
    {"P0206", "Injector 6 Control Circuit"},
    {"P0207", "Injector 7 Control Circuit"},
    {"P0208", "Injector 8 Control Circuit"},
    {"P0209", "Injector 9 Control Circuit"},
    {"P0210", "Injector 10 Control Circuit"},
    {"P0211", "Injector 11 Control Circuit"},
    {"P0212", "Injector 12 Control Circuit"},
    {"P0213", "Cold Start Injector 1"},
    {"P0214", "Cold Start Injector 2"},
    {"P0215", "Engine Shutoff Control Circuit"},
    {"P0216", "Injection Timing Control Circuit"},
    {"P0217", "Engine Overtemp Condition"},
    {"P0218", "Transmission Fluid Overtemperature"},
    {"P0219", "Engine Overspeed Condition"},
    {"P0220", "APP Sensor 2 Circuit"},
    {"P0221", "APP (Throttle Position) Sensor 2 Circuit Performance"},
    {"P0222", "APP (Throttle Position) Sensor 2 Circuit Low Voltage"},
    {"P0223", "APP (Throttle Position) Sensor 2 Circuit High Voltage"},
    {"P0224", "Throttle Position Sensor 2 Intermittent"},
    {"P0225", "APP Sensor 3 Circuit"},
    {"P0226", "APP Sensor 3 Circuit Performance"},
    {"P0227", "APP Sensor 3 Circuit Low Voltage"},
    {"P0228", "APP Sensor 3 Circuit High Voltage"},
    {"P0229", "Throttle Position Sensor 3 Intermittent"},
    {"P0230", "Fuel Pump Relay Control Circuit"},
    {"P0231", "Fuel Pump Feedback Circuit Low Voltage"},
    {"P0232", "Fuel Pump Feedback Circuit High Voltage"},
    {"P0233", "Fuel Pump Secondary Circuit Intermittent"},
    {"P0234", "Turbocharger Engine Overboost Condition"},
    {"P0235", "Turbocharger Boost Sensor 1 Circuit"},
    {"P0236", "Turbocharger Boost System"},
    {"P0237", "Turbocharger Boost Sensor Circuit Low Voltage"},
    {"P0238", "Turbocharger Boost Sensor Circuit High Voltage"},
    {"P0239", "Turbocharger Boost Sensor 2 Circuit"},
    {"P0240", "Turbocharger Boost Sensor 2 Performance"},
    {"P0241", "Turbocharger Boost Sensor 2 Circuit Low Voltage"},
    {"P0242", "Turbocharger Boost Sensor 2 Circuit High Voltage"},
    {"P0243", "Turbocharger Wastegate Solenoid 1"},
    {"P0244", "Turbocharger Wastegate Solenoid 1 Performance"},
    {"P0245", "Turbocharger Wastegate Solenoid 1 Low Voltage"},
    {"P0246", "Turbocharger Wastegate Solenoid 1 High Voltage"},
    {"P0247", "Turbocharger Wastegate Solenoid 2"},
    {"P0248", "Turbocharger Wastegate Solenoid 2 Performance"},
    {"P0249", "Turbocharger Wastegate Solenoid 2 Low Voltage"},
    {"P0250", "Turbocharger Wastegate Solenoid 2 High Voltage"},
    {"P0251", "Injection Pump Fuel Metering Control A Malfunction (Cam/Rotor/Injector)"},
    {"P0252", "Injection Pump Fuel Metering Control A Range/Performance (Cam/Rotor/Injector)"},
    {"P0253", "Injection Pump Fuel Metering Control A Low (Cam/Rotor/Injector)"},
    {"P0254", "Injection Pump Fuel Metering Control A High (Cam/Rotor/Injector)"},
    {"P0255", "Injection Pump Fuel Metering Control A Intermittent (Cam/Rotor/Injector)"},
    {"P0256", "Injection Pump Fuel Metering Control B Malfunction (Cam/Rotor/Injector)"},
    {"P0257", "Injection Pump Fuel Metering Control B Range/Performance (Cam/Rotor/Injector)"},
    {"P0258", "Injection Pump Fuel Metering Control B Low (Cam/Rotor/Injector)"},
    {"P0259", "Injection Pump Fuel Metering Control B High (Cam/Rotor/Injector)"},
    {"P0260", "Injection Pump Fuel Metering Control B Intermittent (Cam/Rotor/Injector)"},
    {"P0261", "Cylinder 1 Injector Circuit Low"},
    {"P0262", "Cylinder 1 Injector Circuit High"},
    {"P0263", "Cylinder 1 Contribution/Balance Fault"},
    {"P0264", "Cylinder 2 Injector Circuit Low"},
    {"P0265", "Cylinder 2 Injector Circuit High"},
    {"P0266", "Cylinder 2 Contribution/Balance Fault"},
    {"P0267", "Cylinder 3 Injector Circuit Low"},
    {"P0268", "Cylinder 3 Injector Circuit High"},
    {"P0269", "Cylinder 3 Contribution/Balance Fault"},
    {"P0270", "Cylinder 4 Injector Circuit Low"},
    {"P0271", "Cylinder 4 Injector Circuit High"},
    {"P0272", "Cylinder 4 Contribution/Balance Fault"},
    {"P0273", "Cylinder 5 Injector Circuit Low"},
    {"P0274", "Cylinder 5 Injector Circuit High"},
    {"P0275", "Cylinder 5 Contribution/Balance Fault"},
    {"P0276", "Cylinder 6 Injector Circuit Low"},
    {"P0277", "Cylinder 6 Injector Circuit High"},
    {"P0278", "Cylinder 6 Contribution/Balance Fault"},
    {"P0279", "Cylinder 7 Injector Circuit Low"},
    {"P0280", "Cylinder 7 Injector Circuit High"},
    {"P0281", "Cylinder 7 Contribution/Balance Fault"},
    {"P0282", "Cylinder 8 Injector Circuit Low"},
    {"P0283", "Cylinder 8 Injector Circuit High"},
    {"P0284", "Cylinder 8 Contribution/Balance Fault"},
    {"P0285", "Cylinder 9 Injector Circuit Low"},
    {"P0286", "Cylinder 9 Injector Circuit High"},
    {"P0287", "Cylinder 9 Contribution/Balance Fault"},
    {"P0288", "Cylinder 10 Injector Circuit Low"},
    {"P0289", "Cylinder 10 Injector Circuit High"},
    {"P0290", "Cylinder 10 Contribution/Balance Fault"},
    {"P0291", "Cylinder 11 Injector Circuit Low"},
    {"P0292", "Cylinder 11 Injector Circuit High"},
    {"P0293", "Cylinder 11 Contribution/Balance Fault"},
    {"P0294", "Cylinder 12 Injector Circuit Low"},
    {"P0295", "Cylinder 12 Injector Circuit High"},
    {"P0296", "Cylinder 12 Contribution/Range Fault"},
    {"P0300", "Engine Misfire Detected"},
    {"P0301", "Cylinder 1 Misfire Detected"},
    {"P0302", "Cylinder 2 Misfire Detected"},
    {"P0303", "Cylinder 3 Misfire Detected"},
    {"P0304", "Cylinder 4 Misfire Detected"},
    {"P0305", "Cylinder 5 Misfire Detected"},
    {"P0306", "Cylinder 6 Misfire Detected"},
    {"P0307", "Cylinder 7 Misfire Detected"},
    {"P0308", "Cylinder 8 Misfire Detected"},
    {"P0309", "Cylinder 9 Misfire Detected"},
    {"P0310", "Cylinder 10 Misfire Detected"},
    {"P0311", "Cylinder 11 Misfire Detected"},
    {"P0312", "Cylinder 12 Misfire Detected"},
    {"P0313", "Misfire Detected With Low Fuel Level"},
    {"P0315", "Crankshaft position (CKP) system variation values are not stored in the PCM memory"},
    {"P0318", "Rough Road Sensor Circuit"},
    {"P0320", "Ignition/Distributor Engine Speed Input Circuit Malfunction"},
    {"P0321", "Ignition/Distributor Engine Speed Input Circuit Range/Performance"},
    {"P0322", "IC Module 4X Reference Circuit No Frequency"},
    {"P0323", "Ignition/Distributor Engine Speed Input Circuit Intermittent"},
    {"P0324", "Knock Sensor (KS) Module Performance"},
    {"P0325", "PCM Knock Sensor Circuit"},
    {"P0326", "Knock Sensor Circuit Excessive Spark Retard"},
    {"P0327", "Knock Sensor Circuit Low Voltage"},
    {"P0328", "Knock Sensor 1 Circuit High Input (Bank 1 or Single Sensor)"},
    {"P0329", "Knock Sensor 1 Circuit Intermittent (Bank 1 or Single Sensor)"},
    {"P0330", "Knock Sensor (KS) Circuit Bank 2"},
    {"P0331", "Knock Sensor 2 Circuit Range/Performance (Bank 2)"},
    {"P0332", "Knock Sensor 2 Circuit Low Input (Bank 2)"},
    {"P0333", "Knock Sensor 2 Circuit High Input (Bank 2)"},
    {"P0334", "Knock Sensor 2 Circuit Intermittent (Bank 2)"},
    {"P0335", "CKP Sensor A Circuit Performance"},
    {"P0336", "Crankshaft Position (CKP) Sensor A Performance"},
    {"P0337", "Crankshaft Position (CKP) Sensor Circuit Low Duty Cycle"},
    {"P0338", "Crankshaft Position (CKP) Sensor Circuit High Duty Cycle"},
    {"P0339", "Crankshaft Position (CKP) Sensor Circuit Intermittent"},
    {"P0340", "Camshaft Position (CMP) Sensor Circuit"},
    {"P0341", "Camshaft Position (CMP) Sensor Performance"},
    {"P0342", "Camshaft Position Sensor Circuit Low Input"},
    {"P0343", "Camshaft Position Sensor Circuit High Input"},
    {"P0344", "Camshaft Position Sensor Circuit Intermittent"},
    {"P0350", "Ignition Coil Primary/Secondary Circuit Malfunction"},
    {"P0351", "Ignition Coil 1 Control Circuit"},
    {"P0352", "Ignition Coil 2 Control Circuit"},
    {"P0353", "Ignition Coil 3 Control Circuit"},
    {"P0354", "Ignition Coil 4 Control Circuit"},
    {"P0355", "Ignition Coil 5 Control Circuit"},
    {"P0356", "Ignition Coil 6 Control Circuit"},
    {"P0357", "Ignition Coil 7 Control Circuit"},
    {"P0358", "Ignition Coil 8 Control Circuit"},
    {"P0359", "Ignition Coil I Primary/Secondary Circuit Malfunction"},
    {"P0360", "Ignition Coil J Primary/Secondary Circuit Malfunction"},
    {"P0361", "Ignition Coil K Primary/Secondary Circuit Malfunction"},
    {"P0362", "Ignition Coil L Primary/Secondary Circuit Malfunction"},
    {"P0370", "Timing Reference High Resolution Signal A Malfunction"},
    {"P0371", "IC 24X Reference Circuit Too Many Pulses"},
    {"P0372", "IC 24X Reference Circuit Missing Pulses"},
    {"P0373", "Timing Reference High Resolution Signal A Intermittent/Erratic Pulses"},
    {"P0374", "Timing Reference High Resolution Signal A No Pulses"},
    {"P0375", "Timing Reference High Resolution Signal B Malfunction"},
    {"P0376", "Timing Reference High Resolution Signal B Too Many Pulses"},
    {"P0377", "Timing Reference High Resolution Signal B Too Few Pulses"},
    {"P0378", "Timing Reference High Resolution Signal B Intermittent/Erratic Pulses"},
    {"P0379", "Timing Reference High Resolution Signal B No Pulses"},
    {"P0380", "Glow Plug/Heater Circuit A Malfunction"},
    {"P0381", "Glow Plug/Heater Indicator Circuit Malfunction"},
    {"P0382", "Exhaust Gas Recirculation Flow Malfunction"},
    {"P0385", "Crankshaft Position (CKP) Sensor B Circuit"},
    {"P0386", "Crankshaft Position (CKP) Sensor B Performance"},
    {"P0387", "Crankshaft Position Sensor B Circuit Low Input"},
    {"P0388", "Crankshaft Position Sensor B Circuit High Input"},
    {"P0389", "Crankshaft Position Sensor B Circuit Intermittent"},
    {"P0400", "Exhaust Gas Recirculation Flow Malfunction"},
    {"P0401", "Exhaust Gas Recirculation (EGR) Flow Insufficient"},
    {"P0402", "Exhaust Gas Recirculation Flow Excessive Detected"},
    {"P0403", "Exhaust Gas Recirculation (EGR) Solenoid Control Circuit"},
    {"P0404", "Exhaust Gas Recirculation (EGR) Open Position Performance"},
    {"P0405", "Exhaust Gas Recirculation (EGR) Position Sensor Circuit Low Voltage"},
    {"P0406", "Exhaust Gas Recirculation Sensor A Circuit High"},
    {"P0407", "Exhaust Gas Recirculation Sensor B Circuit Low"},
    {"P0408", "Exhaust Gas Recirculation Sensor B Circuit High"},
    {"P0410", "Secondary Air Injection (AIR) System"},
    {"P0411", "Secondary Air Injection (AIR) System"},
    {"P0412", "Secondary Air Injection (AIR) Solenoid Relay Control Circuit Bank 1"},
    {"P0413", "Secondary Air Injection System Switching Valve A Circuit Open"},
    {"P0414", "Secondary Air Injection System Switching Valve A Circuit Shorted"},
    {"P0415", "Secondary Air Injection System Switching Valve B Circuit Malfunction"},
    {"P0416", "Secondary Air Injection System Switching Valve B Circuit Open"},
    {"P0417", "Secondary Air Injection System Switching Valve B Circuit Shorted"},
    {"P0418", "Secondary Air Injection (AIR) Pump Relay Control Circuit Bank 1"},
    {"P0419", "Secondary Air Injection (AIR) Pump Relay Control Circuit Bank 2"},
    {"P0420", "Catalyst System Low Efficiency"},
    {"P0421", "Warm Up Catalyst Efficiency Below Threshold (Bank 1)"},
    {"P0422", "Catalyst System Low Efficiency Bank 1"},
    {"P0423", "Heated Catalyst Efficiency Below Threshold (Bank 1)"},
    {"P0424", "Heated Catalyst Temperature Below Threshold (Bank 1)"},
    {"P0430", "Catalyst System Low Efficiency Bank 2"},
    {"P0431", "Warm Up Catalyst Efficiency Below Threshold (Bank 2)"},
    {"P0432", "Catalyst System Low Efficiency Bank 2"},
    {"P0433", "Heated Catalyst Efficiency Below Threshold (Bank 2)"},
    {"P0434", "Heated Catalyst Temperature Below Threshold (Bank 2)"},
    {"P0440", "Evaporative Emission (EVAP) System"},
    {"P0441", "Evaporative Emission Control System Incorrect Purge Flow"},
    {"P0442", "Evaporative Emission (EVAP) System Small Leak Detected"},
    {"P0443", "EVAP Purge Solenoid Valve 1 Control Circuit"},
    {"P0444", "Evaporative Emission Control System Purge Control Valve Circuit Open"},
    {"P0445", "Evaporative Emission Control System Purge Control Valve Circuit Shorted"},
    {"P0446", "EVAP Vent Solenoid Valve Control System"},
    {"P0447", "Evaporative Emission Control System Vent Control Circuit Open"},
    {"P0448", "Evaporative Emission Control System Vent Control Circuit Shorted"},
    {"P0449", "Evaporative Emission (EVAP) Vent Solenoid Control Circuit"},
    {"P0450", "Fuel Tank Pressure Sensor Circuit"},
    {"P0451", "Evaporative Emission Control System Pressure Sensor Range/Performance"},
    {"P0452", "Fuel Tank Pressure Sensor Circuit Low Voltage"},
    {"P0453", "Fuel Tank Pressure Sensor Circuit High Voltage"},
    {"P0454", "Evaporative Emission Control System Pressure Sensor Intermittent"},
    {"P0455", "Evaporative Emission (EVAP) System Leak Detected"},
    {"P0460", "Fuel Level Sensor Circuit"},
    {"P0461", "Fuel Level Sensor Performance"},
    {"P0462", "Fuel Level Sensor Circuit Low Voltage"},
    {"P0463", "Fuel Level Sensor Circuit High Voltage"},
    {"P0464", "Fuel Level Sensor Circuit Intermittent"},
    {"P0465", "Purge Flow Sensor Circuit Malfunction"},
    {"P0466", "Purge Flow Sensor Circuit Range/Performance"},
    {"P0467", "Purge Flow Sensor Circuit Low Input"},
    {"P0468", "Purge Flow Sensor Circuit High Input"},
    {"P0469", "Purge Flow Sensor Circuit Intermittent"},
    {"P0470", "Exhaust Pressure Sensor Malfunction"},
    {"P0471", "Exhaust Pressure Sensor Range/Performance"},
    {"P0472", "Exhaust Pressure Sensor Low"},
    {"P0473", "Exhaust Pressure Sensor High"},
    {"P0474", "Exhaust Pressure Sensor Intermittent"},
    {"P0475", "Exhaust Pressure Control Valve Malfunction"},
    {"P0476", "Exhaust Pressure Control Valve Range/Performance"},
    {"P0477", "Exhaust Pressure Control Valve Low"},
    {"P0478", "Exhaust Pressure Control Valve High"},
    {"P0479", "Exhaust Pressure Control Valve Intermittent"},
    {"P0480", "Cooling Fan Relay 1 Control Circuit"},
    {"P0481", "Cooling Fan Relay 2 Control Circuit"},
    {"P0482", "Cooling Fan 3 Control Circuit Malfunction"},
    {"P0483", "Cooling Fan Rationality Check Malfunction"},
    {"P0484", "Cooling Fan Circuit Over Current"},
    {"P0485", "Cooling Fan Power/Ground Circuit Malfunction"},
    {"P0496", "Evaporative Emission (EVAP) System Flow During NonPurge"},
    {"P0500", "Vehicle Speed Sensor (VSS) Circuit"},
    {"P0501", "Vehicle Speed Sensor Range/Performance"},
    {"P0502", "Vehicle Speed Sensor (VSS) Circuit Low Input"},
    {"P0503", "Vehicle Speed Sensor (VSS) Circuit Intermittent"},
    {"P0505", "Idle Control System Malfunction"},
    {"P0506", "Idle Speed Low"},
    {"P0507", "Idle Speed High"},
    {"P0510", "Closed Throttle Position Switch Malfunction"},
    {"P0512", "Start Switch Circuit"},
    {"P0520", "Engine Oil Pressure Sensor/Switch Circuit Malfunction"},
    {"P0521", "Engine Oil Pressure Sensor/Switch Circuit Range/Performance"},
    {"P0522", "Engine Oil Pressure Sensor/Switch Circuit Low Voltage"},
    {"P0523", "Engine Oil Pressure Sensor/Switch Circuit High Voltage"},
    {"P0526", "Cooling Fan Speed Sensor Circuit"},
    {"P0530", "A/C Refrigerant Pressure Sensor Circuit Malfunction"},
    {"P0531", "A/C Refrigerant Pressure Sensor Circuit Range/Performance"},
    {"P0532", "Air Conditioning (A/C) Refrigerant Pressure Sensor Circuit Low Voltage"},
    {"P0533", "Air Conditioning (A/C) Refrigerant Pressure Sensor Circuit High Voltage"},
    {"P0534", "Air Conditioner Refrigerant Charge Loss"},
    {"P0550", "Power Steering Pressure (PSP) Switch Circuit"},
    {"P0551", "Power Steering Pressure Sensor Circuit Range/Performance"},
    {"P0552", "Power Steering Pressure Sensor Circuit Low Input"},
    {"P0553", "Power Steering Pressure Sensor Circuit High Input"},
    {"P0554", "Power Steering Pressure Sensor Circuit Intermittent"},
    {"P0560", "System Voltage"},
    {"P0561", "System Voltage Unstable"},
    {"P0562", "System Voltage Low (TCM)"},
    {"P0563", "System Voltage High (TCM)"},
    {"P0564", "Cruise Control MultiFunction Switch Circuit (PCM)"},
    {"P0565", "Cruise Control On Signal Malfunction"},
    {"P0566", "Cruise Control Off Signal Malfunction"},
    {"P0567", "Cruise Control Resume Signal Malfunction"},
    {"P0568", "Cruise Control Set Signal Malfunction"},
    {"P0569", "Cruise Control Coast Signal Malfunction"},
    {"P0570", "Cruise Control Accel Signal Malfunction"},
    {"P0571", "Cruise Control Brake Switch Circuit"},
    {"P0573", "Cruise Control/Brake Switch A Circuit High"},
    {"P0574", "Vehicle Speed Too High above 110 mph Cruise Control Disabled"},
    {"P0575", "Cruise Control Related Malfunction"},
    {"P0576", "Cruise Control Related Malfunction"},
    {"P0578", "Cruise Control Related Malfunction"},
    {"P0579", "Cruise Control Related Malfunction"},
    {"P0580", "Cruise Control Related Malfunction"},
    {"P0600", "Serial Communication Link Malfunction"},
    {"P0601", "Control Module Read Only Memory (ROM)"},
    {"P0602", "Control Module Not Programmed"},
    {"P0603", "Control Module Long Term Memory Reset"},
    {"P0604", "Control Module Random Access Memory (RAM)"},
    {"P0605", "Control Module Programming Read Only Memory (ROM)"},
    {"P0606", "Control Module Internal Performance"},
    {"P0607", "ECU Malfunction"},
    {"P0608", "Control Module VSS Output A Malfunction"},
    {"P0609", "Control Module VSS Output B Malfunction"},
    {"P0610", "Control Module Vehicle Options Incorrect"},
    {"P0615", "Starter Relay Control Circuit"},
    {"P0616", "Starter Relay Control Circuit Low Voltage"},
    {"P0617", "Starter Relay Control Circuit High Voltage"},
    {"P0620", "Generator Control Circuit Malfunction"},
    {"P0621", "Generator LTerminal Circuit"},
    {"P0622", "Generator FTerminal Circuit"},
    {"P0625", "Generator FTerminal Circuit Low Voltage"},
    {"P0626", "Generator FTerminal Circuit High Voltage"},
    {"P0628", "Fuel Pump Relay Control Circuit Low Voltage"},
    {"P0629", "Fuel Pump Relay Control Circuit High Voltage"},
    {"P0638", "Throttle Actuator Control (TAC) Command Performance"},
    {"P0641", "PCM voltage out of tolerance condition on the 5volt reference circuit"},
    {"P0645", "Air Conditioning (A/C) Clutch Relay Control Circuit"},
    {"P0646", "Air Conditioning (A/C) Clutch Relay Control Circuit"},
    {"P0647", "Air Conditioning (A/C) Clutch Relay Control Circuit"},
    {"P0650", "Malfunction Indicator Lamp (MIL) Control Circuit"},
    {"P0651", "PCM voltage out of tolerance condition on the 5volt reference circuit"},
    {"P0654", "Engine RPM Output Circuit Malfunction"},
    {"P0655", "Engine Hot Lamp Output Control Circuit Malfucntion"},
    {"P0656", "Fuel Level Output Circuit Malfunction"},
    {"P0660", "Intake Manifold Tuning (IMT) Valve Solenoid Control Circuit"},
    {"P0661", "Intake Manifold Tuning (IMT) Valve Solenoid Control Circuit Low Voltage"},
    {"P0662", "Intake Manifold Tuning (IMT) Valve Solenoid Control Circuit High Voltage"},
    {"P0685", "Engine Controls Ignition Relay Control Circuit (PCM)"},
    {"P0691", "Cooling Fan Relay Control Circuit Low Voltage"},
    {"P0692", "Cooling Fan Relay Control Circuit High Voltage"},
    {"P0693", "Cooling Fan Relay Control Circuit Low Voltage"},
    {"P0694", "Cooling Fan Relay Control Circuit High Voltage"},
    {"P0700", "Transmission Control System Malfunction"},
    {"P0701", "Transmission Control System Range/Performance"},
    {"P0702", "Transmission Control System Electrical"},
    {"P0703", "Brake Switch Circuit Malfunction"},
    {"P0704", "Clutch Switch Input Circuit Malfunction"},
    {"P0705", "Trans Range Switch Circuit"},
    {"P0706", "Trans Range Switch Performance"},
    {"P0707", "Transmission Range Sensor Circuit Low Input"},
    {"P0708", "Transmission Range Sensor Circuit High Input"},
    {"P0709", "Transmission Range Sensor Circuit Intermittent"},
    {"P0710", "Transmission Fluid Temperature Sensor Circuit Malfunction"},
    {"P0711", "TFT Sensor Circuit Range/Performance"},
    {"P0712", "Transmission Fluid Temperature (TFT) Sensor Circuit Low Input"},
    {"P0713", "Transmission Fluid Temperature (TFT) Sensor Circuit High Input"},
    {"P0714", "Transmission Fluid Temperature Sensor Circuit Intermittent"},
    {"P0715", "Input/Turbine Speed Sensor Circuit Malfunction"},
    {"P0716", "Input Speed Sensor Circuit Intermittent"},
    {"P0717", "Input Speed Sensor Circuit Low Input"},
    {"P0718", "Input/Turbine Speed Sensor Circuit Intermittent"},
    {"P0719", "Brake Switch Circuit Low Input"},
    {"P0720", "Output Speed Sensor Circuit Malfunction"},
    {"P0721", "Output Speed Sensor Range/Performance"},
    {"P0722", "Output Speed Sensor Circuit Low Input"},
    {"P0723", "Output Speed Sensor Intermittent"},
    {"P0724", "Brake Switch Circuit High Input"},
    {"P0725", "Engine Speed Input Circuit"},
    {"P0726", "Engine Speed Input Circuit Range/Performance"},
    {"P0727", "Engine Speed Circuit No Signal"},
    {"P0728", "Engine Speed Input Circuit Intermittent"},
    {"P0730", "Incorrect Gear Ratio"},
    {"P0731", "Incorrect 1st Gear Ratio"},
    {"P0732", "Incorrect 2nd Gear Ratio"},
    {"P0733", "Incorrect 3rd Gear Ratio"},
    {"P0734", "Incorrect 4th Gear Ratio"},
    {"P0735", "Gear 5 Incorrect ratio"},
    {"P0736", "Reverse incorrect gear ratio"},
    {"P0740", "TCC Enable Solenoid Circuit Electrical"},
    {"P0741", "TCC System Stuck Off"},
    {"P0742", "TCC System Stuck On"},
    {"P0743", "TCC Enable Solenoid Circuit Electrical"},
    {"P0744", "Torque Converter Clutch Circuit Intermittent"},
    {"P0745", "Pressure Control Solenoid Malfunction"},
    {"P0746", "Pressure Control Solenoid Performance or Stuck Off"},
    {"P0747", "Pressure Control Solenoid Stuck On"},
    {"P0748", "Pressure Control Solenoid Circuit Electrical"},
    {"P0749", "Pressure Control Solenoid Intermittent"},
    {"P0750", "Shift Solenoid A Malfunction"},
    {"P0751", "12 Shift Solenoid Valve Performance No First or Fourth Gear"},
    {"P0752", "12 Shift Solenoid Valve Performance No Second or Third Gear"},
    {"P0753", "12 Shift Solenoid Circuit Electrical"},
    {"P0754", "Shift Solenoid A Intermittent"},
    {"P0755", "Shift Solenoid B Malfunction"},
    {"P0756", "23 Shift Solenoid Valve Performance No First or Second Gear"},
    {"P0757", "23 Shift Solenoid Valve Performance No Third or Fourth Gear"},
    {"P0758", "23 Shift Solenoid Circuit Electrical"},
    {"P0759", "Shift Solenoid B Intermittent"},
    {"P0760", "Shift Solenoid C Malfunction"},
    {"P0761", "Shift Solenoid C Performance or Stuck Off"},
    {"P0762", "Shift Solenoid C Stuck On"},
    {"P0763", "Shift Solenoid C Electrical"},
    {"P0764", "Shift Solenoid C Intermittent"},
    {"P0765", "Shift Solenoid D Malfunction"},
    {"P0766", "Shift Solenoid D Performance or Stuck Off"},
    {"P0767", "Shift Solenoid D Stuck On"},
    {"P0768", "Shift Solenoid D Electrical"},
    {"P0769", "Shift Solenoid D Intermittent"},
    {"P0770", "Shift Solenoid E Malfunction"},
    {"P0771", "Shift Solenoid E Performance or Stuck Off"},
    {"P0772", "Shift Solenoid E Stuck On"},
    {"P0773", "Shift Solenoid E Electrical"},
    {"P0774", "Shift Solenoid E Intermittent"},
    {"P0780", "Shift Malfunction"},
    {"P0781", "12 Shift Malfunction"},
    {"P0782", "23 Shift Malfunction"},
    {"P0783", "34 Shift Malfunction"},
    {"P0784", "45 Shift Malfunction"},
    {"P0785", "32 Shift Solenoid Circuit Electrical"},
    {"P0786", "Shift/Timing Solenoid Range/Performance"},
    {"P0787", "Shift/Timing Solenoid Low"},
    {"P0788", "Shift/Timing Solenoid High"},
    {"P0789", "Shift/Timing Solenoid Intermittent"},
    {"P0790", "Normal/Performance Switch Circuit Malfunction"},
    {"P0801", "Reverse Inhibit Control Circuit Malfunction"},
    {"P0803", "14 Upshift (Skip Shift) Solenoid Control Circuit Malfunction"},
    {"P0804", "14 Upshift (Skip Shift) Lamp Control Circuit Malfunction"},
    {"P0850", "Park/Neutral Position (PNP) Switch Circuit"},
    {"P0856", "Powertrain Indicated Traction Control Malfunction"},
    {"P0894", "Transmission Component Slipping"},
    {"P0897", "PCM detects a calculated transmission fluid life of 10 percent or less"},
    {"P0962", "Line Pressure Control (PC) Solenoid Control Circuit Low Voltage (TCM)"},
    {"P0963", "Line Pressure Control (PC) Solenoid Control Circuit High Voltage (TCM)"},
    {"P0966", "Clutch Pressure Control (PC) Solenoid Control Circuit Low Voltage (TCM)"},
    {"P0967", "Clutch Pressure Control (PC) Solenoid Control Circuit High Voltage (TCM)"},
    {"P0970", "Shift Pressure control (PC) Solenoid Control Circuit Low Voltage (TCM)"},
    {"P0971", "Shift Pressure Control (PC) Solenoid Control High Voltage (TCM)"},
    {"P0973", "PCM detects an open or short to ground in the 12 SS valve circuit"},
    {"P0974", "PCM detects a continuous short to voltage in the 12 SS valve circuit"},
    {"P0976", "PCM detects a continuous open or short to ground in the 23 SS valve circuit"},
    {"P0977", "PCM detects a continuous short to voltage in the 23 SS valve circuit"},
    {"P0979", "Shift Solenoid (SS) 3 Control Circuit Low Voltage (TCM)"},
    {"P0980", "Shift Solenoid (SS) 3 Control Circuit High Voltage (TCM)"},
    {"P0982", "Shift Solenoid (SS) 4 Control Circuit High Voltage (TCM)"},
    {"P0983", "Shift Solenoid (SS) 4 Control Circuit High Voltage (TCM)"},
    {"P0985", "Shift Solenoid (SS) 5 Control Circuit Low Voltage (TCM)"},
    {"P0986", "Shift Solenoid (SS) 5 Control Circuit High Voltage (TCM)"},
    {"P1031", "Heated Oxygen Sensor (HO2S) Heater Current Monitor Control Circuit Banks 1 and 2 Sensor 1"},
    {"P1032", "Heated Oxygen Sensor (HO2S) Heater Warm Up Control Circuit Banks 1 and 2 Sensor 1"},
    {"P1101", "Actual measured airflow from MAF, MAP, EGR, and TP is not within range of the calculated airflow"},
    {"P1105", "Secondary Vacuum Sensor Circuit"},
    {"P1106", "Manifold Absolute Pressure (MAP) Sensor Circuit Intermittent High Voltage"},
    {"P1107", "Manifold Absolute Pressure (MAP) Sensor Circuit Intermittent Low Voltage"},
    {"P1108", "BARO to MAP Sensor Comparison Too High"},
    {"P1109", "Secondary Port Throttle System"},
    {"P1111", "Intake Air Temperature (IAT) Sensor Circuit Intermittent High Voltage"},
    {"P1112", "Intake Air Temperature (IAT) Sensor Circuit Intermittent Low Voltage"},
    {"P1113", "Intake Resonance Switchover Solenoid Control Circuit"},
    {"P1114", "Engine Coolant Temperature (ECT) Sensor Circuit Intermittent Low Voltage"},
    {"P1115", "Engine Coolant Temperature (ECT) Sensor Circuit Intermittent High Voltage"},
    {"P1116", "ECT Signal Unstable or Intermittent"},
    {"P1117", "Engine Coolant Temp. Signal OutOfRange Low"},
    {"P1118", "Engine Coolant Temp. Signal OutOfRange High"},
    {"P1119", "ECT Signal OutOfRange With TFT Sensor"},
    {"P1120", "Throttle Position (TP) Sensor 1 Circuit"},
    {"P1121", "Throttle Position (TP) Sensor Circuit Intermittent High Voltage"},
    {"P1122", "Throttle Position (TP) Sensor Circuit Intermittent Low Voltage"},
    {"P1125", "Accelerator Pedal Position (APP) System"},
    {"P1130", "Heated Oxygen Sensor (HO2S) Circuit Low Variance Bank 1 Sensor 1"},
    {"P1131", "Heated Oxygen Sensor (HO2S) Circuit Low Variance Bank 1 Sensor 2"},
    {"P1132", "Heated Oxygen Sensor (HO2S) Circuit Low Variance Bank 2 Sensor 1"},
    {"P1133", "Heated Oxygen Sensor (HO2S) Insufficient Switching Bank 1 Sensor 1"},
    {"P1134", "Heated Oxygen Sensor (HO2S) Transition Time Ratio Bank 1 Sensor 1"},
    {"P1135", "Heated Oxygen Sensor (HO2S) Lean Mean Bank 1 Sensor 1"},
    {"P1136", "Heated Oxygen Sensor (HO2S) Rich Mean Bank 1 Sensor 1"},
    {"P1137", "Heated Oxygen Sensor (HO2S) Bank 1 Sensor 2 Lean System or Low Voltage"},
    {"P1138", "Heated Oxygen Sensor (HO2S) Bank 1 Sensor 2 Rich or High Voltage"},
    {"P1139", "Heated Oxygen Sensor (HO2S) Insuff. Switching Bank 1 Sensor 2"},
    {"P1140", "Heated Oxygen Sensor (HO2S) Transition Time Ratio Bank 1 Sensor 2"},
    {"P1141", "Heated Oxygen Sensor (HO2S) Heater Control Circuit Bank 1 Sensor 2"},
    {"P1143", "Heated Oxygen Sensor (HO2S) Bank 1 Sensor 3 Lean System or Low Voltage"},
    {"P1144", "Heated Oxygen Sensor (HO2S) Bank 1 Sensor 3 Rich or High Voltage"},
    {"P1145", "Heated Oxygen Sensor (HO2S) Cross Counts Bank 1 Sensor 3"},
    {"P1153", "Heated Oxygen Sensor (HO2S) Insufficient Switching Bank 2 Sensor 1"},
    {"P1154", "Heated Oxygen Sensor (HO2S) Transition Time Ratio Bank 2 Sensor 1"},
    {"P1155", "Heated Oxygen Sensor (HO2S) Lean Mean Bank 2 Sensor 1"},
    {"P1156", "Heated Oxygen Sensor (HO2S) Rich Mean Bank 2 Sensor 1"},
    {"P1157", "Heated Oxygen Sensor (HO2S) Bank 2 Sensor 2 Lean System or Low Voltage"},
    {"P1158", "Heated Oxygen Sensor (HO2S) Bank 2 Sensor 2 Rich or High Voltage"},
    {"P1159", "Heated Oxygen Sensor (HO2S) Cross Counts Bank 2 Sensor 2"},
    {"P1161", "Heated Oxygen Sensor (HO2S) Heater Control Circuit Bank 2 Sensor 2"},
    {"P1163", "Heated Oxygen Sensor (HO2S) Bank 2 Sensor 3 Lean System or Low Voltage"},
    {"P1164", "Heated Oxygen Sensor (HO2S) Bank 2 Sensor 3 Rich or High Voltage"},
    {"P1165", "Heated Oxygen Sensor (HO2S) Cross Counts Bank 2 Sensor 3"},
    {"P1170", "Bank to Bank Fuel Trim Offset"},
    {"P1171", "Fuel System Lean During Acceleration"},
    {"P1172", "Fuel Transfer Pump Flow Insufficient"},
    {"P1185", "Engine Oil Temperature Circuit"},
    {"P1186", "EOT Circuit Performance"},
    {"P1187", "EOT Sensor Circuit Low Voltage"},
    {"P1188", "EOT Sensor Circuit High Voltage"},
    {"P1189", "Engine Oil Pressure (EOP) Switch Circuit"},
    {"P1190", "Engine Vacuum Leak"},
    {"P1191", "Intake Air Duct Air Leak"},
    {"P1200", "Injector Control Circuit"},
    {"P1201", "(Alt. Fuel) Gas Mass Sensor Circuit Range/Performance"},
    {"P1202", "(Alt. Fuel) Gas Mass Sensor Circuit Low Frequency"},
    {"P1203", "(Alt. Fuel) Gas Mass Sensor Circuit High Frequency"},
    {"P1211", "Mass Air Flow Circuit Intermittent High"},
    {"P1212", "Mass Air Flow Circuit Intermittent Low"},
    {"P1214", "Injection Pump Timing Offset"},
    {"P1215", "Ground Fault Detection Indicated"},
    {"P1216", "Fuel Solenoid Response Time Too Short"},
    {"P1217", "Fuel Solenoid Response Time Too Long"},
    {"P1218", "Injection Pump Calibration Circuit"},
    {"P1219", "Throttle Position Sensor Reference Voltage"},
    {"P1220", "Throttle Position (TP) Sensor 2 Circuit"},
    {"P1221", "Fuel Pump Secondary Circuit Low"},
    {"P1222", "Injector Control Circuit Intermittent"},
    {"P1225", "Injector Circuit Cylinder 2 Intermittent"},
    {"P1228", "Injector Circuit Cylinder 3 Intermittent"},
    {"P1231", "Injector Circuit Cylinder 4 Intermittent"},
    {"P1234", "Injector Circuit Cylinder 5 Intermittent"},
    {"P1237", "Injector Circuit Cylinder 6 Intermittent"},
    {"P1240", "Injector Circuit Cylinder 7 Intermittent"},
    {"P1243", "Injector Circuit Cylinder 8 Intermittent"},
    {"P1245", "Intake Plenum Switchover Valve"},
    {"P1250", "Early Fuel Evaporation Heater Circuit"},
    {"P1257", "Supercharger System Overboost"},
    {"P1258", "Engine Coolant Overtemperature above 268°F Protection Mode Active"},
    {"P1260", "Last Test Failed Failed SCC ENTER:More Info."},
    {"P1270", "Accelerator Pedal Position Sensor A/D Converter Error"},
    {"P1271", "Accelerator Pedal Position (APP) Sensor 12 Correlation"},
    {"P1272", "Accelerator Pedal Position Sensor 2"},
    {"P1273", "Accelerator Pedal Position Sensor 1"},
    {"P1274", "Injectors Wired Incorrectly"},
    {"P1275", "Accelerator Pedal Position (APP) Sensor 1 Circuit"},
    {"P1276", "Accelerator Pedal Position Sensor 1 Circuit Performance"},
    {"P1277", "Accelerator Pedal Position Sensor 1 Circuit Low Voltage"},
    {"P1278", "Accelerator Pedal Position Sensor 1 Circuit High Voltage"},
    {"P1280", "Accelerator Pedal Position (APP) Sensor 2 Circuit"},
    {"P1281", "Accelerator Pedal Position Sensor 2 Circuit Performance"},
    {"P1282", "Accelerator Pedal Position Sensor 2 Circuit Low Voltage"},
    {"P1283", "Accelerator Pedal Position Sensor 2 Circuit High Voltage"},
    {"P1285", "Accelerator Pedal Position Sensor 3 Circuit"},
    {"P1286", "Accelerator Pedal Position Sensor 3 Circuit Performance"},
    {"P1287", "Accelerator Pedal Position Sensor 3 Circuit Low Voltage"},
    {"P1288", "Accelerator Pedal Position Sensor 3 Circuit High Voltage"},
    {"P1300", "Ignitor Circuit"},
    {"P1305", "Ignition Coil 2 Primary Feedback Circuit"},
    {"P1310", "Ignition Coil 3 Primary Feedback Circuit"},
    {"P1315", "Ignition Coil 4 Primary Feedback Circuit"},
    {"P1320", "IC 4X Reference Circuit Intermittent"},
    {"P1321", "Electronic Ignition System Fault Line"},
    {"P1322", "EI System or Ignition Control Extra or Missing"},
    {"P1323", "IC 24X Reference Circuit Low Frequency"},
    {"P1324", "Crank RPM Too Low"},
    {"P1335", "CKP Circuit"},
    {"P1336", "Crankshaft Position (CKP) System Variation Not Learned"},
    {"P1345", "Crankshaft Position (CKP)Camshaft Position (CMP) Correlation"},
    {"P1346", "Intake Camshaft Position [CMP] Sensor System Performance"},
    {"P1350", "Ignition Control System"},
    {"P1351", "Ignition Coil Control Circuit High Voltage"},
    {"P1352", "IC Output High/Pulse Detected when GND_Cyl. 2"},
    {"P1353", "IC Output High/Pulse Detected when GND_Cyl. 3"},
    {"P1354", "IC Output High/Pulse Detected when GND_Cyl. 4"},
    {"P1355", "IC Output High/Pulse Detected when GND_Cyl. 5"},
    {"P1356", "IC Output High/Pulse Detected when GND_Cyl. 6"},
    {"P1357", "IC Output High/Pulse Detected when GND_Cyl. 7"},
    {"P1358", "IC Output High/Pulse Detected when GND_Cyl. 8"},
    {"P1359", "Ignition Coil Group 1 Control Circuit"},
    {"P1360", "Ignition Coil Group 2 Control Circuit"},
    {"P1361", "Ignition Coil Control Circuit Low Voltage"},
    {"P1362", "IC Cylinder 2 Not Toggling After Enable"},
    {"P1363", "IC Cylinder 3 Not Toggling After Enable"},
    {"P1364", "IC Cylinder 4 Not Toggling After Enable"},
    {"P1365", "IC Cylinder 5 Not Toggling After Enable"},
    {"P1366", "IC Cylinder 6 Not Toggling After Enable"},
    {"P1367", "IC Cylinder 7 Not Toggling After Enable"},
    {"P1368", "IC Cylinder 8 Not Toggling After Enable"},
    {"P1370", "IC 4X Reference Circuit Too Many Pulses"},
    {"P1371", "IC 4X Reference Circuit Too Few Pulses"},
    {"P1372", "Crankshaft Position (CKP) Sensor AB Correlation"},
    {"P1374", "3X Reference Circuit"},
    {"P1375", "IC 24X Reference Circuit High Voltage"},
    {"P1376", "Ignition Ground Circuit"},
    {"P1377", "IC Cam Pulse To 4X Reference Pulse"},
    {"P1380", "Misfire Detected Rough Road Data Not Available"},
    {"P1381", "Misfire Detected No Communication with Brake Control Module"},
    {"P1390", "Wheel Speed Sensor 1 G Sensor Circuit"},
    {"P1391", "Wheel Speed Sensor 1 G Sensor Circuit Performance"},
    {"P1392", "Wheel Speed Sensor 1 G Sensor Circuit Low Voltage"},
    {"P1393", "Wheel Speed Sensor 1 G Sensor Circuit High Voltage"},
    {"P1394", "Wheel Speed Sensor 1 G Sensor Circuit Intermittent"},
    {"P1395", "Wheel Speed Sensor 2 G Sensor Circuit"},
    {"P1396", "Wheel Speed Sensor 2 G Sensor Circuit Performance"},
    {"P1397", "Wheel Speed Sensor 2 G Sensor Circuit Low Voltage"},
    {"P1398", "Wheel Speed Sensor 2 G Sensor Circuit High Voltage"},
    {"P1399", "Wheel Speed Sensor 2 G Sensor Circuit Intermittent"},
    {"P1403", "Exhaust Gas Recirculation System Valve 1"},
    {"P1404", "Exhaust Gas Recirculation (EGR) Closed Position Performance"},
    {"P1405", "Exhaust Gas Recirculation System Valve 3"},
    {"P1406", "EGR Valve Pintle Position Circuit"},
    {"P1407", "EGR Air Intrusion in Exhaust Supply to EGR Valve"},
    {"P1408", "Intake Manifold Pressure Sensor Circuit"},
    {"P1409", "EGR Vacuum System Leak"},
    {"P1410", "Fuel Tank Pressure System"},
    {"P1415", "Secondary Air Injection (AIR) System Bank 1"},
    {"P1416", "Secondary Air Injection (AIR) System Bank 2"},
    {"P1418", "Secondary Air Injection System Relay A Control Circuit High"},
    {"P1420", "Intake Air Low Pressure Switch Circuit Low Voltage"},
    {"P1421", "Intake Air Low Pressure Switch Circuit High Voltage"},
    {"P1423", "Intake Air High Pressure Switch Circuit High Voltage"},
    {"P1431", "Fuel Level Sensor 2 Circuit Performance"},
    {"P1432", "Fuel Level Sensor 2 Circuit Low Voltage"},
    {"P1433", "Fuel Level Sensor 2 Circuit High Voltage"},
    {"P1441", "Evaporative Emission (EVAP) System Flow During NonPurge"},
    {"P1442", "EVAP Vacuum Sw. High Voltage During Ign. On"},
    {"P1450", "Barometric Pressure Sensor Circuit"},
    {"P1451", "Barometric Press. Sensor Performance"},
    {"P1460", "Cooling Fan Control System"},
    {"P1480", "Cooling Fan 1 Control Circuit High"},
    {"P1481", "Cooling Fan Speed Sensor Circuit"},
    {"P1482", "Cooling Fan Speed Output Circuit"},
    {"P1483", "Engine Cooling System Performance"},
    {"P1484", "Cooling Fan System Performance"},
    {"P1500", "Starter Signal Circuit"},
    {"P1501", "Vehicle Speed Sensor Circuit Intermittent"},
    {"P1502", "Theft Deterrent Fuel Enable Signal Not Received"},
    {"P1503", "Theft Deterrent Fuel Enable Signal Not Correct"},
    {"P1504", "Vehicle Speed Output Circuit"},
    {"P1508", "Idle Speed Low Idle Air Control (IAC) System Not Responding"},
    {"P1509", "Idle Speed High Idle Air Control (IAC) System Not Responding"},
    {"P1510", "Throttle Control System Performance Throttle Limitation Active"},
    {"P1511", "Throttle Control System Backup System Performance"},
    {"P1514", "Airflow to TP Sensor Correlation High"},
    {"P1515", "Electronic Throttle System Throttle Position"},
    {"P1516", "Throttle Actuator Control (TAC) Module Throttle Actuator Position Performance"},
    {"P1517", "Electronic Throttle Module"},
    {"P1518", "Electronic Throttle Module to PCM Communication"},
    {"P1519", "Throttle Actuator Control (TAC) Module Internal Circuit"},
    {"P1520", "Transmission Range Switch Circuit"},
    {"P1521", "Transmission Engaged at High Throttle Angle"},
    {"P1522", "Park/Neutral to Drive/Reverse at High RPM"},
    {"P1523", "Throttle Closed Position Performance"},
    {"P1524", "Throttle Closed Position Performance"},
    {"P1525", "Throttle Body ServiceRequired"},
    {"P1526", "Minimum Throttle Position Not Learned"},
    {"P1527", "Transmission Range to Pressure Switch Correlation"},
    {"P1528", "Governor"},
    {"P1529", "Heated Windshield Request Problem"},
    {"P1530", "Throttle Actuator Control (TAC) Module Internal Circuit"},
    {"P1531", "A/C Low Side Temperature Sensor Fault"},
    {"P1532", "A/C Evaporator Temp. Sens. Circuit Low Voltage"},
    {"P1533", "A/C Evaporator Temp. Sens. Circuit High Voltage"},
    {"P1534", "A/C High Side Temp. Sensor Low Voltage"},
    {"P1535", "A/C High Side Temperature Sensor Circuit"},
    {"P1536", "Engine Coolant Overtemperature Air Conditioning (A/C) Disabled"},
    {"P1537", "A/C Request Circuit Low Voltage"},
    {"P1538", "A/C Request Circuit High Voltage"},
    {"P1539", "A/C Clutch Status Circuit High Voltage"},
    {"P1540", "Air Conditioning (A/C) Refrigerant Overpressure Air Conditioning (A/C) Disabled"},
    {"P1541", "A/C High Side Over Temperature"},
    {"P1542", "A/C System High Pressure High Temperature"},
    {"P1543", "A/C System Performance"},
    {"P1544", "A/C Refrigerant Condition Very Low"},
    {"P1545", "Air Conditioning (A/C) Clutch Relay Control Circuit"},
    {"P1546", "A/C Clutch Status Circuit Low Voltage"},
    {"P1547", "A/C System Performance Degraded"},
    {"P1548", "A/C Recirculation Circuit"},
    {"P1551", "Throttle Valve Rest Position Not Reached During Learn"},
    {"P1554", "Cruise Control Feedback Circuit"},
    {"P1555", "Electronic Variable Orifice Output"},
    {"P1558", "Cruise Control Servo Indicates Low"},
    {"P1559", "Cruise Control Power Management Mode"},
    {"P1560", "Transaxle Not in Drive Cruise Control Disabled"},
    {"P1561", "Cruise Vent Solenoid"},
    {"P1562", "Cruise Vacuum Solenoid"},
    {"P1563", "Cruise Vehicle Speed/Set Speed Difference Too High"},
    {"P1564", "Vehicle Acceleration Too High Cruise Control Disabled"},
    {"P1565", "Cruise Servo Position Sensor"},
    {"P1566", "Engine RPM Too High Cruise Control Disabled"},
    {"P1567", "Active Banking Control Active Cruise Control Disabled"},
    {"P1568", "Cruise Servo Stroke Greater than Commanded in Cruise"},
    {"P1569", "Cruise Servo Stroke High While not in Cruise"},
    {"P1570", "Traction Control Active Cruise Control Disabled"},
    {"P1571", "Traction Control Torque Request Circuit"},
    {"P1572", "ASR Active Circuit Low Too Long"},
    {"P1573", "PCM/EBTCM Serial Data Circuit"},
    {"P1574", "Stoplamp Switch Circuit"},
    {"P1575", "Extended Travel Brake Switch Circuit"},
    {"P1576", "BBV Sensor Circuit High Voltage"},
    {"P1577", "BBV Sensor Circuit Low Voltage"},
    {"P1578", "BBV Sensor Circuit Low Vacuum"},
    {"P1579", "P/N to D/R at High Throttle Angle Power Reduction Mode Active"},
    {"P1580", "Cruise Move Circuit Low Voltage"},
    {"P1581", "Cruise Move Circuit High Voltage"},
    {"P1582", "Cruise Direction Circuit Low Voltage"},
    {"P1583", "Cruise Direction CircuitHigh Voltage"},
    {"P1584", "Cruise Control Disabled"},
    {"P1585", "Cruise Control Inhibit Output Circuit"},
    {"P1586", "Cruise Control Brake Switch 2 Circuit"},
    {"P1587", "Cruise Control Clutch Control Circuit Low"},
    {"P1588", "Cruise Control Clutch Control Circuit High"},
    {"P1599", "Engine Stall or Near Stall Detected"},
    {"P1600", "TCM Internal Watchdog Operation"},
    {"P1601", "Serial Comm. Problem With Device 1"},
    {"P1602", "Knock Sensor (KS) Module Performance"},
    {"P1603", "Loss of SDM Serial Data"},
    {"P1604", "Loss of IPC Serial Data"},
    {"P1605", "Loss of HVAC Serial Data"},
    {"P1606", "Serial Communication Problem With Device 6"},
    {"P1607", "Serial Communication Problem With Device 7"},
    {"P1608", "Serial Communication Problem With Device 8"},
    {"P1609", "Loss Of TCS Serial Data"},
    {"P1610", "Loss of PZM Serial Data"},
    {"P1611", "Loss of CVRTD Serial Data"},
    {"P1612", "Loss of IPM Serial Data"},
    {"P1613", "Loss of DIM Serial Data"},
    {"P1614", "Loss of RIM Serial Data"},
    {"P1615", "Loss of VTD Serial Data"},
    {"P1617", "Engine Oil Level Switch Circuit"},
    {"P1619", "Engine Oil Life Monitor Reset Circuit"},
    {"P1620", "Low Coolant Circuit"},
    {"P1621", "Control Module Long Term Memory Performance"},
    {"P1622", "Cylinder Select"},
    {"P1623", "Transmission Temp PullUp Resistor"},
    {"P1624", "Customer Snapshot Requested Data Available"},
    {"P1625", "TCM System Reset"},
    {"P1626", "Theft Deterrent Fuel Enable Signal Not Received"},
    {"P1627", "A/D Performance"},
    {"P1628", "ECT PullUp Resistor"},
    {"P1629", "Theft Deterrent System Cranking Signal"},
    {"P1630", "Theft Deterrent Learn Mode Active"},
    {"P1631", "Theft Deterrent Start Enable Signal Not Correct"},
    {"P1632", "Theft Deterrent Fuel Disable Signal Received"},
    {"P1633", "Ignition 0 Switch Circuit"},
    {"P1634", "Ignition 1 Switch Circuit"},
    {"P1635", "5 Volt Reference Circuit"},
    {"P1636", "PCM Stack Overrun"},
    {"P1637", "Generator LTerminal Circuit"},
    {"P1638", "Generator FTerminal Circuit"},
    {"P1639", "5 Volt Reference 2 Circuit"},
    {"P1640", "Driver1Input High Voltage"},
    {"P1641", "Malfunction Indicator Lamp (MIL) Control Circuit"},
    {"P1642", "Vehicle Speed Output Circuit"},
    {"P1643", "Engine Speed Output Circuit"},
    {"P1644", "Traction Control Delivered Torque Output Circuit"},
    {"P1645", "Evaporative Emission (EVAP) Vent Solenoid Contorl Circuit"},
    {"P1646", "Evaporative Emission (EVAP) Vent Solenoid Control Circuit"},
    {"P1647", "Driver 1 Line 7"},
    {"P1650", "Control Module Output B Circuit"},
    {"P1651", "Fan 1 Relay Control Circuit"},
    {"P1652", "Powertrain Induced Chassis Pitch Output Circuit"},
    {"P1653", "Oil Level Lamp Control Circuit"},
    {"P1654", "Cruise Control Inhibit Output Circuit"},
    {"P1655", "EVAP Purge Solenoid Control Circuit"},
    {"P1656", "Driver 2 Line 6"},
    {"P1657", "14 Upshift Solenoid Control Circuit"},
    {"P1658", "Starter Enable Relay Control Circuit"},
    {"P1660", "Cooling Fan Control Circuits"},
    {"P1661", "MIL Control Circuit"},
    {"P1662", "Cruise Lamp Control Circuit"},
    {"P1663", "Oil Life Lamp Control Circuit"},
    {"P1664", "14 Upshift Lamp Control Circuit"},
    {"P1665", "Driver 3 Line 5"},
    {"P1666", "Driver 3 Line 6"},
    {"P1667", "Reverse Inhibit Solenoid Control Circuit"},
    {"P1669", "ABS Unit Expected"},
    {"P1670", "Driver 4"},
    {"P1671", "Driver 4 Line 1"},
    {"P1672", "Low Engine Oil Level Lamp Control Circuit"},
    {"P1673", "Engine Hot Lamp Control Circuit"},
    {"P1674", "Tachometer Control Circuit"},
    {"P1675", "EVAP Vent Solenoid Control Circuit"},
    {"P1676", "Driver 4 Line 6"},
    {"P1677", "Driver 4 Line 7"},
    {"P1680", "Driver 5 (ECU Malfunction)"},
    {"P1681", "Driver 5 Line 1"},
    {"P1682", "Driver 5 Line 2"},
    {"P1683", "Driver 5 Line 3"},
    {"P1684", "Driver 5 Line 4"},
    {"P1685", "Driver 5 Line 5"},
    {"P1686", "Driver 5 Line 6"},
    {"P1687", "Driver 5 Line 7"},
    {"P1689", "Delivered Torque Circuit Fault"},
    {"P1690", "ECM Loop Overrun"},
    {"P1691", "Coolant Gage Circuit Low Voltage"},
    {"P1692", "Coolant Gage Circuit High Voltage"},
    {"P1693", "Tachometer Circuit Low Voltage"},
    {"P1694", "Tachometer Circuit High Voltage"},
    {"P1695", "Remote Keyless Entry Circuit Low"},
    {"P1696", "Remote Keyless Entry Voltage High"},
    {"P1700", "Transmission Control Module (TCM) Requested MIL Illumination"},
    {"P1701", "Trans. MIL Request Circuit"},
    {"P1705", "P/N Signal Output Circuit"},
    {"P1719", "Incorrect Shifting Detected (TCM)"},
    {"P1740", "Torque Reduction Signal Circuit"},
    {"P1743", "TP Signal from ECM"},
    {"P1760", "TCM Supply Voltage Interrupted"},
    {"P1779", "Engine Torque Delivered to TCM Signal"},
    {"P1780", "Park/Neutral Position [PNP] Switch Circuit"},
    {"P1781", "Engine Torque Signal Circuit"},
    {"P1790", "Transmission Control Module Checksum"},
    {"P1791", "Throttle/Pedal Position Signal (2000+)"},
    {"P1792", "ECM to TCM Engine Coolant Signal"},
    {"P1793", "Wheel Speed Signal (2000+)"},
    {"P1795", "CAN Bus Throttle Body Position"},
    {"P1800", "TCM Power Relay Control Circuit"},
    {"P1801", "Performance Selector Switch Failure"},
    {"P1804", "Ground Control Relay"},
    {"P1810", "TFP Valve Position Switch Circuit"},
    {"P1811", "Maximum Adapt and Long Shift"},
    {"P1812", "Transmission Over Temperature Condition"},
    {"P1813", "Torque Control"},
    {"P1814", "Torque Converter Overstressed"},
    {"P1815", "Transmission Range Switch Start In Wrong Range"},
    {"P1816", "TFP Valve Position Sw. Park/Neu. With Drive Ratio"},
    {"P1817", "TFP Valve Position Sw. Reverse With Drive Ratio"},
    {"P1818", "TFP Valve Position Sw. Drive Without Drive Ratio"},
    {"P1819", "Internal Mode Switch No StartWrong Range"},
    {"P1820", "Internal Mode Switch Circuit A Low"},
    {"P1822", "Internal Mode Switch Circuit B High"},
    {"P1823", "Internal Mode Switch Circuit P Low"},
    {"P1825", "Internal Mode Switch Invalid Range"},
    {"P1826", "Internal Mode Switch Circuit C High"},
    {"P1831", "Pressure Control (PC) Solenoid Power Circuit Low Voltage"},
    {"P1832", "Pressure Control (PC)/Shift Lock Solenoid Control Circuit High Voltage"},
    {"P1833", "A/T Solenoids Power Circuit Low Voltage"},
    {"P1834", "Torque Converter Clutch (TCC)/Shift Solenoid (SS) Control Circuit High Voltage"},
    {"P1835", "KickDown Switch Circuit"},
    {"P1836", "KickDown Switch Failed Open"},
    {"P1837", "KickDown Switch Failed Short"},
    {"P1842", "12 Shift Solenoid Circuit Low Voltage"},
    {"P1843", "12 Shift Solenoid Circuit High Voltage"},
    {"P1844", "Torque Reduction Signal Circuit Desired By TCM"},
    {"P1845", "23 Shift Solenoid Circuit Low Voltage"},
    {"P1847", "23 Shift Solenoid Circuit High Voltage"},
    {"P1850", "Brake Band Apply Solenoid Circuit"},
    {"P1851", "Brake Band Apply Solenoid Performance"},
    {"P1852", "Brake Band Apply SolenoidLow Voltage"},
    {"P1853", "Brake Band Apply Solenoid High Voltage"},
    {"P1860", "TCC PWM Solenoid Circuit Electrical"},
    {"P1864", "Torque Converter Clutch Circuit"},
    {"P1865", "45 Shift Solenoid (SS) Valve Control Circuit High Voltage"},
    {"P1866", "Torque Converter Clutch (TCC) Pulse Width Modulation (PWM) Solenoid Control Circuit Low Voltage"},
    {"P1867", "Torque Converter Clutch (TCC) Pulse Width Modulation (PWM) Solenoid Control Circuit High Voltage"},
    {"P1868", "Transmission Fluid Life"},
    {"P1870", "Transmission Component Slipping"},
    {"P1871", "Undefined Gear Ratio"},
    {"P1873", "TCC Stator Temp. Switch Circuit Low"},
    {"P1874", "TCC Stator Temp. Switch Circuit High"},
    {"P1875", "4WD Low Switch Circuit Electrical"},
    {"P1884", "TCC Enable/Shift Light Circuit"},
    {"P1886", "Shift Timing Solenoid"},
    {"P1887", "TCC Release Switch Circuit"},
    {"P1890", "Throttle Position Signal Input"},
    {"P1891", "Throttle Position Sensor PWM Signal Low"},
    {"P1892", "Throttle Position Sensor PWM Signal High"},
    {"P1893", "Engine Torque Signal Low Voltage"},
    {"P1894", "Engine Torque Signal High Voltage"},
    {"P1895", "TCM to ECM Torque Reduction Circuit"},
    {"P2008", "Intake Manifold Runner Control (IMRC) Solenoid Control Circuit"},
    {"P2009", "Intake Manifold Runner Control (IMRC) Solenoid Control Circuit Low Voltage"},
    {"P2010", "Intake Manifold Runner Control (IMRC) Solenoid Control Circuit High Voltage"},
    {"P2066", "Fuel Level Sensor 2 Performance"},
    {"P2067", "Fuel Level Sensor 2 Circuit Low Voltage"},
    {"P2068", "Fuel Level Sensor 2 Circuit High Voltage"},
    {"P2096", "Post Catalyst Fuel Trim System Low Limit"},
    {"P2097", "Post Catalyst Fuel Trim System High Limit"},
    {"P2098", "Post Catalyst Fuel Trim System Low Limit"},
    {"P2099", "Post Catalyst Fuel Trim System High Limit"},
    {"P2100", "Throttle Actuator Control (TAC) Motor Control Circuit"},
    {"P2101", "Control Module Throttle Actuator Position Performance"},
    {"P2105", "Throttle Actuator Control (TAC) System Forced Engine Shutdown"},
    {"P2107", "Throttle Actuator Control (TAC) Module Internal Circuit"},
    {"P2108", "Throttle Actuator Control (TAC) Module Performance"},
    {"P2119", "Throttle Closed Position Performance"},
    {"P2120", "Accelerator Pedal Position (APP) Sensor 1 Circuit"},
    {"P2121", "Accelerator Pedal Position (APP) Sensor 1 Performance"},
    {"P2122", "Accelerator Pedal Position (APP) Sensor 1 Circuit Low Voltage"},
    {"P2123", "Accelerator Pedal Position (APP) Sensor 1 Circuit High Voltage"},
    {"P2125", "Accelerator Pedal Position (APP) Sensor 2 Circuit"},
    {"P2127", "Accelerator Pedal Position (APP) Sensor 2 Circuit Low Voltage"},
    {"P2128", "Accelerator Pedal Position (APP) Sensor 2 Circuit High Voltage"},
    {"P2135", "Throttle Position (TP) Sensor 12 Correlation"},
    {"P2138", "Accelerator Pedal Position (APP) Sensor 12 Correlation"},
    {"P2176", "Minimum Throttle Position Not Learned"},
    {"P2500", "Generator LTerminal Circuit Low Voltage"},
    {"P2501", "Generator LTerminal Circuit High Voltage"},
    {"P2535", "Ignition 1 Switch Circuit High Voltage (PCM)"},
    {"P2610", "ECU Malfunction"},
    {"P2763", "Short to voltage in the TCC Solenoid"},
    {"P2764", "Open or Short to ground in the TCC PWM solenoid valve circuit"},
    {"P2A00", "HO2S Circuit Closed Loop (CL) Performance Bank 1 Sensor 1 (PCM)"},
    {"P2A01", "HO2S Circuit Closed Loop (CL) Performance Bank 1 Sensor 2 (PCM)"},
    {"P3000", "Hacking AWH Data"},
    {"U0001", "High Speed CAN Communication Bus"},
    {"U0002", "High Speed CAN Communication Bus Performance"},
    {"U0003", "High Speed CAN Communication Bus (+) Open"},
    {"U0004", "High Speed CAN Communication Bus (+) Low"},
    {"U0005", "High Speed CAN Communication Bus (+) High"},
    {"U0006", "High Speed CAN Communication Bus () Open"},
    {"U0007", "High Speed CAN Communication Bus () Low"},
    {"U0008", "High Speed CAN Communication Bus () High"},
    {"U0009", "High Speed CAN Communication Bus () shorted to Bus (+)"},
    {"U0010", "Medium Speed CAN Communication Bus"},
    {"U0011", "Medium Speed CAN Communication Bus Performance"},
    {"U0012", "Medium Speed CAN Communication Bus (+) Open"},
    {"U0013", "Medium Speed CAN Communication Bus (+) Low"},
    {"U0014", "Medium Speed CAN Communication Bus (+) High"},
    {"U0015", "Medium Speed CAN Communication Bus () Open"},
    {"U0016", "Medium Speed CAN Communication Bus () Low"},
    {"U0017", "Medium Speed CAN Communication Bus () High"},
    {"U0018", "Medium Speed CAN Communication Bus () shorted to Bus (+)"},
    {"U0019", "Low Speed CAN Communication Bus"},
    {"U0020", "Low Speed CAN Communication Bus Performance"},
    {"U0021", "Low Speed CAN Communication Bus (+) Open"},
    {"U0022", "Low Speed CAN Communication Bus (+) Low"},
    {"U0023", "Low Speed CAN Communication Bus (+) High"},
    {"U0024", "Low Speed CAN Communication Bus () Open"},
    {"U0025", "Low Speed CAN Communication Bus () Low"},
    {"U0026", "Low Speed CAN Communication Bus () High"},
    {"U0027", "Low Speed CAN Communication Bus () shorted to Bus (+)"},
    {"U0028", "Vehicle Communication Bus A"},
    {"U0029", "Vehicle Communication Bus A Performance"},
    {"U0030", "Vehicle Communication Bus A (+) Open"},
    {"U0031", "Vehicle Communication Bus A (+) Low"},
    {"U0032", "Vehicle Communication Bus A (+) High"},
    {"U0033", "Vehicle Communication Bus A () Open"},
    {"U0034", "Vehicle Communication Bus A () Low"},
    {"U0035", "Vehicle Communication Bus A () High"},
    {"U0036", "Vehicle Communication Bus A () shorted to Bus A (+)"},
    {"U0037", "Vehicle Communication Bus B"},
    {"U0038", "Vehicle Communication Bus B Performance"},
    {"U0039", "Vehicle Communication Bus B (+) Open"},
    {"U0040", "Vehicle Communication Bus B (+) Low"},
    {"U0041", "Vehicle Communication Bus B (+) High"},
    {"U0042", "Vehicle Communication Bus B () Open"},
    {"U0043", "Vehicle Communication Bus B () Low"},
    {"U0044", "Vehicle Communication Bus B () High"},
    {"U0045", "Vehicle Communication Bus B () shorted to Bus B (+)"},
    {"U0046", "Vehicle Communication Bus C"},
    {"U0047", "Vehicle Communication Bus C Performance"},
    {"U0048", "Vehicle Communication Bus C (+) Open"},
    {"U0049", "Vehicle Communication Bus C (+) Low"},
    {"U0050", "Vehicle Communication Bus C (+) High"},
    {"U0051", "Vehicle Communication Bus C () Open"},
    {"U0052", "Vehicle Communication Bus C () Low"},
    {"U0053", "Vehicle Communication Bus C () High"},
    {"U0054", "Vehicle Communication Bus C () shorted to Bus C (+)"},
    {"U0055", "Vehicle Communication Bus D"},
    {"U0056", "Vehicle Communication Bus D Performance"},
    {"U0057", "Vehicle Communication Bus D (+) Open"},
    {"U0058", "Vehicle Communication Bus D (+) Low"},
    {"U0059", "Vehicle Communication Bus D (+) High"},
    {"U0060", "Vehicle Communication Bus D () Open"},
    {"U0061", "Vehicle Communication Bus D () Low"},
    {"U0062", "Vehicle Communication Bus D () High"},
    {"U0063", "Vehicle Communication Bus D () shorted to Bus D (+)"},
    {"U0064", "Vehicle Communication Bus E"},
    {"U0065", "Vehicle Communication Bus E Performance"},
    {"U0066", "Vehicle Communication Bus E (+) Open"},
    {"U0067", "Vehicle Communication Bus E (+) Low"},
    {"U0068", "Vehicle Communication Bus E (+) High"},
    {"U0069", "Vehicle Communication Bus E () Open"},
    {"U0070", "Vehicle Communication Bus E () Low"},
    {"U0071", "Vehicle Communication Bus E () High"},
    {"U0072", "Vehicle Communication Bus E () shorted to Bus E (+)"},
    {"U0073", "Control Module Communication Bus Off"},
    {"U0074", "Reserved by Document U0075 Reserved by Document"},
    {"U0076", "Reserved by Document"},
    {"U0077", "Reserved by Document"},
    {"U0078", "Reserved by Document"},
    {"U0079", "Reserved by Document"},
    {"U0080", "Reserved by Document"},
    {"U0081", "Reserved by Document"},
    {"U0082", "Reserved by Document"},
    {"U0083", "Reserved by Document"},
    {"U0084", "Reserved by Document"},
    {"U0085", "Reserved by Document"},
    {"U0086", "Reserved by Document"},
    {"U0087", "Reserved by Document"},
    {"U0088", "Reserved by Document"},
    {"U0089", "Reserved by Document"},
    {"U0090", "Reserved by Document"},
    {"U0091", "Reserved by Document"},
    {"U0092", "Reserved by Document"},
    {"U0093", "Reserved by Document"},
    {"U0094", "Reserved by Document"},
    {"U0095", "Reserved by Document"},
    {"U0096", "Reserved by Document"},
    {"U0097", "Reserved by Document"},
    {"U0098", "Reserved by Document"},
    {"U0099", "Reserved by Document"},
    {"U0100", "Lost Communication With ECM/PCM A"},
    {"U0101", "Lost Communication with TCM"},
    {"U0102", "Lost Communication with Transfer Case Control Module"},
    {"U0103", "Lost Communication With Gear Shift Module"},
    {"U0104", "Lost Communication With Cruise Control Module"},
    {"U0105", "Lost Communication With Fuel Injector Control Module"},
    {"U0106", "Lost Communication With Glow Plug Control Module"},
    {"U0107", "Lost Communication With Throttle Actuator Control Module"},
    {"U0108", "Lost Communication With Alternative Fuel Control Module"},
    {"U0109", "Lost Communication With Fuel Pump Control Module"},
    {"U0110", "Lost Communication With Drive Motor Control Module"},
    {"U0111", "Lost Communication With Battery Energy Control Module A"},
    {"U0112", "Lost Communication With Battery Energy Control Module B"},
    {"U0113", "Lost Communication With Emissions Critical Control Information"},
    {"U0114", "Lost Communication With FourWheel Drive Clutch Control Module"},
    {"U0115", "Lost Communication With ECM/PCM B"},
    {"U0116", "Reserved by Document"},
    {"U0117", "Reserved by Document"},
    {"U0118", "Reserved by Document"},
    {"U0119", "Reserved by Document"},
    {"U0120", "Reserved by Document"},
    {"U0121", "Lost Communication With AntiLock Brake System (ABS) Control Module"},
    {"U0122", "Lost Communication With Vehicle Dynamics Control Module"},
    {"U0123", "Lost Communication With Yaw Rate Sensor Module"},
    {"U0124", "Lost Communication With Lateral Acceleration Sensor Module"},
    {"U0125", "Lost Communication With Multiaxis Acceleration Sensor Module"},
    {"U0126", "Lost Communication With Steering Angle Sensor Module"},
    {"U0127", "Lost Communication With Tire Pressure Monitor Module"},
    {"U0128", "Lost Communication With Park Brake Control Module"},
    {"U0129", "Lost Communication With Brake System Control Module"},
    {"U0130", "Lost Communication With Steering Effort Control Module"},
    {"U0131", "Lost Communication With Power Steering Control Module"},
    {"U0132", "Lost Communication With Ride Level Control Module"},
    {"U0133", "Reserved by Document"},
    {"U0134", "Reserved by Document"},
    {"U0135", "Reserved by Document"},
    {"U0136", "Reserved by Document"},
    {"U0137", "Reserved by Document"},
    {"U0138", "Reserved by Document"},
    {"U0139", "Reserved by Document"},
    {"U0140", "Lost Communication With Body Control Module"},
    {"U0141", "Lost Communication With Body Control Module A"},
    {"U0142", "Lost Communication With Body Control Module B"},
    {"U0143", "Lost Communication With Body Control Module C"},
    {"U0144", "Lost Communication With Body Control Module D"},
    {"U0145", "Lost Communication With Body Control Module E"},
    {"U0146", "Lost Communication With Gateway A"},
    {"U0147", "Lost Communication With Gateway B"},
    {"U0148", "Lost Communication With Gateway C"},
    {"U0149", "Lost Communication With Gateway D"},
    {"U0150", "Lost Communication With Gateway E"},
    {"U0151", "Lost Communication With Restraints Control Module"},
    {"U0152", "Lost Communication With Side Restraints Control Module"},
    {"U0153", "Lost Communication With Side Restraints Control Module"},
    {"U0154", "Lost Communication With Restraints Occupant Sensing Control Module"},
    {"U0155", "Lost Communication With Instrument Panel Cluster (IPC) Control Module"},
    {"U0156", "Lost Communication With Information Center A"},
    {"U0157", "Lost Communication With Information Center B"},
    {"U0158", "Lost Communication With Head Up Display"},
    {"U0159", "Lost Communication With Parking Assist Control Module"},
    {"U0160", "Lost Communication With Audible Alert Control Module"},
    {"U0161", "Lost Communication With Compass Module"},
    {"U0162", "Lost Communication With Navigation Display Module"},
    {"U0163", "Lost Communication With Navigation Control Module"},
    {"U0164", "Lost Communication With HVAC Control Module"},
    {"U0165", "Lost Communication With HVAC Control Module"},
    {"U0166", "Lost Communication With Auxiliary Heater Control Module"},
    {"U0167", "Lost Communication With Vehicle Immobilizer Control Module"},
    {"U0168", "Lost Communication With Vehicle Security Control Module"},
    {"U0169", "Lost Communication With Sunroof Control Module"},
    {"U0170", "Lost Communication With Restraints System Sensor A"},
    {"U0171", "Lost Communication With Restraints System Sensor B"},
    {"U0172", "Lost Communication With Restraints System Sensor C"},
    {"U0173", "Lost Communication With Restraints System Sensor D"},
    {"U0174", "Lost Communication With Restraints System Sensor E"},
    {"U0175", "Lost Communication With Restraints System Sensor F"},
    {"U0176", "Lost Communication With Restraints System Sensor G"},
    {"U0177", "Lost Communication With Restraints System Sensor H"},
    {"U0178", "Lost Communication With Restraints System Sensor I"},
    {"U0179", "Lost Communication With Restraints System Sensor J"},
    {"U0180", "Lost Communication With Automatic Lighting Control Module"},
    {"U0181", "Lost Communication With Headlamp Leveling Control Module"},
    {"U0182", "Lost Communication With Lighting Control Module"},
    {"U0183", "Lost Communication With Lighting Control Module"},
    {"U0184", "Lost Communication With Radio"},
    {"U0185", "Lost Communication With Antenna Control Module"},
    {"U0186", "Lost Communication With Audio Amplifier"},
    {"U0187", "Lost Communication With Digital Disc Player/Changer Module A"},
    {"U0188", "Lost Communication With Digital Disc Player/Changer Module B"},
    {"U0189", "Lost Communication With Digital Disc Player/Changer Module C"},
    {"U0190", "Lost Communication With Digital Disc Player/Changer Module D"},
    {"U0191", "Lost Communication With Television"},
    {"U0192", "Lost Communication With Personal Computer"},
    {"U0193", "Lost Communication With Digital Audio Control Module A"},
    {"U0194", "Lost Communication With Digital Audio Control Module B"},
    {"U0195", "Lost Communication With Subscription Entertainment Receiver Module"},
    {"U0196", "Lost Communication With Rear Seat Entertainment Control Module"},
    {"U0197", "Lost Communication With Telephone Control Module"},
    {"U0198", "Lost Communication With Telematic Control Module"},
    {"U0199", "Lost Communication With Door Control Module A"},
    {"U0200", "Lost Communication With Door Control Module B"},
    {"U0201", "Lost Communication With Door Control Module C"},
    {"U0202", "Lost Communication With Door Control Module D"},
    {"U0203", "Lost Communication With Door Control Module E"},
    {"U0204", "Lost Communication With Door Control Module F"},
    {"U0205", "Lost Communication With Door Control Module G"},
    {"U0206", "Lost Communication With Folding Top Control Module"},
    {"U0207", "Lost Communication With Movable Roof Control Module"},
    {"U0208", "Lost Communication With Seat Control Module A"},
    {"U0209", "Lost Communication With Seat Control Module B"},
    {"U0210", "Lost Communication With Seat Control Module C"},
    {"U0211", "Lost Communication With Seat Control Module D"},
    {"U0212", "Lost Communication With Steering Column Control Module"},
    {"U0213", "Lost Communication With Mirror Control Module"},
    {"U0214", "Lost Communication With Remote Function Actuation"},
    {"U0215", "Lost Communication With Door Switch A"},
    {"U0216", "Lost Communication With Door Switch B"},
    {"U0217", "Lost Communication With Door Switch C"},
    {"U0218", "Lost Communication With Door Switch D"},
    {"U0219", "Lost Communication With Door Switch E"},
    {"U0220", "Lost Communication With Door Switch F"},
    {"U0221", "Lost Communication With Door Switch G"},
    {"U0222", "Lost Communication With Door Window Motor A"},
    {"U0223", "Lost Communication With Door Window Motor B"},
    {"U0224", "Lost Communication With Door Window Motor C"},
    {"U0225", "Lost Communication With Door Window Motor D"},
    {"U0226", "Lost Communication With Door Window Motor E"},
    {"U0227", "Lost Communication With Door Window Motor F"},
    {"U0228", "Lost Communication With Door Window Motor G"},
    {"U0229", "Lost Communication With Heated Steering Wheel Module"},
    {"U0230", "Lost Communication With Rear Gate Module"},
    {"U0231", "Lost Communication With Rain Sensing Module"},
    {"U0232", "Lost Communication With Side Obstacle Detection Control Module"},
    {"U0233", "Lost Communication With Side Obstacle Detection Control Module"},
    {"U0234", "Lost Communication With Convenience Recall Module"},
    {"U0235", "Lost Communication With Cruise Control Front Distance Range Sensor"},
    {"U0300", "Internal Control Module Software Incompatibility"},
    {"U0301", "Software Incompatibility with ECM/PCM"},
    {"U0302", "Software Incompatibility with Transmission Control Module"},
    {"U0303", "Software Incompatibility with Transfer Case Control Module"},
    {"U0304", "Software Incompatibility with Gear Shift Control Module"},
    {"U0305", "Software Incompatibility with Cruise Control Module"},
    {"U0306", "Software Incompatibility with Fuel Injector Control Module"},
    {"U0307", "Software Incompatibility with Glow Plug Control Module"},
    {"U0308", "Software Incompatibility with Throttle Actuator Control Module"},
    {"U0309", "Software Incompatibility with Alternative Fuel Control Module"},
    {"U0310", "Software Incompatibility with Fuel Pump Control Module"},
    {"U0311", "Software Incompatibility with Drive Motor Control Module"},
    {"U0312", "Software Incompatibility with Battery Energy Control Module A"},
    {"U0313", "Software Incompatibility with Battery Energy Control Module B"},
    {"U0314", "Software Incompatibility with FourWheel Drive Clutch Control Module"},
    {"U0315", "Software Incompatibility with AntiLock Brake System Control Module"},
    {"U0316", "Software Incompatibility with Vehicle Dynamics Control Module"},
    {"U0317", "Software Incompatibility with Park Brake Control Module"},
    {"U0318", "Software Incompatibility with Brake System Control Module"},
    {"U0319", "Software Incompatibility with Steering Effort Control Module"},
    {"U0320", "Software Incompatibility with Power Steering Control Module"},
    {"U0321", "Software Incompatibility with Ride Level Control Module"},
    {"U0322", "Software Incompatibility with Body Control Module"},
    {"U0323", "Software Incompatibility with Instrument Panel Control Module"},
    {"U0324", "Software Incompatibility with HVAC Control Module"},
    {"U0325", "Software Incompatibility with Auxiliary Heater Control Module"},
    {"U0326", "Software Incompatibility with Vehicle Immobilizer Control Module"},
    {"U0327", "Software Incompatibility with Vehicle Security Control Module"},
    {"U0328", "Software Incompatibility with Steering Angle Sensor Module"},
    {"U0329", "Software Incompatibility with Steering Column Control Module"},
    {"U0330", "Software Incompatibility with Tire Pressure Monitor Module"},
    {"U0331", "Software Incompatibility with Body Control Module A"},
    {"U0400", "Invalid Data Received U0401 Invalid Data Received From ECM/PCM"},
    {"U0402", "Invalid Data Received From Transmission Control Module"},
    {"U0403", "Invalid Data Received From Transfer Case Control Module"},
    {"U0404", "Invalid Data Received From Gear Shift Control Module"},
    {"U0405", "Invalid Data Received From Cruise Control Module"},
    {"U0406", "Invalid Data Received From Fuel Injector Control Module"},
    {"U0407", "Invalid Data Received From Glow Plug Control Module"},
    {"U0408", "Invalid Data Received From Throttle Actuator Control Module"},
    {"U0409", "Invalid Data Received From Alternative Fuel Control Module"},
    {"U0410", "Invalid Data Received From Fuel Pump Control Module"},
    {"U0411", "Invalid Data Received From Drive Motor Control Module"},
    {"U0412", "Invalid Data Received From Battery Energy Control Module A"},
    {"U0413", "Invalid Data Received From Battery Energy Control Module B"},
    {"U0414", "Invalid Data Received From FourWheel Drive Clutch Control Module"},
    {"U0415", "Invalid Data Received From AntiLock Brake System Control Module"},
    {"U0416", "Invalid Data Received From Vehicle Dynamics Control Module"},
    {"U0417", "Invalid Data Received From Park Brake Control Module"},
    {"U0418", "Invalid Data Received From Brake System Control Module"},
    {"U0419", "Invalid Data Received From Steering Effort Control Module"},
    {"U0420", "Invalid Data Received From Power Steering Control Module"},
    {"U0421", "Invalid Data Received From Ride Level Control Module"},
    {"U0422", "Invalid Data Received From Body Control Module"},
    {"U0423", "Invalid Data Received From Instrument Panel Control Module"},
    {"U0424", "Invalid Data Received From HVAC Control Module"},
    {"U0425", "Invalid Data Received From Auxiliary Heater Control Module"},
    {"U0426", "Invalid Data Received From Vehicle Immobilizer Control Module"},
    {"U0427", "Invalid Data Received From Vehicle Security Control Module"},
    {"U0428", "Invalid Data Received From Steering Angle Sensor Module"},
    {"U0429", "Invalid Data Received From Steering Column Control Module"},
    {"U0430", "Invalid Data Received From Tire Pressure Monitor Module"},
    {"U0431", "Invalid Data Received From Body Control Module A"}
};



// Define the static DTC count
const int OBD2Class::DTC_ENTRY_COUNT = sizeof(DTC) / sizeof(DTC[0]);

const char* OBD2Class::getDTCDescription(const char* code) {
    // Binary search for the DTC code
    int left = 0;
    int right = DTC_ENTRY_COUNT - 1;

    while (left <= right) {
        int mid = (left + right) / 2;
        const char* midCode = (const char*)pgm_read_ptr(&DTC[mid].code);
        int cmp = strcmp_P(code, midCode);

        if (cmp == 0) {
            // Found the code, return its description
            return (const char*)pgm_read_ptr(&DTC[mid].description);
        } else if (cmp < 0) {
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }

    // Code not found
    return nullptr;
}

int OBD2Class::pidReadForDtc(uint8_t mode, void *data, int length) // third byte for single frame is number of dtc codes for signle frame
{
  unsigned long lastResponseDelta = millis() - _lastPidResponseMillis;
  if (lastResponseDelta < 60)
  {
    delay(60 - lastResponseDelta);
  }

  CAN_FRAME outgoing;
  outgoing.id = _useExtendedAddressing ? OBD2_EXT_ID_REQ : OBD2_STD_ID_REQ;
  outgoing.length = 8;
  outgoing.extended = _useExtendedAddressing;
  outgoing.rtr = 0;
  outgoing.data.uint8[0] = 0x01;
  outgoing.data.uint8[1] = mode;
  outgoing.data.uint8[2] = 0x00;
  outgoing.data.uint8[3] = 0x00;
  outgoing.data.uint8[4] = 0x00;
  outgoing.data.uint8[5] = 0x00;
  outgoing.data.uint8[6] = 0x00;
  outgoing.data.uint8[7] = 0x00;

  for (int retries = 10; retries > 0; retries--)
  {
    if (CAN0.sendFrame(outgoing))
    {
      // send success
      break;
    }
    else if (retries <= 1)
    {
      return 0;
    }
  }

  CAN_FRAME incoming;
  for (unsigned long start = millis(); (millis() - start) < _responseTimeout;)
  {
    if (CAN0.read(incoming) != 0)
    {
      _lastPidResponseMillis = millis();

      // Check if the response ID is in valid range
      if (_useExtendedAddressing) {
        if (incoming.id < OBD2_EXT_ID_RES_MIN || incoming.id > OBD2_EXT_ID_RES_MAX) {
          continue;
        }
      } else {
        if (incoming.id < OBD2_STD_ID_RES_MIN || incoming.id > OBD2_STD_ID_RES_MAX) {
          continue;
        }
      }

      if (((incoming.data.uint8[0] & 0xF0) != 0x10) && incoming.data.uint8[1] == (mode | 0x40))
      {
        if (incoming.data.uint8[2] == 0) // byte 2 is dtc count
        {
          return incoming.data.uint8[2]; // no dtc
        }
        
        for (uint8_t i = 0; i < 4; i++) // byte 0 length, byte 1 mode, byte 2 dtc count, byte 3 4 5 6 data
        {
          ((uint8_t *)data)[i] = incoming.data.uint8[i + 3];
        }
        return incoming.data.uint8[2]; // dtc count
      }

      // Is multiple packets
      if ((incoming.data.uint8[0] & 0xF0) == 0x10) // check for most significant nibble, if exist than it is first frame of multiple packets

      {

        int bytesRead = 0;
        int size = (((incoming.data.uint8[0] & 0x0F) << 8) | incoming.data.uint8[1]) - 2; // low nipple along with the [1] byte depicts length of the data. -2 for metadata including mode,  and dtc count byte
        int firstFrameDataBytes = 4;
        while (bytesRead < firstFrameDataBytes)
        {
          ((uint8_t *)data)[bytesRead] = incoming.data.uint8[4 + bytesRead];
          bytesRead++;
        } // asummed first two bytes are sequence number and size, third byte is mode fourth is dtc countand rest is data
        // sent flow control packet
        incoming.data.uint8[0] = 0;
        incoming.data.uint8[1] = 0;
        incoming.data.uint8[2] = 0;
        incoming.data.uint8[3] = 0;
        incoming.data.uint8[4] = 0;
        incoming.data.uint8[5] = 0;
        incoming.data.uint8[6] = 0;
        incoming.data.uint8[7] = 0;
        CAN_FRAME outgoing;
        outgoing.id = _useExtendedAddressing ? (incoming.id - 8) : 0x7E0;
        outgoing.length = 8;
        outgoing.extended = _useExtendedAddressing;
        outgoing.rtr = 0;
        outgoing.data.uint8[0] = 0x30; // flow control byte
        outgoing.data.uint8[1] = 0x00;
        outgoing.data.uint8[2] = 0x00;
        outgoing.data.uint8[3] = 0x00;
        outgoing.data.uint8[4] = 0x00;
        outgoing.data.uint8[5] = 0x00;
        outgoing.data.uint8[6] = 0x00;
        outgoing.data.uint8[7] = 0x00;

        for (int retries = 10; retries > 0; retries--) // sent 10 more times if not successful
        {
          if (CAN0.sendFrame(outgoing))
          {
            // send success
            break;
          }
          else if (retries <= 1)
          {

            return 0;
          }
        }

        for (int pck = 0; bytesRead < size; pck++) // if we read the enough bytes (which is size) exit the loop, pck is for each consecutive frame which is intended to set low nipple of first byte of consecutive frame and wrap around 16
        {

          // Loop through rest of multiple packets

          // CAN_FRAME incoming; // do we need to shading here? if we do not will previos incoming variable pass the check below since it is set before?
          //  wait for (proper) response
          unsigned long start_time = millis();
          while (CAN0.read(incoming) == 0 || (incoming.data.uint8[0] != (0x20 | ((pck + 1) & 0x0F)))) // wait until packet with the true sequence number is received may be too strict check? || (incoming.data.uint8[0] != (0x20 | ((pck + 1) & 0x0F)))
          {

            if ((millis() - start_time) > _responseTimeout)
            { // exit if timeout occurred

              return 0;
            }
          }

          // Something recieved
          int remaining = size - bytesRead;             // how many bytes are still needed
          int toRead = (remaining < 7) ? remaining : 7; // read only what's left, max 7

          for (int i = 0; i < toRead; i++)
          {
            ((uint8_t *)data)[bytesRead++] = incoming.data.uint8[i + 1];
          }

          _lastPidResponseMillis = millis(); // set the last response timestamp
        }
        return bytesRead;
      }
    }
  }
  return 0;
}



int OBD2Class::clearDTC(void *data)
{
  unsigned long lastResponseDelta = millis() - _lastPidResponseMillis;
  if (lastResponseDelta < 60)
  {
    delay(60 - lastResponseDelta);
  }

  // First check if there are any DTCs
  CAN_FRAME outgoing;
  outgoing.id = _useExtendedAddressing ? OBD2_EXT_ID_REQ : OBD2_STD_ID_REQ;
  outgoing.length = 8;
  outgoing.extended = _useExtendedAddressing;
  outgoing.rtr = 0;
  outgoing.data.uint8[0] = 0x01;
  outgoing.data.uint8[1] = 0x03; // Mode 03 to check DTCs
  outgoing.data.uint8[2] = 0x00;
  outgoing.data.uint8[3] = 0x00;
  outgoing.data.uint8[4] = 0x00;
  outgoing.data.uint8[5] = 0x00;
  outgoing.data.uint8[6] = 0x00;
  outgoing.data.uint8[7] = 0x00;

  // Send the request to check DTCs
  for (int retries = 10; retries > 0; retries--)
  {
    if (CAN0.sendFrame(outgoing))
    {
      break;
    }
    else if (retries <= 1)
    {
      return 0;
    }
  }

  // Check response for DTCs
  CAN_FRAME incoming;
  int dtcCount = 0;
  
  for (unsigned long start = millis(); (millis() - start) < _responseTimeout;)
  {
    if (CAN0.read(incoming) != 0)
    {
      _lastPidResponseMillis = millis();

      // Check if the response ID is in valid range
      if (_useExtendedAddressing) {
        if (incoming.id < OBD2_EXT_ID_RES_MIN || incoming.id > OBD2_EXT_ID_RES_MAX) {
          continue;
        }
      } else {
        if (incoming.id < OBD2_STD_ID_RES_MIN || incoming.id > OBD2_STD_ID_RES_MAX) {
          continue;
        }
      }

      if (((incoming.data.uint8[0] & 0xF0) != 0x10) && incoming.data.uint8[1] == (0x03 | 0x40))
      {
        dtcCount = incoming.data.uint8[2] / 2; // Each DTC takes 2 bytes
        break;
      }

      // Handle multi-frame response
      if ((incoming.data.uint8[0] & 0xF0) == 0x10)
      {
        dtcCount = ((((incoming.data.uint8[0] & 0x0F) << 8) | incoming.data.uint8[1]) - 2) / 2; // -2 for mode and count bytes, divide by 2 as each DTC takes 2 bytes
        break;
      }
    }
  }

  // If no DTCs found, return success with message
  if (dtcCount == 0)
  {
    strcpy((char*)data, "No DTCs found to be cleared");
    return 1;
  }

  char message[50];
  sprintf(message, "%d DTCs found, attempting to clear...", dtcCount);
  strcpy((char*)data, message);

  // If DTCs exist, send clear command (Mode 04)
  
  outgoing.id = _useExtendedAddressing ? OBD2_EXT_ID_REQ : OBD2_STD_ID_REQ;
  outgoing.length = 8;
  outgoing.extended = _useExtendedAddressing;
  outgoing.rtr = 0;
  outgoing.data.uint8[0] = 0x01;
  outgoing.data.uint8[1] = 0x04;
  outgoing.data.uint8[2] = 0x00;
  outgoing.data.uint8[3] = 0x00;
  outgoing.data.uint8[4] = 0x00;
  outgoing.data.uint8[5] = 0x00;
  outgoing.data.uint8[6] = 0x00;
  outgoing.data.uint8[7] = 0x00;
  
  // Send the clear request
  for (int retries = 10; retries > 0; retries--)
  {
    if (CAN0.sendFrame(outgoing))
    {
      break;
    }
    else if (retries <= 1)
    {
      return 0;
    }
  }

  // Wait for clear confirmation
  for (unsigned long start = millis(); (millis() - start) < _responseTimeout;)
  {
    if (CAN0.read(incoming) != 0)
    {
      _lastPidResponseMillis = millis();

      // Check if the response ID is in valid range
      if (_useExtendedAddressing) {
        if (incoming.id < OBD2_EXT_ID_RES_MIN || incoming.id > OBD2_EXT_ID_RES_MAX) {
          continue;
        }
      } else {
        if (incoming.id < OBD2_STD_ID_RES_MIN || incoming.id > OBD2_STD_ID_RES_MAX) {
          continue;
        }
      }

      // Check for positive response to clear command
      if (((incoming.data.uint8[0] & 0xF0) != 0x10) && incoming.data.uint8[1] == (0x04 | 0x40))
      {
        sprintf(message, "%d DTCs successfully cleared", dtcCount);
        strcpy((char*)data, message);
        return 1; // Successfully cleared DTCs
      }
    }
  }
  
  return 0; // Failed to clear DTCs
}

OBD2Class OBD2;