// Maze Traversing Module for Firex by Robin_D on 4.8.19
// Configured for TRINITY COLLEGE INTERNATIONAL FIRE FIGHTING ROBOT CONTEST 2019
// Robot: Firex by George School
// Main Frame for the Robot

/*
  CALIBRATES EVERY TIME BEFORE COMPETE!!!!!!!!!1
*/

#define MAX 9999

////// PINS //////
const byte leftIRLineSensorPin = A9;
const byte rightIRLineSensorPin = A8;
const byte lowIRSensorPin = A6;           //For both Robin and Andrew's code
const byte highIRSensorPin = A6;          //Missing in pinMode, what is the use of this?

const int valvePin = A2;                  //the versa valve pin

const byte rightinfaredsensor = A5;       //For Candle Detection
const byte leftinfaredsensor = A4;        //For Candle Detection
const byte midinfaredsensor = A3;         //For Candle Detection

//const int AUDIO_INPUT_PIN = A19;          // Input ADC pin for audio data.  Connect OUT pin of mic to any teensy analog pin
// and change AUDIO_INPUT_PIN to that pin

const int LEDPin = 13;                    //Onboard LED Pin
const int leftServoPin = 11;
const int rightServoPin = 12;

const byte rightFrontEchoPin = 6;
const byte rightFrontTrigPin = 5;
const byte rightRearEchoPin = 10;
const byte rightRearTrigPin = 9;
const byte leftFrontEchoPin = 4;
const byte leftFrontTrigPin = 3;
const byte leftRearEchoPin = 8;
const byte leftRearTrigPin = 7;

const int clawServoPin = 24;               // servo pin for LEFT wheel (THIS IS CHANGED TO 14) (USING BIGGER BOARDS)

const int REDPin = 0;                      //LED Breaker Board
const int BLUEPin = 2;                     //LED Breaker Board
const int GREENPin = 1;

const int photoresist= A15;
//////////////////

/////// Robin's Constant and Variables //////////
char side = 'R';                          // focusing side of wall following

const float minFar = 40;                  // minimum distance from sensor that would be considered far

const float minFurther = 0.75;            // minimum distance difference between two sensors that would make the robot readjust itself
const float minVeryFurther = 3;
const float minWalkable = 15;             // minimum distance needed in front of the bot for it to walk forward (wall detection)
const float minDiffIsDog = -8.2;          // range of distance difference between two high-low front sensors to mark the obstacle as the dog
const float maxDiffIsDog = 0.3;           // these are calibrated, for robin's maze follower prototype
const int minTickRoomEnabled = 50;        
const int minLeftRearOKRoom4 = 30;

const float minTooFar = 25;               // (UNUSED) minimum distance difference between two sensors that would make the robot comes closer to the wall
const float maxTooClose = 5;              // (UNUSED) minimum distance difference between two sensors that would make the robot moves away from the wall

const int roomEnterSteps = 85;            // steps the bot should take after entering/exiting the room
const int roomEndSteps = 70;              // steps the bot should take to back into the circle
const int roomExitSteps = 20;

const float stepsPerCm = 100.0 / 323.5;    // 100 steps is 62.5 cm
const float stepsPerCmH = 100 / 137;
const float doorWidth = 46;
const float botWidth = 18;
//const float preferredDistance = 4;

long long tick;
float lastSense = 15;                     // last sensed distance to the following wall
int lastRoomTick = -MAX;
int roomEntered;
int turnsAfterRoom3;
bool changeYet = false;
bool foundDog = false;
bool fireExtinguished = false;
int roomsAfterFire;
int turnsAfterFire;
//long long banTick;
char track[1005];
int trackSize;
bool goingBack;
bool stillInRoom;
int roomWithFire;
int turnsAfterExit;
bool dumb;
int turns;
int fireDeg;
bool changeToLeftAfterExit;
int turnsAfterExtinguished;

/////////////////////////////////////////


///////Sam's Constant and Variables//////////

