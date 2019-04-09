// Maze Traversing Module for Firex by Robin_D on 4.8.19
// Configured for TRINITY COLLEGE INTERNATIONAL FIRE FIGHTING ROBOT CONTEST 2019
// Robot: Firex by George School

#define MAX 9999

////// PINS //////
const int LEDPin = 13;
//////////////////

char side = 'L';                          // focusing side of wall following

const float minFurther = 0.75;            // minimum distance difference between two sensors that would make the robot readjust itself
const float minWalkable = 15;             // minimum distance needed in front of the bot for it to walk forward (wall detection)


const float minTooFar = 25;               // (UNUSED) minimum distance difference between two sensors that would make the robot comes closer to the wall
const float maxTooClose = 5;              // (UNUSED) minimum distance difference between two sensors that would make the robot moves away from the wall

const int roomEnterSteps = 10;            // steps the bot should take after entering/exiting the room

const float stepsPerCm = 100.0 / 32.5;    // 100 steps is 32.5 cm
const float doorWidth = 50;
const float botWidth = 18;

int tick;
float lastSense = 15;                     // last sensed distance to the following wall
int lastRoomTick = -MAX;

void setup() {
  Serial.begin(9600);                     // Open the serial port
  delay(100);
  pinMode(LEDPin, OUTPUT);                // onboard LED
  readyServo();
  readySonic();
  readyIRL();
  readyIR();
  for (int i = 0; i < 10; i++) {          // prevent sensor's unfinished initialization
    getRangeRightFront();
    getRangeRightRear();
    delay(10);
  }
  blinkOK(3);                             // ok now

  if (isFar(getRangeRightFront())) {      // in case of unfortunate starting directions!
    right90(1);
  }
}

void loop() {
  //debugKeepMoving();
  //debugCheckSensors();
  //debugCheckIRLine();

  float rangeFrontLow = 0;
  float s1, s2, s3;
  s1 = getRangeFrontLow();
  s2 = getRangeFrontLow();
  s3 = getRangeFrontLow();
  rangeFrontLow = selectRange(s1, s2, s3);
  if (side == 'R') {
    float rangeRightFront = 0;
    float rangeRightRear = 0;  
    s1 = getRangeRightFront();
    s2 = getRangeRightFront();
    s3 = getRangeRightFront();
    rangeRightFront = selectRange(s1, s2, s3);
    s1 = getRangeRightRear();
    s2 = getRangeRightRear();
    s3 = getRangeRightRear();
    rangeRightRear = selectRange(s1, s2, s3);

    // if it's currently straight (already sufficient from switching to sharp turn) and not far --> update last sensed distance
    if (!isFar(rangeRightFront)) {
      // also needed to hzPosBad(rangeRightFront) == 0, but unused
      lastSense = rangeRightFront;
    }

    // if bot detect the opened door
    if (detectLine()) {
      digitalWrite(LEDPin, HIGH);
      alignBot();
      forwardFast(roomEnterSteps);

      // THESE ARE WIP
      // the bot is in the room and facing forward at this point
      robotStop(20);
      // search for candle
      right90(2);
      // the bot has to face the exit at this point

      forwardFast(roomEnterSteps);
      lastSense = doorWidth - (lastSense + botWidth);
      digitalWrite(LEDPin, LOW);
      lastRoomTick = tick;
    }

    // IF DETECTS THE WALL
    else if (!isFarIR(rangeFrontLow) && rangeFrontLow < minWalkable){
      // LET THE SENSOR FACE THE WALL
      left90(1);
    }

    // IF SIDE-FRONT SENSOR IS FAR
    else if (isFar(rangeRightFront)) {
      // SHARP 90 DEG TURN
      right90Ex(1, (tick-lastRoomTick == 1 || isFar(rangeRightRear)));
    }


    // IF SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    else if (!isFar(rangeRightRear) && rangeRightFront - rangeRightRear >= minFurther) {  // since this if(), rangeRightFront is conditionally guaranteed not far
      // TURN RIGHT SLIGHTLY
      rightSlightly(1);
      forwardSlightly(1);
    }
    
    // IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
    else if (rangeRightRear - rangeRightFront >= minFurther) {                            // rangeRightRear must not too far too (auto detect)
      // TURN LEFT SLIGHTLY
      leftSlightly(1);
      forwardSlightly(1);
    }

    /*// IF TOO FAR OR TOO CLOSE
    else if (hzPosBad(rangeRightFront)) {
      if (hzPosBad(rangeRightFront) == 1) {
        right90(1);
        forwardFast((int)(stepsPerCm * (rangeRightFront - (minTooFar - maxTooClose) / 2.0)));
        left90(1);
      }
      else {
        left90(1);
        forwardFast((int)(stepsPerCm * ((minTooFar - maxTooClose) / 2.0 - rangeRightFront)));
        right90(1);
      }
    }*/
    
    // IT CAN NOW MOVES FORWARD
    else {
      // FORWARD SLIGHTLY
      forwardSlightly(1);
    }
  }
  else if (side == 'L') {
    float rangeLeftFront = 0;
    float rangeLeftRear = 0;  
    s1 = getRangeLeftFront();
    s2 = getRangeLeftFront();
    s3 = getRangeLeftFront();
    rangeLeftFront = selectRange(s1, s2, s3);
    s1 = getRangeLeftRear();
    s2 = getRangeLeftRear();
    s3 = getRangeLeftRear();
    rangeLeftRear = selectRange(s1, s2, s3);

    if (!isFar(rangeLeftFront)) {
      lastSense = rangeLeftFront;
    }

    if (detectLine()) {
      digitalWrite(LEDPin, HIGH);
      alignBot();
      forwardFast(roomEnterSteps);

      // THESE ARE WIP
      robotStop(20);
      // search for candle
      left90(2);

      forwardFast(roomEnterSteps);
      lastSense = doorWidth - (lastSense + botWidth);
      digitalWrite(LEDPin, LOW);
      lastRoomTick = tick;
    }

    else if (!isFarIR(rangeFrontLow) && rangeFrontLow < minWalkable){
      right90(1);
    }

    else if (isFar(rangeLeftFront)) {
      left90Ex(1, (tick-lastRoomTick == 1 || isFar(rangeLeftRear)));
    }

    else if (!isFar(rangeLeftRear) && rangeLeftFront - rangeLeftRear >= minFurther) {
      leftSlightly(1);
      forwardSlightly(1);
    }
    
    else if (rangeLeftRear - rangeLeftFront >= minFurther) {
      rightSlightly(1);
      forwardSlightly(1);
    }
    
    else {
      forwardSlightly(1);
    }
  }
  tick++;
}


