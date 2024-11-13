// Pin Assignments
const int LDRPin1 = A5;       // LDR pin
const int voltagePin1 = A0;   // Voltage sensor 1
const int voltagePin2 = A1;   // Voltage sensor 2
const int voltagePin3 = A2;   // Voltage sensor 3
const int currentPin = A3;    // ACS 712 Current Sensor
const int tempPin = A4;       // LM35 Temperature Sensor

void setup() {
  Serial.begin(19200);         // Initialize serial communication
  // Other sensor initializations can go here
}

void loop() {
  // Reading from sensors
  int ldrValue1 = analogRead(LDRPin1);
  int voltageValue1 = analogRead(voltagePin1);
  int voltageValue2 = analogRead(voltagePin2);
  int voltageValue3 = analogRead(voltagePin3);
  int currentValue = analogRead(currentPin);
  int tempValue = analogRead(tempPin);

  // Processing or converting the sensor data if necessary
  float temperatureC = (tempValue * 5.0 / 1023.0) * 100.0;  // LM35 Temp in Celsius

  // Send data to Serial Monitor
  Serial.print("LDR1 Value: "); Serial.println(ldrValue1);
  Serial.print("Voltage Sensor 1: "); Serial.println(voltageValue1);
  Serial.print("Voltage Sensor 2: "); Serial.println(voltageValue2);
  Serial.print("Voltage Sensor 3: "); Serial.println(voltageValue3);
  Serial.print("Current Sensor Value: "); Serial.println(currentValue);
  Serial.print("Temperature (C): "); Serial.println(temperatureC);

  // Delay before next loop
  delay(1000);
}
