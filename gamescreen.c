#include "gamescreen.h"

#include <tari/drawing.h>
#include <tari/log.h>
#include <tari/input.h>
#include <tari/animation.h>

#include "shotHandler.h"
#include "gameStateLogic.h"

static GameScreenData gGameScreenData;

static void setupGameScreen(char* mainScriptPath){
	log("Setup game screen.");
	setupPhysics(&gGameScreenData.physics);	
	setupShotHandling();
	setupCollision(&gGameScreenData.collision);
	setupPlayer(&gGameScreenData.player);
	setupScripts(&gGameScreenData.scripts, mainScriptPath);
	setupUserInterface(&gGameScreenData.userInterface, &gGameScreenData.player);
	setupGameStateLogic(&gGameScreenData.winFlag, &gGameScreenData.gameOverFlag);
	setupAnimationHandler();
}

static GameScreenReturnType checkIfStillRunning(){

	if(hasPressedAbortFlank()) return GAMESCREEN_RETURN_ABORT;
	if(gGameScreenData.winFlag) return GAMESCREEN_RETURN_SUCCESS;
	if(gGameScreenData.gameOverFlag) return GAMESCREEN_RETURN_FAILURE;

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
	updateAnimationHandler();
	
	return checkIfStillRunning();
}

static void draw(){
	debugLog("Draw game screen.");
	waitForScreen();
	startDrawing();
	drawPlayer(&gGameScreenData.player);
	drawShotHandling();
	//drawUserInterface(&gGameScreenData.userInterface);
	drawScripts(&gGameScreenData.scripts);
	drawCollisions(&gGameScreenData.collision);
	drawHandledAnimations();
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

static void shutdownGameScreen(){
	shutdownPhysics(&gGameScreenData.physics);	
	shutdownShotHandling();
	shutdownCollision(&gGameScreenData.collision);
	
	shutdownPlayer(&gGameScreenData.player);
	shutdownScripts(&gGameScreenData.scripts);
	shutdownUserInterface(&gGameScreenData.userInterface);
	shutdownGameStateLogic();
	shutdownAnimationHandler();

	logInteger(getAvailableTextureMemory());
	logMemoryState();
	logTextureMemoryState();
}

GameScreenReturnType startGameScreen(char* mainScriptPath){
	
	setupGameScreen(mainScriptPath);

	GameScreenReturnType ret = gameScreenRoutine();

	shutdownGameScreen();	
	return ret;
}
