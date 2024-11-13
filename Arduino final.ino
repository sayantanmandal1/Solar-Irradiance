#include <SoftwareSerial.h>

// Define RX and TX pins for SoftwareSerial (for NodeMCU)
SoftwareSerial nodeMCUSerial(3,2); // RX, TX pins (you can change these)

// Define the sensor pins
const int LDRPin1 = A5;
const int LDRPin2 = A6;
const int LDRPin3 = A7;
const int LDRPin4 = 4;
const int LDRPin5 = 5;
const int voltagePin1 = A0;
const int voltagePin2 = A1;
const int voltagePin3 = A2;
const int currentPin = A3;
const int tempPin = A4;

// Function Prototypes
float readLDR(int pin);
float readVoltage(int pin);
float readCurrent(int pin);
float readTemperature(int pin);

void setup() {
  Serial.begin(9600);         // Initialize serial for debugging
  nodeMCUSerial.begin(9600);  // Initialize serial communication with NodeMCU
}

void loop() {

  // Read all sensor values
  float LDR1 = readLDR(LDRPin1);
  float LDR2 = readLDR(LDRPin2);
  float LDR3 = readLDR(LDRPin3);
  float LDR4 = readLDR(LDRPin4);
  float LDR5 = readLDR(LDRPin5);
  float avgLDR = (LDR1 + LDR2 + LDR3 + LDR4 + LDR5) / 5;

  float V1 = readVoltage(voltagePin1);
  float V2 = readVoltage(voltagePin2);
  float V3 = readVoltage(voltagePin3);
  float avgVoltage = (V1 + V2 + V3) / 3;

  float current = readCurrent(currentPin);
  float temperature = readTemperature(tempPin);

  // Send the data to NodeMCU over SoftwareSerial
  String data = String(LDR1) + "," + String(LDR2) + "," + String(LDR3) + "," + 
                String(LDR4) + "," + String(LDR5) + "," + String(avgLDR) + "," + 
                String(V1) + "," + String(V2) + "," + String(V3) + "," + 
                String(avgVoltage) + "," + String(current) + "," + String(temperature);

  Serial.println("Sending data: " + data); // Debugging output
  nodeMCUSerial.println(data); // Send the data to NodeMCU
  delay(1000);  // Wait for 1 second before sending again
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
  return (currentValue - 2.5) / 0.066;  // Adjust based on ACS 712 module (0.066 V/A for 5A model)
}

// Function to read temperature from LM35 sensor
float readTemperature(int pin) {
  float voltage = analogRead(pin) * (5.0 / 1023.0);  // Convert to voltage
  return voltage * 10;  // LM35 outputs 10mV per degree Celsius
}
