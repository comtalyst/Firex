// "SonicRangerModule.ino" by c.d.odom on 8.16.17
// Modified for Trinity by Robin D. on 3.30.19
// The SR04 ultrasonic sensor must be powered with 5V!

/*  these need to be added to the globals section and setup() function in the main sketch!
  // GLOBALS
  const byte trigPin1 = 5;      // trigger pin for sonic ranger #1
  const byte echoPin1 = A5;     // echo (data) pin for sonic ranger #1
  const byte LEDPin = 13;       // onboard LED pin (for diagnostics)

  void setup() {
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);    // set the pinMode for trigger pin #1
  pinMode(echoPin1, INPUT);     // set the pinMode for echo pin #1
  pinMode(LEDPin, OUTPUT);      // set the pinMode for the onboard LED
  delay(100);       // allow the serial window time to open
  }

*/


float sonicRange(byte trigPin, byte echoPin, float maxRange) {
  // function, with arguments: (trigger pin, echo pin, maximum range (in cm))
  float timeout;                      // # of microseconds sensor will search for object
  long timeRoundTrip;                 // total time (from echo pin) for sound to travel
  float distanceToObject;             // the calculated distance to the object (in cm)
  float const Vs_cmpus = 0.034524;    // speed of sound in centimeters per microseconds

  // calculate the sensor’s timeout value using the maxRange argument and Equation xxx
  timeout = 2.0 * maxRange / Vs_cmpus;

  // DEBUG ONLY: Print the timeout value to the Serial Monitor:
  //  Serial.println("Sensor will time-out in " + (String) timeout + " us.");

  // make sure the trigger pin starts in the LOW state:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1500);        // delay to prevent over-sampling (may be smaller)

  // tell SR04 to begin data collection by sending a 10us-pulse to trigger pin:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);        // 10us is the minimum trigger pulse width
  digitalWrite(trigPin, LOW);

  // grab the pulse width from echo pin (i.e., sound’s total travel time from Tx to Rx):
  timeRoundTrip = pulseIn(echoPin, HIGH, timeout);

  // DEBUG ONLY: Print the round-trip time to the Serial Monitor:
  //   Serial.println("Round-trip time = " + (String) timeRoundTrip + " us");

  // calculate distance to the object (in cm) using Equation xxx:
  distanceToObject = Vs_cmpus * (float)timeRoundTrip / 2.0;

  return distanceToObject;
}

void sonicGraphPlotter(int trigPin, int echoPin, int maxExpectedRange) {
  // sonic ranger trigger pin, echo pin, and maximum expected range (in cm)
  float range = sonicRange(trigPin, echoPin, maxExpectedRange);      // open the Serial Plotter to check if your sonion ranger is working.
  Serial.println((String)range + ", 0, maxExpectedRange + 10");
}

void sonicDistanceTest(int trigPin, int echoPin, int maxExpectedRange) {
  // sonic ranger trigger pin, echo pin, and maximum expected range (in cm)
  // take a distance reading from ultrasonic sensor by calling the sonicRange()

  float distance = sonicRange(trigPin, echoPin, maxExpectedRange);   // (trigger, echo, maxRange in cm)

  // Print the distance to the object on the Serial Monitor:
  Serial.println("Distance = " + (String)distance + " cm");

  // Act on the distance reading:
  if (distance == 0) {
    Serial.println("No object is detected.");
    digitalWrite(LEDPin, LOW);     // turn OFF the LED
  }
  else if (distance < 15) {
    Serial.println("Object is near!");
    digitalWrite(LEDPin, HIGH);    // turn ON the LED
  }
  else {
    // object detected between 15cm and maxExpectedRange, therefore blink the LED:
    digitalWrite(LEDPin, HIGH);    // turn ON the LED
    delay(100);                    // some arbitrary delay time
    digitalWrite(LEDPin, LOW);     // turn OFF the LED
    delay(100);                    // some arbitrary delay time
  }

  delay(200);      // pause so user can visualize the data
}

//////////////////// Robin's Section ////////////////////
const byte rightFrontEchoPin = 6;
const byte rightFrontTrigPin = 5;
const byte rightRearEchoPin = 10;
const byte rightRearTrigPin = 9;
const byte leftFrontEchoPin = 4;
const byte leftFrontTrigPin = 3;
const byte leftRearEchoPin = 8;
const byte leftRearTrigPin = 7;
const float minFar = 40;              // minimum distance from sensor that would be considered far

void readySonic() {
  pinMode(rightFrontTrigPin, OUTPUT);
  pinMode(rightFrontEchoPin, INPUT);
  pinMode(rightRearTrigPin, OUTPUT);
  pinMode(rightRearEchoPin, INPUT);
  pinMode(leftFrontTrigPin, OUTPUT);
  pinMode(leftFrontEchoPin, INPUT);
  pinMode(leftRearTrigPin, OUTPUT);
  pinMode(leftRearEchoPin, INPUT);
}

float getRangeRightFront() {
  return sonicRange(rightFrontTrigPin, rightFrontEchoPin, minFar);
}
float getRangeRightRear() {
  return sonicRange(rightRearTrigPin, rightRearEchoPin, minFar);
}
float getRangeLeftFront() {
  return sonicRange(leftFrontTrigPin, leftFrontEchoPin, minFar);
}
float getRangeLeftRear() {
  return sonicRange(leftRearTrigPin, leftRearEchoPin, minFar);
}
bool isFar(float range) {
  if (range == 0 || range >= minFar) {
    return true;
  }
  else {
    return false;
  }
}
