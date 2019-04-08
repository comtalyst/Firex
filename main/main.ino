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

const int sensorAvgTimes = 3;
const float minFurther = 0.75;        // minimum distance difference between two sensors that would make the robot readjust itself
const float minSuperFurther = MAX*2;   // minimum distance difference between two sensors that would trigger isFar()
const float minTooFar = 25;        // minimum distance difference between two sensors that would make the robot comes closer to the wall
const float maxTooClose = 5;       // minimum distance difference between two sensors that would make the robot moves away from the wall
const float minWalkable = 15;      // minimum distance needed in front of the bot for it to walk forward (wall detection)
const float maxDiffConsider = MAX*2;  // maximum distance difference of the side front sensor between a step that considered not anomaly (anomaly won't cause balance)

const int roomEnterSteps = 10;     // steps the bot should take after entering/exiting the room
const float stepsPerCm = 100.0 / 32.5;    // 100 steps is 32.5 cm

const float doorWidth = 50;
const float botWidth = 18;

int tick;
int lastBalance;                   // last tick that the bot balance itself (for calculations)
float lastSense = 15;              // last sensed distance to the following wall
int lastLineDetect = -MAX;         // last tick that the bot detect the line (for if only)
int lastTwist = -MAX;              // last tick that the bot undergo big moves (90 turns or exit the room), use to ignore the minSuperFurther stuff
float lastActualSense = 0.0;

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

  if(isFar(getRangeRightFront())){
    right90(1);
  }
}

