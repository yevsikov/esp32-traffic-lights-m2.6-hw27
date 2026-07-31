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

// Timings in milliseconds
constexpr unsigned long RED_MS = 5000;
constexpr unsigned long RED_YELLOW_MS = 2000;
constexpr unsigned long GREEN_MS = 7000;
constexpr unsigned long GREEN_BLINK_TOGGLE_MS = 500;
constexpr uint8_t GREEN_BLINK_CYCLES = 3;
constexpr unsigned long YELLOW_MS = 1000;
constexpr unsigned long EMERGENCY_BLINK_MS = 500;

constexpr unsigned long BUTTON_DEBOUNCE_MS = 40;

Mode mode = Mode::Normal;
NormalState normalState = NormalState::Red;

unsigned long stateStartedAt = 0;
unsigned long emergencyToggledAt = 0;
bool emergencyYellowOn = false;

unsigned long greenBlinkToggledAt = 0;
uint8_t greenBlinkTogglesDone = 0;
bool greenBlinkLedOn = false;

bool lastButtonSample = LOW;
bool stableButtonState = LOW;
unsigned long buttonLastChangedAt = 0;

void setLeds(bool red, bool yellow, bool green) {
  digitalWrite(LED_RED_PIN, red ? HIGH : LOW);
  digitalWrite(LED_YELLOW_PIN, yellow ? HIGH : LOW);
  digitalWrite(LED_GREEN_PIN, green ? HIGH : LOW);
}

void enterNormalState(NormalState next, unsigned long now) {
  normalState = next;
  stateStartedAt = now;

  switch (normalState) {
    case NormalState::Red:
      setLeds(true, false, false);
      break;
    case NormalState::RedYellow:
      setLeds(true, true, false);
      break;
    case NormalState::Green:
      setLeds(false, false, true);
      break;
    case NormalState::GreenBlink:
      greenBlinkToggledAt = now;
      greenBlinkTogglesDone = 0;
      greenBlinkLedOn = true;
      setLeds(false, false, true);
      break;
    case NormalState::Yellow:
      setLeds(false, true, false);
      break;
  }
}


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

