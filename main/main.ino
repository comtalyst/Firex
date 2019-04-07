/*
   Maze traversing module

*/

/* Known issues
   If the robot is still too far, but needed to turn around the corner --> the path would be too wide
   That caused from too far fixing is not fast enough
*/

#define MAX 9999

const int LEDPin = 13;

char side = 'R';            // focusing side of wall following

const float minFurther = 1;        // minimum distance difference between two sensors that would make the robot readjust itself
const float minTooFar = 21;
const float maxTooClose = 5;
const float minWalkable = 15;       // minimum distance needed in front of the bot for it to walk forward
const int minTickToBeStraight = 6;
const int roomEnterSteps = 10;
const float doorWide = 50;
const float botWide = 14;
const int stepsForwardAfterTurn = 18; // measure from distance between the bot at door's end to another

int tick;
int lastBalance;
float lastSense = 15;               // last sensed distance to the l/r wall
int lastFar = -MAX;               // last tick that detected far l/r wall
int lastLineDetect = -MAX;
int forceForward = 0;

void setup() {
  Serial.begin(9600);                     // Open the serial port
  delay(100);
  pinMode(LEDPin, OUTPUT);                // onboard LED
  readyServo();
  readySonic();
  readyIRL();
  readyIR();
  blinkOK(3);                             // ok now
}

void loop() {
  while(true){
    forwardFast(1);
    delay(200);
  }
  /*while (true){
    Serial.println(String(getRangeRightFront()) + ", " + String(getRangeRightRear()));
    Serial.println(getRangeFrontLow());
    delay(100);
  }*/

  if (side == 'R') {
    float rangeRightFront = getRangeRightFront();
    float rangeRightRear = getRangeRightRear();
    float rangeFrontLow = getRangeFrontLow();

    // update last far
    if (isFar(rangeRightFront)) {
      lastFar = tick;
    }
    // if it's currently straight and not far --> update last sensed distance
    else if (tick - lastFar >= minTickToBeStraight && !isFar(rangeRightFront) && hzPosBad(rangeRightFront) == 0) {
      lastSense = rangeRightFront;
    }

    // if bot detect the opened door
    if (tick - lastLineDetect > roomEnterSteps * 2 && detectLine()) {
      alignBot();
      forwardFast(roomEnterSteps);
      robotStop(20);                // tmp
      // search for candle
      right90(2);
      forwardFast(roomEnterSteps);
      lastBalance = tick;
      lastFar = -MAX;
      lastSense = doorWide - lastSense - botWide;
      lastLineDetect = tick;
    }
    // IF SIDE-FRONT SENSOR IS FAR
    else if (isFar(rangeRightFront)) {
      // begin smooth 90 deg turn
      rightSlightly(1);
      float turnForwardValue = pow(lastSense*0.5,0.5);   // steps : wall vs sensor (the more the broader) old = lastsense*4/13
      int stepsPerRotate;
      if(fmod(turnForwardValue,1.0) < 0.5){
        stepsPerRotate = (int)turnForwardValue;
      }
      else{
        stepsPerRotate = (int)turnForwardValue + 1;
      }
      forwardSlightly(stepsPerRotate);   
      lastBalance = tick;
      lastFar = tick;
    }
    // IF TOO FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    else if (hzPosBad(rangeRightFront) == 1 || rangeRightFront - (isFar(rangeRightRear) ? MAX : rangeRightRear) >= minFurther) { // since this if()`, rangeRightFront is conditionally guaranteed not far
      // TURN RIGHT SLIGHTLY
      if (lastBalance == tick - 1) {
        rightSlightly(1);
      }
      else {
        rightSlightly(2);
        for (int i = 0; i < tick - lastBalance; i++) {
          forwardSlightly(1);
          if (isFar(getRangeRightFront()) || rangeFrontLow < minWalkable) {
            break;
          }
        }
        leftSlightly(1);
      }
      lastBalance = tick;
    }
    // IF TOO CLOSE OR SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
    else if (hzPosBad(rangeRightFront) == -1 || rangeRightRear - rangeRightFront >= minFurther) {     // rangeRightRear must not too far too (auto detect)
      // TURN LEFT SLIGHTLY
      if (lastBalance == tick - 1) {
        leftSlightly(1);
      }
      else {
        leftSlightly(2);
        for (int i = 0; i < tick - lastBalance; i++) {
          forwardSlightly(1);
          if (isFar(getRangeRightFront()) || rangeFrontLow < minWalkable) {
            break;
          }
        }
        rightSlightly(1);
      }
      lastBalance = tick;
    }
    else if (isFarIR(rangeFrontLow) || rangeFrontLow >= minWalkable) {
      // FORWARD SLIGHTLY
      forwardSlightly(1);
    }
    else {
      // LET THE SENSOR FACE THE WALL
      left90(1);
    }
  }
  else if (side == 'L') {
    // IF SIDE-FRONT SENSOR IS FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    if (isFar(getRangeLeftFront()) || getRangeLeftFront() - getRangeLeftRear() >= minFurther) {
      // TURN LEFT SLIGHTLY
      leftSlightly(1);
    }
    // IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
    else if (getRangeLeftRear() - getRangeLeftFront() >= minFurther) {
      // TURN RIGHT SLIGHTLY
      rightSlightly(1);
    }
    else if (getRangeFrontLow() >= minWalkable) {
      // FORWARD SLIGHTLY
      forwardSlightly(1);
    }
    else {
      digitalWrite(LEDPin, HIGH);
      // LET THE SENSOR FACE THE WALL
      right90(1);
      digitalWrite(LEDPin, LOW);
    }
  }
  else { // something's wrong
    Serial.print("ERROR: loop(): There is no such side > ");
    Serial.println(side);
    while (true);
  }
  tick++;
}

int hzPosBad(float range) {
  //return 0;
  if (range <= maxTooClose) {
    return -1;
  }
  else if (range >= minTooFar) {
    return 1;
  }
  else {
    return 0;
  }
}
