void printSensorLoop(int sPin) {
  int sensorValue;  // variable to store the value coming from the sensor

  while (true) {
    // read the value from the sensor:
    sensorValue = analogRead(sPin);

    // print the value
    Serial.print("sensorValue = ");
    Serial.println(sensorValue);
    Serial.print("");
    delay(250);
  }
}

void printSensor(int sPin) {
  int sensorValue;  // variable to store the value coming from the sensor

  // read the value from the sensor:
  sensorValue = analogRead(sPin);

  // print the value
  Serial.print("sensorValue = ");
  Serial.println(sensorValue);
  Serial.print("");
  delay(50);
}


void sensorGraphSerial(int sPin) {
  int sensorValue;  // variable to store the value coming from the sensor

  while (true) {
    sensorValue = analogRead(sPin);
    Serial.println(" sensorValue = " + (String)sensorValue);
    for (int i = 0; i <= sensorValue / 10; i++) {
      Serial.print(" ");
    }
    Serial.println("*");
    delay(100);
  }
}

void sensorGraphPlotter(int sPin) {
  int sensorValue;  // variable to store the value coming from the sensor

  while (true) {
    sensorValue = analogRead(sPin);
      Serial.println((String)sensorValue + ", 0, 1023");
    delay(1);
  }
}
