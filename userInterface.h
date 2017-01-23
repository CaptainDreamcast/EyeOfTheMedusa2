#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <tari/texture.h>
#include <tari/drawing.h>

#include "player.h"

typedef struct{

	int* bombAmount;
	TextureData bombTexture;
	Position bombPosition;

	int* lifeAmount;
	TextureData lifeTexture;
	Position lifePosition;

	TextureData UITexture;

	int score;
	Position scorePosition;

	int* fireEnergy;
	Position fireEnergyPosition;
	int* laserEnergy;
	Position laserEnergyPosition;
	int* homingEnergy;
	Position homingEnergyPosition;

	char conversationText[100];
	Position conversationTextPosition;
	

} UserInterfaceData;

void setupUserInterface(UserInterfaceData* uData, PlayerData* pData);
void updateUserInterface(UserInterfaceData* uData);
void drawUserInterface(UserInterfaceData* uData);

#endif
