#include "congratulationsscreen.h"

#include <tari/drawing.h>
#include <tari/timer.h>
#include <tari/input.h>
#include <tari/log.h>
#include <tari/system.h>

#define BACKGROUND_Z 1

static struct {
	TextureData background;
	int isWaiting;
} gData;

static void setup() {
	gData.isWaiting = 0;

	gData.background = loadTexturePKG("/sprites/CONGLATURATIONS.pkg");
	setupTimer();
}

static void shutdownScreen() {
	unloadTexture(gData.background);
	shutdownTimer();
}

static void update() {
	updateSystem();
	updateInput();
	updateTimer();
}

static void drawBackground() {
	Rectangle r = makeRectangleFromTexture(gData.background);
	drawSprite(gData.background, makePosition(0, 0, BACKGROUND_Z), r);
}

static void draw() {
	waitForScreen();
	startDrawing();
	drawBackground();
	stopDrawing();
}

static void finalizeCB(void * caller) {
	gData.isWaiting = 2;
}

static GameScreenReturnType getScreenState() {
	if(hasPressedAbortFlank()) return GAMESCREEN_RETURN_ABORT;
	
	if(gData.isWaiting == 2) return GAMESCREEN_RETURN_SUCCESS;
	if(gData.isWaiting == 1) return GAMESCREEN_RETURN_CONTINUE;


	if(hasPressedStartFlank()) {
		gData.isWaiting = 1;
		addTimerCB(100, finalizeCB, NULL);
	}

	return GAMESCREEN_RETURN_CONTINUE;
}

GameScreenReturnType startCongratulationsScreen(){
	logg("Start Congrats screen.");
	logMemoryState();
	setup();
	
	GameScreenReturnType ret = GAMESCREEN_RETURN_CONTINUE;
	
	while(ret == GAMESCREEN_RETURN_CONTINUE){
		update();
		draw();
		ret = getScreenState();
	}
	shutdownScreen();

	logg("Exit Congrats screen.");	
	logMemoryState();

	return ret;
}