void loop() {
  //debugKeepMoving();
  //debugCheckSensors();
  //debugCheckIRLine();

  if (side == 'R') {
    float rangeRightFront = 0;
    float rangeRightRear = 0;
    float rangeFrontLow = 0;
    float s1, s2, s3;
    /*for(int i = 0; i < sensorAvgTimes; i++){
      rangeRightFront += getRangeRightFront();
      rangeRightRear += getRangeRightRear();
      rangeFrontLow += getRangeFrontLow();
    }
    rangeRightFront /= sensorAvgTimes;
    rangeRightRear /= sensorAvgTimes;
    rangeFrontLow /= sensorAvgTimes;*/
    s1 = getRangeRightFront();
    s2 = getRangeRightFront();
    s3 = getRangeRightFront();
    rangeRightFront = selectRange(s1, s2, s3);
    s1 = getRangeRightRear();
    s2 = getRangeRightRear();
    s3 = getRangeRightRear();
    rangeRightRear = selectRange(s1, s2, s3);
    s1 = getRangeFrontLow();
    s2 = getRangeFrontLow();
    s3 = getRangeFrontLow();
    rangeFrontLow = selectRange(s1, s2, s3);
    
    bool turnOK = false;

    // if it's currently straight (already sufficient from switching to sharp turn) and not far --> update last sensed distance
    if (!isFar(rangeRightFront) && hzPosBad(rangeRightFront) == 0) {
      lastSense = rangeRightFront;
      if(lastActualSense == 0.0 || rangeRightFront - lastActualSense <= maxDiffConsider){
        lastActualSense = rangeRightFront;
        turnOK = true;
      }
    }

    // if bot detect the opened door
    if (tick - lastLineDetect > roomEnterSteps * 2 && detectLine()) {
      digitalWrite(LEDPin, HIGH);
      alignBot();
      forwardFast(roomEnterSteps);

      // THESE ARE WIP
      // the bot is in the room and facing forward at this point
      robotStop(20);
      // search for candle
      right90(2);
      // the bot has to face the exit at this point

      //forwardFast(roomEnterSteps);
      lastBalance = tick;
      lastSense = doorWidth - (lastSense + botWidth);
      lastLineDetect = tick;
      lastTwist = tick;
      lastActualSense = 0.0;
      digitalWrite(LEDPin, LOW);
    }
    // IF SIDE-FRONT SENSOR IS FAR
    else if (isFar(rangeRightFront) || (lastTwist - tick > 1 && !isFar(rangeRightRear) && rangeRightFront - rangeRightRear >= minSuperFurther)) {
      // SHARP 90 DEG TURN
      bool isLine = right90Ex(1);
      lastBalance = tick;
      lastTwist = tick;
      lastActualSense = 0.0;
    }
    // IF TOO FAR OR TOO CLOSE
    else if (hzPosBad(rangeRightFront)){
      if(hzPosBad(rangeRightFront) == 1){
        right90(1);
        forwardFast((int)(stepsPerCm*(rangeRightFront-(minTooFar-maxTooClose)/2.0)));
        left90(1);
      }
      else{
        left90(1);
        forwardFast((int)(stepsPerCm*((minTooFar-maxTooClose)/2.0-rangeRightFront)));
        right90(1);
      }
      lastTwist = tick;
      lastActualSense = 0.0;
      lastBalance = tick;
    }
    // IF TOO FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    else if (turnOK && !isFar(rangeRightRear) && rangeRightFront - rangeRightRear >= minFurther) { // since this if(), rangeRightFront is conditionally guaranteed not far
      // TURN RIGHT SLIGHTLY
      rightSlightly(1);
      forwardSlightly(1);
      /*if (lastBalance == tick - 1) {    // no need for advanced adjustment
        rightSlightly(1);
      }
      else {
        rightSlightly(2);
        for (int i = 0; i < tick - lastBalance; i++) {
          forwardSlightly(1);
          if (detectLine() || (isFar(rangeRightFront) || (!isFar(rangeRightRear) && rangeRightFront - rangeRightRear >= minSuperFurther)) || rangeFrontLow < minWalkable) {
            break;
          }
        }
        leftSlightly(1);
      }*/
      lastBalance = tick;
      lastActualSense = 0.0;
    }
    // IF TOO CLOSE OR SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
    else if (hzPosBad(rangeRightFront) == -1 || rangeRightRear - rangeRightFront >= minFurther) {     // rangeRightRear must not too far too (auto detect)
      // TURN LEFT SLIGHTLY
      leftSlightly(1);
      forwardSlightly(1);
      /*if (lastBalance == tick - 1) {
        leftSlightly(1);
      }
      else {
        leftSlightly(2);
        for (int i = 0; i < tick - lastBalance; i++) {
          forwardSlightly(1);
          if (detectLine() || (isFar(rangeRightFront) || (isFar(rangeRightRear) && rangeRightFront - rangeRightRear >= minSuperFurther)) || rangeFrontLow < minWalkable) {
            break;
          }
        }
        rightSlightly(1);
      }*/
      lastBalance = tick;
      lastActualSense = 0.0;
    }
    // CAN IT NOW MOVES FORWARD?
    else if (isFarIR(rangeFrontLow) || rangeFrontLow >= minWalkable) {
      // FORWARD SLIGHTLY
      forwardSlightly(1);
    }
    else {
      // LET THE SENSOR FACE THE WALL
      left90(1);
      lastTwist = tick;
      lastActualSense = 0.0;
    }
  }
  tick++;
}

int hzPosBad(float range) {                   // this will be triggered at the corner (unreporteds)
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
}

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
    for(int i = 0; i < sensorAvgTimes; i++){
      rangeRightFront += getRangeRightFront();
      rangeRightRear += getRangeRightRear();
      rangeFrontLow += getRangeFrontLow();
    }
    rangeRightFront /= sensorAvgTimes;
    rangeRightRear /= sensorAvgTimes;
    rangeFrontLow /= sensorAvgTimes;
    Serial.println(String(rangeRightFront) + ", " + String(rangeRightRear) + ", Diff = " + String(rangeRightFront-rangeRightRear));
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
float selectRange(float s1, float s2, float s3){
    if(s3 < s2){
      float tmp = s2;
      s2 = s3;
      s3 = tmp;
    }
    if(s2 < s1){
      float tmp = s1;
      s1 = s2;
      s2 = tmp;
    }
    if(s3 < s2){
      float tmp = s2;
      s2 = s3;
      s3 = tmp;
    }
    if(s2-s1 < s3-s2){
      return s1;
    }
    else{
      return s3;
    }
}
