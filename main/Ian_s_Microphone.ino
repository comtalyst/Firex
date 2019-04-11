
//int led = 13;

/*void setup() {
  Serial.begin(9600);
  setupMicrophone();
  pinMode(led, OUTPUT);
  blinkOK(3);
  microphone();
  }
*/

/*void loop() {
  }
*/
/*void blinkOK(int numBlinks) {
  // blink the onboard LED to let the user know the code was properly uploaded
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
    delay(100);
  }
  }
*/
//
void microphone() {

  if (samplingIsDone()) {
    Serial.println(getFreq());
    delay(100);
    int count = 0;
    while (true) {
      delay(100);
      Serial.println(count++);
      if ( getFreq() > 3150 && getFreq() < 4300 ) {
        digitalWrite(LEDPin, HIGH);   // turn the LED on (HIGH is the voltage level)        //Changed from the led to LEDPin
        delay(1000);               // wait for a second
        digitalWrite(LEDPin, LOW);    // turn the LED off by making the voltage LOW         //Changed from the led to LEDPin
        delay(1000);
        //        Serial.println("good");
        break;
        // wait for a second
        // heard the sound --> turn the robot on
      }

    }
    //    Serial.println (" I broke the loop");
  }
}
