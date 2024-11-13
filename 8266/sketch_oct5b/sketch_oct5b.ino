#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ThingSpeak.h>

const char* ssid = "SAYANTAN 1117";  // Your WiFi SSID
const char* password = "123456789";        // Your WiFi password
WiFiClient client;

// Replace with your Channel ID and API Key
unsigned long Channel_ID = 2682988;
const char * API_key = "BWFS0HACRKG7Z5MO";

SoftwareSerial nodeMCUSerial(3,2); // RX, TX pins (adjust as needed)

void setup() {
  Serial.begin(57600);
  nodeMCUSerial.begin(57600);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client); // Initialize ThingSpeak
  connectToWiFi();
  ThingSpeak.begin(client);
}

void loop() {

  Serial.println("Waiting");
  
  //if (nodeMCUSerial.available()) {
    String data = nodeMCUSerial.readStringUntil('\n');

    // Print incoming data for debugging
    Serial.print("Received data: ");
    Serial.println(data);

    float LDR1, LDR2, LDR3, LDR4, LDR5, avgLDR, V1, V2, V3, avgVoltage, current, temperature;

    // Parse the incoming data
    
    int parsed = sscanf(data.c_str(), 
                "LDR1:%f,LDR2:%f,LDR3:%f,LDR4:%f,LDR5:%f,avgLDR:%f,V1:%f,V2:%f,V3:%f,avgVoltage:%f,current:%f,temperature:%f", 
                &LDR1, &LDR2, &LDR3, &LDR4, &LDR5, &avgLDR, &V1, &V2, &V3, &avgVoltage, &current, &temperature);
    
    // Check if data was parsed successfully
    if (parsed == 13) {
      Serial.println("Parsed data successfully!");

      // Set ThingSpeak fields
      ThingSpeak.setField(1, avgLDR);       // Average LDR value
      ThingSpeak.setField(2, avgVoltage);   // Average Voltage
      ThingSpeak.setField(3, current);      // Current
      ThingSpeak.setField(4, temperature);  // Temperature

      // Write the fields to ThingSpeak
      int responseCode = ThingSpeak.writeFields(Channel_ID, API_key);
      
      // Enhanced Debugging: Check and print HTTP response code
      if (responseCode == 200) {
        Serial.println("Update successful.");
      } else {
        Serial.print("Error updating channel. HTTP code: ");
        Serial.println(responseCode);
        
        // Additional debug: print WiFi status and other info
        Serial.print("WiFi Status: ");
        Serial.println(WiFi.status());
        
        if (WiFi.status() != WL_CONNECTED) {
          Serial.println("WiFi disconnected, attempting to reconnect...");
          connectToWiFi();  // Reconnect to WiFi if lost
        }
      }
    //} //else {
      //Serial.println("Error parsing data.");
    //}
  }

  delay(15000); // Ensure ThingSpeak rate limit is respected
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}


