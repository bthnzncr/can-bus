#include "BluetoothSerial.h"
#include "WiFi.h"
#include <HTTPClient.h>
#include <esp32_can.h>  // the ESP32_OBD2 library depends on the https://github.com/collin80/esp32_can and https://github.com/collin80/can_common CAN libraries
#include <esp32_obd2.h>

#define PASSKEY "messi"  // Define the required passkey here

#define RX_PIN 33
#define TX_PIN 32
const char* ssids[] = { "TurkTelekom_TP372A_2.4GHz", "1111" };
const char* passwords[] = { "P4DhtgATf9Re", "12365411" };
const char* serverUrl = "http://35.246.196.214:8080/api/vehicle-data";

bool isConnected = false;
bool liveMonitoring = false;

const char* mapFuelType(uint8_t A) {
  switch (A) {
    case 0:  return "Not available";
    case 1:  return "Gasoline";
    case 2:  return "Methanol";
    case 3:  return "Ethanol";
    case 4:  return "Diesel";
    case 5:  return "LPG";
    case 6:  return "CNG";
    case 7:  return "Propane";
    case 8:  return "Electric";
    case 9:  return "Bifuel running Gasoline";
    case 10: return "Bifuel running Methanol";
    case 11: return "Bifuel running Ethanol";
    case 12: return "Bifuel running LPG";
    case 13: return "Bifuel running CNG";
    case 14: return "Bifuel running Propane";
    case 15: return "Bifuel running Electricity";
    case 16: return "Bifuel running electric and combustion engine";
    case 17: return "Hybrid gasoline";
    case 18: return "Hybrid Ethanol";
    case 19: return "Hybrid Diesel";
    case 20: return "Hybrid Electric";
    case 21: return "Hybrid running electric and combustion engine";
    case 22: return "Hybrid Regenerative";
    case 23: return "Bifuel running diesel";
    default: return "Unknown";
  }
}

// Map the raw OBD standards value to a human-readable string
const char* mapObdStandards(uint8_t A) {
  switch (A) {
    case 0x00: return "Not available";
    case 0x01: return "OBD-II as defined by the CARB";
    case 0x02: return "OBD as defined by the EPA";
    case 0x03: return "OBD and OBD-II";
    case 0x04: return "OBD-I";
    case 0x05: return "Not OBD compliant";
    case 0x06: return "EOBD (Europe)";
    case 0x07: return "EOBD and OBD-II";
    case 0x08: return "EOBD and OBD";
    case 0x09: return "EOBD, OBD and OBD-II";
    case 0x0A: return "JOBD (Japan)";
    case 0x0B: return "JOBD and OBD-II";
    case 0x0C: return "JOBD and EOBD";
    case 0x0D: return "JOBD, EOBD and OBD-II";
    default:   return "Reserved/Unknown";
  }
}

// Map the raw emission requirement code to a human-readable string
const char* mapEmissionRequirements(uint8_t code) {
  switch (code) {
    case 0x00: return "Not available";
    case 0x01: return "Tier 1";
    case 0x02: return "Tier 2";
    case 0x03: return "Euro I";
    case 0x04: return "Euro II";
    case 0x05: return "Euro III";
    case 0x06: return "Euro IV";
    case 0x07: return "Euro V";
    case 0x08: return "Euro VI";
    case 0x09: return "California LEV I";
    case 0x0A: return "California LEV II";
    case 0x0B: return "California LEV III";
    default:   return "Reserved/Unknown";
  }
}

const uint8_t permanentPIDs[] = {
    FUEL_TYPE,
    OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO,
    EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED,
    TIME_SINCE_TROUBLE_CODES_CLEARED,
    WARM_UPS_SINCE_CODES_CLEARED,
    DISTANCE_TRAVELED_SINCE_CODES_CLEARED,
    MONITOR_STATUS_SINCE_DTCS_CLEARED,
    MONITOR_STATUS_THIS_DRIVE_CYCLE,
    TIME_RUN_WITH_MIL_ON
};


