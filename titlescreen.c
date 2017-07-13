#include "titlescreen.h"

#include <tari/texture.h>
#include <tari/timer.h>
#include <tari/input.h>
#include <tari/system.h>

#include "script.h"
#include "shotHandler.h"
#include "collision.h"
#include "shotScript.h"
#include "gamescreen.h"

#define BACKGROUND_Z 5

static struct {
	
	script* fireWorkShot;
	TextureData background;

	CollisionData cData;
	int isClosing;

	Duration shotAddNow;
	Duration shotAddDuration;
	
} gData;

static void setupTitleScreen() {
	setupShotHandling();
	setupGameCollision(&gData.cData);

	gData.background = loadTexturePKG("/sprites/TITLESCREEN.pkg");
	gData.fireWorkShot = loadShotScript("/scripts/SHOT1.txt");
	gData.isClosing = 0;
	gData.shotAddDuration = 60;
	gData.shotAddNow = 0;

	hasPressedStartFlank(); // TODO: fix flank impl; put to update
}

static void shutdownScreen(){
	unloadTexture(gData.background);
	gData.fireWorkShot->func.unload(gData.fireWorkShot);
	
	shutdownGameCollision(&gData.cData);
	shutdownShotHandling();
}

static void drawBackground() {
	Rectangle r = makeRectangleFromTexture(gData.background);
	drawSprite(gData.background, makePosition(0, 0, BACKGROUND_Z), r);
}

static void draw(){
	drawBackground();
	drawShotHandling();
}

static void updateFirework(){
	int isTime = handleDurationAndCheckIfOver(&gData.shotAddNow, gData.shotAddDuration);
	if(!isTime) return;

	gData.shotAddNow = 0;

	Position p = vecAdd(makePosition(320, 240, 2), variatePosition(makePosition(320, 240, 0)));
	updateShotScriptBasePosition(gData.fireWorkShot, p);
	int amount = 100;
	while(amount--) gData.fireWorkShot->func.update(gData.fireWorkShot);
}



static void shutdownCB(void* caller) {
	gData.isClosing = 2;
}

static void getRunningState() {

	if (hasPressedAbortFlank()) {
		abortScreenHandling();
	}

	if (gData.isClosing == 2) {
		setMainScriptPath("/scripts/LEVEL1.txt");
		setNewScreen(&GameScreen);
	}

	if(hasPressedStartFlank()) {
		addTimerCB(100, shutdownCB, &gData);
	}

}

static void updateTitleScreen() {
	updateSystem();
	updateShotHandling();
	updateCollision(&gData.cData);
	updateInput();
	updateTimer();
	updateFirework();
	getRunningState();
}

Screen TitleScreen = {
	.mLoad = setupTitleScreen,
	.mUnload = shutdownScreen,
	.mUpdate = updateTitleScreen,
	.mDraw = draw,
};
