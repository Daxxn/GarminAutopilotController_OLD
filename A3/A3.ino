#include <RotaryEncoder.h>
#include <Wire.h>

#define debugMode true

const int id = 30;
const int pins[11] = {};

const int Apins[5] = {4,5,7,9,12}; // NEED TO GET THE PINOUT FOR THE ALT-ADJ!!
const int Bpins[5] = {3,6,8,10,13};

const int encoderCount = 5;
int directions[5];

RotaryEncoder encoders[5] = {
  RotaryEncoder(Apins[0], Bpins[0], RotaryEncoder::LatchMode::TWO03),
  RotaryEncoder(Apins[1], Bpins[1], RotaryEncoder::LatchMode::TWO03),
  RotaryEncoder(Apins[2], Bpins[2], RotaryEncoder::LatchMode::TWO03),
  RotaryEncoder(Apins[3], Bpins[3], RotaryEncoder::LatchMode::TWO03),
  RotaryEncoder(Apins[4], Bpins[4], RotaryEncoder::LatchMode::TWO03),
};

void setup() {
  Wire.begin(id);
  Wire.onRequest(RequestEvent);

  if (debugMode) {
    Serial.begin(9600);
  }
}

void loop() {
  Update();
  for (int i = 0; i < 5; i++) {
    directions[i] = (int)encoders[i].getDirection();
  }

  Debug();
}

void Update() {
  for (int i = 0; i < 5; i++) {
    encoders[i].tick();
  }
}

void RequestEvent() {
  for (int i = 0; i < encoderCount; i++) {
    int offset = directions[i] + 2;
    Wire.write((byte)offset);
  }
}

void Debug() {
  if (debugMode) {
    for (int i = 0; i < 5; i++) {
      Serial.print(directions[i]);
      Serial.print(' ');
    }
    Serial.println();
  }
}