const uint8_t engineStatusPIDs[] = {
    FUEL_SYSTEM_STATUS,
    CALCULATED_ENGINE_LOAD,
    ENGINE_COOLANT_TEMPERATURE,
    ENGINE_RPM,
    TIMING_ADVANCE,
    AIR_INTAKE_TEMPERATURE,
    MAF_AIR_FLOW_RATE
};


const uint8_t fuelEmissionsPIDs[] = {
    FREEZE_DTC,
    OXYGEN_SENSORS_PRESENT_IN_4_BANKS,
    OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO,
    FUEL_AIR_COMMANDED_EQUIVALENCE_RATE,
    FUEL_INJECTION_TIMING,
    ENGINE_FUEL_RATE
};


const uint8_t throttleAirIntakePIDs[] = {
    THROTTLE_POSITION,
    RELATIVE_THROTTLE_POSITION,
    ABSOLUTE_THROTTLE_POSITION_B,
    ABSOLUTE_THROTTLE_POSITION_C,
    ABSOLUTE_THROTTLE_POSITION_D
};


const uint8_t temperaturePressurePIDs[] = {
    ENGINE_COOLANT_TEMPERATURE,
    AIR_INTAKE_TEMPERATURE,
    ABSOLUTE_BAROMETRIC_PRESSURE, // Note the spelling in the enum
    AMBIENT_AIR_TEMPERATURE,
    ENGINE_OIL_TEMPERATURE
};


const uint8_t oxygenSensorPIDs[] = {
    OXYGEN_SENSORS_PRESENT_IN_2_BANKS,
    OXYGEN_SENSOR_1_SHORT_TERM_FUEL_TRIM,
    OXYGEN_SENSOR_2_SHORT_TERM_FUEL_TRIM,
    OXYGEN_SENSOR_1_FUEL_AIR_EQUIVALENCE_RATIO
   
};


const uint8_t diagnosticMonitoringPIDs[] = {
    MONITOR_STATUS_SINCE_DTCS_CLEARED,
    WARM_UPS_SINCE_CODES_CLEARED,
    MONITOR_STATUS_THIS_DRIVE_CYCLE,
    TIME_RUN_WITH_MIL_ON,
    TIME_SINCE_TROUBLE_CODES_CLEARED,
    DISTANCE_TRAVELED_SINCE_CODES_CLEARED
};


BluetoothSerial SerialBT;
bool isAuthenticated = false;    // Flag to track authentication status
bool isDeviceConnected = false;  // Track connection status

void setup() {
  Serial.begin(115200);
  Serial.println("Serial monitor has been initialized.");

  SerialBT.begin("Diagnostic Tool");  // Start the Bluetooth Serial
  Serial.println("Bluetooth Device is Ready to Pair");
  
  CAN0.setCANPins((gpio_num_t)RX_PIN, (gpio_num_t)TX_PIN);
  while (true) {
    Serial.print(F("Attempting to connect to OBD2 CAN bus ... "));

    if (!OBD2.begin()) {
      Serial.println(F("failed!"));
      delay(1000);
    } else {
      Serial.println(F("success"));
      break;
    }
  }
}

void loop() {
  if (liveMonitoring) {
    checkWifiConnection();
    monitor();
    delay(1000);  // Add a delay to prevent flooding
  }
  
  // Check the connection status
  bool currentConnectionStatus = SerialBT.hasClient();
  if (currentConnectionStatus && !isDeviceConnected) {
    // Device just connected
    isDeviceConnected = true;
    isAuthenticated = false;  // Reset authentication for the new connection
    Serial.println("Device connected.");
    SerialBT.println("Welcome to Vehicle Diagnostic Interface");
    SerialBT.println("Please enter the passkey to authenticate:");
  } else if (!currentConnectionStatus && isDeviceConnected) {
    // Device just disconnected
    isDeviceConnected = false;
    isAuthenticated = false;  // Reset authentication when device disconnects
    Serial.println("Device disconnected.");

  }

  // Handle incoming Bluetooth communication
  if (SerialBT.available()) {
    String input = SerialBT.readStringUntil('\n');
    input.trim();  // Clean up the input string
  if (liveMonitoring && input.equalsIgnoreCase("disconnect")) {
      SerialBT.println("Live monitoring stopped.");
      liveMonitoring = false;
      return;
    }
    // Handle authentication
    if (!isAuthenticated) {
      if (input.equals(PASSKEY)) {
        isAuthenticated = true;
        SerialBT.println("Authentication successful!");
        printCar();
        SerialBT.println("Vehicle Diagnostic Tool is Ready. Type <menu> for options.");
      } else {
        SerialBT.println("Incorrect passkey. Please enter the correct passkey:");
      }
    } else {
      // Continue with normal operation after authentication
      if (input.equalsIgnoreCase("menu")) {
        showMenu();
      } else {
        handleCommand(input);
      }
    }
  }
}