const int clawPWOpen = 1400;    //sets the pulse width for opening the claw
const int clawPWClose = 2000;   //sets the pulse width for closing the claw
int i;    //a counting for loop integer
const int call = 0;  //the beginning number for the for loop of the counting cycle
int callcounter;    //ammount of times that the robot goes through one cycle
int numOpenSteps;   //number of steps to open claw
int numCloseSteps;    //number of steps to close claw

///////////////////////////////////////////////////////////

///////Ian's Constant and Variables//////////

//const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
//const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
//int SAMPLE_RATE_HZ = 9000;
//float TONE_THRESHOLD_DB = 45.0;        // Threshold (in decibels) each tone must be above other frequencies to count.
//const int FFT_SIZE = 256;              // Size of the FFT.  Realistically can only be at most 256
// without running out of memory for buffers and other state.


///////////////////////////////////////////////////////////

///////Andrew's Constant and Variables//////////

int rightIRvalue;
int leftIRvalue;
int midIRvalue;
int botIRvalue;
int rightline;
int leftline;
const int steps360 = 310;
const int steps180 = steps360/2;

boolean IfFire = false;

////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);                     // Open the serial port
  delay(100);

  ////pinMode OUTPUT////
  pinMode(LEDPin, OUTPUT);                // onboard LED

  pinMode(REDPin, OUTPUT);
  pinMode(BLUEPin, OUTPUT);
  pinMode (GREENPin, OUTPUT);             //TURN ON TO DETECT BABY
  
//  pinMode(leftIRLineSensorPin, INPUT);
//  pinMode(rightIRLineSensorPin, INPUT);
  pinMode(lowIRSensorPin, INPUT);
  pinMode(highIRSensorPin, INPUT);

  pinMode(rightFrontTrigPin, OUTPUT);
  pinMode(rightFrontEchoPin, INPUT);
  pinMode(rightRearTrigPin, OUTPUT);
  pinMode(rightRearEchoPin, INPUT);
  pinMode(leftFrontTrigPin, OUTPUT);
  pinMode(leftFrontEchoPin, INPUT);
  pinMode(leftRearTrigPin, OUTPUT);
  pinMode(leftRearEchoPin, INPUT);

  pinMode(leftServoPin, OUTPUT);
  pinMode(rightServoPin, OUTPUT);


  pinMode(clawServoPin, OUTPUT);
  pinMode(valvePin, OUTPUT);

  pinMode (rightinfaredsensor, INPUT);
  pinMode (leftinfaredsensor, INPUT);
  pinMode (midinfaredsensor, INPUT);

  // Ian's Setup:
  setupMicrophone();  // from the microphone module

  //////////////////////////////////////////////////////////////

  blinkOK(3);                             // ok now
  microphone();                           // listen ot the microphone
  for (int i = 0; i < 10; i++) {          // prevent sensor's unfinished initialization
    getRangeRightFront();
    getRangeRightRear();
    delay(10);
  }
}

