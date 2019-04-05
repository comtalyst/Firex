void seedRNG(int pinNumber){
  // seed the RNG with a reading from a desired analog signal pin
  randomSeed(analogRead(pinNumber));        // seed RNG w/ a random voltage value from an analog pin
}

int randomInt(int minNumInt, int maxNumInt){
  // Returns random integer between two values
  int xInt = random(minNumInt, maxNumInt + 1);        // the random integer
  
  return xInt;
}

float randomFloat(float minNumFloat, float maxNumFloat, byte numDecimals){
  // Returns random floating number of desired precision between two decimals

  int minNumInt, maxNumInt;  // the converted boundary (integer) values
  int scalingFactor;  // power of ten factor to convert between float and int values
  int xInt; // random integer value
  float xFloat; // random float value
  
  scalingFactor = pow(10, numDecimals);    // determine the scaling factor for proper precision
  
  minNumInt = (int)(minNumFloat * scalingFactor); // convert min decimal value to integer
  maxNumInt = (int)(maxNumFloat * scalingFactor); // convert max decimal value to integer
  
  xInt = random(minNumInt, maxNumInt + 1);  // temporary random integer
  xFloat = (float)xInt / scalingFactor; // random float in desired range and precision

  return xFloat;
}

