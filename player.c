#include "player.h"

#include <tari/input.h>

#include "collision.h"

#define SCREEN_SIZE_X 640
#define SCREEN_SIZE_Y 480


PlayerData* gData;

void playerHit(int shotID, CollisionType otherShotType){
	(void) shotID;
	(void) otherShotType;
	
}

void setupPlayer(PlayerData* pData){
	resetPhysicsObject(&pData->physics);
	pData->col = makeCollisionObjectCirc(makePosition(0, 0, 0), 0, &pData->physics);
	pData->isFocused = 0;
	pData->bomb.amount = 2;
	pData->bomb.active = 0;
	resetAnimation(&pData->bomb.animation);
	resetPhysicsObject(&pData->bomb.physics);
	pData->bomb.col = makeCollisionObjectRect(makePosition(0,0,0), makePosition(SCREEN_SIZE_X, SCREEN_SIZE_Y, 0), &pData->bomb.physics);
	pData->bomb.shotID = -1;
	(void) pData->bomb.textures; // TODO	
	(void) pData->shots; //TODO
	
	resetAnimation(&pData->animation);
	(void) pData->textures; // TODO
	pData->texturePosition = makeRectangle(0, 0, pData->textures[0].mTextureSize.x, pData->textures[0].mTextureSize.y);

	pData->collisionID = addPlayerCirc(&pData->col, playerHit);

	gData = pData;
	
}

void moveLeft(){
	gData->physics.mAcceleration.x = -1;
}

void moveRight(){
	gData->physics.mAcceleration.x = 1;
}

void moveUp() {
	gData->physics.mAcceleration.y = -1;
}

void moveDown() {
	gData->physics.mAcceleration.y = 1;
}

void normalizeMovement(){
	gData->physics.mAcceleration = normalizeVelocity(gData->physics.mAcceleration);
	
	if(gData->isFocused){
		gData->physics.mAcceleration = vecScale(gData->physics.mAcceleration, 1);
	} else {
		gData->physics.mAcceleration = vecScale(gData->physics.mAcceleration, 3);
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

void actionAfterBombHitsEnemy(int shotID, CollisionType otherShotType){
	(void) shotID;
	(void) otherShotType;
}

void usePlayerBomb(){
	if(!gData->bomb.amount) return;
	if(gData->bomb.active) return;

	gData->bomb.active = 1;
	gData->bomb.shotID = addPlayerShotRect(&gData->bomb.col, &gData->bomb.physics, &gData->bomb.animation, gData->bomb.textures, actionAfterBombHitsEnemy);
	gData->bomb.amount--;
}

void fireActionAfter(int shotID, CollisionType otherShotType){
	(void) otherShotType;
	removePlayerShot(shotID);
	
}

void updatePlayerShot(){
	PlayerShotData* sData = &gData->shots;
	if(handleDurationAndCheckIfOver(&sData->now, sData->duration)){
		if(sData->currentType == PLAYER_SHOT_FIRE){
			int level = sData->fireLevel;
			addPlayerShotCirc(&sData->fire[level].col, &sData->fire[level].physics, &sData->fire[level].animation, sData->fire[level].textures, fireActionAfter);
		}
		// TODO: OTHER SHOT TYPES
	}
}

void shootPlayerShot(){
	updatePlayerShot();
}

void updatePlayer(PlayerData* pData){

	updateInput();
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
}

void drawPlayer(PlayerData* pData){
	animate(&gData->animation);
	drawSprite(gData->textures[gData->animation.mFrame], gData->physics.mPosition, gData->texturePosition);
}

Position getPlayerPosition(){
	return gData->physics.mPosition;
}
