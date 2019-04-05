/* SKIF.ino
 c.d.odom 10.19.14
 a sketch that houses a variety of homemade functions to grab keyboard input
 NB: No setup(), no loop()
*/


byte readByte(){
  while (Serial.available() == 0);      // pause indefinately until the serial port has data.
  byte getByte = Serial.read();         // grab a byte (character) stored in the serial port buffer
  //  Serial.print("Character = " );
  //  Serial.write(getByte);                            // writes the binary data to the Serial Monitor
  //  Serial.println(", ASCII = " + String(getByte));   // writes the ASCII code of the binary data
  //  Serial.println("");
  return getByte;
}

char readChar(){
  while (Serial.available() == 0);      // pause indefinately until the serial port has data.
  char getChar = Serial.read();         // grab a character (byte) stored in the serial port buffer
  //  Serial.println("Character = " + String(getChar));
  //  Serial.println("");

  // clean up the keyboard buffer in case user enters more than one character:
  while (Serial.available() > 0)  
  { 
    char junk = Serial.read() ;       // empty the keyboard buffer one character at a time
  }    
  return getChar;
}


String readString(){
  String buildString = "";

  while (Serial.available() == 0);      // pause indefinately until the serial port has data.

  // loop while there are bytes in the serial port 
  while (Serial.available() > 0){
    char getChar = Serial.read();        // grab a character (byte) stored in the serial port buffer
    if (getChar == '\n'){                // if the character is a carriage return (ENTER) ...
      // ... do nothing ... the input textbox is now empty
    }
    else{
      buildString += getChar;      // build the string one character at a time
    }
  }
  return buildString;
}

int readInt(){
  while (Serial.available() == 0);                     // pause indefinately until the serial port has data.
  int getInt = Serial.parseInt();                      // grab an integer from th the serial port buffer
  // note delay with parseInt!  try entering a letter
  //  Serial.println("Integer Value = " + String(getInt));        
  //  Serial.println("");

  // parse sometimes leaves junk characters, so this cleans up the keyboard buffer:
  while (Serial.available() > 0)  
  { 
    char junk = Serial.read() ;       // empty the keyboard buffer one character at a time
  }      
  return getInt;
}

float readFloat(){
  // NB: not accurate for large numbers.  For more precision, use snprintf
  // NB: does not recognize scientific notation

  while (Serial.available() == 0);                       // pause indefinately until the serial port has data.
  float getFloat = Serial.parseFloat();                    // grab a floating number from the serial port buffer
  // note delay with parseFloat!  try entering a letter
  //  Serial.println("Float Value = " + String(getFloat));     // if you need more decimals, use snprintf.       
  //  Serial.println("");
  //  Serial.println("");

  // parse sometimes leaves junk characters, so this cleans up the keyboard buffer:
  while (Serial.available() > 0)  
  { 
    char junk = Serial.read() ;       // empty the keyboard buffer one character at a time
  }   
  return getFloat;  
}

//void printFloat(){
//  Serial.println("Enter one FLOAT in the input textbox on the Serial Monitor");
//  char buf[100];          // buffer for character array for snprintf command
//  // NB: not accurate for large numbers
//  // NB: does not recognize scientific notation
//  
//  while (Serial.available() == 0) ;
//  float getFloat = Serial.parseFloat();                    // note delay with parseFloat!  try entering a letter
//  snprintf(buf, 100, "Float Value = %f", getFloat);
//  Serial.println(buf);     // if you need more decimals, use snprintf.       
//  Serial.println("");
//
//  // parseFloat() sometimes leaves junk characters, so this cleans up the keyboard buffer:
//  while (Serial.available() > 0)  
//  { 
//    char junk = Serial.read() ;       // clear the keyboard buffer
//  }      
//}