void loop() {
  //debugAllSensors();
  //printSensors();
  //debugKeepMoving();
  //debugCheckSensors();
  //debugCheckIRLine();
  //debugInRoom();
  //debugDetectDog();
  //debugMisc();

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
    if (!isFarIR(rangeFrontLow) && rangeFrontLow < minWalkable) {
      left90(1);
    }

    // IF SIDE-FRONT SENSOR IS FAR
    else if (isFar(rangeRightFront)) {
      // SHARP 90 DEG TURN
      right90Ex(1,0);      // includes stick
    }


    // IF SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    else if (!isFar(rangeRightRear) && rangeRightFront - rangeRightRear >= minFurther) {  // since this if(), rangeRightFront is conditionally guaranteed not far
      // TURN RIGHT SLIGHTLY
      rightSlightly(1);
      forwardSlightly(3 - (rangeRightFront - rangeRightRear >= minVeryFurther) * 2);
    }

    // IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
    else if (rangeRightRear - rangeRightFront >= minFurther) {                            // rangeRightRear must not too far too (auto detect)
      // TURN LEFT SLIGHTLY
      leftSlightly(1);
      forwardSlightly(1);
    }
    // IT CAN NOW MOVES FORWARD
    else {
      // FORWARD SLIGHTLY
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
  int steps = 0;
  while(true){
    for(int i = 0; i < 150; i++){
      forwardFast(1);
      steps++;
    }
    right90(1);
    robotStop(20);
    left90(1);
  }
}
void debugCheckSensors() {
  while (true) {
    float rangeLeftFront = 0;
    float rangeLeftRear = 0;
    float rangeFrontLow = 0;
    float rangeFrontHigh = 0;
    float s1, s2, s3;
    s1 = getRangeLeftFront();
    s2 = getRangeLeftFront();
    s3 = getRangeLeftFront();
    rangeLeftFront = selectRange(s1, s2, s3);
    s1 = getRangeLeftRear();
    s2 = getRangeLeftRear();
    s3 = getRangeLeftRear();
    rangeLeftRear = selectRange(s1, s2, s3);
    s1 = getRangeFrontLow();
    s2 = getRangeFrontLow();
    s3 = getRangeFrontLow();
    rangeFrontLow = selectRange(s1, s2, s3);
    s1 = getRangeFrontHigh();
    s2 = getRangeFrontHigh();
    s3 = getRangeFrontHigh();
    rangeFrontHigh = selectRange(s1, s2, s3);
    Serial.println("Side > " + String(rangeLeftFront) + ", " + String(rangeLeftRear) + ", Diff = " + String(rangeLeftFront - rangeLeftRear));
    //Serial.println("Front > " + String(rangeFrontLow) + ", " + String(rangeFrontHigh) + ", Diff = " + String(rangeFrontLow - rangeFrontHigh));
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

void getOutOffRoom() {
  goingBack = true;
  right90(1);
  robotStop(1);
  right90(1);
  /*for(int i = trackSize-1; i >= 0; i--){
    checkfire();
    checkline();
    if(track[i] == 'F'){
      forwardStepFast();
      movingtoward();
    }
    else if(track[i] == 'L'){
      //forwardStepSlow();
      rightStepSlow();
    }
    else{
      //forwardStepSlow();
      leftSlow(1);
    }
    }*/
  while (!detectLine()) {
    forwardSlightly(1);
  }
  alignBot();
  goingBack = false;
}

/*void debugAllSensors() {
  while (true) {
    Serial.println("leftIRLineSensorPin > " + String(analogRead(leftIRLineSensorPin)));
    Serial.println("rightIRLineSensorPin > " + String(analogRead(rightIRLineSensorPin)));
    Serial.println("lowIRSensorPin > " + String(analogRead(lowIRSensorPin)));
    Serial.println("highIRSensorPin > " + String(analogRead(highIRSensorPin)));

    Serial.println("rightinfraredsensor > " + String(analogRead(rightinfaredsensor)));
    Serial.println("leftinfraredsensor > " + String(analogRead(leftinfaredsensor)));
    Serial.println("midinfraredsensor > " + String(analogRead(midinfaredsensor)));
    delay(50);
  }
}
*/
/*void debugInRoom() {                        // rightStepSlow = leftSlow(1), calibrated for approx. equal walk
  for (i = 1; i <= 500; i++) {
    checkfire();
    checkline();
    if (i % 75 == 0) {
      rightStepSlow();
      track[trackSize++] = 'R';
    }
    /*else if(i%15 == 0){
      leftSlow(1);
      track[trackSize++] = 'L';
      }
    else {
      forwardStepFast();
      movingtoward();
      track[trackSize++] = 'F';
    }
  }
  getOutOffRoom();
  while (true);
  }*/
void debugMisc(){
  while(true){
    for (int i = 0; i < 100; i++) {
      if (detectLine()) {                     // this got 100 / 62.5
        return;
      }
      /*if (detectLine() || (!isFarIR(getRangeFrontLow()) && getRangeFrontLow() < minWalkable)) {     // this 100 / 137
        Serial.println("555");
      }
      forwardFast(1);
      float s1 = getRangeLeftFront();
      float s2 = getRangeLeftFront();
      float s3 = getRangeLeftFront();
      float rangeLeftFront = selectRange(s1, s2, s3);*/
      forwardFast(1);
    }
    robotStop(30);
  }
}
