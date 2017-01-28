#include "titlescreen.h"

#include <tari/texture.h>
#include <tari/timer.h>
#include <tari/input.h>

#include "script.h"
#include "shotHandler.h"
#include "collision.h"
#include "shotScript.h"

#define BACKGROUND_Z 5

static struct {
	
	script* fireWorkShot;
	TextureData background;

	CollisionData cData;
	int isClosing;

	Duration shotAddNow;
	Duration shotAddDuration;
	
} gData;

static void setup() {
	setupTimer();
	setupShotHandling();
	setupCollision(&gData.cData);

	gData.background = loadTexturePKG("/sprites/TITLESCREEN.pkg");
	gData.fireWorkShot = loadShotScript("/scripts/SHOT1.txt");
	gData.isClosing = 0;
	gData.shotAddDuration = 120;
	gData.shotAddNow = 0;

	hasPressedStartFlank(); // TODO: fix flank impl; put to update
}

static void shutdownScreen(){
	unloadTexture(gData.background);
	gData.fireWorkShot->func.unload(gData.fireWorkShot);
	
	shutdownCollision(&gData.cData);
	shutdownShotHandling();
	shutdownTimer();
}

static void drawBackground() {
	Rectangle r = makeRectangleFromTexture(gData.background);
	drawSprite(gData.background, makePosition(0, 0, BACKGROUND_Z), r);
}

static void draw(){
	waitForScreen();
	startDrawing();
	drawBackground();
	drawShotHandling();
	stopDrawing();
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

static void update() {
	updateShotHandling();
	updateCollision(&gData.cData);
	updateInput();
	updateTimer();
	updateFirework();
}

static void shutdownCB(void* caller) {
	gData.isClosing = 2;
}

static GameScreenReturnType getRunningState() {

	if(hasPressedAbortFlank()) return GAMESCREEN_RETURN_ABORT;

	if(gData.isClosing == 1) return GAMESCREEN_RETURN_CONTINUE;
	if(gData.isClosing == 2) return GAMESCREEN_RETURN_SUCCESS;


	if(hasPressedStartFlank()) {
		addTimerCB(100, shutdownCB, &gData);
	}

	return GAMESCREEN_RETURN_CONTINUE;
}


GameScreenReturnType startTitleScreen(){
	setup();
	GameScreenReturnType ret = GAMESCREEN_RETURN_CONTINUE;
	while(ret == GAMESCREEN_RETURN_CONTINUE){
		update();
		draw();
		ret = getRunningState();
	}
	shutdownScreen();	

	return ret;
}

