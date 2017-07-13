#include "enemyScript.h"

#include <tari/animation.h>
#include <tari/collision.h>
#include <tari/log.h>
#include <tari/system.h>

#include "shotScript.h"
#include "collision.h"
#include "shotHandler.h"
#include "itemHandler.h"

#define ENEMY_POSITION_Z 5

typedef struct{
	Animation animation;
	TextureData textures[10];

	Animation deathAnimation;
	TextureData deathTextures[10];
	PhysicsObject physics;
	CollisionObjectCirc col;

	int shotAmount;
	script* shotTypes[100];

	int shotID;
	int health;
	int isDying;

	int dropType;
	int dropAmount;

	Duration lifeDuration;
	Duration lifeNow;

	Duration duration;	
	Duration now;

} EnemyScriptData;

void loadEnemyTexture(script* this, EnemyScriptData* data, TextureData* textures, Animation* animation){
	char path[100];
	this->pointers.cur = getNextMedusaWord(this->pointers.cur, path);


	textures[animation->mFrameAmount] = loadTexturePKG(path);
	animation->mFrameAmount++;
}

void loadEnemyShot(script* this, EnemyScriptData* data){
	char scriptName[100];
	this->pointers.cur = getNextMedusaWord(this->pointers.cur, scriptName);
	char path[100];
	getScriptPath(path, scriptName);

	int shot = data->shotAmount;
	data->shotAmount++;
	data->shotTypes[shot] = loadShotScript(path);
}


void loadEnemyAssets(script* this, EnemyScriptData* data){
	this->pointers.cur = this->pointers.loadStart;


	data->animation = createEmptyAnimation();
	resetPhysicsObject(&data->physics);
	data->physics.mPosition.z = ENEMY_POSITION_Z;	
	data->col = makeCollisionObjectCirc(makePosition(0, 0, 0), 0, &data->physics);
	data->shotID = -1;
	data->health = 0;
	data->lifeDuration = 0;
	data->lifeNow = 0;
	data->isDying = 0;
	data->now = 0;
	data->duration = 0;
	data->dropType = 0;
	data->dropAmount = 0;
	data->deathAnimation = createEmptyAnimation();

	data->shotAmount = 0;	

	while(this->pointers.cur != NULL){
		char word[100];
		this->pointers.cur = getNextMedusaWord(this->pointers.cur, word);
		
		if(!strcmp("TEXTURE", word)){
			loadEnemyTexture(this, data, data->textures, &data->animation);
		} if(!strcmp("DEATH_TEXTURE", word)){
			loadEnemyTexture(this, data, data->deathTextures, &data->deathAnimation);
		} else if(!strcmp("DURATION", word)){
			int v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->animation.mDuration = v;
		} else if(!strcmp("DEATH_DURATION", word)){
			int v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->deathAnimation.mDuration = v;
		} else if(!strcmp("POSITION", word)){
			int v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->physics.mPosition.x = v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->physics.mPosition.y = v;
		} else if(!strcmp("SHOT", word)){
			loadEnemyShot(this, data);
		} else if(!strcmp("COL_CENTER", word)){
			int v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->col.mCol.mCenter.x = v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->col.mCol.mCenter.y = v;
		} else if(!strcmp("COL_RADIUS", word)){
			int v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->col.mCol.mRadius = v;
		} else if(!strcmp("HEALTH", word)){
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &data->health);
		} else if(!strcmp("LIFE_TIME", word)){
			int v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->lifeDuration = v;
		}  else if(!strcmp("DROP", word)){
			this->pointers.cur = getNextMedusaWord(this->pointers.cur, word);
			if(!strcmp("POWER", word)) data->dropType = ITEM_TYPE_POWER;
			else {
				logError("Unable to parse drop type.");
				logErrorString(word);
			}
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &data->dropAmount);
		}

		this->pointers.cur = toNextInstruction(this->pointers.cur, this->pointers.loadEnd);
	}

}

script* loadEnemyScript(char* path){
	script* ret = malloc(sizeof(script));
	ret->func.load = loadEnemyScript;
	ret->func.unload = unloadEnemyScript;
	ret->func.update = updateEnemyScript;
	ret->func.getScriptDrawingData = getEnemyScriptDrawingData;

	initScriptData(ret, path);

	ret->data = malloc(sizeof(EnemyScriptData));
	EnemyScriptData* data = ret->data;
	loadEnemyAssets(ret, data);

	ret->pointers.cur = ret->pointers.mainStart;

	return ret;
}

