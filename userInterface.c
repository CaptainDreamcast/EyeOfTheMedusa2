#include "userInterface.h"

static UserInterfaceData* gData;

#define GUI_LOWER_Z 3
#define GUI_UPPER_Z 2


void setupUserInterface(UserInterfaceData* uData, PlayerData* pData){	

	uData->bombAmount = &pData->bomb.amount;
	uData->bombTexture = loadTexturePKG("/sprites/bomb_UI.pkg");
	uData->bombPosition = makePosition(100, 100, GUI_UPPER_Z);
	
	uData->lifeAmount = &pData->lifeAmount;
	uData->lifeTexture = loadTexturePKG("/sprites/life_UI.pkg");
	uData->lifePosition = makePosition(150, 100, GUI_UPPER_Z);
	
	uData->UITexture = loadTexturePKG("/sprites/main_UI.pkg");

	uData->score = 0;
	uData->scorePosition = makePosition(200, 100, GUI_UPPER_Z);

	uData->fireEnergy = &pData->shots.fireLevel;
	uData->fireEnergyPosition = makePosition(250, 100, GUI_UPPER_Z);
	uData->laserEnergy = &pData->shots.laserLevel;
	uData->laserEnergyPosition = makePosition(300, 100, GUI_UPPER_Z);
	uData->homingEnergy = &pData->shots.homingLevel;
	uData->homingEnergyPosition = makePosition(350, 100, GUI_UPPER_Z);

	uData->conversationText[0] = '\0';
	uData->conversationTextPosition = makePosition(400, 100, GUI_UPPER_Z);

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

void drawBombAmount(){
	drawMultipleAmount(gData->bombTexture, gData->bombPosition, *gData->bombAmount);
}

void drawLifeAmount(){
	drawMultipleAmount(gData->lifeTexture, gData->lifePosition, *gData->lifeAmount);
}


void drawScore(){
	char score[100];	
	sprintf(score, "%d", gData->score);
	drawText(score, gData->scorePosition, 20, COLOR_WHITE);	
}

void drawShotEnergies(){
	char text[100];	
	sprintf(text, "%d/128", *gData->fireEnergy);
	drawText(text, gData->fireEnergyPosition, 20, COLOR_WHITE);
	sprintf(text, "%d/128", *gData->laserEnergy);
	drawText(text, gData->laserEnergyPosition, 20, COLOR_WHITE);
	sprintf(text, "%d/128", *gData->homingEnergy);
	drawText(text, gData->homingEnergyPosition, 20, COLOR_WHITE);	
}

void drawConversation(){
	drawText(gData->conversationText, gData->conversationTextPosition, 20, COLOR_WHITE);	
}

void drawMainUI(){
	int w = gData->UITexture.mTextureSize.x;
	int h = gData->UITexture.mTextureSize.y;
	drawSprite(gData->UITexture, makePosition(0, 0, GUI_LOWER_Z), makeRectangle(0, 0, w, h));
}

void drawUserInterface(UserInterfaceData* uData){
	drawMainUI();
	drawBombAmount();
	drawLifeAmount();
	drawScore();
	drawShotEnergies();
	drawConversation();
}
