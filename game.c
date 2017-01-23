#include "game.h"

#include "gamescreen.h"

void startGame(){
	GameScreenReturnType result = startGameScreen("/scripts/SECTION1.txt");

	(void)result; //TODO
}
