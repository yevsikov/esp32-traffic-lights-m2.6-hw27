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

void switchMode(Mode nextMode, unsigned long now) {
  mode = nextMode;

  if (mode == Mode::Normal) {
    enterNormalState(NormalState::Red, now);
  } else {
    emergencyToggledAt = now;
    emergencyYellowOn = true;
    setLeds(false, true, false);
  }
}

void updateButton(unsigned long now) {
  bool sample = digitalRead(BTN_PIN);

  if (sample != lastButtonSample) {
    lastButtonSample = sample;
    buttonLastChangedAt = now;
  }

  if ((now - buttonLastChangedAt) >= BUTTON_DEBOUNCE_MS && sample != stableButtonState) {
    stableButtonState = sample;

    // External pulldown: pressed == HIGH
    if (stableButtonState == HIGH) {
      Mode next = (mode == Mode::Normal) ? Mode::Emergency : Mode::Normal;
      switchMode(next, now);
    }
  }
}

void updateNormalMode(unsigned long now) {
  switch (normalState) {
    case NormalState::Red:
      if ((now - stateStartedAt) >= RED_MS) {
        enterNormalState(NormalState::RedYellow, now);
      }
      break;

    case NormalState::RedYellow:
      if ((now - stateStartedAt) >= RED_YELLOW_MS) {
        enterNormalState(NormalState::Green, now);
      }
      break;

    case NormalState::Green:
      if ((now - stateStartedAt) >= GREEN_MS) {
        enterNormalState(NormalState::GreenBlink, now);
      }
      break;

    case NormalState::GreenBlink:
      if ((now - greenBlinkToggledAt) >= GREEN_BLINK_TOGGLE_MS) {
        greenBlinkToggledAt = now;
        greenBlinkLedOn = !greenBlinkLedOn;
        setLeds(false, false, greenBlinkLedOn);
        greenBlinkTogglesDone++;

        if (greenBlinkTogglesDone >= GREEN_BLINK_CYCLES * 2) {
          enterNormalState(NormalState::Yellow, now);
        }
      }
      break;

    case NormalState::Yellow:
      if ((now - stateStartedAt) >= YELLOW_MS) {
        enterNormalState(NormalState::Red, now);
      }
      break;
  }
}

void updateEmergencyMode(unsigned long now) {
  if ((now - emergencyToggledAt) >= EMERGENCY_BLINK_MS) {
    emergencyToggledAt = now;
    emergencyYellowOn = !emergencyYellowOn;
    setLeds(false, emergencyYellowOn, false);
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