void showMenu() {
  SerialBT.println("--- Menu Options ---");
  SerialBT.println("1: About Car");
  SerialBT.println("2: Show All Supported Diagnostics:");
  SerialBT.println("3: Show Engine Status");
  SerialBT.println("4: Show Fuel Emissions Related Diagnostics");
  SerialBT.println("5: Show Temperature and Pressure Related Diagnostics");
  SerialBT.println("6: Show Oxygen Sensor Status");
  SerialBT.println("7: Show Fault Code Related Diagnostics");
  SerialBT.println("8: Show Fault Codes (DTCs)");
  SerialBT.println("9: Clear DTC Fault Codes");
  SerialBT.println("10: Live Car Monitoring");
  SerialBT.println("11: Exit");
  SerialBT.println("Select an option by typing the number (e.g., '1').");
}

void handleCommand(String command) {
  switch (command.toInt()) {
    case 1:
      car_details();
      SerialBT.println();
      aboutCar();
      break;
    case 2:
      SerialBT.println("Showing all supported PIDs:");
      for (int pid = 0; pid < 96; pid++) {
        processPid(pid);
      }
      SerialBT.println();
      break;
    case 3:
      SerialBT.println("Engine Status:");
      engineStatus();
      break;
    case 4:
      SerialBT.println("Fuel Emissions:");
      fuelEmissions();
      break;
    case 5:
    SerialBT.println("Temperature and Pressure Information:");
      temperaturePressure();
      break;
    case 6:
    SerialBT.println("Oxygen Information:");
      oxygenSensor();
      break;
    case 7:
    SerialBT.println("Diagnostic Information:");
      diagnosticMonitoring();
      break;
    case 8:
      dtcRead();
      break;
    case 9:
      clearDTCs();
      break;
    case 10:
      SerialBT.println("Initializing WiFi and server connections...");
      initializeWifi();
      if (WiFi.status() == WL_CONNECTED) {
        
        if (isConnected) {
          SerialBT.println("Live Car Monitoring started. Type 'disconnect' to stop live monitoring.");
          liveMonitoring = true; // enable live monitoring mode
        } else {
          SerialBT.println("Failed to connect to server. Live monitoring not available.");
        }
      } else {
        SerialBT.println("Failed to connect to WiFi. Live monitoring not available.");
      }
      break;
    case 11:
      SerialBT.println("Exiting...");
      break;
    default:
      SerialBT.println("Invalid option. Please try again.");
      break;
  }
}

