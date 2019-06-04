#include <bits/stdc++.h>
using namespace std;

/*
Algo
- Follow right wall
- after check in room --> turn around
- if found a stick (<> ---) or a dog --> change to left
- if be at the same position with same arrangements (radius checking to prevent error) --> switch
*/

//!!!!!!!!!! CANT IMPLEMENT vMAZE BECAUSE OF LIMITED MEMORY

// NEXT --> DO HORIZONTAL RADIUS STUFF

#define DIM 1000
#define CENTER 500
#define MAX 9999

char side = 'R';						// focusing side of wall following
int tick;								// tick (excluded switching focus)
deque<int> history;						// history of previous actions
int turnsInStack;						// weight of turn actions in history (left = -1, right = +1, forward = 0)
char chk[DIM+5][DIM+5];					// marking paper (virtual step tracker)
int chkX = 192, chkY = 413;		// marker's location (virtual bot location)
char chkDr = 'D';						// marker's direction (virtual bot direction) (U,L,D,R), update only when detect 90/180
int lastTurnTick = -MAX;				// last tick that the bot initiate 90 degrees turn

const int maxConsecutiveTurnTicks = ;	// maximum tick difference between two 90 degrees turn to be considered 180 degrees 
const int minFurther = ;				// minimum distance difference between two sensors that would make the robot readjust itself
const int maxHistorySize = ;			// maximum size of action history
const int minFar = 30;					// minimum distance from sensor that would be considered far
const int minTurnsDensity = ;			// minimum weight of turns in history to assume it's turning 90 degrees
const int minWalkable = ;				// minimum distance needed in front of the bot for it to walk forward
const int horizonChkRadius = ;			// maximum horizontal distance from the bot to be considered same path (parallel) 

const int turnSteps = ;					// steps taken per turn action
const int forwardSteps = ;				// steps taken per walk action

main(){
	while(true) loop();
}

void virtualMove(){
	if(chkDr == 'U'){
		chkX--;
	}
	else if(chkDr == 'L'){
		chkY--;
	}
	else if(chkDr == 'D'){
		chkX++;
	}
	else if(chkDr == 'R'){
		chkY++;
	}
	else{	// something's wrong
		Serial.print("ERROR: virtualMove(): There is no such direction > ");
		Serial.println(chkDr);
		while(true);
	}
	chk[chkX][chkY] = side;
}
void virtualTurn(char dr){
	if(chkDr == 'U'){
		chkDr = dr;
	}
	else if(chkDr == 'L'){
		if(dr == 'L'){
			chkDr = 'D';
		}
		else if(dr == 'D'){
			chkDr = 'R';
		}
		else if(dr == 'R'){
			chkDr = 'U';
		}
	}
	else if(chkDr == 'D'){
		if(dr == 'L'){
			chkDr = 'R';
		}
		else if(dr == 'D'){
			chkDr = 'U';
		}
		else if(dr == 'R'){
			chkDr = 'L';
		}
	}
	else if(chkDr == 'R'){
		if(dr == 'L'){
			chkDr = 'U';
		}
		else if(dr == 'D'){
			chkDr = 'L';
		}
		else if(dr == 'R'){
			chkDr = 'D';
		}
	}
	else{	// something's wrong
		Serial.print("ERROR: virtualTurn(): There is no such direction > ");
		Serial.println(chkDr);
		while(true);
	}
}

void loop(){
	if(side == 'R'){
		// IF SIDE-FRONT SENSOR IS FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
		if(isFar(getRange(rightFrontPin)) || getRange(rightFrontPin) - getRange(rightRearPin) >= minFurther){
			/*TURN RIGHT SLIGHTLY*/
			history.emplace_back(1);
		}
		// IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
		else if(getRange(rightRearPin) - getRange(rightFrontPin) >= minFurther){
			/*TURN LEFT SLIGHTLY*/
			history.emplace_back(-1);
		}
		else if(/*THERE IS A PARALLEL MARK IN HORIZONTAL RANGE (BOT WAS HERE WITH SAME SIDE)*/){
			side = 'L';
			return;
		}
		// FOUND A DOG
		else if(abs(getRange(frontLowPin) - getRange(frontHighPin)) >= minDifIsDog){
			side = 'L';
			return;
		}
		else if(getRange(frontLowPin) >= minWalkable){
			/*FORWARD SLIGHTLY AND MARK*/
			virtualMove();
			history.emplace_back(0);
		}
		else{
			/*TURN RIGHT 90*/
		}
	}
	else if(side == 'L'){
		// IF SIDE-FRONT SENSOR IS FAR OR SIDE-FRONT SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-REAR SENSOR
		if(isFar(getRange(leftFrontPin)) || getRange(leftFrontPin) - getRange(leftRearPin) >= minFurther){
			/*TURN LEFT SLIGHTLY*/
			history.emplace_back(-1);
		}
		// IF SIDE-REAR SENSOR IS SIGNIFICANTLY FURTHER THAN SIDE-FRONT SENSOR
		else if(getRange(leftRearPin) - getRange(leftFrontPin) >= minFurther){
			/*TURN RIGHT SLIGHTLY*/
			history.emplace_back(1);
		}
		else if(/*THERE IS A PARALLEL MARK IN HORIZONTAL RANGE (BOT WAS HERE WITH SAME SIDE)*/){
			side = 'R';
			return;
		}
		// FOUND A DOG
		else if(abs(getRange(frontLowPin) - getRange(frontHighPin)) >= minDifIsDog){
			side = 'R';
			return;
		}
		else if(getRange(frontLowPin) >= minWalkable){
			/*FORWARD SLIGHTLY AND MARK*/
			virtualMove();
			history.emplace_back(0);
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

	tick++;										// process tick and history
	turnsInStack += history.back();
	if(history.size > maxHistorySize){
		turnsInStack -= history.front();
		history.pop_front();
	}

	// IF BOT TURNS FOR 90 DEGREES, CHECK FROM DENSITY OF TURNS DURING THE LAST MOVES
	if(abs(turnsInStack) >= minTurnsDensity){
		// UPDATE MARKING DIRECTION
		virtualTurn((turnsInStack > 0)? 'R':'L');
		// RESET HISTORY (IF NOT, TURN "CREDITS" WILL BE REUSED)
		while(!history.empty()) history.pop_front();
		turnsInStack = 0;
		if(/*DETECTED A DOOR WHILE TURNING*/){
			/*SEARCH FOR CANDLE AND EXTINGUISH IT (NOT MY PART)*/
			/*TURN AROUND (THE WALL'S EDGE WILL BE BETWEEN THE SENSORS)*/
			// UPDATE MARKING DIRECTION	
			virtualTurn('D');
			lastTurnTick = -MAX;					// pretend bot actually search the room (in order to ignore side flipping on 90)
		}
		// IF BOT TURNS FOR 180 DEGREES, CHECK FROM LAST TIME THE TURN HAVE BEEN INITIATED (IF CONSECUTIVE --> 180!)
		else if(tick - lastTurnTick <= maxConsecutiveTurnTicks){
			if(side == 'R'){
				side = 'L';
			}
			else if(side == 'L'){
				side = 'R';
			}
			else{	// something's wrong
				Serial.print("ERROR: loop(): There is no such side > ");
				Serial.println(side);
				while(true);
			}
			lastTurnTick = tick;
		}
		else{										// just to not violate the lastTurnTick = -MAX above
			lastTurnTick = tick;
		}
		
	}
	
}
