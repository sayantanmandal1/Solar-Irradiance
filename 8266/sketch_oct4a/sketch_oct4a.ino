#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include <SoftwareSerial.h>

const char* ssid = "Sayantan’s iPhone"; // Your WiFi SSID
const char* password = "12345678"; // Your WiFi password
const char* server="api.thingspeak.com";
WiFiClient client;

unsigned long Channel_ID = 2682988;  // Replace with your Channel ID
const char * API_key = "KUYKSFJQ70M57CTZ"; // Replace with your API key

unsigned long last_time = 0;
unsigned long delayTime = 10000; // Adjust the delay as needed

SoftwareSerial nodeMCUSerial(2, 3); // RX, TX pins (you can change these)

void setup() {
  Serial.begin(9600);
  nodeMCUSerial.begin(9600);
  
  WiFi.mode(WIFI_STA);
  
  ThingSpeak.begin(client); // Initialize ThingSpeak
  
  connectToWiFi();
}

void loop() {
  if (nodeMCUSerial.available()) {
    String data = nodeMCUSerial.readStringUntil('\n');
    Serial.println(data);

    float LDR, voltage, current, temperature;

    // Parse the incoming data (assuming format is: LDR:val,V1:val,V2:val,A:val,T:val)
    int ldrIndex = data.indexOf("LDR:");
    int v1Index = data.indexOf("V1:");
    int v2Index = data.indexOf("V2:");
    int aIndex = data.indexOf("A:");
    int tIndex = data.indexOf("T:");

    if (ldrIndex != -1 && v1Index != -1 && v2Index != -1 && aIndex != -1 && tIndex != -1) {
      LDR = data.substring(ldrIndex + 4, v1Index - 1).toFloat();
      voltage = data.substring(v1Index + 3, aIndex - 1).toFloat();
      current = data.substring(aIndex + 2, tIndex - 1).toFloat();
      temperature = data.substring(tIndex + 2).toFloat();

      if ((millis() - last_time) > delayTime) {
        // Send data to ThingSpeak
        ThingSpeak.writeField(Channel_ID, 1, LDR, API_key);
        ThingSpeak.writeField(Channel_ID, 2, voltage, API_key);
        ThingSpeak.writeField(Channel_ID, 3, current, API_key);
        ThingSpeak.writeField(Channel_ID, 4, temperature, API_key);

        last_time = millis();
      }
    }
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected.");
}
