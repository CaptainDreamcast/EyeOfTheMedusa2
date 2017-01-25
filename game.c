#include "game.h"

#include "gamescreen.h"

void startGame(){
	GameScreenReturnType result = GAMESCREEN_RETURN_SUCCESS;
	while(result == GAMESCREEN_RETURN_SUCCESS){
		result = startGameScreen("/scripts/SECTION1.txt");
	}
}
