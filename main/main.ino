// Maze Traversing Module for Firex by Robin_D on 4.8.19
// Configured for TRINITY COLLEGE INTERNATIONAL FIRE FIGHTING ROBOT CONTEST 2019
// Robot: Firex by George School

/*
after complete 3rd room and 2 90s --> switch following side --> room 1 doors will be out of concern
if meet dog --> change side then face it like the wall (as planned)
 */

#define MAX 9999

////// PINS //////
const int LEDPin = 13;
//////////////////

char side = 'R';                          // focusing side of wall following

const float minFurther = 0.75;            // minimum distance difference between two sensors that would make the robot readjust itself
const float minWalkable = 15;             // minimum distance needed in front of the bot for it to walk forward (wall detection)
const float minDiffIsDog = -8.2;          // range of distance difference between two high-low front sensors to mark the obstacle as the dog
const float maxDiffIsDog = -4.4;          // these are calibrated, for robin's maze follower prototype

const float minTooFar = 25;               // (UNUSED) minimum distance difference between two sensors that would make the robot comes closer to the wall
const float maxTooClose = 5;              // (UNUSED) minimum distance difference between two sensors that would make the robot moves away from the wall

const int roomEnterSteps = 60;            // steps the bot should take after entering/exiting the room

const float stepsPerCm = 100.0 / 32.5;    // 100 steps is 32.5 cm
const float doorWidth = 50;
const float botWidth = 18;

int tick;
float lastSense = 15;                     // last sensed distance to the following wall
int lastRoomTick = -MAX;
int roomEntered;
int turnsAfterRoom3;

void setup() {
  Serial.begin(9600);                     // Open the serial port
  delay(100);
  pinMode(LEDPin, OUTPUT);                // onboard LED
  readyServo();
  readySonic();
  readyIRL();
  readyIR();
  // MICROPHONE LISTENER HERE (IAN'S)
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

  if(roomEntered >= 3 && turnsAfterRoom3 >= 2){
    turnsAfterRoom3 = -MAX;
    side = 'L';
  }
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

      // THESE ARE WIP (ANDREW'S)
      // the bot is in the room and facing forward at this point
      robotStop(20);
      // search for candle
      right90(2);
      // the bot has to face the exit at this point

      forwardFast(roomEnterSteps);
      lastSense = doorWidth - (lastSense + botWidth);
      digitalWrite(LEDPin, LOW);
      lastRoomTick = tick;
      roomEntered++;
    }

    // IF DETECTS THE WALL
    else if (!isFarIR(rangeFrontLow) && rangeFrontLow < minWalkable){
      // IS THIS THE DOG?
      s1 = getRangeFrontHigh();
      s2 = getRangeFrontHigh();
      s3 = getRangeFrontHigh();
      float rangeFrontHigh = selectRange(s1, s2, s3);
      if(rangeFrontLow - rangeFrontHigh < minDiffIsDog || rangeFrontLow - rangeFrontHigh > maxDiffIsDog){
        side = 'L';
        if(isFar(rangeRightRear)){
          right90(1);
        }
        right90(1);
      }
      // JUST A WALL, LET THE SENSOR FACE THE WALL
      else{
        left90(1);
      }
    }

    // IF SIDE-FRONT SENSOR IS FAR
    else if (isFar(rangeRightFront)) {
      // SHARP 90 DEG TURN
      right90Ex(1, (tick-lastRoomTick == 1 || isFar(rangeRightRear)));        // includes stick
      if(roomEntered >= 3){
        turnsAfterRoom3++;
      }
    }


    // IF SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    else if (!isFar(rangeRightRear) && rangeRightFront - rangeRightRear >= minFurther) {  // since this if(), rangeRightFront is conditionally guaranteed not far
      // TURN RIGHT SLIGHTLY
      rightSlightly(1);
      forwardSlightly(2);
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

      // THESE ARE WIP (ANDREW'S)
      robotStop(20);
      // search for candle
      left90(2);

      forwardFast(roomEnterSteps);
      lastSense = doorWidth - (lastSense + botWidth);
      digitalWrite(LEDPin, LOW);
      lastRoomTick = tick;
      roomEntered++;
    }

    else if (!isFarIR(rangeFrontLow) && rangeFrontLow < minWalkable){
      s1 = getRangeFrontHigh();
      s2 = getRangeFrontHigh();
      s3 = getRangeFrontHigh();
      float rangeFrontHigh = selectRange(s1, s2, s3);
      if(rangeFrontLow - rangeFrontHigh < minDiffIsDog || rangeFrontLow - rangeFrontHigh > maxDiffIsDog){
        side = 'R';
        if(isFar(rangeLeftRear)){
          left90(1);
        }
        left90(1);
      }
      else{
        right90(1);
      }
    }

    else if (isFar(rangeLeftFront)) {
      left90Ex(1, (tick-lastRoomTick == 1 || isFar(rangeLeftRear)));
    }

    else if (!isFar(rangeLeftRear) && rangeLeftFront - rangeLeftRear >= minFurther) {
      leftSlightly(1);
      forwardSlightly(2);
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
    float rangeFrontHigh = 0;
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
    s1 = getRangeFrontHigh();
    s2 = getRangeFrontHigh();
    s3 = getRangeFrontHigh();
    rangeFrontHigh = selectRange(s1, s2, s3);
    //Serial.println("Side > " + String(rangeRightFront) + ", " + String(rangeRightRear) + ", Diff = " + String(rangeRightFront - rangeRightRear));
    Serial.println("Front > " + String(rangeFrontLow) + ", " + String(rangeFrontHigh) + ", Diff = " + String(rangeFrontLow - rangeFrontHigh));
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
