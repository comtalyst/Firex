//const byte rightlinefollow = A3;
//const byte leftlinefollow = A4;
//const byte frontIRPin = A2;
//const byte rightinfaredsensor = A5;
//const byte leftinfaredsensor = A6;
//const byte midinfaredsensor = A7;
//const int REDPin = 0;
//const int BLUEPin = 2;
//const int LEDPin = 13;
//const int leftServoPin = 12;               // servo pin for LEFT wheel
//const int rightServoPin = 11;              // servo pin for RIGHT wheel

//int rightIRvalue;
//int leftIRvalue;
//int midIRvalue;
//int rightline;
//int leftline;

//boolean IfFire = false;

/*void setup() {
  pinMode (rightinfaredsensor, INPUT);
  pinMode (leftinfaredsensor, INPUT);
  pinMode (midinfaredsensor, INPUT);
  pinMode (rightlinefollow, INPUT);
  pinMode (leftlinefollow, INPUT);
  pinMode (frontIRPin, INPUT);
  pinMode (REDPin, OUTPUT);
  pinMode (BLUEPin, OUTPUT);
  pinMode (LEDPin, OUTPUT);
  pinMode (leftServoPin, OUTPUT);
  pinMode (rightServoPin, OUTPUT);
  blinkOK(3);                               //In the servo module
  candledetected();                         //Testing Red LED Pin
  forwardSlow (200);                         //Taking 200 steps forward to ensure that the largest room detects  IMPORTANT: This has to correlate with the distance going back

  //  rightSlow (379);                          //Going Around 360 Degrees
}
*/

/*void loop() {
  //  printSensors();
  senseandmove();
}*/

//Sam's code

//const int clawPWOpen = 1400;    //sets the pulse width for opening the claw
//const int clawPWClose = 2000;   //sets the pulse width for closing the claw
//int i;    //a counting for loop integer
//const int call = 0;  //the beginning number for the for loop of the counting cycle
//int callcounter;    //ammount of times that the robot goes through one cycle
//const int clawServoPin = 10;               // servo pin for LEFT wheel
////const int LEDPin = 13;    \                // the onboard LED pin (for diagnostics)
//const int valvePin = A1;        //the versa valve pin
//int numOpenSteps;   //number of steps to open claw
//int numCloseSteps;    //number of steps to close claw


//My code
//int rightIRvalue;
//int leftIRvalue;
//int midIRvalue;
//int rightline;
//int leftline;
//int botIRvalue;
//
//boolean IfFire = false;

/*void setup() {
  pinMode (rightinfaredsensor, INPUT);
  pinMode (leftinfaredsensor, INPUT);
  pinMode (midinfaredsensor, INPUT);
  pinMode (rightlinefollow, INPUT);
  pinMode (leftlinefollow, INPUT);
  pinMode (frontIRPin, INPUT);
  pinMode (REDPin, OUTPUT);
  pinMode (BLUEPin, OUTPUT);
  pinMode (LEDPin, OUTPUT);
  pinMode (leftServoPin, OUTPUT);
  pinMode (rightServoPin, OUTPUT);

  //Sams pin
  pinMode(clawServoPin, OUTPUT);
  pinMode(valvePin, OUTPUT);

  blinkOK(3);                               //In the servo module
  //  candledetected();                         //Testing Red LED Pin
  //    rightSlow (379);                       //Going Around 360 Degrees
  detectionStory();
  //release();
}

void loop() {
  //  printSensors();
  //  release();
}*/

void detectionStory() {
  gointotheroom();
}

void gointotheroom() {
  turning360();
}

void printSensors() {
  rightIRvalue = analogRead (rightinfaredsensor);
  leftIRvalue = analogRead (leftinfaredsensor);
  midIRvalue = analogRead (midinfaredsensor);
  rightline = analogRead (rightIRLineSensorPin);
  leftline = analogRead (leftIRLineSensorPin);
  Serial.println ("Left Sensor Value = " + String (leftIRvalue) + " Middle Sensor Value = " + String (midIRvalue) + " Right Sensor Value = " + String (rightIRvalue) + " Left Line Value = " + String (leftline) + " Right Line Value = " + String (rightline));
}

void checkfire() {
  rightIRvalue = analogRead (rightinfaredsensor);
  leftIRvalue = analogRead (leftinfaredsensor);
  midIRvalue = analogRead (midinfaredsensor);
  botIRvalue = analogRead (lowIRSensorPin);

}

