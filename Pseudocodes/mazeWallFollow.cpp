#include <bits/stdc++.h>
using namespace std;


char side = 'R';						// focusing side of wall following

const float minFurther = ;				// minimum distance difference between two sensors that would make the robot readjust itself
const float minFar = ;					// minimum distance from sensor that would be considered far
const float minWalkable = ;				// minimum distance needed in front of the bot for it to walk forward

const int turnSteps = ;					// steps taken per turn action
const int forwardSteps = ;				// steps taken per walk action

main(){
	while(true) loop();
}

void loop(){
	if(side == 'R'){
		// IF SIDE-FRONT SENSOR IS FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
		if(isFar(getRange(rightFrontPin)) || getRange(rightFrontPin) - getRange(rightRearPin) >= minFurther){
			/*TURN RIGHT SLIGHTLY*/
		}
		// IF SIDE-REAR SEint NSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
		else if(getRange(rightRearPin) - getRange(rightFrontPin) >= minFurther){
			/*TURN LEFT SLIGHTLY*/
		}
		else if(getRange(frontLowPin) >= minWalkable){
			/*FORWARD SLIGHTLY*/
		}
		else{
			/*TURN RIGHT 90*/
		}
	}
	else if(side == 'L'){
		// IF SIDE-FRONT SENSOR IS FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
		if(isFar(getRange(leftFrontPin)) || getRange(leftFrontPin) - getRange(leftRearPin) >= minFurther){
			/*TURN LEFT SLIGHTLY*/
		}
		// IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
		else if(getRange(leftRearPin) - getRange(leftFrontPin) >= minFurther){
			/*TURN RIGHT SLIGHTLY*/
		}
		else if(getRange(frontLowPin) >= minWalkable){
			/*FORWARD SLIGHTLY*/
		}
		else{
			/*TURN LEFT 90*/
		}
	}
	else{	// something's wrong
		Serial.print("ERROR: loop(): There is no such side > ");
		Serial.println(side);
		while(true);
	}
}
