#include "gameoverscreen.h"

#include <tari/timer.h>
#include <tari/drawing.h>
#include <tari/optionhandler.h>
#include <tari/input.h>
#include <tari/log.h>

static struct {
	TextureData background;	
	GameScreenReturnType returnValue;
	int isWaiting;
} gData;

#define OPTION_Z 10
#define BACKGROUND_Z 1

static void finishCB(void* caller) {
	gData.isWaiting = 2;
}

static void waitForCB() {
	disableOptionHandler();
	gData.isWaiting = 1;
	addTimerCB(100, finishCB, &gData);
}

static void continueCB(void* caller){
	gData.returnValue = GAMESCREEN_RETURN_FAILURE;
	waitForCB();
}

static void noContinueCB(void* caller){
	gData.returnValue = GAMESCREEN_RETURN_SUCCESS;
	waitForCB();
}

static void setup(){
	gData.background = loadTexturePKG("/sprites/GAMEOVER.pkg");
	gData.returnValue = 0;
	gData.isWaiting = 0;

	setupOptionHandler();
	addOption(makePosition(200, 200, OPTION_Z), "YES", continueCB, NULL);
	addOption(makePosition(200, 300, OPTION_Z), "NO", noContinueCB, NULL);
	setOptionTextSize(40);
	setOptionButtonA();
	setOptionButtonStart();
	setupTimer();

	hasPressedAFlank(); hasPressedStartFlank(); // TODO: remove when fixed
}

static void shutdownScreen(){
	unloadTexture(gData.background);
	shutdownOptionHandler();
	shutdownTimer();
}

static GameScreenReturnType updateState() {
	if(hasPressedAbortFlank()) return GAMESCREEN_RETURN_ABORT;

	if(gData.isWaiting == 2) return gData.returnValue;
	
	return GAMESCREEN_RETURN_CONTINUE;
}

static void update() {
	updateInput();
	updateOptionHandler();
	updateTimer();
}

static void drawBackground() {
	return;
	Rectangle r = makeRectangleFromTexture(gData.background);
	drawSprite(gData.background, makePosition(0, 0, BACKGROUND_Z), r);
}

static void draw() {
	waitForScreen();
	startDrawing();
	drawBackground();
	drawOptionHandler();
	stopDrawing();
}

GameScreenReturnType startGameOverScreen(){
	log("Setup Game Over screen.");
	logMemoryState();
	setup();
	log("Start Game Over screen.");
	GameScreenReturnType ret = GAMESCREEN_RETURN_CONTINUE;
	
	while(ret == GAMESCREEN_RETURN_CONTINUE){
		update();
		draw();
		ret = updateState();
	}
	shutdownScreen();
	log("Exit Game Over screen.");	
	logMemoryState();

	return ret;
}
