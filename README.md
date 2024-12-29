# ESP32 OBD2 Project

## Dependencies
The following libraries are required for this project:

* [ArduinoJson](https://github.com/bblanchon/ArduinoJson.git)
* [ESP32 CAN](https://github.com/collin80/esp32_can.git)
* [CAN Common](https://github.com/collin80/can_common.git)
* [Arduino OBD2](https://github.com/sandeepmistry/arduino-OBD2.git)

## Important Note
The original OBD2 library files (`esp32_obd2.h` and `esp32_obd2.cpp`) in the Arduino OBD2 library need to be replaced with the versions provided in this repository due to:

1. Compatibility issues with newer ESP32 IDF versions, not working with the new ESP32 IDF versions.
2. Added support for:
   * Multiframe CAN responses
   * VIN reading
   * DTC (Diagnostic Trouble Codes) reading and mapping
   * DTC (Diagnostic Trouble Codes) clearing
   * Error handling
   * Additional PIDs

## Project Components

### Tool.ino
The main Arduino firmware for ESP32 that provides:
* Bluetooth connectivity with password protection
* OBD2 communication over CAN bus
* Real-time vehicle diagnostics including:
  * Engine status monitoring
  * Fault code (DTC) reading and clearing
  * Live monitoring capabilities

### Interface Folder (Web Dashboard)
A web-based monitoring interface containing:
* `server.py`: Python backend server for data handling
* `index.html`: Main dashboard interface
* `chart.js` & `chart.css`: Real-time data visualization components
* Static assets and configuration files for the web interface