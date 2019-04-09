/* servoMotionModule.ino
   A wheeled robot servo module
   by C.D.Odom on 2.17.2015.  Updated 7.18.17.
   Configured for TRINITY COLLEGE INTERNATIONAL FIRE FIGHTING ROBOT CONTEST 2019  by Robin D. on 3.30.19
*/

const int leftServoPin = 11;
const int rightServoPin = 12;


// Left Servo Pulse Width Constants
const int left_forward_fast = 1800;       // CCW Fast
const int left_forward_slow = 1600;       // CCW Slow
const int left_stop = 1500;               // Center position
const int left_reverse_slow = 1400;       // CW Slow
const int left_reverse_fast = 1000;       // CW Fast


// Right Servo Pulse Width Constants
const int right_forward_fast = 1000;      // CW Fast
const int right_forward_slow = 1400;      // CW Slow
const int right_stop = 1500;              // Center position
const int right_reverse_slow = 1600;      // CCW Slow
const int right_reverse_fast = 2000;      // CCW Fast

/* For Fast motors
// Left Servo Pulse Width Constants
const int left_forward_fast = 1350;       // CCW Fast
const int left_forward_slow = 1400;       // CCW Slow
const int left_stop = 1500;               // Center position
const int left_reverse_slow = 1600;       // CW Slow
const int left_reverse_fast = 2000;       // CW Fast


// Right Servo Pulse Width Constants
const int right_forward_fast = 2000;      // CW Fast
const int right_forward_slow = 1600;      // CW Slow
const int right_stop = 1500;              // Center position
const int right_reverse_slow = 1400;      // CCW Slow
const int right_reverse_fast = 1000;      // CCW Fast
*/
// Current issue with the new motor is that it explicitly turns left at the start of the first "long" forward


void blinkOK(int numBlinks) {
  // blink the onboard LED to let the user know the code was properly uploaded
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(LEDPin, HIGH);
    delay(250);
    digitalWrite(LEDPin, LOW);
    delay(250);
  }
}

void motionFastTest() {
  // a diagnostic test of basic robot motion at high speed
  forwardFast(88);    // take 88 quick "steps" forward (about 31 cm with my OneBot)
  robotStop(10);      // stop motors to prevent high current drain
  leftFast(50);       // pivot left 50 quick "steps" (about 180 degrees with my OneBot)
  robotStop(10);
  forwardFast(88);    // return to starting point
  robotStop(10);
  rightFast(94);      // pivot right 94 quick "steps" (about 360 degrees with my OneBot)
  robotStop(10);
  reverseFast(88);    // take 88 quick "steps" in reverse (about 33 cm with my OneBot)
}

void motionSlowTest() {
  // a diagnostic test of basic robot motion at slow speed
  forwardSlow(88);    // take 88 slow "steps" forward (about 9 cm with my OneBot)
  robotStop(10);      // stop motors to prevent high current drain
  leftSlow(220);      // pivot left 220 slow "steps" (about 180 degrees with my OneBot)
  robotStop(10);
  forwardSlow(88);    // return to starting point
  robotStop(10);
  rightSlow(453);     // pivot right 453 slow "steps" (about 360 degrees with my OneBot)
  robotStop(10);
  reverseSlow(88);    // take 88 slow "steps" in reverse (about 8 cm with my OneBot)
}


// These movement functions are the HIGHEST LEVEL commands. They cause the robot to
// take a certain number of steps in the designated direction and speed, or to stop
// for a certain number of "steps". These functions call the "step" functions below.

void robotStop(int x) {
  for (int i = 0; i <= x ; i++) {
    stopStep();     // a hard stop (not a coasting stop)
  }
}

void forwardFast(int x) {
  for (int i = 0; i <= x ; i++) {
    forwardStepFast();
  }
}

void forwardSlow(int x) {
  for (int i = 0; i <= x ; i++) {
    forwardStepSlow();
  }
}

void reverseFast(int x) {
  for (int i = 0; i <= x ; i++) {
    reverseStepFast();
  }
}

void reverseSlow(int x) {
  for (int i = 0; i <= x ; i++) {
    reverseStepSlow();
  }
}

void rightFast(int x) {
  for (int i = 0; i <= x ; i++) {
    rightStepFast();
  }
}

void rightSlow(int x) {
  for (int i = 0; i <= x ; i++) {
    rightStepSlow();
  }
}

void leftFast(int x) {
  for (int i = 0; i <= x ; i++) {
    leftStepFast();
  }
}

void leftSlow(int x) {
  for (int i = 0; i <= x ; i++) {
    leftStepSlow();
  }
}


// These "step" functions are the SECOND LOWEST LEVEL of the movement commands. They are
// used to turn both servo wheels in concert to make the robot take exactly one step in
// some direction, or to send a hard stop to both wheels.  This is done by calling the
// createPulse() function, once for each wheel. It is CRITICAL that you keep the 20ms
// delay, because it is necessary to give the motors time to respond!

void stopStep() {
  createPulse(leftServoPin, left_stop);
  createPulse(rightServoPin, right_stop);
  delay(20);      // do NOT alter this delay time!
}

void forwardStepFast() {
  createPulse(leftServoPin, left_forward_fast);
  createPulse(rightServoPin, right_forward_fast);
  delay(20);      // do NOT alter this delay time!
}

