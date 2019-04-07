const byte leftIRLineSensorPin = A9;
const byte rightIRLineSensorPin = A8;

const int IRLMaxWhite = 300;          // maximum sensor value that is considered white tape

void readyIRL() {
  pinMode(leftIRLineSensorPin, INPUT);
  pinMode(rightIRLineSensorPin, INPUT);
}

bool detectLine() {                   // is any of the sensors detect the tape?
  int leftValue = analogRead(leftIRLineSensorPin);
  int rightValue = analogRead(rightIRLineSensorPin);
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
