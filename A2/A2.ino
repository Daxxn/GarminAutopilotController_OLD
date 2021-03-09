#include <RotaryEncoder.h>
#include <Wire.h>

const int id = 10;
const int btnPins[9] = {5,6,7,8,9,10,12,13,14};
const int rotorPins[2] = {10,11};

RotaryEncoder encoders[1] = {
  RotaryEncoder(rotorPins[0], rotorPins[1], RotaryEncoder::LatchMode::TWO03)
};

const int totalInputs = sizeof(btnPins) + sizeof(encoders);

byte btnStates[sizeof(btnPins)];

void setup() {
  Wire.begin(id);
  Wire.onRequest(RequestEvent);
  pinSetup();
}

void loop() {
  ReadEncoders();
}

void RequestEvent() {
  byte enc0St = (byte)encoders[0].getDirection();
  byte tempData[totalInputs] = {
    btnStates,
    enc0St
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
