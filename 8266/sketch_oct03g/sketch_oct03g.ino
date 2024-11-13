#include <SoftwareSerial.h>

// Define RX and TX pins for SoftwareSerial (for communication with Arduino Nano)
SoftwareSerial arduinoSerial(5, 4); // RX (GPIO 5), TX (GPIO 4)

// Initialize variables to hold sensor values
float LDRValue, voltage1, voltage2, voltage3, current, temperature;

void setup() {
    // Initialize serial communication for ESP8266
    Serial.begin(115200);
    
    // Initialize the software serial communication for Arduino Nano
    arduinoSerial.begin(9600);
}

void loop() {
    // Check if data is available from Arduino
    if (arduinoSerial.available()) {
        String data = arduinoSerial.readStringUntil('\n'); // Read the incoming string

        // Parse the received data
        parseData(data);
        
        // Print received values to Serial Monitor
        Serial.print("Received Data: ");
        Serial.println(data);
        Serial.print("LDR: "); Serial.println(LDRValue);
        Serial.print("Voltage 1: "); Serial.println(voltage1);
        Serial.print("Voltage 2: "); Serial.println(voltage2);
        Serial.print("Voltage 3: "); Serial.println(voltage3);
        Serial.print("Current: "); Serial.println(current);
        Serial.print("Temperature: "); Serial.println(temperature);
    }

    delay(1000); // Wait for a second before the next loop
}

// Function to parse the incoming data string
void parseData(String data) {
    // Split the string by commas
    int ldrIndex = data.indexOf("LDR:");
    int v1Index = data.indexOf("V1:");
    int v2Index = data.indexOf("V2:");
    int v3Index = data.indexOf("V3:");
    int currentIndex = data.indexOf("A:");
    int tempIndex = data.indexOf("T:");

    if (ldrIndex >= 0) LDRValue = data.substring(ldrIndex + 4, v1Index - 1).toFloat();
    if (v1Index >= 0) voltage1 = data.substring(v1Index + 3, v2Index - 1).toFloat();
    if (v2Index >= 0) voltage2 = data.substring(v2Index + 3, v3Index - 1).toFloat();
    if (v3Index >= 0) voltage3 = data.substring(v3Index + 3, currentIndex - 1).toFloat();
    if (currentIndex >= 0) current = data.substring(currentIndex + 2, tempIndex - 1).toFloat();
    if (tempIndex >= 0) temperature = data.substring(tempIndex + 2).toFloat();
}
