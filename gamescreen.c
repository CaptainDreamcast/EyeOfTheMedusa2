#include "gamescreen.h"

#include <tari/drawing.h>
#include <tari/log.h>
#include <tari/input.h>

#include "shotHandler.h"

static GameScreenData gGameScreenData;

static void setupGameScreen(char* mainScriptPath){
	log("Setup game screen.");
	setupPhysics(&gGameScreenData.physics);	
	setupShotHandling();
	setupCollision(&gGameScreenData.collision);
	setupPlayer(&gGameScreenData.player);
	setupScripts(&gGameScreenData.scripts, mainScriptPath);
	setupUserInterface(&gGameScreenData.userInterface, &gGameScreenData.player);
}

static GameScreenReturnType checkIfStillRunning(){

	if(hasPressedAbortFlank()) return GAMESCREEN_RETURN_ABORT;

	return GAMESCREEN_RETURN_CONTINUE;
}

static GameScreenReturnType update(){
	debugLog("Update game screen.");
	updateInput();
	updatePlayer(&gGameScreenData.player);
	updateScripts(&gGameScreenData.scripts);
	updatePhysics(&gGameScreenData.physics);
	updateShotHandling();
	updateCollision(&gGameScreenData.collision);
	updateUserInterface(&gGameScreenData.userInterface);
	
	return checkIfStillRunning();
}

static void draw(){
	debugLog("Draw game screen.");
	waitForScreen();
	startDrawing();
	drawPlayer(&gGameScreenData.player);
	drawShotHandling();
	//drawUserInterface(&gGameScreenData.userInterface);
	//drawScripts(&gGameScreenData.scripts);
	//drawCollisions(&gGameScreenData.collision);
	stopDrawing();
}

static GameScreenReturnType gameScreenRoutine(){
	log("Begin game routine.");
	int gameIsRunning = 1;
	GameScreenReturnType ret;

	while(gameIsRunning){
		ret = update();
		draw();

		if(ret != GAMESCREEN_RETURN_CONTINUE) gameIsRunning = 0;
	}

	return ret;
}

GameScreenReturnType startGameScreen(char* mainScriptPath){
	
	setupGameScreen(mainScriptPath);

	return gameScreenRoutine();
}
