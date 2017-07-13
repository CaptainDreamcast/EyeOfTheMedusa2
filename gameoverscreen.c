#include "gameoverscreen.h"

#include <tari/timer.h>
#include <tari/drawing.h>
#include <tari/optionhandler.h>
#include <tari/input.h>
#include <tari/log.h>
#include <tari/system.h>

#include "gamescreen.h"
#include "titlescreen.h"

static struct {
	TextureData background;	
	Screen* mReturn;
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
	gData.mReturn = &GameScreen;
	waitForCB();
}

static void noContinueCB(void* caller){
	gData.mReturn = &TitleScreen;
	waitForCB();
}

static void setup(){
	gData.background = loadTexturePKG("/sprites/GAMEOVER.pkg");
	gData.mReturn = NULL;
	gData.isWaiting = 0;

	setupOptionHandler();
	addOption(makePosition(280, 330, OPTION_Z), "YES", continueCB, NULL);
	addOption(makePosition(280, 380, OPTION_Z), "NO", noContinueCB, NULL);
	setOptionTextSize(20);
	setOptionButtonA();
	setOptionButtonStart();

	hasPressedAFlank(); hasPressedStartFlank(); // TODO: remove when fixed
}

static void shutdownScreen(){
	unloadTexture(gData.background);
	shutdownOptionHandler();
}

static void updateState() {
	if (hasPressedAbortFlank()) {
		setNewScreen(&TitleScreen);
	}

	if(gData.isWaiting == 2) setNewScreen(gData.mReturn);

}

static void update() {
	updateSystem();
	updateInput();
	updateOptionHandler();
	updateTimer();
	updateState();
}

static void drawBackground() {
	Rectangle r = makeRectangleFromTexture(gData.background);
	drawSprite(gData.background, makePosition(0, 0, BACKGROUND_Z), r);
}

static void draw() {
	drawBackground();
	drawOptionHandler();
}



Screen GameOverScreen = {
	.mLoad = setup,
	.mUnload = shutdownScreen,
	.mUpdate = update,
	.mDraw = draw,
};