/*int hzPosBad(float range) {                   // this will be unexpectedly triggered at the corner (unreporteds)
  return 0;                                   
  if (range <= maxTooClose) {
    return -1;
  }
  else if (range >= minTooFar) {
    return 1;
  }
  else {
    return 0;
  }
}*/

void debugKeepMoving() {
  while (true) {
    forwardFast(1);
  }
}
void debugCheckSensors() {
  while (true) {
    float rangeRightFront = 0;
    float rangeRightRear = 0;
    float rangeFrontLow = 0;
    float s1, s2, s3;
    s1 = getRangeLeftFront();
    s2 = getRangeLeftFront();
    s3 = getRangeLeftFront();
    rangeRightFront = selectRange(s1, s2, s3);
    s1 = getRangeLeftRear();
    s2 = getRangeLeftRear();
    s3 = getRangeLeftRear();
    rangeRightRear = selectRange(s1, s2, s3);
    s1 = getRangeFrontLow();
    s2 = getRangeFrontLow();
    s3 = getRangeFrontLow();
    rangeFrontLow = selectRange(s1, s2, s3);
    Serial.println(String(rangeRightFront) + ", " + String(rangeRightRear) + ", Diff = " + String(rangeRightFront - rangeRightRear));
    //Serial.println(getRangeFrontLow());
    delay(200);
  }
}
void debugCheckIRLine() {
  while (true) {
    Serial.println(debugSense());
    delay(100);
  }
}
float selectRange(float s1, float s2, float s3) {
  if (s3 < s2) {
    float tmp = s2;
    s2 = s3;
    s3 = tmp;
  }
  if (s2 < s1) {
    float tmp = s1;
    s1 = s2;
    s2 = tmp;
  }
  if (s3 < s2) {
    float tmp = s2;
    s2 = s3;
    s3 = tmp;
  }
  if (s2 - s1 < s3 - s2) {
    return s1;
  }
  else {
    return s3;
  }
}
