#include <Joystick.h>
#include <Wire.h>

const bool debugMode = true;

Joystick_ Joystick;

const int commIds[3] = {10, 20, 30};
//const int commSizes[3] = {14,11,0};
const int commSizes[3] = {1,1,1};

enum class RotorDirection {
  NOROTATION = 2,
  CLOCKWISE = 3,
  CCW = 1
};

//const int btnMap[25] = {
//  1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
//};
const int rotorMap[10];

byte btnStates[14 + 11];

void setup() {
  Wire.begin();
//  Joystick.begin();
  InitState();

  if (debugMode) {
    Serial.begin(9600);
  }
}

void loop() {
  Serial.println("loop");
  for (int i = 0; i < sizeof(commIds); i++) {
    if (commSizes[i] > 0) {
      Wire.requestFrom(commIds[i], commSizes[i]);
      byte data[commSizes[i]] = {};
      int index = 0;
      while (Wire.available()) {
        if (index < commSizes[i]) {
          data[index] = Wire.read();
          index++;
        }
      }
    }
  }

  PrintDebug();
}

void PrintDebug() {
  for (int i = 0; i < sizeof(btnStates); i++) {
    Serial.print(btnStates[i]);
  }
  Serial.println();
}

void InitState() {
  for (int i = 0; i < sizeof(btnStates); i++) {
    btnStates[i] = 0;
  }
}
