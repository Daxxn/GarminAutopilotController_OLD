#include <Joystick.h>
#include <Wire.h>

const bool debugMode = true;

Joystick_ Joystick;

const int delayTimer = 50;
const int commIds[3] = {10, 20, 30};

const int buttonMsgSize = 12;
const int encoderCount = 5;

const int localButtonCount = 6;
const int buttonPins[6] = {4,5,6,7,8,9};

enum class RotorDirection {
  NOROTATION = 2,
  CLOCKWISE = 3,
  CCW = 1
};

struct State {
  byte extButtons[12];
  byte localButtons[6];
  byte encoders[5];

  int extButtonCount = 12;
  int localButtonCount = 6;
  int encoderCount = 5;

  int localButtonMap[6] = {0,1,2,3,4,5};
  int extButtonMap[12] = {6,7,8,9,10,11,12,13,14,15,16,17};
  int encoderMap[10] = {18,19,20,21,22,23,24,25,26,27};
};

State StateData;

int joystickDelayCount = 0;
const int rotorMap[10];

void setup() {
  Wire.begin();
  Joystick.begin(false);
  InitButtons();

  if (debugMode) {
    Serial.begin(9600);
  }
}

void loop() {
  ReadButtons();
  GetExtButtonStates();
  GetEncoderStates();

  SetJoystickButtons();
  SetJoystickEncoders();

  if (joystickDelayCount <= delayTimer) {
    Joystick.sendState();
    joystickDelayCount = 0;
  } else {
    joystickDelayCount++;
  }
  PrintDebug();
}

void ReadButtons() {
  for (int i = 0; i < StateData.localButtonCount; i++) {
    byte temp = !digitalRead(buttonPins[i]);
    if (temp != StateData.localButtons[i]) {
      StateData.localButtons[i] = temp;
    }
  }
}

void GetExtButtonStates() {
  Wire.requestFrom(commIds[0], buttonMsgSize);

  byte tempState = 0;
//  for (int i = 0; i < buttonMsgSize; i++) {
//    if (Wire.available()) {
//      StateData.buttons[i] = (byte)Wire.read();
//    }
//  }
  int index = 0;
  while (Wire.available()) {
    StateData.extButtons[index] = !Wire.read();
    index++;
  }
}

void GetEncoderStates() {
  Wire.requestFrom(commIds[2], encoderCount);

  byte temp = 0;
  for (int i = 0; i < encoderCount; i++) {
    if (Wire.available()) {
      temp = Wire.read();
//      if (temp != encoderStates[i]) {
//        encoderStates[i] = temp;
      if (temp != StateData.encoders[i]) {
        StateData.encoders[i] = temp;
      }
    }
  }
}

void PrintDebug() {
  if (debugMode) {
    for (int i = 0; i < 12; i++) {
      Serial.print(StateData.extButtons[i]);
    }
    Serial.print(' ');
    for (int i = 0; i < 6; i++) {
      Serial.print(StateData.localButtons[i]);
    }
    Serial.print(' ');
    for (int i = 0; i < 5; i++) {
      Serial.print(StateData.encoders[i]);
    }
    Serial.println();
  }
}

void InitButtons() {
  for (int i = 0; i < localButtonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

// Joystick Control:
void SetJoystickButtons() {
  for (int i = 0; i < StateData.localButtonCount; i++) {
    Joystick.setButton(StateData.localButtonMap[i], StateData.localButtons[i]);
  }

  for (int i = 0; i < StateData.extButtonCount; i++) {
    Joystick.setButton(StateData.extButtonMap[i], StateData.extButtons[i]);
  }
}

void SetJoystickEncoders() {
  for (int i = 0; i < StateData.encoderCount * 2; i += 2) {
    Joystick.setButton(StateData.encoderMap[i], StateData.encoders[i] == 1 ? 1 : 0);
    Joystick.setButton(StateData.encoderMap[i + 1], StateData.encoders[i] == 3 ? 1 : 0);
  }
}
