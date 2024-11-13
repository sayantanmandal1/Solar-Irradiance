#include <Config.h>
#include <FirebaseClient.h>

#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h>

// WiFi credentials
char ssid[] = SECRET_SSID;   // Your network SSID (from secrets.h)
char pass[] = SECRET_PASS;    // Your network password (from secrets.h)
WiFiClient client;

// ThingSpeak credentials
unsigned long myChannelNumber = SECRET_CH_ID;  // Your ThingSpeak channel number (from secrets.h)
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;  // Your ThingSpeak Write API key (from secrets.h)

// Firebase credentials
#define API_KEY "AIzaSyDq_F4Uv4aKZ__UvLY98FAmP7_suEpWhic" // Your Firebase API Key
#define DATABASE_URL "solar-c1ca6-default-rtdb.firebaseio.com" // Your Firebase Database URL

#define USER_EMAIL "msayantan05@gmail.com"
#define USER_PASSWORD "detailing123"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

SoftwareSerial arduinoSerial(4, 5); // RX, TX pins for SoftwareSerial (change as needed)

void setup() {
  Serial.begin(9600);  // Initialize serial for debugging
  arduinoSerial.begin(9600);  // Initialize serial communication with Arduino

  // Connect to WiFi
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(SECRET_SSID);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(5000);     
  } 
  Serial.println("\nConnected.");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Initialize Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Check if there's any data available from the Arduino
  if (arduinoSerial.available()) {
    String data = arduinoSerial.readStringUntil('\n'); // Read the line sent from Arduino
    Serial.println("Received data: " + data); // Debugging output

    // Split the data string into individual sensor values
    int commaIndex = 0;
    float sensorValues[12];
    for (int i = 0; i < 12; i++) {
      commaIndex = data.indexOf(',');
      if (commaIndex > 0) {
        sensorValues[i] = data.substring(0, commaIndex).toFloat(); // Get value
        data.remove(0, commaIndex + 1); // Remove processed value
      } else {
        sensorValues[i] = data.toFloat(); // Last value
        break; // Exit if no more commas
      }
    }

    // Update ThingSpeak fields
    ThingSpeak.setField(1, sensorValues[5]); // Field 1: Average LDR
    ThingSpeak.setField(2, sensorValues[9]); // Field 2: Average Voltage
    ThingSpeak.setField(3, sensorValues[10]); // Field 3: Current
    ThingSpeak.setField(4, sensorValues[11]); // Field 4: Temperature

    // Write to ThingSpeak
    int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if(response == 200) {
      Serial.println("Channel update successful.");
    } else {
      Serial.println("Problem updating channel. HTTP error code " + String(response));
    }

    // Send data to Firebase
    if (Firebase.ready()) {
      Serial.printf("Set LDR... %s\n", Firebase.setFloat(fbdo, F("/sensorData/averageLDR"), sensorValues[5]) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Set Voltage... %s\n", Firebase.setFloat(fbdo, F("/sensorData/averageVoltage"), sensorValues[9]) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Set Current... %s\n", Firebase.setFloat(fbdo, F("/sensorData/current"), sensorValues[10]) ? "ok" : fbdo.errorReason().c_str());
      Serial.printf("Set Temperature... %s\n", Firebase.setFloat(fbdo, F("/sensorData/temperature"), sensorValues[11]) ? "ok" : fbdo.errorReason().c_str());
    } else {
      Serial.println("Firebase not ready.");
    }
  } else {
    Serial.println("No data available from Arduino."); // Debugging output
  }
  
  delay(20000); // Wait 20 seconds to update the channel again
}
