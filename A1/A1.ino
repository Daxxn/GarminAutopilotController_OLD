#include <RotaryEncoder.h>
#include <Wire.h>

const bool debugMode = true;

const int id = 10;
const int btnPins[10] = {1,2,7,8,9,10,13,14,15,16};
const int rotorPins[4] = {6,5,11,12};

RotaryEncoder encoders[2] = {
  RotaryEncoder(rotorPins[0], rotorPins[1], RotaryEncoder::LatchMode::TWO03),
  RotaryEncoder(rotorPins[2], rotorPins[3], RotaryEncoder::LatchMode::TWO03),
};

const int totalInputs = sizeof(btnPins) + sizeof(encoders);

byte btnStates[sizeof(btnPins)];

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
  ReadEncoders();
  PrintDebug();
}

void RequestEvent() {
  byte enc0St = (byte)encoders[0].getDirection();
  byte enc1St = (byte)encoders[1].getDirection();
  byte tempData[totalInputs] = {
    btnStates,
    enc0St,
    enc1St
  };

  for (int i = 0; i < totalInputs; i++) {
    Wire.write(tempData[i]);
  }
}

void ReadEncoders() {
  for (int i = 0; i < sizeof(encoders); i++) {
    encoders[i].tick();
  }
}

void ReadBtns() {
  for ( int i = 0; i < sizeof(btnPins); i++) {
    byte tempState = digitalRead(btnPins[i]);
    if (tempState != btnStates[i]) {
      btnStates[i] = tempState;
    }
  }
}

void pinSetup() {
  for (int i = 0; i < sizeof(btnPins); i++) {
    pinMode(btnPins[i], INPUT_PULLUP);
  }
}

void PrintDebug() {
  if (debugMode) {
    for (int i = 0; i < sizeof(btnStates); i++) {
      Serial.print(btnStates[i]);
    }
    Serial.println();
  }
}
