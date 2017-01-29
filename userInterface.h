#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <tari/texture.h>
#include <tari/drawing.h>

#include "player.h"

typedef struct{

	int* bombAmount;
	TextureData bombTexture;

	int* lifeAmount;
	TextureData lifeTexture;

	TextureData UITexture;
	TextureData pauseTexture;

	int* fireEnergy;

	int isPaused;
	int pauseID;

} UserInterfaceData;

void setupUserInterface(UserInterfaceData* uData, PlayerData* pData);
void shutdownUserInterface(UserInterfaceData* uData);
void updateUserInterface(UserInterfaceData* uData);
void drawUserInterface(UserInterfaceData* uData);

#endif
