#include <Arduino.h>

#define LED_RED_PIN 5
#define LED_YELLOW_PIN 6
#define LED_GREEN_PIN 7
#define BTN_PIN 8
#define BAUDRATE 115200

void setup() {
  Serial.begin(BAUDRATE);

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  // init traffic lighter via yellow blinking
  digitalWrite(LED_YELLOW_PIN, HIGH);
  delay(500);
  digitalWrite(LED_YELLOW_PIN, LOW);
  delay(500);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  delay(500);
  digitalWrite(LED_YELLOW_PIN, LOW);

}

void loop() {
  
  delay(500);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_RED_PIN, HIGH);
  delay(5000);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_RED_PIN, LOW);
  delay(250);
  digitalWrite(LED_YELLOW_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);
  delay(7000);
  digitalWrite(LED_GREEN_PIN, LOW);
  delay(500);
  digitalWrite(LED_GREEN_PIN, HIGH);
  delay(500);
  digitalWrite(LED_GREEN_PIN, LOW);
  delay(500);
  digitalWrite(LED_GREEN_PIN, HIGH);
  delay(500);
  digitalWrite(LED_GREEN_PIN, LOW);
  delay(500);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  delay(1000);


}

