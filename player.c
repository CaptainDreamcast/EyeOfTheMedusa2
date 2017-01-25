#include "player.h"

#include <tari/input.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/math.h>

#include "collision.h"


#define SCREEN_SIZE_X 640
#define SCREEN_SIZE_Y 480

#define PLAYER_Z 5

static PlayerData* gData;

void playerHit(void* this, int shotID, int strength){
	(void) this;
	(void) shotID;
	(void) strength;
	
}

void loadPlayerBombTexture(PlayerData* pData){
	pData->bomb.textures[0] = loadTexturePKG("/sprites/bomb1.pkg");
	pData->bomb.animation.mFrameAmount = 1;	
}

void loadPlayerShotFire(PlayerShotFire* fData){	
	resetPhysicsObject(&fData->physics);
	fData->strength = 100;
	fData->physics.mPosition.x = 5;
	fData->physics.mVelocity.x = 5;
	fData->col = makeCollisionCirc(makePosition(8,8,0), 8);
	fData->animation = createEmptyAnimation();
	fData->animation.mFrameAmount = 1;
	fData->animation.mDuration = 5;
	fData->textures[0] = loadTexturePKG("/sprites/fire1.pkg");	
	fData->duration[0] = 10;
	fData->duration[1] = 10;
	fData->duration[2] = 5;
	fData->duration[3] = 1;
}

void loadPlayerShots(PlayerData* pData){
	PlayerShotData* sData = &pData->shots;
	sData->now = 0;
	sData->currentType = PLAYER_SHOT_FIRE;
	sData->fireLevel = 0;
	loadPlayerShotFire(&sData->fire);
	// TODO: LASER
	sData->laserLevel = 0;
	// TODO: HOMING
	sData->homingLevel = 0;	
}

void loadPlayerTextures(PlayerData* pData){
	pData->animation = createEmptyAnimation();
	pData->animation.mFrameAmount = 1;
	pData->textures[0] = loadTexturePKG("/sprites/kat1.pkg");
	pData->texturePosition = makeRectangle(0, 0, pData->textures[0].mTextureSize.x - 1, pData->textures[0].mTextureSize.y - 1);
}

void setupPlayer(PlayerData* pData){
	pData->lifeAmount = 3;
	resetPhysicsObject(&pData->physics);
	pData->physics.mPosition.x = 100;
	pData->physics.mPosition.y = 100;
	pData->physics.mPosition.z = PLAYER_Z;
	pData->col = makeCollisionObjectCirc(makePosition(32, 16, 0), 6, &pData->physics);
	pData->isFocused = 0;
	pData->bomb.amount = 2;
	pData->bomb.active = 0;
	pData->bomb.animation = createEmptyAnimation();
	resetPhysicsObject(&pData->bomb.physics);
	pData->bomb.col = makeCollisionRect(makePosition(0,0,0), makePosition(SCREEN_SIZE_X, SCREEN_SIZE_Y, 0));
	pData->bomb.shotID = -1;
	loadPlayerBombTexture(pData);
	loadPlayerShots(pData);
	
	loadPlayerTextures(pData);

	pData->collisionID = addPlayerCirc((void*)pData, &pData->col, playerHit);

	gData = pData;
	
}

void moveLeft(){
	gData->physics.mVelocity.x = -1;
}

void moveRight(){
	gData->physics.mVelocity.x = 1;
}

void moveUp() {
	gData->physics.mVelocity.y = -1;
}

void moveDown() {
	gData->physics.mVelocity.y = 1;
}

void normalizeMovement(){
	if(vecLength(gData->physics.mVelocity) > 0){
		gData->physics.mVelocity = normalizeVelocity(gData->physics.mVelocity);
	}

	if(gData->isFocused){
		gData->physics.mVelocity = vecScale(gData->physics.mVelocity, 1);
	} else {
		gData->physics.mVelocity = vecScale(gData->physics.mVelocity, 3);
	}
}

void focusPlayer(){
	gData->isFocused = 1;
}

void unfocusPlayer(){
	gData->isFocused = 0; 
}

void shutdownBomb(){
	gData->bomb.active = 0;
	removePlayerShot(gData->bomb.shotID);
	resetAnimation(&gData->bomb.animation);
}

void actionAfterBombHitsEnemy(void* this, int shotID, int strength){
	(void) this;
	(void) shotID;
	(void) strength;
}

void usePlayerBomb(){
	if(!gData->bomb.amount) return;
	if(gData->bomb.active) return;

	gData->bomb.active = 1;
	gData->bomb.shotID = addPlayerShotRect((void*)gData, INF, gData->bomb.col, gData->bomb.physics, gData->bomb.animation, gData->bomb.textures, actionAfterBombHitsEnemy);
	gData->bomb.amount--;
}

void fireActionAfter(void* this, int shotID, int strength){
	(void) this;
	(void) strength;
	removePlayerShot(shotID);
	
}

void updatePlayerShot(){
	PlayerShotData* sData = &gData->shots;
	Duration duration = 0;
	if(sData->currentType == PLAYER_SHOT_FIRE){
			int level = sData->fireLevel;
			duration = sData->fire.duration[level];
	} else {
		logError("Invalid Player shot type.");
		logErrorInteger(sData->currentType);
		abortSystem();
	}

	if(handleDurationAndCheckIfOver(&sData->now, duration)){
		if(sData->currentType == PLAYER_SHOT_FIRE){
			int level = sData->fireLevel;
			PhysicsObject pos = sData->fire.physics;
			pos.mPosition = vecAdd(sData->fire.physics.mPosition, gData->physics.mPosition);
			addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, pos, sData->fire.animation, sData->fire.textures, fireActionAfter);
			if(level >= 3){
				PhysicsObject copy = pos;
				copy.mPosition.y -= 10;
				addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, copy, sData->fire.animation, sData->fire.textures, fireActionAfter);
				copy = pos;
				copy.mPosition.y += 10;
				addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, copy, sData->fire.animation, sData->fire.textures, fireActionAfter);

			}
		} else {
			logError("Invalid Player shot type.");
			logErrorInteger(sData->currentType);
		}
		// TODO: OTHER SHOT TYPES
		sData->now = 0;
	}
}

void shootPlayerShot(){
	updatePlayerShot();
}

void updatePlayer(PlayerData* pData){
	if(hasPressedA()){
		shootPlayerShot();
	}

	if(hasPressedBFlank()){
		usePlayerBomb();
	}

	if(hasPressedR()){
		focusPlayer();
	} else {
		unfocusPlayer();
	}
	if(hasPressedLeft()){
		moveLeft();
	}
	if(hasPressedRight()){
		moveRight();
	}
	if(hasPressedUp()){
		moveUp();
	}
	if(hasPressedDown()){
		moveDown();
	}

	normalizeMovement();

	handlePhysics(&gData->physics);
	gData->physics.mVelocity = makePosition(0,0,0);

}

void drawPlayer(PlayerData* pData){
	animate(&gData->animation);
	
	drawSprite(gData->textures[gData->animation.mFrame], gData->physics.mPosition, gData->texturePosition);	
}

Position getPlayerPosition(){
	return gData->physics.mPosition;
}