void printCar() {
  SerialBT.println();
  SerialBT.println();
  delay(50);
  SerialBT.println("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⣴⣶⣶⣿⠿⠿⠿⢿⣶⣶⣤⣀⣀⣀⣠⣤⣤⣦");
  delay(50);
  SerialBT.println("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⣿⡿⠿⠛⠛⠉⠉⠀⠀⠀⠀⠀⠈⢿⡏⠉⢻⣿⣿⣿⣿⣿");
  delay(50);
  SerialBT.println("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣼⠋⠀⠀⠀⣴⣶⡄⠀⠀⢰⣿⠀⠀⠀⠘⣷⡀⠀⢹⣿⣿⣿⣿⣿");
  delay(50);
  SerialBT.println("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣾⣇⣀⣤⣤⣤⣾⣿⣶⣶⣶⣿⣿⣿⣿⣿⣿⣿⣷⣾⣿⣿⣿⣿⣿⣿");
  delay(50);
  SerialBT.println("⠀⠀⣠⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠛⠉⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿");
  delay(50);
  SerialBT.println("⠀⣰⠋⠛⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣏⣁⣀⣠⣤⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿");
  delay(50);
  SerialBT.println("⣰⣷⣦⣤⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠟⠛⣿⣿⣿⣿⣿⣿⣿⠁⠈⠙⢿⣿⣿⣿⣿⠀⣿");
  delay(50);
  SerialBT.println("⣿⣿⣿⣿⣿⣷⡀⠀⠈⠉⠉⠉⠉⠁⠀⠀⠀⠀⢀⣼⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠘⣿⣿⣿⣿⠀⣿");
  delay(50);
  SerialBT.println("⣿⣿⣿⣿⣿⣿⣷⣤⣀⣀⣀⣀⣀⣀⣀⣠⣤⣾⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⢀⣿⣿⣿⣿⣀⣿");
  delay(50);
  SerialBT.println("⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⢀⣼⣿⣿⠛⠛⠛⠃");
  delay(50);
  SerialBT.println("⠀⠈⠙⠻⢿⣿⣿⣿⠿⠟⠛⠛⠛⠛⠛⠉⠉⠉⠉⠉⠀⠈⠻⣿⣿⣿⣷⣶⣶⣿⡿");
  delay(50);
  SerialBT.println("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠉⠉⠉⠉⠉");
  delay(50);
  SerialBT.println();
  SerialBT.println();
}
void aboutCar() {
  for (int i = 0; i < sizeof(permanentPIDs) / sizeof(permanentPIDs[0]); i++) {
    processPid(permanentPIDs[i]);
  }
}

void monitor() {
  static unsigned long lastSendTime = 0;
  static bool permanentPIDsSent = false;
  
  // Send permanent PIDs first
  
    String jsonData = "{\"message_type\":\"vehicle_info\",\"pids\":[";
    bool firstPid = true;
    
    // Add VIN first
    String vin = OBD2.vinRead();
    jsonData += "{\"name\":\"VIN\",\"value\":\"" + vin + "\",\"category\":\"vehicle_info\"}";
    firstPid = false;
    
    // Add other permanent PIDs
    for (int i = 0; i < sizeof(permanentPIDs) / sizeof(permanentPIDs[0]); i++) {
      int pid = permanentPIDs[i];
      if (!OBD2.pidSupported(pid)) {
        continue;
      }

      if (!firstPid) {
        jsonData += ",";
      }
      firstPid = false;

      jsonData += "{\"name\":\"" + String(pid) + "\",";
      
      if (pid == FUEL_TYPE || 
          pid == OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO || 
          pid == EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED) {
        unsigned long rawVal = OBD2.pidReadRaw(pid);
        const char* resultStr = "Unknown";
        
        if (pid == FUEL_TYPE) {
          resultStr = mapFuelType((uint8_t)rawVal);
        } else if (pid == OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO) {
          resultStr = mapObdStandards((uint8_t)rawVal);
        } else if (pid == EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED) {
          resultStr = mapEmissionRequirements((uint8_t)rawVal);
        }
        
        jsonData += "\"value\":\"" + String(resultStr) + "\",\"category\":\"vehicle_info\"}";
      } else {
        if (OBD2.pidValueRaw(pid)) {
          unsigned long pidRawValue = OBD2.pidReadRaw(pid);
          jsonData += "\"value\":\"0x" + String(pidRawValue, HEX) + "\",\"category\":\"vehicle_info\"}";
        } else {
          float pidValue = OBD2.pidRead(pid);
          if (isnan(pidValue)) {
            jsonData += "\"value\":\"error\",\"category\":\"vehicle_info\"}";
          } else {
            jsonData += "\"value\":\"" + String(pidValue) + " " + String(OBD2.pidUnits(pid)) + "\",\"category\":\"vehicle_info\"}";
          }
        }
      }
    }
    
    jsonData += "]}";
    
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/json");
      
      int httpResponseCode = http.POST(jsonData);
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Vehicle info sent successfully");
        SerialBT.println("Vehicle info sent successfully");
        permanentPIDsSent = true;
      } else {
        Serial.println("Error sending vehicle info");
        SerialBT.println("Error sending vehicle info");
      }
      http.end();
    }
  

  // Send real-time data every 2 seconds
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= 2000) {  // 2000ms = 2 seconds
    String jsonData = "{\"message_type\":\"realtime_data\",\"pids\":[";
    bool firstPid = true;
    
    // Iterate through PIDs from 0 to 96
    for (int pid = 0; pid <= 96; pid++) {
      
      // Skip if PID is in permanentPIDs array
      bool isPermanent = false;
      for (int j = 0; j < sizeof(permanentPIDs) / sizeof(permanentPIDs[0]); j++) {
        if (pid == permanentPIDs[j]) {
          isPermanent = true;
          break;
        }
      }
      if (isPermanent || !OBD2.pidSupported(pid) || (OBD2.pidValueRaw(pid)) ) {
        continue;
      }

      if (!firstPid) {
        jsonData += ",";
      }
      firstPid = false;

      jsonData += "{\"name\":\"" + String(pid) + "\",";
        
        float pidValue = OBD2.pidRead(pid);
        if (isnan(pidValue)) {
          jsonData += "\"value\":\"error\",\"category\":\"realtime\"}";
        } else {
          jsonData += "\"value\":\"" + String(pidValue) + " " + String(OBD2.pidUnits(pid)) + "\",\"category\":\"realtime\"}";
        }
      
    }
    
    jsonData += "]}";
    
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverUrl);
      http.addHeader("Content-Type", "application/json");
      
      int httpResponseCode = http.POST(jsonData);
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Real-time data sent successfully");
      } else {
        Serial.println("Error sending real-time data");
      }
      http.end();
    }
    
    lastSendTime = currentTime;
  }
}


