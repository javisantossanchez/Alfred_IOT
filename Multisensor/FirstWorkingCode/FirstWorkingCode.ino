#include <Wire.h>
#include <BH1750.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

#define timeSeconds 10
#define LED_BUILTIN 2   

BH1750 lightMeter; //Default address for this sensor is 0x23
Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

//PIR
const int led = 2;
const int motionSensor = 18;
//mmwave
const int sensor_pin = 25;
// PIR Timer: Auxiliary variables
unsigned long now = millis();
unsigned long lastTrigger = 0;
boolean startTimer = false;

// PIR: Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(LED_BUILTIN, HIGH);
  startTimer = true;
  lastTrigger = millis();
}

void setup() {
    Serial.begin(9600);

    // Initialize the I2C bus (BH1750 library doesn't do this automatically)
    Wire.begin();

    while(!Serial);    // time to get serial running
    Serial.println(F("BME280 test"));

    unsigned status;
    
    // default settings
    status = bme.begin(0x76);  //Default BME280 address
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    lightMeter.begin();
    Serial.println("-- Default Test --");
    delayTime = 1000;

    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(LED_BUILTIN, OUTPUT);
    // PIR Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

    // PIR Set LED to LOW
    pinMode(led, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    //PIR END
    pinMode(sensor_pin, INPUT);
    Serial.println();
}


void loop() { 

    //PIR Current time
    now = millis();

    //PIR Turn off the LED after the number of seconds defined in the timeSeconds variable
    if(startTimer && (now - lastTrigger > (timeSeconds*1000))) {
      Serial.println("Motion stopped...");
      digitalWrite(LED_BUILTIN, LOW);
      startTimer = false;
    }

    printValues();
    float lux = lightMeter.readLightLevel();
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");

    if (digitalRead(sensor_pin) == HIGH) {
      Serial.println("MM Presencia detectada");
      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);
    }
    if (digitalRead(sensor_pin) == LOW) {
      Serial.println("MMWAVE No presence detected");
      digitalWrite(led, HIGH);
      delay(1000);
      digitalWrite(led, LOW);
  }

    delay(delayTime);
}


void printValues() {
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");

    Serial.print("Pressure = ");

    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.println();
}
