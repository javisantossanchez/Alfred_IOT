#define LED_BUILTIN 2    
void setup() {
Serial.begin(115200);
pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {

Serial.println("Node MCU blink!");
  // turn the LED on (the built-in led on a Node MCU board is active low)
digitalWrite(LED_BUILTIN, LOW);
  // wait for a second
delay(1000);
  // turn the LED off (the built-in led on a Node MCU board is active low)
digitalWrite(LED_BUILTIN, HIGH);

   // wait for a second
delay(1000);
}