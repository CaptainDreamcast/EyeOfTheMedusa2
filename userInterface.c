#include "userInterface.h"

static UserInterfaceData* gData;

#define GUI_LOWER_Z 11
#define GUI_UPPER_Z 12


void setupUserInterface(UserInterfaceData* uData, PlayerData* pData){	

	uData->bombAmount = &pData->bomb.amount;
	uData->bombTexture = loadTexturePKG("/sprites/bomb_UI.pkg");

	uData->lifeAmount = &pData->lifeAmount;
	uData->lifeTexture = loadTexturePKG("/sprites/life_UI.pkg");

	uData->UITexture = loadTexturePKG("/sprites/white.pkg");

	uData->fireEnergy = &pData->shots.fireLevel;

	gData = uData;
}

void shutdownUserInterface(UserInterfaceData* uData){	
	unloadTexture(uData->bombTexture);
	unloadTexture(uData->lifeTexture);
	unloadTexture(uData->UITexture);
}



void updateUserInterface(UserInterfaceData* uData){
	(void)uData;
}

void drawMultipleAmount(TextureData tData, Position sPos, int amount){
	int i;
	for(i = 0; i < amount; i++){
		int w = tData.mTextureSize.x;
		int h = tData.mTextureSize.y;
		drawSprite(tData, sPos, makeRectangle(0, 0, w, h));
		sPos.x += w + 10;
	}
}

static void drawBombAmount(){
	drawText("BOMBS: ",  makePosition(20, 433, GUI_UPPER_Z), 20, COLOR_BLACK);
	drawMultipleAmount(gData->bombTexture, makePosition(150, 430, GUI_UPPER_Z), *gData->bombAmount);
}

static void drawLifeAmount(){
	drawText("LIFES: ",  makePosition(20, 385, GUI_UPPER_Z), 20, COLOR_BLACK);
	drawMultipleAmount(gData->lifeTexture, makePosition(150, 380, GUI_UPPER_Z), *gData->lifeAmount);
}

static void drawShotEnergies(){
	char text[100];	
	sprintf(text, "SHOT ENERGY: %d/128", *gData->fireEnergy);
	drawText(text, makePosition(300, 400, GUI_UPPER_Z), 15, COLOR_BLACK);
}

void drawMainUI(){
	int w = gData->UITexture.mTextureSize.x;
	int h = gData->UITexture.mTextureSize.y;

	Position p = makePosition(0, 380, GUI_LOWER_Z);
	scaleDrawing3D(makePosition(640.0/w, 100.0/h, 1), p);
	drawSprite(gData->UITexture, p, makeRectangle(0, 0, w, h));
	setDrawingParametersToIdentity();
}

void drawUserInterface(UserInterfaceData* uData){
	drawMainUI();
	drawBombAmount();
	drawLifeAmount();
	drawShotEnergies();
}
