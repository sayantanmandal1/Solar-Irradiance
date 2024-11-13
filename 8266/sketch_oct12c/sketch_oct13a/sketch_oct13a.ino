#include <InfluxDbClient.h>
#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <SoftwareSerial.h>
#include <FirebaseESP8266.h>  // Ensure this is the correct Firebase library

#if defined(ESP32)
  #include <WiFiMulti.h>
  WiFiMulti wifiMulti;
  #define DEVICE "ESP32"
#elif defined(ESP8266)
  #include <ESP8266WiFiMulti.h>
  ESP8266WiFiMulti wifiMulti;
  #define DEVICE "ESP8266"
#endif

// WiFi credentials
char ssid[] = SECRET_SSID;   // Your network SSID (from secrets.h)
char pass[] = SECRET_PASS;   // Your network password (from secrets.h)
WiFiClient client;

// ThingSpeak credentials
unsigned long myChannelNumber = SECRET_CH_ID;  // Your ThingSpeak channel number (from secrets.h)
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;  // Your ThingSpeak Write API key (from secrets.h)

// Firebase credentials
#define API_KEY "AIzaSyDq_F4Uv4aKZ__UvLY98FAmP7_suEpWhic" // Replace with your Firebase API Key
#define DATABASE_URL "https://solar-c1ca6-default-rtdb.firebaseio.com/" // Ensure this URL is correct
#define USER_EMAIL "msayantan05@gmail.com"
#define USER_PASSWORD "detailing123"

// InfluxDB credentials
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "kav3zrhcuPzkq69Bf1t1h3zFawAxEyLPVwVyzzjqYX57nFWBhYmKXpZjmZOHtzMVgUBXTaGYVO4wtz5tBoXljA=="
#define INFLUXDB_ORG "c8c2a9b838588f1b"
#define INFLUXDB_BUCKET "Solar Project"

// Time zone info
#define TZ_INFO "UTC5.5"

// Declare InfluxDB client instance
InfluxDBClient influxClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

// Declare Data point
Point sensor("wifi_status");

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

SoftwareSerial arduinoSerial(4, 5); // RX, TX pins for SoftwareSerial (change as needed)

void setup() {
  Serial.begin(115200);  // Initialize serial for debugging
  arduinoSerial.begin(9600);  // Initialize serial communication with Arduino

  // Setup WiFi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(ssid, pass);

  Serial.print("Connecting to WiFi...");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println("\nConnected to WiFi.");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);

  // Initialize Firebase
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Add tags to the data point
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());
  
  // Check InfluxDB connection
  if (influxClient.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(influxClient.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(influxClient.getLastErrorMessage());
  }
}

void loop() {
  // Check if there's any data available from the Arduino
  if (arduinoSerial.available()) {
    String data = arduinoSerial.readStringUntil('\n'); // Read the line sent from Arduino
    Serial.println("Received data: " + data); // Debugging output

    // Split the data string into individual sensor values
    float sensorValues[12]; // Adjust size if needed
    int commaIndex = 0;
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
      // Set Firebase data
      if (Firebase.setFloat(fbdo, "/sensorData/averageLDR", sensorValues[5])) {
        Serial.println("Set LDR successfully.");
      } else {
        Serial.printf("Error setting LDR: %s\n", fbdo.errorReason().c_str());
      }
      
      if (Firebase.setFloat(fbdo, "/sensorData/averageVoltage", sensorValues[9])) {
        Serial.println("Set Voltage successfully.");
      } else {
        Serial.printf("Error setting Voltage: %s\n", fbdo.errorReason().c_str());
      }
      
      if (Firebase.setFloat(fbdo, "/sensorData/current", sensorValues[10])) {
        Serial.println("Set Current successfully.");
      } else {
        Serial.printf("Error setting Current: %s\n", fbdo.errorReason().c_str());
      }
      
      if (Firebase.setFloat(fbdo, "/sensorData/temperature", sensorValues[11])) {
        Serial.println("Set Temperature successfully.");
      } else {
        Serial.printf("Error setting Temperature: %s\n", fbdo.errorReason().c_str());
      }
    } else {
      Serial.println("Firebase not ready.");
    }

    // Clear fields for reusing the point. Tags will remain the same as set above.
    sensor.clearFields();
  
    // Store measured value into point
    // Writing sensor data to InfluxDB
    sensor.addField("averageLDR", sensorValues[5]);
    sensor.addField("averageVoltage", sensorValues[9]);
    sensor.addField("current", sensorValues[10]);
    sensor.addField("temperature", sensorValues[11]);
    sensor.addField("rssi", WiFi.RSSI());

    // Print what are we exactly writing
    Serial.print("Writing to InfluxDB: ");
    Serial.print("averageLDR: "); Serial.print(sensorValues[5]);
    Serial.print(", averageVoltage: "); Serial.print(sensorValues[9]);
    Serial.print(", current: "); Serial.print(sensorValues[10]);
    Serial.print(", temperature: "); Serial.print(sensorValues[11]);
    Serial.print(", rssi: "); Serial.println(WiFi.RSSI());

    // Write point
    if (!influxClient.writePoint(sensor)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(influxClient.getLastErrorMessage());
    } else {
      Serial.println("Data written to InfluxDB.");
    }

    delay(2000); // Adjust the delay as needed
  } else {
    Serial.println("No data available from Arduino."); // Debugging output
  }

  delay(20000); // Wait 20 seconds to update the channel again
}
