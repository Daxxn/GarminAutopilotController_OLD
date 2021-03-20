#include <Wire.h>

const bool debugMode = true;

const int id = 10;
const int buttonCount = 12;
const int btnPins[12] = {2,3,4,5,6,7,8,9,10,11,12,13};

byte buttonStates[12];

void setup() {
  Wire.begin(id);
  Wire.onRequest(RequestEvent);
  pinSetup();

  if (debugMode) {
    Serial.begin(9600);
  }
}

void loop() {
  ReadBtns();
  PrintDebug();
}

void RequestEvent() {
  for (int i = 0; i < buttonCount; i++) {
    Wire.write(buttonStates[i]);
  }
}

void ReadBtns() {
  for ( int i = 0; i < buttonCount; i++) {
    byte tempState = digitalRead(btnPins[i]);
    if (tempState != buttonStates[i]) {
      buttonStates[i] = tempState;
    }
  }
}

void pinSetup() {
  for (int i = 0; i < buttonCount; i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }
}

void PrintDebug() {
  if (debugMode) {
    for (int i = 0; i < buttonCount; i++) {
      Serial.print(buttonStates[i]);
    }
    Serial.println();
  }
}
