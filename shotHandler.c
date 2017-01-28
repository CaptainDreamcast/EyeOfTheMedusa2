#include "shotHandler.h"

#include <tari/log.h>
#include <tari/drawing.h>

// TODO: rewrite with a set

typedef struct ShotElement_internal{
	int active;
	int id;	
	int type;
	Color color;
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

static int gShotTypeAmount;
static ShotType gShotTypes[10];

static ShotElement gShots[10000];
static ShotHandlerData gData;


void loadSingleShot(char* name) {
	int id = gShotTypeAmount;
	char path[100];
	sprintf(path, "/sprites/%s.pkg", name);
	gShotTypes[id].textures[0] = loadTexturePKG(path);
	gShotTypes[id].animation = createEmptyAnimation();
	gShotTypes[id].animation.mFrameAmount = 1;
	gShotTypes[id].animation.mDuration = 1000;

	gShotTypeAmount++;

}

void loadShotTypes(){
	gShotTypeAmount = 0;
	
	loadSingleShot("shot1");
	loadSingleShot("power_item");
}

void unloadShotTypes(){
	int i;
	for(i = 0; i < gShotTypeAmount; i++){
		int j;
		for(j = 0; j < gShotTypes[i].animation.mFrameAmount; j++){
			unloadTexture(gShotTypes[i].textures[j]);
		}
	}
}

void setupShotHandling(){

	loadShotTypes();
	int i;
	for(i = 0; i < 10000; i++){
		gShots[i].active = 0;
	}

	gData.curID = 0;
}

void shutdownShotHandling(){

	unloadShotTypes();
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

#include <math.h>

void drawShotHandling(){
	int i;
	for(i = 0; i < 10000; i++){
		if(!gShots[i].active) continue; 
		TextureData texture = gShots[i].textures[gShots[i].animation.mFrame];
		Rectangle rect = makeRectangleFromTexture(texture);
		double angle;
		if(!vecLength(gShots[i].physics.mVelocity)) angle = 0;
		else angle = getAngleFromDirection(gShots[i].physics.mVelocity);

		Position rotPosition = vecAdd(gShots[i].physics.mPosition, getTextureMiddlePosition(texture));		
		setDrawingRotationZ(angle, rotPosition);

		setDrawingBaseColor(gShots[i].color);
		drawSprite(texture, gShots[i].physics.mPosition, rect);
		setDrawingParametersToIdentity();
	}
}



PhysicsObject* addToShotHandlingInternal(int shotID, PhysicsObject physics, Animation animation, TextureData* textures, int type, Color color){
	debugLog("Adding shot.");
	debugInteger(shotID);
	debugInteger(type);

	gShots[shotID].active = 1;
	gShots[shotID].id = shotID;
	gShots[shotID].type = type;
	gShots[shotID].physics = physics;
	gShots[shotID].animation = animation;
	gShots[shotID].textures = textures;
	gShots[shotID].color = color;
	return &gShots[shotID].physics;
}

PhysicsObject* addToShotHandling(int shotID, PhysicsObject physics, Animation animation, TextureData* textures, Color color){
	return addToShotHandlingInternal(shotID, physics, animation, textures, -1, color);
}

PhysicsObject* addToShotHandlingType(int shotID, PhysicsObject physics, int type, Color color){
	return addToShotHandlingInternal(shotID, physics, gShotTypes[type].animation, gShotTypes[type].textures, type, color);
}




void removeFromShotHandling(int shotID){
	gShots[shotID].active = 0;
}
