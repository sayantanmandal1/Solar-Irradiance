#include <SoftwareSerial.h>

// Define RX and TX pins for SoftwareSerial (for NodeMCU)
SoftwareSerial nodeMCUSerial(2, 3); // RX, TX pins (you can change these)

// Define the sensor pins
const int LDRPin1 = A5;  // LDR pin 1
const int LDRPin2 = A6;  // LDR pin 2
const int LDRPin3 = A7;  // LDR pin 3
const int LDRPin4 = 4;   // LDR pin 4 (digital)
const int LDRPin5 = 5;   // LDR pin 5 (digital)

const int voltagePin1 = A0;  // Voltage sensor 1
const int voltagePin2 = A1;  // Voltage sensor 2
const int voltagePin3 = A2;  // Voltage sensor 3
const int currentPin = A3;    // ACS 712 Current Sensor
const int tempPin = A4;       // LM35 Temperature Sensor

// Function Prototypes
float readLDR(int pin);
float readVoltage(int pin);
float readCurrent(int pin);
float readTemperature(int pin);
void sendDataToNodeMCU(float LDR1, float LDR2, float LDR3, float LDR4, float LDR5, float avgLDR, float V1, float V2, float V3, float avgVoltage, float current, float temp);

void setup() {
  // Initialize serial communication for Arduino Nano
  Serial.begin(9600);

  // Initialize the software serial communication for NodeMCU
  nodeMCUSerial.begin(9600);

  // Setup pin modes
  pinMode(LDRPin1, INPUT);
  pinMode(LDRPin2, INPUT);
  pinMode(LDRPin3, INPUT);
  pinMode(LDRPin4, INPUT);
  pinMode(LDRPin5, INPUT);
  pinMode(voltagePin1, INPUT);
  pinMode(voltagePin2, INPUT);
  pinMode(voltagePin3, INPUT);
  pinMode(currentPin, INPUT);
  pinMode(tempPin, INPUT);
}

void loop() {
  // Call individual sensor functions and print the results
  float LDRValue1 = readLDR(LDRPin1);
  float LDRValue2 = readLDR(LDRPin2);
  float LDRValue3 = readLDR(LDRPin3);
  float LDRValue4 = readLDR(LDRPin4);
  float LDRValue5 = readLDR(LDRPin5);

  // Calculate the average LDR value
  float averageLDR = (LDRValue1 + LDRValue2 + LDRValue3 + LDRValue4 + LDRValue5) / 5;

  float voltage1 = readVoltage(voltagePin1);
  float voltage2 = readVoltage(voltagePin2);
  float voltage3 = readVoltage(voltagePin3);

  // Calculate the average voltage value
  float averageVoltage = (voltage1 + voltage2 + voltage3) / 3;

  float current = readCurrent(currentPin);
  float temperature = readTemperature(tempPin);

  // Display individual LDR values and the average LDR
  Serial.print("LDR 1 (V): "); Serial.println(LDRValue1);
  Serial.print("LDR 2 (V): "); Serial.println(LDRValue2);
  Serial.print("LDR 3 (V): "); Serial.println(LDRValue3);
  Serial.print("LDR 4 (V): "); Serial.println(LDRValue4);
  Serial.print("LDR 5 (V): "); Serial.println(LDRValue5);
  Serial.print("Average LDR (V): "); Serial.println(averageLDR);

  // Display individual voltage values and the average voltage
  Serial.print("Voltage 1 (V): "); Serial.println(voltage1);
  Serial.print("Voltage 2 (V): "); Serial.println(voltage2);
  Serial.print("Voltage 3 (V): "); Serial.println(voltage3);
  Serial.print("Average Voltage (V): "); Serial.println(averageVoltage);

  // Display other sensor readings
  Serial.print("Current (A): "); Serial.println(current);
  Serial.print("Temperature (°C): "); Serial.println(temperature);
  Serial.print("Raw Temperature Reading: ");
  Serial.println(analogRead(tempPin));

  // Send data to NodeMCU via SoftwareSerial
  sendDataToNodeMCU(LDRValue1, LDRValue2, LDRValue3, LDRValue4, LDRValue5, averageLDR, voltage1, voltage2, voltage3, averageVoltage, current, temperature);

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
  return (currentValue - 2.5) / 0.066;  // Adjust based on ACS 712 module (0.066 V/A for the 5A model)
}

// Function to read temperature from LM35 sensor
float readTemperature(int pin) {
  // Read analog value (0-1023) and convert to voltage (0-5V)
  float voltage = analogRead(pin) * (5.0 / 1023.0);
  // Convert voltage to temperature (10mV per °C)
  float temperatureC = voltage * 100; // LM35 outputs 10mV per °C
  return temperatureC;
}

// Function to send data to NodeMCU
void sendDataToNodeMCU(float LDR1, float LDR2, float LDR3, float LDR4, float LDR5, float avgLDR, float V1, float V2, float V3, float avgVoltage, float current, float temp) {
  String data = "LDR1:" + String(LDR1) + ",LDR2:" + String(LDR2) + ",LDR3:" + String(LDR3) + ",LDR4:" + String(LDR4) + ",LDR5:" + String(LDR5) + 
                ",AvgLDR:" + String(avgLDR) + ",V1:" + String(V1) + ",V2:" + String(V2) + ",V3:" + String(V3) + 
                ",AvgVoltage:" + String(avgVoltage) + ",A:" + String(current) + ",T:" + String(temp);
  nodeMCUSerial.println(data);
}
