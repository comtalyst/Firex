// "IRSensorModule.ino" by Robin_D on 4.8.19
// Configured for TRINITY COLLEGE INTERNATIONAL FIRE FIGHTING ROBOT CONTEST 2019
// Robot: Firex by George School

#define MAX 9999

////// PINS //////
const byte lowIRSensorPin = A7;
/////////////////

void readyIR() {
  pinMode(lowIRSensorPin, INPUT);
}

float getIRRange(int IRValue) {
  float IRRange = pow( (4414.8 / float(IRValue) ), 1.22);
  return IRRange;
}

float getRangeFrontLow() {        // UNFINISHED
  float value = analogRead(lowIRSensorPin);
  return getIRRange(value);
}


bool isFarIR(float range) {
  if (range >= 30) {
    return true;
  }
  else {
    return false;
  }
}
