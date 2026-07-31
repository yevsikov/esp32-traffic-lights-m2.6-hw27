#include <Arduino.h>

#define LED_RED_PIN 5
#define LED_YELLOW_PIN 6
#define LED_GREEN_PIN 7
#define BTN_PIN 8
#define BAUDRATE 115200

enum class Mode {
  Normal,
  Emergency,
};

enum class NormalState {
  Red,
  RedYellow,
  Green,
  GreenBlink,
  Yellow,
};


void setup() {
  Serial.begin(BAUDRATE);

  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT);

  unsigned long now = millis();
  enterNormalState(NormalState::Red, now);

  lastButtonSample = digitalRead(BTN_PIN);
  stableButtonState = lastButtonSample;
  buttonLastChangedAt = now;
}

void loop() {
  unsigned long now = millis();

  updateButton(now);

  if (mode == Mode::Normal) {
    updateNormalMode(now);
  } else {
    updateEmergencyMode(now);
  }
}

