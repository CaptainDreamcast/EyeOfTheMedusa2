#include "player.h"

#include <stdlib.h>

#include <tari/input.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/math.h>
#include <tari/timer.h>

#include "collision.h"
#include "gameStateLogic.h"

#define SCREEN_SIZE_X 640
#define SCREEN_SIZE_Y 480

#define PLAYER_Z 5

static PlayerData* gData;


static void focusPlayer(){
	gData->isFocused = 1;
	setDrawPlayerFocus();
}

static void unfocusPlayer(){
	gData->isFocused = 0;
	setDoNotDrawPlayerFocus(); 
}

static void setVisible() {
	gData->isInvisible = 0;
}

static void setInvisible(){
	gData->isInvisible = 1;
	removePlayer(gData->collisionID);
	unfocusPlayer();
}

static void setVulnerable(void* caller); 

static void setInvincible() {
	gData->isInvincible = 1;
}

static void finishGame(void* this){
	if(gData->lifeAmount <= 0){
		setGameLost();
	} else {
		setInvincible();
		setVisible();
		addTimerCB(60, setVulnerable, NULL);
	}
}



static void receivePowerup(){
	PlayerShotData* sData = &gData->shots;
	if(sData->currentType == PLAYER_SHOT_FIRE){
			sData->fireLevel++;
			sData->fireLevel = min(128, sData->fireLevel);
	} else {
		logError("Unrecognized shot type.");
		logErrorInteger(sData->currentType);
	}
}

static void receiveLife(){
	gData->lifeAmount++;
}

static void halveShotPower() {
	if(gData->shots.currentType == PLAYER_SHOT_FIRE){
			gData->shots.fireLevel /= 2;
	} else {
		logError("Invalid Player shot type.");
		logErrorInteger(gData->shots.currentType);
		abortSystem();
	}
}

static void die(){
	

	gData->lifeAmount--;	
	halveShotPower();
	Rectangle deathTexturePosition = makeRectangleFromTexture(gData->deathTextures[0]);
	playAnimation(gData->physics.mPosition, gData->deathTextures, gData->deathAnimation, deathTexturePosition, finishGame, (void*)gData);
	setInvisible();

}

void playerHit(void* this, int shotID, int strength){
	if(strength > 0) die();
	else if(strength == -1) {
		receivePowerup();	
	} else if(strength == -2){
		receiveLife();
	}
}

static void setVulnerable(void* caller) {
	gData->isInvincible = 0;
	gData->collisionID = addPlayerCirc((void*)gData, &gData->col, playerHit);
}

void loadPlayerBombTexture(PlayerData* pData){
	pData->bomb.textures[0] = loadTexturePKG("/sprites/bomb1.pkg");
	pData->bomb.animation.mFrameAmount = 1;	
}

void unloadPlayerBombTexture(PlayerData* pData){
	int i;
	for(i = 0; i < (int)pData->bomb.animation.mFrameAmount; i++){
		unloadTexture(pData->bomb.textures[i]);
	}
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
	fData->duration[0] = 20;
	int i;
	for(i = 2; i < 17; i+=2) {
		fData->duration[i] = fData->duration[i-1] = fData->duration[i-2] - 1;
	}
}