void forwardStepSlow() {
  createPulse(leftServoPin, left_forward_slow);
  createPulse(rightServoPin, right_forward_slow);
  delay(20);      // do NOT alter this delay time!
}

void reverseStepFast() {
  createPulse(leftServoPin, left_reverse_fast);
  createPulse(rightServoPin, right_reverse_fast);
  delay(20);      // do NOT alter this delay time!
}

void reverseStepSlow() {
  createPulse(leftServoPin, left_reverse_slow);
  createPulse(rightServoPin, right_reverse_slow);
  delay(20);      // do NOT alter this delay time!
}

void rightStepSlow() {
  createPulse(leftServoPin, left_forward_slow);
  createPulse(rightServoPin, right_reverse_slow);
  delay(20);      // do NOT alter this delay time!
}

void leftStepSlow() {
  createPulse(leftServoPin, left_reverse_slow);
  createPulse(rightServoPin, right_forward_slow);
  delay(20);      // do NOT alter this delay time!
}

void rightStepFast() {
  createPulse(leftServoPin, left_forward_fast);
  createPulse(rightServoPin, right_reverse_fast);
  delay(20);      // do NOT alter this delay time!
}

void leftStepFast() {
  createPulse(leftServoPin, left_reverse_fast);
  createPulse(rightServoPin, right_forward_fast);
  delay(20);      // do NOT alter this delay time!
}


// createPulse() is the LOWEST LEVEL (most basic) of the movement commands.
// It simply creates a single pulse of a given pulse width on a given pin.
// This command makes ONE servo (set by the servoPin variable) take ONE
// step whose speed and direction is set by the pulseWidth variable.

void createPulse(byte servoPin, int pulseWidth) {
  // take one step with one servo
  digitalWrite(servoPin, HIGH);        // create the rising edge of the pulse
  delayMicroseconds(pulseWidth);       // precisely set pulse width in microseconds!
  digitalWrite(servoPin, LOW);         // create the falling edge of the pulse
}

////////////// Robin's

const int turnSteps = 1;         // steps taken per turn action
const int forwardSteps = 1;      // steps taken per walk action
const int right90Steps = 24;
const int left90Steps = 26;

const float firstSensorTailDistance = 15.0;         // distance between front sensor to robot's tail
const float firstSensorDoorwayDistance = 3.0;
const int stepsForwardAfterTurn = 25 * stepsPerCm;  // measure from distance between the bot at door's end to another, min steps to confirm the "stick" (180 turn)
const int stepsForwardAfterTurn2 = 12;              // steps forward the robot will take after side90Ex() has been suspended to prevent false isFar() or stick
const int stepsAwayBefore90 = 5;                    // initialize turn(this) in opposite direction before do the sharp turn to prevent crashing

void readyServo() {
  pinMode(leftServoPin, OUTPUT);
  pinMode(rightServoPin, OUTPUT);
}

void rightSlightly(int multiplier) {
  rightSlow(turnSteps * multiplier);
}
void leftSlightly(int multiplier) {
  leftSlow(turnSteps * multiplier);
}
void forwardSlightly(int multiplier) {
  forwardFast(forwardSteps * multiplier);
}
void right90(int multiplier) {
  rightFast(right90Steps * multiplier);
}
void left90(int multiplier) {
  leftFast(left90Steps * multiplier);
}
void right90Ex(float lastSense, bool justComeOut) {
  if(!justComeOut){
    leftSlightly(stepsAwayBefore90);
  }
  const float tailAdder = 4.0;
  int moveSteps = (int)((tailAdder + firstSensorTailDistance - firstSensorDoorwayDistance*justComeOut) * stepsPerCm + lastSense * stepsPerCm);
  forwardFast(moveSteps);
  right90(1);
  for (int i = 0; i < stepsForwardAfterTurn; i++) {
    if (detectLine()) {
      return;
    }
    forwardFast(1);
    float s1 = getRangeRightFront();
    float s2 = getRangeRightFront();
    float s3 = getRangeRightFront();
    float rangeRightFront = selectRange(s1, s2, s3);
    if(!isFar(rangeRightFront)){
      for(int j = 0; j < stepsForwardAfterTurn2; j++){
        if (detectLine()){
          return;
        }
        forwardFast(1);
      }
      return;
    }
  }
}
void left90Ex(float lastSense, bool justComeOut) {
  if(!justComeOut){
    rightSlightly(stepsAwayBefore90);
  }
  const float tailAdder = 4.0;
  int moveSteps = (int)((tailAdder + firstSensorTailDistance - firstSensorDoorwayDistance*justComeOut) * stepsPerCm + lastSense * stepsPerCm);
  forwardFast(moveSteps);
  left90(1);
  for (int i = 0; i < stepsForwardAfterTurn; i++) {
    if (detectLine()) {
      return;
    }
    forwardFast(1);
    float s1 = getRangeLeftFront();
    float s2 = getRangeLeftFront();
    float s3 = getRangeLeftFront();
    float rangeLeftFront = selectRange(s1, s2, s3);
    if(!isFar(rangeLeftFront)){
      for(int j = 0; j < stepsForwardAfterTurn2; j++){
        if (detectLine()){
          return;
        }
        forwardFast(1);
      }
      return;
    }
  }
}
