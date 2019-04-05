#include <bits/stdc++.h>
using namespace std;

/*
Assume the bot have no actual front and could move in four directions (not practical) <-- FIX THIS

NEXT TIME CANCEL ALL SEARCH ON HALLWAY (fix traces)
*/

#define MAXDIM 500
#define CENTER 250
#define MAZE_BLANK '.'
#define MAZE_BLOCK '#'

struct{
	int x, y, r = 0, c = 0;
//	char dr;
	char radar[MAXDIM+5][MAXDIM+5];
	pair<int,int> pulse(char dr){

	}
	void observe(){
		// find the candle IF BOT IS CURRENTLY IN THE ROOM AND IT IS GOING TO TURN, but not my part
	}
}maze;

struct{
	char from[MAXDIM+5][MAXDIM+5];
	bool chk[MAXDIM+5][MAXDIM+5];
}vMaze;

struct{
	int x, y;
//	char dr;
	const int size = 1;
	const int chkRadius = 2;
	bool leavesTrace(){
		for(int i = x-chkRadius; i <= x+chkRadius; i++){	// need to work on blocking the signal using wall
			if(i < 1 || i > MAXDIM){
				continue;
			}
			for(j = y-chkRadius; j <= y+chkRadius; j++){
				if(j < 1 || j > MAXDIM){
					continue;
				}
				vMaze.chk[i][j] = true;
			}
		}
		return false;
	}
}bot;

bool leftAble(){
	pair<int,int> pulseGet = maze.pulse('L');
	if(pulseGet.first <= chkRadius || pulseGet.second <= chkRadius){
		return false;
	}
	for(int j = bot.y-bot.chkRadius; j <= bot.y+bot.chkRadius; j++){	// there will also be another bug of ignored corner case
		if(vMaze.chk[bot.x-chkRadius-1][j]){
			return false;
		}
	}
	return true;
}

void dfs(char fdr){
	vMaze.from[bot.x][bot.y] = fdr;

	// find the candle and mark the area
	leavesTrace();
	if(leftAble()){	

	}

}

main(){

	// read the maze file
	freopen("maze.txt","r",stdin);
	while(scanf("%s",maze.radar[++maze.r]+1) != EOF);
	maze.c = strlen(maze.radar[1]+1);

	// initialize reference coordinates
	bot.x = CENTER;
	bot.y = CENTER;
	if(CENTER > MAXDIM || CENTER < 1){
		printf("ERROR: Reference start point is not in the map\n");
		return 0;
	}

	// initialize actual coordinates
	bool found = false;
	for(int i = 1; i <= maze.r; i++){
		for(int j = 1; j <= maze.c; j++){
			if(maze.radar[i][j] != MAZE_BLANK && maze.radar[i][j] != MAZE_BLOCK){
				maze.x = i;
				maze.y = j;
//				maze.dr = bot.dr = maze.radar[i][j];
				found = true;
				break;	
			}
		}
		if(found){
			break;
		}
	}
	if(!found){
		printf("ERROR: Start point not found\n");
		return 0;
	}

	// let's run
	dfs('S');
}