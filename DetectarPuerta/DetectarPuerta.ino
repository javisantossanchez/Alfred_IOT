
#include <WiFi.h>
#include <PubSubClient.h>

#define DOOR_SENSOR_PIN  19  // ESP32 pin GIOP19 connected to door sensor's pin

const char* ssid = "DIGIFIBRA-Ncy7";
const char* password = "76dGkPfbAe";

const char* mqttServer = "192.168.1.194";
const int mqttPort = 1883;
const char* mqttUser = "yourMQTTuser";
const char* mqttPassword = "yourMQTTpassword";


EspMQTTClient client(
  "192.168.1.194",  // MQTT Broker server ip
  1883,     // Client name that uniquely identify your device
  "ESP8266"              // The MQTT port, default to 1883. this line can be omitted
);



int currentDoorState; // current state of door sensor
int lastDoorState;    // previous state of door sensor

void setup() {
  Serial.begin(9600);                     // initialize serial
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  currentDoorState = digitalRead(DOOR_SENSOR_PIN); // read state

  delay(1000);


    //CONEXION WIFI
    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.println("Connecting to WiFi..");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    
    // Optional functionalities of EspMQTTClient
    client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
    client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overridded with enableHTTPWebUpdater("user", "password").
    client.enableOTA(); // Enable OTA (Over The Air) updates. Password defaults to MQTTPassword. Port is the default OTA port. Can be overridden with enableOTA("password", port).
    client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
 

}


void onConnectionEstablished() {


}


void loop() {

  client.loop();
  lastDoorState = currentDoorState;              // save the last state
  currentDoorState  = digitalRead(DOOR_SENSOR_PIN); // read new state
  
  
  lastDoorState = currentDoorState;              // save the last state
  currentDoorState  = digitalRead(DOOR_SENSOR_PIN); // read new state
  if (lastDoorState == LOW && currentDoorState == HIGH) { // state change: LOW -> HIGH
    Serial.println("The door-opening event is detected");
    client.publish("esp/test", "door-opening event detected");

    // TODO: turn on alarm, light or send notification ...
  }
  else
  if (lastDoorState == HIGH && currentDoorState == LOW) { // state change: HIGH -> LOW
    Serial.println("The door-closing event is detected");
    client.publish("esp/test", "door-closing event detected");
  }
  
}