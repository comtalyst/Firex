// Firex

#define MAX 9999

const int LEDPin = 13;

char side = 'R';            // focusing side of wall following

const float minFurther = 1;        // minimum distance difference between two sensors that would make the robot readjust itself
const float minFar = 20;            // minimum distance from sensor that would be considered far
const float minWalkable = 0;       // minimum distance needed in front of the bot for it to walk forward

int tick;
int lastBalance;

void setup() {
  Serial.begin(9600);                     // Open the serial port
  delay(100);
  pinMode(LEDPin, OUTPUT);                // onboard LED
  readyServo();
  readySonic();
  // readyIR();
  blinkOK(3);                             // ok now
}

void loop() {
  if(side == 'R'){      // TOMORROW --> MIMIC TO THE LEFT
    float rangeRightFront = getRangeRightFront();
    float rangeRightRear = getRangeRightRear();
    // IF SIDE-FRONT SENSOR IS FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    if(isFar(rangeRightFront)){
      digitalWrite(LEDPin, HIGH);
      rightSlightly();
      digitalWrite(LEDPin, LOW);
      lastBalance = tick;
    }
    else if(rangeRightFront - (isFar(rangeRightRear)? MAX:rangeRightRear) >= minFurther){
      // TURN RIGHT SLIGHTLY
      rightSlightly();
      rightSlightly();
      for(int i = 0; i < tick-lastBalance; i++){
        forwardSlightly();
      }
      rightSlightly();
      lastBalance = tick;
    }
    // IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
    else if(rangeRightRear - rangeRightFront >= minFurther){
      // TURN LEFT SLIGHTLY
      leftSlightly();
      lastBalance = tick;
    }
    else if(getRangeFrontLow() >= minWalkable){
      // FORWARD SLIGHTLY
      forwardSlightly();
    }
    else{
      // TURN RIGHT 90
      right90();
    }
  }
  else if(side == 'L'){
    // IF SIDE-FRONT SENSOR IS FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
    if(isFar(getRangeLeftFront()) || getRangeLeftFront() - getRangeLeftRear() >= minFurther){
      // TURN LEFT SLIGHTLY
      leftSlightly();
    }
    // IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
    else if(getRangeLeftRear() - getRangeLeftFront() >= minFurther){
      // TURN RIGHT SLIGHTLY
      rightSlightly();
    }
    else if(getRangeFrontLow() >= minWalkable){
      // FORWARD SLIGHTLY
      forwardSlightly();
    }
    else{
      // TURN LEFT 90
      left90();
    }
  }
  else{ // something's wrong
    Serial.print("ERROR: loop(): There is no such side > ");
    Serial.println(side);
    while(true);
  }
  tick++;
}