void dtcRead() {
  String dtcs = OBD2.dtcRead();
  
  if (dtcs == "No DTC Found") {
    SerialBT.println("No DTCs found in the system.");
    return;
  }

  SerialBT.println("Found DTCs:");
  
  // Split the DTC string into individual codes
  int start = 0;
  int commaIndex;
  
  while (start < dtcs.length()) {
    commaIndex = dtcs.indexOf(',', start);
    String dtcCode;
    
    if (commaIndex == -1) {
      dtcCode = dtcs.substring(start);
      start = dtcs.length();
    } else {
      dtcCode = dtcs.substring(start, commaIndex);
      start = commaIndex + 1;
    }
    
    dtcCode.trim();  // Remove any whitespace
    if (dtcCode.length() > 0) {
      // Get the description for this DTC
      const char* description = OBD2.getDTCDescription(dtcCode.c_str());
      
      SerialBT.print("  ");
      SerialBT.print(dtcCode);
      if (description != nullptr) {
        SerialBT.print(" - ");
        SerialBT.println(description);
      } else {
        SerialBT.println(" - Description not available");
      }
    }
  }
}
void car_details() {
  String vin = OBD2.vinRead();

  String print_vin = "VIN = " + vin;
  SerialBT.println(print_vin);
}

void processPid(int pid) {
  if (!OBD2.pidSupported(pid)) {
    return;
  }

  SerialBT.print(OBD2.pidName(pid));
  SerialBT.print(F(" = "));

  // Check if PID expects a special mapping
  if (pid == FUEL_TYPE || 
      pid == OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO || 
      pid == EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED) {
    // For these PIDs, we read the raw value and map it to a string
    unsigned long rawVal = OBD2.pidReadRaw(pid);

    const char* resultStr = "Unknown";
    if (pid == FUEL_TYPE) {
      resultStr = mapFuelType((uint8_t)rawVal);
    } else if (pid == OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO) {
      resultStr = mapObdStandards((uint8_t)rawVal);
    } else if (pid == EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED) {
      resultStr = mapEmissionRequirements((uint8_t)rawVal);
    }

    SerialBT.println(resultStr);
    return;
  }

  // For other PIDs, original logic applies
  if (OBD2.pidValueRaw(pid)) {
    // Print raw value
    unsigned long pidRawValue = OBD2.pidReadRaw(pid);
    SerialBT.print(F("0x"));
    SerialBT.print(pidRawValue, HEX);
  } else {
    float pidValue = OBD2.pidRead(pid);
    if (isnan(pidValue)) {
      SerialBT.print("error");
    } else {
      SerialBT.print(pidValue);
      SerialBT.print(F(" "));
      SerialBT.print(OBD2.pidUnits(pid));
    }
  }

  SerialBT.println();
}
void engineStatus() {
  for (int i = 0; i < sizeof(engineStatusPIDs) / sizeof(engineStatusPIDs[0]); i++) {
    processPid(engineStatusPIDs[i]);
  }
}
void oxygenSensor() {
  for (int i = 0; i < sizeof(oxygenSensorPIDs) / sizeof(oxygenSensorPIDs[0]); i++) {
    processPid(oxygenSensorPIDs[i]);
  }
}
void temperaturePressure() {
  for (int i = 0; i < sizeof(temperaturePressurePIDs) / sizeof(temperaturePressurePIDs[0]); i++) {
    processPid(temperaturePressurePIDs[i]);
  }
}
void throttleAirIntake() {
  for (int i = 0; i < sizeof(throttleAirIntakePIDs) / sizeof(throttleAirIntakePIDs[0]); i++) {
    processPid(throttleAirIntakePIDs[i]);
  }
}
void fuelEmissions() {
  for (int i = 0; i < sizeof(fuelEmissionsPIDs) / sizeof(fuelEmissionsPIDs[0]); i++) {
    processPid(fuelEmissionsPIDs[i]);
  }
}
void diagnosticMonitoring() {
  for (int i = 0; i < sizeof(diagnosticMonitoringPIDs) / sizeof(diagnosticMonitoringPIDs[0]); i++) {
    processPid(diagnosticMonitoringPIDs[i]);
  }
}

