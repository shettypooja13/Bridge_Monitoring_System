//#include <WiFiClient.h>
//#include <ESP8266WebServer.h>
//theiotprojects.com
#include <Ultrasonic.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <EEPROM.h>
//#define ServoPort D1

  // create servo object to control a servo
// twelve servo objects can be created on most boards

int val;
//#define USE_SERIAL Serial
int Vib =12; 
// Instantiate trig and echo pins for ultrasonic sensors
Ultrasonic ultrasonic (2, 0);
// Replace with your network credentials
const char* ssid = "Realme 1";
const char* password = "passwordchange";

//Servo myservo;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
//ESP8266WebServer server1(80); 

String getDistance() {
  // Read Distance
  float d = ultrasonic.read();
  if (isnan(d)) {
    Serial.println("Failed to read from HC-SR04 sensor!");
    return "";
  }
  else {
    Serial.println(d);
    return String(d);
  }
}
String getVibration() {
  // Read Vibration
  val = vibration();
  if (isnan(val)) {
    Serial.println("Failed to read from Vibration sensor!");
    return "";
  }
  else {
    Serial.println(val);
    return String(val);
  }  
}

long vibration(){
  long measurement=pulseIn (Vib, HIGH);  //wait for the pin to get HIGH and returns measurement
  return measurement/10;
}

/*void handleServo(){

  String POS = server1.arg("servoPOS");

  int pos = POS.toInt();

  myservo.write(pos);   //--> Move the servo motor according to the POS value

  delay(15);

  Serial.print("Servo Angle:");

  Serial.println(pos);

  server1.send(200, "text/plain","");

}*/


void setup () {

 // myservo.attach(ServoPort); 
  // Serial port for debugging purposes
  Serial.begin (115200);

  // Initialize SPIFFS
  if (! SPIFFS.begin ()) {
    Serial.println ("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for web page
  server.on ("/", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request-> send (SPIFFS, "/index.html");
  });
  
  
  server.on ("/vibration", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request-> send_P (200, "text / plain", getVibration(). c_str ());
  }); 
  server.on ("/distance", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request-> send_P (200, "text / plain", getDistance(). c_str ());
  });
 //server1.on("/setPOS",handleServo); 

  // start server
  server.begin ();
}
void loop() {
  //server1.handleClient();
}