void unloadPlayerShotFire(PlayerShotFire* fData){	
	int i;
	for(i = 0; i < (int)fData->animation.mFrameAmount; i++) unloadTexture(fData->textures[i]);	
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

void unloadPlayerShots(PlayerData* pData){
	PlayerShotData* sData = &pData->shots;
	unloadPlayerShotFire(&sData->fire);
}

void loadPlayerTextures(PlayerData* pData){
	pData->animation = createEmptyAnimation();
	pData->animation.mFrameAmount = 1;
	pData->textures[0] = loadTexturePKG("/sprites/kat1.pkg");
	pData->texturePosition = makeRectangle(0, 0, pData->textures[0].mTextureSize.x - 1, pData->textures[0].mTextureSize.y - 1);	
	pData->deathAnimation = createEmptyAnimation();
	pData->deathAnimation.mFrameAmount = 3;
	pData->deathAnimation.mDuration = 10;
	pData->deathTextures[0] = loadTexturePKG("/sprites/kat_death1.pkg");
	pData->deathTextures[1] = loadTexturePKG("/sprites/kat_death2.pkg");
	pData->deathTextures[2] = loadTexturePKG("/sprites/kat_death3.pkg");
	
}


void unloadPlayerTextures(PlayerData* pData){
	int i;
	for(i = 0; i < (int)pData->animation.mFrameAmount; i++) unloadTexture(pData->textures[i]);
	for(i = 0; i < (int)pData->deathAnimation.mFrameAmount; i++) unloadTexture(pData->deathTextures[i]);	
}


void setupPlayer(PlayerData* pData){
	pData->lifeAmount = 3;
	pData->isInvisible = 0;
	pData->isInvincible = 0;
	resetPhysicsObject(&pData->physics);
	pData->physics.mPosition.x = 100;
	pData->physics.mPosition.y = 100;
	pData->physics.mPosition.z = PLAYER_Z;
	pData->col = makeCollisionObjectCirc(makePosition(32, 16, 0), 3, &pData->physics);
	pData->colCollection = makeCollisionObjectCirc(makePosition(32, 16, 0), 10, &pData->physics);
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
	pData->collectionID = addPlayerCollectCirc((void*)pData, &pData->colCollection, playerHit);

	gData = pData;
	
}

void shutdownPlayer(PlayerData* pData){
	unloadPlayerBombTexture(pData);
	unloadPlayerShots(pData);
	
	unloadPlayerTextures(pData);	
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

void shutdownBomb(void* caller){
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

	halveShotPower();
	removeAllEnemyShots();

	addTimerCB(120, shutdownBomb, NULL);
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
			int level = sData->fireLevel / 8;
			duration = sData->fire.duration[level];
	} else {
		logError("Invalid Player shot type.");
		logErrorInteger(sData->currentType);
		abortSystem();
	}

	if(handleDurationAndCheckIfOver(&sData->now, duration)){
		if(sData->currentType == PLAYER_SHOT_FIRE){
			int level = sData->fireLevel / 8;
			PhysicsObject pos = sData->fire.physics;
			pos.mPosition = vecAdd(sData->fire.physics.mPosition, gData->physics.mPosition);
			addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, pos, sData->fire.animation, sData->fire.textures, fireActionAfter);
			if(level >= 4){
				PhysicsObject copy = pos;
				copy.mPosition.y -= 10;
				addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, copy, sData->fire.animation, sData->fire.textures, fireActionAfter);
				copy = pos;
				copy.mPosition.y += 10;
				addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, copy, sData->fire.animation, sData->fire.textures, fireActionAfter);
			}
			if(level >= 8){
				PhysicsObject copy = pos;
				copy.mPosition.y -= 20;
				addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, copy, sData->fire.animation, sData->fire.textures, fireActionAfter);
				copy = pos;
				copy.mPosition.y += 20;
				addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, copy, sData->fire.animation, sData->fire.textures, fireActionAfter);
			}
			if(level >= 16){
				PhysicsObject copy = pos;
				copy.mPosition.y -= 30;
				addPlayerShotCirc((void*)gData, sData->fire.strength, sData->fire.col, copy, sData->fire.animation, sData->fire.textures, fireActionAfter);
				copy = pos;
				copy.mPosition.y += 30;
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
	if(pData->isInvisible) return;

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
	gData->physics.mPosition.x = max(0, gData->physics.mPosition.x);
	gData->physics.mPosition.x = min(640-20, gData->physics.mPosition.x);
	gData->physics.mPosition.y = max(0, gData->physics.mPosition.y);
	gData->physics.mPosition.y = min(380-22, gData->physics.mPosition.y);
	

}

void drawPlayer(PlayerData* pData){
	if(pData->isInvisible) return;
	animate(&gData->animation);
	
	if(pData->isInvincible) {
		setDrawingTransparency(0.4);
	}
	drawSprite(gData->textures[gData->animation.mFrame], gData->physics.mPosition, gData->texturePosition);
	setDrawingParametersToIdentity();	
}

Position getPlayerPosition(){
	return gData->physics.mPosition;
}
