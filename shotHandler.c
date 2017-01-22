#include "shotHandler.h"

#include <tari/log.h>
#include <tari/drawing.h>

// TODO: rewrite with a set

typedef struct ShotElement_internal{
	int active;
	int id;	
	int type;
	Animation animation;
	PhysicsObject physics;
	TextureData* textures;
} ShotElement;

typedef struct {
	Animation animation;
	TextureData textures[1];
} ShotType;

typedef struct {

	int curID;

} ShotHandlerData;

#define SHOT_TYPE_PLAYER_F 0
#define SHOT_TYPE_PLAYER_L 1
#define SHOT_TYPE_PLAYER_H 2

int gShotTypeAmount;
ShotType gShotTypes[10];

ShotElement gShots[10000];
ShotHandlerData gData;


void loadSingleShot(char* name) {
	int id = gShotTypeAmount;
	char path[100];
	sprintf(path, "/assets/shots/%s.pkg", name);
	gShotTypes[id].textures[0] = loadTexturePKG(path);
	resetAnimation(&gShotTypes[id].animation);
	gShotTypes[id].animation.mFrameAmount = 1;
	gShotTypes[id].animation.mDuration = 1000;

	gShotTypeAmount++;

}

void loadShotTypes(){
	gShotTypeAmount = 0;
	
	loadSingleShot("ayaasas");
	
}

void setupShotHandling(){

	loadShotTypes();
	int i;
	for(i = 0; i < 10000; i++){
		gShots[i].active = 0;
	}

	gData.curID = 0;
}

int getNextShotId (){
	int start = gData.curID;
	while(gShots[gData.curID].active){
		gData.curID = (gData.curID+1) % 10000;
		if(gData.curID == start){
			logError("No more free shot spots");
			return -1;
		}
	}
	return gData.curID;
}

void updateShotHandling(){
	int i;
	for(i = 0; i < 10000; i++){
		if(!gShots[i].active) continue;
		animate(&gShots[i].animation);
		handlePhysics(&gShots[i].physics);	
	}
}


void drawShotHandling(){
	int i;
	for(i = 0; i < 10000; i++){
		if(!gShots[i].active) continue; 
		TextureData texture = gShots[i].textures[gShots[i].animation.mFrame];
		Rectangle rect = makeRectangle(0, 0, texture.mTextureSize.x, texture.mTextureSize.y);

		drawSprite(texture, gShots[i].physics.mPosition, rect);
		
	}
}


PhysicsObject* addToShotHandlingInternal(int shotID, PhysicsObject physics, Animation animation, TextureData* textures, int type){
	gShots[shotID].active = 1;
	gShots[shotID].id = shotID;
	gShots[shotID].type = type;
	gShots[shotID].physics = physics;
	gShots[shotID].animation = animation;
	gShots[shotID].textures = textures;
	return &gShots[shotID].physics;
}

PhysicsObject* addToShotHandling(int shotID, PhysicsObject physics, Animation animation, TextureData* textures){
	return addToShotHandlingInternal(shotID, physics, animation, textures, -1);
}

PhysicsObject* addToShotHandlingType(int shotID, PhysicsObject physics, int type){
	return addToShotHandlingInternal(shotID, physics, gShotTypes[type].animation, gShotTypes[type].textures, type);
}




void removeFromShotHandling(int shotID){
	gShots[shotID].active = 0;
}
