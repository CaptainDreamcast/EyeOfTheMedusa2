#include "congratulationsscreen.h"

#include <tari/drawing.h>
#include <tari/timer.h>
#include <tari/input.h>
#include <tari/log.h>
#include <tari/system.h>

#include "titlescreen.h"

#define BACKGROUND_Z 1

static struct {
	TextureData background;
	int isWaiting;
} gData;

static void setup() {
	gData.isWaiting = 0;

	gData.background = loadTexturePKG("/sprites/CONGLATURATIONS.pkg");
}

static void shutdownScreen() {
	unloadTexture(gData.background);
}



static void drawBackground() {
	Rectangle r = makeRectangleFromTexture(gData.background);
	drawSprite(gData.background, makePosition(0, 0, BACKGROUND_Z), r);
}

static void draw() {
	drawBackground();
}

static void finalizeCB(void * caller) {
	gData.isWaiting = 2;
}

static void getScreenState() {
	if (hasPressedAbortFlank()) setNewScreen(&TitleScreen);
	
	if(gData.isWaiting == 2) setNewScreen(&TitleScreen);
	if (gData.isWaiting == 1) return;


	if(hasPressedStartFlank()) {
		gData.isWaiting = 1;
		addTimerCB(100, finalizeCB, NULL);
	}
}

static void update() {
	updateSystem();
	updateInput();
	updateTimer();
	getScreenState();
}

Screen CongratsScreen = {
	.mLoad = setup,
	.mUnload = shutdownScreen,
	.mUpdate = update,
	.mDraw = draw,
};
