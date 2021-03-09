#include <RotaryEncoder.h>
#include <Wire.h>

const int id = 30;
const int pins[12] = {1,2,5,6,7,8,9,10,11,14,15,16};

void setup() {
  Wire.begin(id);
  Wire.onRequest(RequestEvent);
  pinSetup();
}

void loop() {
  
}

void RequestEvent() {
  Wire.write("!");
}

void pinSetup() {
  for (int i = 0; i < sizeof(pins); i++) {
    pinMode(pins[i], INPUT_PULLUP);
  }
}
