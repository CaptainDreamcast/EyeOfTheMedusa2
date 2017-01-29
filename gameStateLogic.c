#include "gameStateLogic.h"

static int* gWinFlag;
static int* gGameOverFlag;


void setupGameStateLogic(int* winFlag, int* gameOverFlag){
	*winFlag = 0;
	*gameOverFlag = 0;

	gWinFlag = winFlag;
	gGameOverFlag = gameOverFlag;
}

void shutdownGameStateLogic(){
	// does not hold memory
}

void setGameWon(){
	*gWinFlag = 1;
}

void setGameLost(){
	*gGameOverFlag = 1;
}
