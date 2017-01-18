#include "player.h"

#include <tari/input.h>

PlayerData* gData;

void setupPlayer(PlayerData* pData){
	resetPhysicsObject(&pData->physics);
	pData->col = makeCollisionObjectCirc(makePosition(0, 0, 0), 0, &pData->physics);
	pData->isFocused = 0;

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

void drawPlayer(PlayerData* pData);
