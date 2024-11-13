#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>  

SoftwareSerial arduinoSerial(2, 3); 


const char* ssid = "Sayantan’s iPhone";  
const char* password = "12345678";        

void setup() {
  Serial.begin(9600);
  arduinoSerial.begin(9600);
  
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("NodeMCU IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (arduinoSerial.available()) {
    String data = arduinoSerial.readStringUntil('\n'); 
    Serial.println("Received from Arduino: " + data); 
  }
}
