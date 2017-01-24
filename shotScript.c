#include "shotScript.h"

#include <math.h>

#include <tari/math.h>

#include "collision.h"
#include "player.h"

#define ENEMY_SHOT_POSITION_Z 4

typedef struct{

	int shotType;
	int isTowardsPlayer;
	int isAngle;
	int angle;
	int speed;
	int strength;
	PhysicsObject physics;
	PhysicsObject randomization;
	Position basePosition;
	CollisionCirc col;

} ShotScriptData;

void loadShotAssets(script* this, ShotScriptData* data){
	this->pointers.cur = this->pointers.loadStart;

	data->shotType = 0;
	data->isTowardsPlayer = 0;
	data->isAngle = 0;
	data->angle = 0;
	data->strength = INF;
	resetPhysicsObject(&data->physics);
	resetPhysicsObject(&data->randomization);
	data->physics.mPosition.z = ENEMY_SHOT_POSITION_Z;	
	data->basePosition = makePosition(0, 0, 0);
	data->col = makeCollisionCirc(makePosition(0, 0, 0), 0);

	while(this->pointers.cur != NULL){
		char word[100];
		this->pointers.cur = getNextWord(this->pointers.cur, word);
			
		if(!strcmp("TYPE", word)){
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->shotType);
		} else if(!strcmp("POSITION", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->physics.mPosition.x= v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->physics.mPosition.y= v;
		} else if(!strcmp("VELOCITY", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->physics.mVelocity.x= v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->physics.mVelocity.y= v;
		} else if(!strcmp("ACCEL", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->physics.mAcceleration.x= v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->physics.mAcceleration.y= v;
		} else if(!strcmp("RANDOMIZE_POSITION", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->randomization.mPosition.x= v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->randomization.mPosition.y= v;
		} else if(!strcmp("RANDOMIZE_VELOCITY", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->randomization.mVelocity.x= v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->randomization.mVelocity.y= v;
		} else if(!strcmp("RANDOMIZE_ACCEL", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->randomization.mAcceleration.x= v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);	
			data->randomization.mAcceleration.y= v;
		} else if(!strcmp("ANGLE", word)){
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->angle);
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->speed);
			data->isAngle = 1;
		} else if(!strcmp("TOWARDS_PLAYER", word)){
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->isTowardsPlayer);
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->speed);
		}  else if(!strcmp("HIT_CENTER", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);
			data->col.mCenter.x = v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);
			data->col.mCenter.y = v;
		} else if(!strcmp("HIT_RADIUS", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);
			data->col.mRadius = v;
		} else if(!strcmp("STRENGTH", word)){
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->strength);
		}

		this->pointers.cur = toNextInstruction(this->pointers.cur, this->pointers.loadEnd);
	}

}

script* loadShotScript(char* path){
	script* ret = malloc(sizeof(script));
	ret->func.load = loadShotScript;
	ret->func.unload = unloadShotScript;
	ret->func.update = updateShotScript;
	ret->func.getScriptDrawingData = getShotScriptDrawingData;

	initScriptData(ret, path);

	ret->data = malloc(sizeof(ShotScriptData));
	ShotScriptData* data = ret->data;
	loadShotAssets(ret, data);

	ret->pointers.cur = ret->pointers.mainStart;

	return ret;
}

void unloadShotScript(script * this){
	free(this->data);
	destroyScript(this);
}

Position variatePosition(Position base){
	Position ret;
	ret.x = randfrom(0, base.x);
	ret.y = randfrom(0, base.y);
	ret.z = randfrom(0, base.z);
	return ret;
}

void removeShotScriptShot(void* this, int shotID, int strength){
	(void) this;
	(void) strength;
	removeEnemyShot(shotID);
}

ScriptResult updateShotScript(script * this){
	ShotScriptData* data = this->data;

	PhysicsObject physics;
	resetPhysicsObject(&physics);

	Position p = vecAdd(data->basePosition, data->physics.mPosition);
	Position addition = variatePosition(data->randomization.mPosition);
	p = vecAdd(p, addition);
	physics.mPosition = p;

	if(data->isTowardsPlayer){
		Position player = getPlayerPosition();
		Position direction = getDirection(p, player);
		direction = normalizeVelocity(direction);
		physics.mVelocity = vecScale(direction, data->speed);
	}
	else if(data->isAngle){
		Position direction;
		direction.x = cos(data->angle);
		direction.y = sin(data->angle);
		direction = normalizeVelocity(direction);
		physics.mVelocity = vecScale(direction, data->speed);
	}

	addition = variatePosition(data->randomization.mVelocity);
	physics.mVelocity = vecAdd(data->physics.mVelocity, addition);

	addition = variatePosition(data->randomization.mAcceleration);
	physics.mAcceleration = vecAdd(data->physics.mVelocity, addition);
	
	addEnemyShotCirc((void*)this, data->strength, data->col, data->shotType, physics, removeShotScriptShot);

	return SCRIPT_RESULT_END;
}

ScriptDrawingData getShotScriptDrawingData(script * this){
	ScriptDrawingData ret;
	return ret;

}

void updateShotScriptBasePosition(script * this, Position p){
	ShotScriptData* data = this->data;
	data->basePosition = p;
	data->basePosition.z = 0;
}
