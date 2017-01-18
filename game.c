#include "game.h"

#include "gamescreen.h"

void startGame(){
	GameScreenReturnType result = startGameScreen("scripts/test.txt");

	(void)result; //TODO
}
