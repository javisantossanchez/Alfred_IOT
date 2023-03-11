#include <HardwareSerial.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Define las credenciales de conexión Wi-Fi
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

// Define las credenciales de conexión MQTT
const char* mqtt_server = "TU_BROKER_MQTT";
const int mqtt_port = 1883;
const char* mqtt_user = "TU_USUARIO_MQTT";
const char* mqtt_password = "TU_PASSWORD_MQTT";

// Define el pin al que está conectado el sensor HLK-LD2410
const int sensorPin = 13;

// Inicializa el objeto de comunicación serial
HardwareSerial SerialHLK(2);  // UART2

// Inicializa el objeto de conexión Wi-Fi
WiFiClient espClient;

// Inicializa el objeto de conexión MQTT
PubSubClient client(espClient);

void setup() {
  // Inicializa la comunicación serial a 9600 baudios
  SerialHLK.begin(9600, SERIAL_8N1, 16, 17);  // RX, TX

  // Conecta el ESP32 a la red Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  // Conecta el ESP32 al broker MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.connect("ESP32Client", mqtt_user, mqtt_password);

  // Configura el pin del sensor HLK-LD2410 como entrada
  pinMode(sensorPin, INPUT);
}

void loop() {
  // Lee el valor del sensor HLK-LD2410
  int sensorValue = digitalRead(sensorPin);

  // Envía el valor del sensor al broker MQTT
  if (client.connected()) {
    client.publish("TU_TOPIC", String(sensorValue).c_str());
  }

  // Espera 1 segundo antes de volver a leer el sensor
  delay(1000);
}