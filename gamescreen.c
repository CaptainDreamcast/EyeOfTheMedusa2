#include "gamescreen.h"

GameScreenData gGameScreenData;

void setupGameScreen(char* mainScriptPath){
	setupScripts(&gGameScreenData.scripts, mainScriptPath);
	setupPlayer(&gGameScreenData.player);
	setupCollision(&gGameScreenData.collision);
	setupPhysics(&gGameScreenData.physics);	
	setupUserInterface(&gGameScreenData.userInterface);
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
