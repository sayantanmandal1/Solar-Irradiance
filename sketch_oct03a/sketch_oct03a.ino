#include <SoftwareSerial.h>

// Define RX and TX pins for SoftwareSerial (for NodeMCU)
SoftwareSerial nodeMCUSerial(2, 3); // RX, TX pins (you can change these)

const int LDRPin1 = A5;  // LDR pin
const int voltagePin1 = A0;  // Voltage sensor 1
const int voltagePin2 = A1;  // Voltage sensor 2
const int voltagePin3 = A2;  // Voltage sensor 3
const int currentPin = A3;   // ACS 712 Current Sensor
const int tempPin = A4;      // LM35 Temperature Sensor

void setup() {
  // Initialize serial communication for Arduino Nano
  Serial.begin(9600);

  // Initialize the software serial communication for NodeMCU
  nodeMCUSerial.begin(9600);

  // Setup pin modes
  pinMode(LDRPin1, INPUT);
  pinMode(voltagePin1, INPUT);
  pinMode(voltagePin2, INPUT);
  pinMode(voltagePin3, INPUT);
  pinMode(currentPin, INPUT);
  pinMode(tempPin, INPUT);
}

void loop() {
  // Call individual sensor functions and print the results
  float LDRValue = readLDR(LDRPin1);
  float voltage1 = readVoltage(voltagePin1);
  float voltage2 = readVoltage(voltagePin2);
  float voltage3 = readVoltage(voltagePin3);
  float current = readCurrent(currentPin);
  float temperature = readTemperature(tempPin);

  // Display values on Serial Monitor
  Serial.print("LDR (V): "); Serial.println(LDRValue);
  Serial.print("Voltage 1 (V): "); Serial.println(voltage1);
  Serial.print("Voltage 2 (V): "); Serial.println(voltage2);
  Serial.print("Voltage 3 (V): "); Serial.println(voltage3);
  Serial.print("Current (A): "); Serial.println(current);
  Serial.print("Temperature (°C): "); Serial.println(temperature);

  // Send data to NodeMCU via SoftwareSerial
  sendDataToNodeMCU(LDRValue, voltage1, voltage2, voltage3, current, temperature);

  delay(1000);  // Wait for a second
}

// Function to read LDR value
float readLDR(int pin) {
  return analogRead(pin) * (5.0 / 1023.0);  // Convert to voltage
}

// Function to read voltage sensor
float readVoltage(int pin) {
  return analogRead(pin) * (25.0 / 1023.0);  // Assuming 25V max
}

// Function to read current sensor (ACS 712)
float readCurrent(int pin) {
  float currentValue = analogRead(pin) * (5.0 / 1023.0);  // Convert to voltage
  return (currentValue - 2.5) / 0.066;  // Adjust based on ACS 712 module
}

// Function to read temperature from LM35 sensor
float readTemperature(int pin) {
  return analogRead(pin) * (5.0 / 1023.0) * 10;  // Convert to °C
}

// Function to send data to NodeMCU
void sendDataToNodeMCU(float LDR, float V1, float V2, float V3, float current, float temp) {
  String data = "LDR:" + String(LDR) + ",V1:" + String(V1) + ",V2:" + String(V2) + ",V3:" + String(V3) + ",A:" + String(current) + ",T:" + String(temp);
  nodeMCUSerial.println(data);
}
