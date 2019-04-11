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

void printSensors() {
  rightIRvalue = analogRead (rightinfaredsensor);
  leftIRvalue = analogRead (leftinfaredsensor);
  midIRvalue = analogRead (midinfaredsensor);
  rightline = analogRead (rightIRLineSensorPin);         //Modified to Robin's code
  leftline = analogRead (leftIRLineSensorPin);           //Modified to Robin's code
  Serial.println ("Left Sensor Value = " + String (leftIRvalue) + " Middle Sensor Value = " + String (midIRvalue) + " Right Sensor Value = " + String (rightIRvalue) + " Left Line Value = " + String (leftline) + " Right Line Value = " + String (rightline));
}

void checkfire() {
  rightIRvalue = analogRead (rightinfaredsensor);
  leftIRvalue = analogRead (leftinfaredsensor);
  midIRvalue = analogRead (midinfaredsensor);
  botIRvalue = analogRead (lowIRSensorPin);               //Modified to Robin's code

}

void checkline() {
  rightline = analogRead (rightIRLineSensorPin);        //Modified to Robin's code
  leftline = analogRead (leftIRLineSensorPin);          //Modified to Robin's code
}

void senseandmove () {
  for (int x = 0; x < 379; x++) {
    checkfire();
    if (rightIRvalue < 1023 && leftIRvalue < 1023 && midIRvalue < 1023) {
      IfFire = true;
      digitalWrite (BLUEPin, HIGH);
      break;
    }
    rightStepSlow();
  }
  if (IfFire == true) {
    startmoving();                            //Calling to the start moving module
    while(true){
      robotStop(100);
    }
  }
  else {
    delay (500);
    rightSlow (194);                           //Turn 180 degrees   (Need change)
    forwardSlow (200);                         //Move back toward the line    (Need change)
    digitalWrite (BLUEPin, LOW);
    // return to maze following
  }
}

void startmoving() {
  checkfire();
  checkline();

  if (midIRvalue < rightIRvalue && midIRvalue < leftIRvalue) {
    forwardStepSlow();
    movingtoward();
  }
  else if (midIRvalue >= rightIRvalue) {
    rightStepSlow();
  }
  else if (midIRvalue >= leftIRvalue) {
    leftStepSlow();
  }
}


void movingtoward() {                                                 //Important! This is detecting BLACK TAPE, NOT WHITE TAPE (For Competition)
  checkfire();
  float range_cm = IRRange (botIRvalue);
  if (midIRvalue < 1023 && range_cm <= 10.0) {
    stopStep();
    digitalWrite (BLUEPin, LOW);
    candledetected();
    samcode();                                                        //Calling Sam's tonsofCO2
  }
}

void samcode() {
  tonsofCO2();
  while (true);                                                       //After extinguishes, robot stop
}

void robincode() {
  while (true);                                                      //What function to call here?
}
void candledetected() {                         //Blink Red LED
  digitalWrite(REDPin, HIGH);
  delay(500);
  digitalWrite(REDPin, LOW);
  delay(500);
}

float IRRange (int IRValue) {
  float range = pow ( (4414.8 / float (IRValue) ), 1.220);
  return range;
}





/*Backup Codes (Just in case) */


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
