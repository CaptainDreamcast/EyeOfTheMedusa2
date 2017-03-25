#include "gamescreen.h"

#include <tari/drawing.h>
#include <tari/log.h>
#include <tari/input.h>
#include <tari/animation.h>
#include <tari/timer.h>
#include <tari/system.h>

#include "shotHandler.h"
#include "gameStateLogic.h"

static GameScreenData gGameScreenData;

static void setupGameScreen(char* mainScriptPath){
	logg("Setup game screen.");
	setupPhysics(&gGameScreenData.physics);	
	setupShotHandling();
	setupGameCollision(&gGameScreenData.collision);
	setupPlayer(&gGameScreenData.player);
	setupScripts(&gGameScreenData.scripts, mainScriptPath);
	setupUserInterface(&gGameScreenData.userInterface, &gGameScreenData.player);
	setupGameStateLogic(&gGameScreenData.winFlag, &gGameScreenData.gameOverFlag);
	setupAnimationHandler();
	setupTimer();
}

static GameScreenReturnType checkIfStillRunning(){

	if(hasPressedAbortFlank()) return GAMESCREEN_RETURN_ABORT;
	if(gGameScreenData.winFlag) return GAMESCREEN_RETURN_SUCCESS;
	if(gGameScreenData.gameOverFlag) return GAMESCREEN_RETURN_FAILURE;

	return GAMESCREEN_RETURN_CONTINUE;
}

static GameScreenReturnType update(){
	updateSystem();
	updateInput();
	updatePlayer(&gGameScreenData.player);
	updateScripts(&gGameScreenData.scripts);
	updatePhysics(&gGameScreenData.physics);
	updateShotHandling();
	updateCollision(&gGameScreenData.collision);
	updateUserInterface(&gGameScreenData.userInterface);
	updateAnimationHandler();
	updateTimer();
	
	return checkIfStillRunning();
}

static void draw(){
	waitForScreen();
	startDrawing();
	drawPlayer(&gGameScreenData.player);
	drawShotHandling();
	drawUserInterface(&gGameScreenData.userInterface);
	drawScripts(&gGameScreenData.scripts);
	drawCollisions(&gGameScreenData.collision);
	drawHandledAnimations();
	stopDrawing();
}

static GameScreenReturnType gameScreenRoutine(){
	logg("Begin game routine.");
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
	shutdownGameCollision(&gGameScreenData.collision);
	
	shutdownPlayer(&gGameScreenData.player);
	shutdownScripts(&gGameScreenData.scripts);
	shutdownUserInterface(&gGameScreenData.userInterface);
	shutdownGameStateLogic();
	shutdownAnimationHandler();
	shutdownTimer();

	// TODO: make properly
	resumePhysics();
	resumeDurationHandling();

	debugInteger(getAvailableTextureMemory());
	logMemoryState();
	logTextureMemoryState();
}

GameScreenReturnType startGameScreen(char* mainScriptPath){
	
	setupGameScreen(mainScriptPath);

	GameScreenReturnType ret = gameScreenRoutine();

	shutdownGameScreen();	
	return ret;
}
