#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi y MQTT detalles de conexión
const char* ssid = "nombre_de_la_red_wifi";
const char* password = "contraseña_de_la_red_wifi";
const char* mqtt_server = "dirección_del_servidor_mqtt";
const char* mqtt_username = "usuario_mqtt";
const char* mqtt_password = "contraseña_mqtt";
const char* mqtt_topic = "temperatura_y_humedad";

// Configuración del sensor BM280
Adafruit_BME280 bme;
float temperature, humidity, pressure;

// Objeto para la conexión MQTT
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // Inicialización del sensor BM280
  if (!bme.begin(0x76)) {
    Serial.println("No se pudo encontrar el sensor BM280. Verifique las conexiones y la dirección I2C.");
    while (1);
  }

  // Conexión a Wi-Fi
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conexión WiFi establecida");
  Serial.println("Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Conexión a MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.print("Conectando a MQTT...");
    if (client.connect("esp32", mqtt_username, mqtt_password)) {
      Serial.println("conectado");
    } else {
      Serial.print("falló, rc=");
      Serial.print(client.state());
      Serial.println(" intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }

  // Suscribirse a un topic MQTT
  client.subscribe("comando");
}

void loop() {
  // Lectura del sensor BM280
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;

  // Publicación de mediciones en MQTT
  String payload = "{\"temperatura\": ";
  payload += temperature;
  payload += ", \"humedad\": ";
  payload += humidity;
  payload += ", \"presion\": ";
  payload += pressure;
  payload += "}";

  client.publish(mqtt_topic, payload.c_str());

  // Esperar antes de la siguiente lectura
  delay(5000);
}

// Función de callback para mensajes entrantes
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}