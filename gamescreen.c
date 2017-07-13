#include "gamescreen.h"

#include <tari/drawing.h>
#include <tari/log.h>
#include <tari/input.h>
#include <tari/animation.h>
#include <tari/timer.h>
#include <tari/system.h>

#include "shotHandler.h"
#include "gameStateLogic.h"
#include "titlescreen.h"
#include "congratulationsscreen.h"
#include "gameoverscreen.h"

static GameScreenData gGameScreenData;
static char gMainScriptPath[1024];

void setMainScriptPath(char* tMainScriptPath) {
	strcpy(gMainScriptPath, tMainScriptPath);
}

static void setupGameScreen(){
	logg("Setup game screen.");
	setupPhysics(&gGameScreenData.physics);	
	setupShotHandling();
	setupGameCollision(&gGameScreenData.collision);
	setupPlayer(&gGameScreenData.player);
	setupScripts(&gGameScreenData.scripts, gMainScriptPath);
	setupUserInterface(&gGameScreenData.userInterface, &gGameScreenData.player);
	setupGameStateLogic(&gGameScreenData.winFlag, &gGameScreenData.gameOverFlag);
}

static void checkIfStillRunning(){

	if(hasPressedAbortFlank()) setNewScreen(&TitleScreen);
	if(gGameScreenData.winFlag) setNewScreen(&CongratsScreen);
	if (gGameScreenData.gameOverFlag) setNewScreen(&GameOverScreen);

}

static void update(){
	updateSystem();
	updateInput();
	updatePlayer(&gGameScreenData.player);
	updateScripts(&gGameScreenData.scripts);
	updatePhysics(&gGameScreenData.physics);
	updateShotHandling();
	updateCollision(&gGameScreenData.collision);
	updateUserInterface(&gGameScreenData.userInterface);
	checkIfStillRunning();
}

static void draw(){
	drawPlayer(&gGameScreenData.player);
	drawShotHandling();
	drawUserInterface(&gGameScreenData.userInterface);
	drawScripts(&gGameScreenData.scripts);
	drawCollisions(&gGameScreenData.collision);
	drawHandledAnimations();
}


static void shutdownGameScreen(){
	shutdownPhysics(&gGameScreenData.physics);	
	shutdownShotHandling();
	shutdownGameCollision(&gGameScreenData.collision);
	
	shutdownPlayer(&gGameScreenData.player);
	shutdownScripts(&gGameScreenData.scripts);
	shutdownUserInterface(&gGameScreenData.userInterface);
	shutdownGameStateLogic();

	// TODO: make properly
	resumePhysics();
	resumeDurationHandling();

	debugInteger(getAvailableTextureMemory());
	logMemoryState();
	logTextureMemoryState();
}

Screen GameScreen = {
	.mLoad = setupGameScreen,
	.mUnload = shutdownGameScreen,
	.mUpdate = update,
	.mDraw = draw,
};

