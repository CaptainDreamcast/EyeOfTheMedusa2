#include "userInterface.h"

UserInterfaceData* gData;

void setupUserInterface(UserInterfaceData* uData){	
	uData->bombTexture = loadTexturePKG("/UI/bombs.pkg");
	uData->lifeTexture = loadTexturePKG("/UI/life.pkg");
	uData->UITexture = loadTexturePKG("/UI/main.pkg");
	gData = uData;
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
	drawSprite(gData->UITexture, makePosition(0, 0, UI_MAIN_Z), makeRectangle(0, 0, w, h));
}

void drawUserInterface(UserInterfaceData* uData){
	drawMainUI();
	drawBombAmount();
	drawLifeAmount();
	drawScore();
	drawShotEnergies();
	drawConversation();
}
