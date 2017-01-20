#include "gamescreen.h"

GameScreenData gGameScreenData;

void setupGameScreen(char* mainScriptPath){
	setupPhysics(&gGameScreenData.physics);	
	setupCollision(&gGameScreenData.collision);
	setupPlayer(&gGameScreenData.player);
	setupUserInterface(&gGameScreenData.userInterface);
	setupScripts(&gGameScreenData.scripts, mainScriptPath);
}

void update(){
	updatePlayer(&gGameScreenData.player);
	updateScripts(&gGameScreenData.scripts);
	updatePhysics(&gGameScreenData.physics);
	updateCollision(&gGameScreenData.collision);
	updateUserInterface(&gGameScreenData.userInterface);
}

void draw(){
	drawPlayer(&gGameScreenData.player);
	drawUserInterface(&gGameScreenData.userInterface);
	drawScripts(&gGameScreenData.scripts);
}

void gameScreenRoutine(){
	int gameIsRunning = 1;
	while(gameIsRunning){
		update();
		draw();
	}
}

GameScreenReturnType startGameScreen(char* mainScriptPath){
	
	setupGameScreen(mainScriptPath);

	gameScreenRoutine();

	return GAMESCREEN_RETURN_SUCCESS;
}
