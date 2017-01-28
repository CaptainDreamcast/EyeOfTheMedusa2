#include "game.h"

#include "gamescreen.h"
#include "titlescreen.h"
#include "congratulationsscreen.h"
#include "gameoverscreen.h"

void startGame(){
	GameScreenReturnType result = GAMESCREEN_RETURN_SUCCESS;
	while(result != GAMESCREEN_RETURN_ABORT){	
		if(result == GAMESCREEN_RETURN_SUCCESS) {
			result = startTitleScreen();
			if(result == GAMESCREEN_RETURN_ABORT) break;
		}

		result = startGameScreen("/scripts/SECTION1.txt");
		if(result == GAMESCREEN_RETURN_SUCCESS) result = startCongratulationsScreen();
		else if(result == GAMESCREEN_RETURN_FAILURE) result = startGameOverScreen();
		
		if(result == GAMESCREEN_RETURN_ABORT) result = GAMESCREEN_RETURN_SUCCESS;
		
	}
}
