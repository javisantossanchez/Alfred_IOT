
#include <WiFi.h>
#include <PubSubClient.h>

#define DOOR_SENSOR_PIN  19  // ESP32 pin GIOP19 connected to door sensor's pin

const char* ssid = "DIGIFIBRA-Ncy7";
const char* password = "76dGkPfbAe";

const char* mqtt_server = "192.168.1.194";
const int mqttPort = 1883;
const char* mqttUser = "yourMQTTuser";
const char* mqttPassword = "yourMQTTpassword";



int currentDoorState; // current state of door sensor
int lastDoorState;    // previous state of door sensor

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);                     // initialize serial
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // set ESP32 pin to input pull-up mode
  currentDoorState = digitalRead(DOOR_SENSOR_PIN); // read state
  delay(1000);
  setup_wifi();
  client.setServer(mqtt_server, 1883);


 

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup_wifi(){
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
}


void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
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