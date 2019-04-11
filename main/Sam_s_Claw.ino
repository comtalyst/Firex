
//CO2 release code:

//GLOBALS:  
//const int clawPWOpen = 1400;    //sets the pulse width for opening the claw
//const int clawPWClose = 2000;   //sets the pulse width for closing the claw
//int i;    //a counting for loop integer
//const int call = 0;  //the beginning number for the for loop of the counting cycle
//int callcounter;    //ammount of times that the robot goes through one cycle
//const int clawServoPin = 10;               // servo pin for LEFT wheel
//const int LEDPin = 13;                    // the onboard LED pin (for diagnostics)
//const int valvePin = A2;        //the versa valve pin
//int numOpenSteps;   //number of steps to open claw
//int numCloseSteps;    //number of steps to close claw

//////////////////////////////////////////////////////////////////////////////////////

//CO2 cartridge #1 test: 1200 ms, test #2: 3200 ms
//Vin 


//////////////////////////////////////////////////////////////////////////////////////

/*void setup() {
  //sets the respective pins to the "OUTPUT" voltage mode:
  pinMode(clawServoPin, OUTPUT);
  pinMode(valvePin, OUTPUT);
  pinMode(LEDPin, OUTPUT);
  blinkOK(3);                // a diagnostic tool to let user know all is OK
}*/



/*void loop() {
  //   put your main code here, to run repeatedly:
  //  digitalWrite(A2, HIGH);
  //  delay(100);
  //  digitalWrite(A2, LOW);
  //  delay(100);

  //callClaw();
  //clawOpen(20);
  tonsofCO2();
  while (true);

}*/

//////////////////////////////////////////////////////////////////////////////////////

void valveOn() {
  //opens the valve:
  digitalWrite(valvePin, HIGH);

}

//////////////////////////////////////////////////////////////////////////////////////

void valveOff() {
  //closes the valve:
  digitalWrite(valvePin, LOW);
}

//////////////////////////////////////////////////////////////////////////////////////

void tonsofCO2() {

  numOpenSteps = 20;
  numCloseSteps = 40;
  
  //opens the claw by the input number of steps:
  clawOpen(numOpenSteps);
  //opens up the valve:
  valveOn();
  //delays by 200 ms
  delay(200);
  //closes the claw for 40 steps
  clawClose(numCloseSteps);
  clawOpen(numOpenSteps);
  //closes the valve:
  valveOff();
 

}

//////////////////////////////////////////////////////////////////////////////////////

void clawOpen(int x) {
  //opens the claw by the input number of steps:
  for (int i = 0; i <= x ; i++) {
    clawOpenStep();
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void clawClose(int x) {
  //closes the claw by the input number of steps:
  for (int i = 0; i <= x ; i++) {
    clawCloseStep();
  }
}

//////////////////////////////////////////////////////////////////////////////////////

void clawCloseStep() {
  //creates one pulse or "step" that closes the claw:
  createClawPulse(clawServoPin, clawPWClose);
  delay(20);      // do NOT alter this delay time!
}

//////////////////////////////////////////////////////////////////////////////////////

void clawOpenStep() {
  //creates one pulse or "step" that opens the claw:
  createClawPulse(clawServoPin, clawPWOpen);
  delay(20);      // do NOT alter this delay time!
}



// createPulse() is the LOWEST LEVEL (most basic) of the movement commands.
// It simply creates a single pulse of a given pulse width on a given pin.
// This command makes ONE servo (set by the servoPin variable) take ONE
// step whose speed and direction is set by the pulseWidth variable.

//////////////////////////////////////////////////////////////////////////////////////

void createClawPulse(byte servoPin, int pulseWidth) {
  // take one step with the claw:
  digitalWrite(servoPin, HIGH);        // create the rising edge of the pulse
  delayMicroseconds(pulseWidth);       // precisely set pulse width in microseconds!
  digitalWrite(servoPin, LOW);         // create the falling edge of the pulse
}

//////////////////////////////////////////////////////////////////////////////////////

/*void blinkOK(int numBlinks) {
  // blink the onboard LED to let the user know the code was properly uploaded
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(LEDPin, HIGH);
    delay(500);
    digitalWrite(LEDPin, LOW);
    delay(500);
  }
}*/


//////////////////////////////////////////////////////////////////////////////////////