void unloadTexturesAndShots(script* this, EnemyScriptData* data){
	int i;
	for(i = 0; i < (int)data->animation.mFrameAmount; i++){
		unloadTexture(data->textures[i]);
	}

	for(i = 0; i < (int)data->deathAnimation.mFrameAmount; i++){
		unloadTexture(data->deathTextures[i]);
	}	

	for(i = 0; i < data->shotAmount; i++){
		data->shotTypes[i]->func.unload(data->shotTypes[i]);
	}
}

void unloadEnemyScript(script * this){
	EnemyScriptData* data = this->data;
	unloadTexturesAndShots(this, data);
	free(this->data);
	destroyScript(this);
}

int isEnemyWaiting(script* this, EnemyScriptData* data){
	if(data->duration == 0) return 0;

	int isOver = handleDurationAndCheckIfOver(&data->now, data->duration);
	if(isOver){
		data->now = 0;
		data->duration = 0;
		return 0;
	}

	return 1;
}

void readNextEnemyInstruction(script* this, EnemyScriptData* data){
	char word[100];
	this->pointers.cur = getNextMedusaWord(this->pointers.cur, word);

	if(!strcmp("SHOT", word)){
		int shotType;
		this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &shotType);
		int amount;
		this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &amount);
		
	
		int i;
		for(i = 0; i < amount; i++) {
			updateShotScriptBasePosition(data->shotTypes[shotType], data->physics.mPosition);
			data->shotTypes[shotType]->func.update(data->shotTypes[shotType]);
		}
	} else if(!strcmp("WAIT", word)){
		int v;
		this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
		data->duration = v;
		data->now = 0;
	} else if(!strcmp("MOVE", word)){
			int v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->physics.mVelocity.x = v;
			this->pointers.cur = getNextMedusaScriptInteger(this->pointers.cur, &v);
			data->physics.mVelocity.y = v;
	} else if(!strcmp("STOP", word)){
			data->physics.mVelocity.x = data->physics.mVelocity.y = 0;
	} else {
		logError("Unrecognized token");
		logErrorString(word);
		abortSystem();
	}		

	this->pointers.cur = toNextInstruction(this->pointers.cur, this->pointers.mainEnd);
}

static void endEnemyScript(void* caller){
	script* this = caller;
	EnemyScriptData* data = this->data;
	data->isDying = 2;
}

static void die(script* this){
	EnemyScriptData* data = this->data;
	removeEnemy(data->shotID);
	Rectangle deathTexturePosition = makeRectangleFromTexture(data->deathTextures[0]);
	playAnimation(data->physics.mPosition, data->deathTextures, data->deathAnimation, deathTexturePosition, endEnemyScript, (void*)this);
	data->isDying = 1;
}

static void dropItems(script* this){
	EnemyScriptData* data = this->data;
	if(!data->dropType || !data->dropAmount) return;
	if(data->dropType == ITEM_TYPE_POWER) addPowerItems(data->physics.mPosition, data->dropAmount);
	else {
		logError("Unrecognized item type");
		logInteger(data->dropType);
	}
}

ScriptResult updateEnemyScript(script * this){
	EnemyScriptData* data = this->data;

	if(data->isDying == 1) return SCRIPT_RESULT_CONTINUE;	
	else if(data->isDying == 2) return SCRIPT_RESULT_END;	

	if(data->health <= 0){
		dropItems(this);
		die(this);
		return SCRIPT_RESULT_CONTINUE;	
	}

	animate(&data->animation);
	handlePhysics(&data->physics);

	if(isEnemyWaiting(this, data)) return SCRIPT_RESULT_CONTINUE;

	int isScriptOver = handleDurationAndCheckIfOver(&data->lifeNow, data->lifeDuration);
	if(isScriptOver) {
		die(this);
		return SCRIPT_RESULT_CONTINUE;
	}

	if(this->pointers.cur == NULL) this->pointers.cur = this->pointers.mainStart;

	readNextEnemyInstruction(this, data);

	return SCRIPT_RESULT_CONTINUE;
}


ScriptDrawingData getEnemyScriptDrawingData(script * this){
	ScriptDrawingData ret;
	ret.size = 0;
	EnemyScriptData* data = this->data;
	if(data->isDying) return ret;

	TextureData texture = data->textures[data->animation.mFrame];
	Rectangle rect = makeRectangleFromTexture(texture);
	drawSprite(texture, data->physics.mPosition, rect);

	return ret;
}

void takeDamage(void* this, int shotID, int strength) {
	script* s = (script*)this;
	EnemyScriptData* data = s->data;
	data->health -= strength;
	
}

void vitalizeEnemy(script* this){
	EnemyScriptData* data = this->data;
	data->shotID = addEnemyCirc((void*)this, &data->col, takeDamage);
	

}

