// "IRLineSensorModule.ino" by Robin_D on 4.8.19
// Configured for TRINITY COLLEGE INTERNATIONAL FIRE FIGHTING ROBOT CONTEST 2019
// Robot: Firex by George School

/*
////// PINS //////
const byte leftIRLineSensorPin = A9;
const byte rightIRLineSensorPin = A8;
/////////////////
*/
const int IRLMaxWhite = 400;          // maximum sensor value that is considered white tape

void readyIRL() {
  pinMode(leftIRLineSensorPin, INPUT);
  pinMode(rightIRLineSensorPin, INPUT);
}

bool detectLine() {                   // is any of the sensors detect the tape?
  if(tick < minTickRoomEnabled){
    return false;
  }
  int leftValue = analogRead(leftIRLineSensorPin);
  int rightValue = analogRead(rightIRLineSensorPin);
  float rangeLeftRear, s1, s2, s3;
  s1 = getRangeLeftRear();
  s2 = getRangeLeftRear();
  s3 = getRangeLeftRear();
  rangeLeftRear = selectRange(s1, s2, s3);
  if (leftValue <= IRLMaxWhite || rightValue <= IRLMaxWhite) {
    return true;
  }
  return false;
}

void alignBot() {
  while (true) {
    int leftValue = analogRead(leftIRLineSensorPin);
    int rightValue = analogRead(rightIRLineSensorPin);
    if (leftValue <= IRLMaxWhite && rightValue <= IRLMaxWhite) {              // both are on the tape!
      break;
    }
    else if (rightValue <= IRLMaxWhite) {                                     // right sensor is on the tape
      rightFast(1);
    }
    else if (leftValue <= IRLMaxWhite) {                                      // left sensor is on the tape
      leftFast(1);
    }
    else {                                                                    // none is on the tape
      forwardFast(1);
    }
  }
}

int debugSense() {
  return analogRead(leftIRLineSensorPin);
}