void clearDTCs() {
  char result[50];  // Buffer to store the result message
  SerialBT.println("Attempting to clear DTCs...");
  int status = OBD2.clearDTC(result);
  
  if (status) {
    SerialBT.println(result);  // This will print either "No DTCs found to be cleared" or "X DTCs successfully cleared"
  } else {
    SerialBT.println("Failed to clear DTCs. Please try again.");
  }
}
void checkWifiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Attempting to reconnect...");
    SerialBT.println("WiFi connection lost. Attempting to reconnect...");
    initializeWifi();
  }
}

void initializeWifi() {
    WiFi.mode(WIFI_STA);
    bool connected = false;
    
    for (int i = 0; i < 2 && !connected; i++) {
        Serial.printf("Attempting to connect to %s\n", ssids[i]);
        SerialBT.printf("Attempting to connect to %s\n", ssids[i]);
        WiFi.begin(ssids[i], passwords[i]);
        
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 20) {
            delay(500);
            Serial.print(".");
            SerialBT.print(".");
            attempts++;
        }
        
        if (WiFi.status() == WL_CONNECTED) {
            connected = true;
            isConnected = true;
            Serial.printf("\nConnected to %s\n", ssids[i]);
            Serial.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
            SerialBT.printf("\nConnected to %s\n", ssids[i]);
            SerialBT.printf("IP address: %s\n", WiFi.localIP().toString().c_str());
        } else {
            Serial.printf("\nFailed to connect to %s\n", ssids[i]);
            SerialBT.printf("\nFailed to connect to %s\n", ssids[i]);
        }
    }
    
    if (!connected) {
        Serial.println("Failed to connect to any WiFi network");
        SerialBT.println("Failed to connect to any WiFi network");
        isConnected = false;
    }
}