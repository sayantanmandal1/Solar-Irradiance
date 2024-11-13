
# Hardware Setup Guide

This document explains the complete hardware setup, including connections and calibration notes for all sensors used in the Arduino Nano and ESP8266 system.

## 1. **Hardware Components**
- **Arduino Nano**: Main microcontroller for reading sensor data and sending it to the ESP8266.
- **ESP8266 (NodeMCU)**: WiFi module used for receiving data from the Arduino Nano and uploading it to a server or cloud.
- **Sensors**:
  - **LDR (Light Dependent Resistor)**: Measures light intensity.
  - **Voltage Sensors**: Reads voltages from connected sources.
  - **ACS712 Current Sensor**: Measures current.
  - **LM35 Temperature Sensor**: Reads ambient temperature.

---

## 2. **Connections**

### 2.1 **Arduino Nano**
The Arduino Nano reads data from the sensors and communicates with the ESP8266 through SoftwareSerial.

| Component          | Arduino Nano Pin | Description                                        |
|--------------------|------------------|----------------------------------------------------|
| **LDR 1**          | `A0`             | Measures ambient light intensity.                  |
| **Voltage Sensor 1** | `A1`           | Reads voltage from the first source.               |
| **Voltage Sensor 2** | `A2`           | Reads voltage from the second source.              |
| **Voltage Sensor 3** | `A3`           | Reads voltage from the third source.               |
| **ACS712 Current Sensor** | `A4`     | Measures current; analog output.                   |
| **LM35 Temperature Sensor** | `A5`   | Measures temperature; analog output.               |
| **ESP8266 TX**     | `D3`             | TX pin for SoftwareSerial communication.           |
| **ESP8266 RX**     | `D2`             | RX pin for SoftwareSerial communication.           |

> **Note:** All sensor outputs (voltage and current sensors) should be connected to the analog input pins on the Arduino, as they provide analog data.

---

### 2.2 **ESP8266 (NodeMCU)**
The ESP8266 module receives sensor data from the Arduino Nano using the SoftwareSerial library.

| Component   | ESP8266 Pin | Description                              |
|-------------|-------------|------------------------------------------|
| **Arduino Nano TX** | `D2` | TX pin for receiving data from Nano.    |
| **Arduino Nano RX** | `D1` | RX pin for sending data to Nano.        |

### 2.3 **Sensor Connections**

#### **LDR (Light Dependent Resistor)**
- **Type**: Analog
- **Pins**: One pin goes to `A0` on the Nano, the other to GND.
- **Setup**: Use a voltage divider circuit by adding a fixed resistor (typically 10kΩ) between the LDR and GND.
  
#### **Voltage Sensors**
- **Type**: Analog
- **Pins**: Output pin connects to `A1`, `A2`, and `A3` on the Nano.
- **Notes**: Ensure that the sensors can read up to your expected voltage range (adjust the scaling factor in code as necessary).

#### **ACS712 Current Sensor**
- **Type**: Analog
- **Pins**: Connects to `A4` on the Nano.
- **Notes**: Provides an analog voltage output proportional to the current flowing through it. Calibrate in code based on the sensor’s rated sensitivity (e.g., 66mV/A).

#### **LM35 Temperature Sensor**
- **Type**: Analog
- **Pins**: Connects to `A5` on the Nano.
- **Notes**: The output is directly proportional to the temperature in Celsius (10mV per °C). Adjust scaling in the code accordingly.

---

## 3. **Power Connections**
Ensure you provide adequate power to the system:
- **Arduino Nano**: Powered via USB or external power source (5V).
- **ESP8266**: Requires 3.3V; connect to the 3.3V pin on the Arduino Nano or a separate 3.3V power supply.
  - **Important**: Do not connect 5V to the ESP8266 as it can damage the module.

---

## 4. **Calibration Notes**
Each sensor may need calibration for accurate measurements.

### **LDR Calibration**
1. Record the LDR output in different lighting conditions.
2. Use these values to map light intensity in code.

### **Voltage Sensor Calibration**
1. Measure the output voltage manually and compare it with the sensor readings.
2. Adjust scaling factors in the code based on the expected voltage range.

### **Current Sensor (ACS712) Calibration**
1. Measure the actual current flowing through the sensor and note the reading.
2. Adjust the offset and scaling factor based on the rated sensitivity (e.g., 66mV/A).

### **Temperature Sensor (LM35) Calibration**
1. Measure the temperature with a thermometer and compare it to the sensor’s output.
2. Ensure the code converts the voltage to temperature accurately (10mV per °C).

--- 

## 5. **Final Notes**
- Ensure proper grounding for all components to prevent noise or inaccurate readings.
- When programming, verify that all pin numbers match your hardware setup.
- Test each sensor individually first, then integrate them into the complete system.

---

This completes the hardware setup guide. Make sure all connections are secure before powering on the system for testing and calibration.
