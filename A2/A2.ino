#include <Wire.h>

#define debugMode true

const int id = 20;

const int led_S_1 = 10;
const int led_CLK_1 = 9;
const int led_LE_1 = 8;
const int led_OE_1 = 7;

const int led_S_2 = 4;
const int led_CLK_2 = 3;
const int led_LE_2 = 2;
const int led_OE_2 = 5;

const int ledStateCount = 32;
const int ledState2ndHalf = 16;
const int msgSize = 16;
byte ledMsgState[msgSize];
byte ledState[32];
const int backlightInd[14] = {0,6,8,10,13,15,16,18,20,22,25,26,28,31};
const int indicateIndex[15] = {1,4,7,9,11,12,14,17,19,21,23,24,27,29,30};

bool backlight = true;

int stateA = 0;
int stateB = 0;

void setup() {
  Wire.begin(id);
  pinSetup();

  if (debugMode) {
    Serial.begin(9600);
  }
}

void loop() {
//  GetLEDState();
//  SendLEDState();
  SendLEDState_Test();
}

void SendLEDState() {
  CalcShiftInteger(true);
  CalcShiftInteger(false);
  ShiftLEDs(true, stateA);
  ShiftLEDs(false, stateB);
}

void SendLEDState_Test() {
  for (byte i = 0; i < 16; i++) {
    Serial.println(i);
    ShiftLEDs(true, i);
    delay(500);
    ShiftLEDs(false, i);
    delay(500);
  }
}

void GetLEDState() {
  // Read from I2C bus...
  // Still not sure how im gonna do it.
  // Maybe i should just send the bytes pre-calc-ed.
  for (int i = 0; i < msgSize; i++) {
    if (Wire.available()) {
      ledMsgState[i] = Wire.read();
    }
  }
}

void CalcShiftInteger(bool isFirst) {
  if (isFirst) {
    stateA = 0;
    for (int i = 0; i < ledState2ndHalf; i++) {
      stateA += (1 << i) * ledState[i];
    }
  } else {
    stateB = 0;
    for (int i = ledState2ndHalf; i < ledStateCount; i++) {
      stateB += (1 << i) * ledState[i];
    }
  }
}

void SetBacklightLEDs(bool state) {
  for (int i = 0; i < sizeof(backlightInd); i++) {
    ledState[backlightInd[i]] = state;
  }
}

void ShiftLEDs(byte data, bool isFirst) {
  if (isFirst) {
    digitalWrite(led_OE_1, HIGH);
    digitalWrite(led_LE_1, HIGH);
    shiftOut(led_S_1, led_CLK_1, LSBFIRST, data);
    digitalWrite(led_LE_1, LOW);
    digitalWrite(led_OE_1, LOW);
  } else {
    digitalWrite(led_OE_2, HIGH);
    digitalWrite(led_LE_2, HIGH);
    shiftOut(led_S_2, led_CLK_2, LSBFIRST, data);
    digitalWrite(led_LE_2, LOW);
    digitalWrite(led_OE_2, LOW);
  }
}

void pinSetup() {
  pinMode(led_S_1, OUTPUT);
  pinMode(led_CLK_1, OUTPUT);
  pinMode(led_LE_1, OUTPUT);
  pinMode(led_OE_1, OUTPUT);

  pinMode(led_S_2, OUTPUT);
  pinMode(led_CLK_2, OUTPUT);
  pinMode(led_LE_2, OUTPUT);
  pinMode(led_OE_2, OUTPUT);
}