void checkline() {
  rightline = analogRead (rightIRLineSensorPin);
  leftline = analogRead (leftIRLineSensorPin);
}

void turning360 () {
  int x = 0;
  for (x; x < 379; x++) {
    checkfire();
    if (rightIRvalue < 1023 || leftIRvalue < 1023 || midIRvalue < 1023) {
      IfFire = true;
      break;
    }
    rightStepSlow();
  }
  if (IfFire == true) {
    candleLEDon();
    movingtowardcandle();                            //Calling to the candle moving module
  }
  else {
    //    delay (500);
  }
}

void movingtowardcandle() {
  while (true) {
    checkfire();
    float range_cm = IRRange (botIRvalue);
    //  checkline();  // Not needed (no Circle Detection)

    if (midIRvalue < rightIRvalue && midIRvalue < leftIRvalue) {
      if (range_cm <= 8.00) {
        break;
      }
      else {
        forwardStepSlow();
      }
    }
    else if (midIRvalue > rightIRvalue) {
      rightStepSlow();
    }
    else if (midIRvalue > leftIRvalue) {
      leftStepSlow();
    }
  }
  // After stopping in front of the candle, start extinguishing
  stopStep();                     //Stop the robot from moving
  tonsofCO2();                    //Calling Sam's code to extinguish without step
  turning360version2();           //Double check fire, and turning 360 degrees
}

void turning360version2() {
  // Check if fire is out, if not, RamboRelease, if so, return home.
  while (true) {
    reverseFast(50);                //Moving back, ready for RamboRelease();
    IfFire = false;
    int x = 0;
    for (x; x < 758; x++) {
      checkfire();
      if (rightIRvalue < 1023 || leftIRvalue < 1023 || midIRvalue < 1023) {
        IfFire = true;
        break;
      }
      rightStepSlow();
    }
    if (IfFire == true) {
      candleLEDon();
      movingtowardcandleversion2();                            //Calling to the candle moving module
    }
    else {
      //    delay (500);
      candleLEDoff();
      fireExtinguished = true;
    }
  }
}

void movingtowardcandleversion2() {  //Moving toward candle, but RamboRelease
  while (true) {
    checkfire();
    float range_cm = IRRange (botIRvalue);
    //  checkline();  // Not needed (no Circle Detection)

    if (midIRvalue < rightIRvalue && midIRvalue < leftIRvalue) {
      if (range_cm <= 8.00) {
        break;
      }
      else {
        forwardStepSlow();
      }
    }
    else if (midIRvalue > rightIRvalue) {
      rightStepSlow();
    }
    else if (midIRvalue > leftIRvalue) {
      leftStepSlow();
    }
  }
  // After stopping in front of the candle, start extinguishing
  stopStep();                     //Stop the robot from moving
  RamboRelease();                    //Calling Sam's code to extinguish without step
  turning360version2();           //Double check fire, and turning 360 degrees
}

/*void stopatcandle() {

  if (midIRvalue < 200 && range_cm <= 8.0) {
    stopStep();
    digitalWrite (BLUEPin, LOW);
    candledetected();
    samcode();
  }
  turning360ver2();
  }

  void samcode() {
  tonsofCO2();
  }*/

void robincodegotodoor() {
  while (true);
}

void candleLEDon() {                         //Turn on LED once candle detected
  digitalWrite(REDPin, HIGH);
}

void candleLEDoff() {
  digitalWrite (REDPin, LOW);                //Turn off LED once candle extinguished
}

float IRRange (int IRValue) {
  float range = pow ( (4414.8 / float (IRValue) ), 1.220);
  return range;
}



/*Extra Codes (Just in case) */


/*  while (rightIRvalue == 1023 || leftIRvalue == 1023) {
    checkfire();
    rightStepSlow();
    Serial.println ("No flame detected");
  }
  if (rightIRvalue < 700) {
    leftSlow (30);
    Serial.println ("Flame detected (Right side)");
  }
  else if (leftIRvalue < 700) {
    rightSlow (30);
    Serial.println ("Flame detected (Left side)");
  }
  else if (leftIRvalue < 700 && rightIRvalue < 700) {
    startmoving();
    Serial.println ("Moving toward candle");
  }
  else {
    rightFast(100);
    //Robin's maze code here
  }
  }*/
