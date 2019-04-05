const byte lowIRSensorPin = A7;

#define MAX 9999

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
