#include "shotScript.h"

#include <math.h>
#include <stdlib.h>

#include <tari/math.h>
#include <tari/log.h>
#include <tari/system.h>
#include <tari/drawing.h>

#include "collision.h"
#include "player.h"

#define ENEMY_SHOT_POSITION_Z 6

typedef struct{

	int shotType;
	int isTowardsPlayer;
	int isAngle;
	int angle;
	int speed;
	int strength;
	int isPositionAbsolute;
	Color color;
	PhysicsObject physics;
	PhysicsObject randomization;
	Position basePosition;
	CollisionCirc col;

} ShotScriptData;

Color stringToShotColor(char* s){
	if(!strcmp("BLACK", s)){
		return COLOR_BLACK;
	} else if(!strcmp("RED", s)){
		return COLOR_RED;
	} else if(!strcmp("GREEN", s)){
		return COLOR_GREEN;
	} else if(!strcmp("BLUE", s)){
		return COLOR_BLUE;
	} else if(!strcmp("YELLOW", s)){
		return COLOR_YELLOW;
	} else if(!strcmp("WHITE", s)){
		return COLOR_WHITE;
	} else if(!strcmp("RANDOM", s)){
		return -1;
	} else {
		logError("Unable to parse color.");
		logErrorString(s);
		abortSystem();
	}

	return COLOR_WHITE;
}

Color getRandomColor(){
	int id = (rand() % 5) + 1;
	return id;
}

void loadShotAssets(script* this, ShotScriptData* data){
	this->pointers.cur = this->pointers.loadStart;

	data->shotType = 0;
	data->isTowardsPlayer = 0;
	data->isAngle = 0;
	data->angle = 0;
	data->strength = INF;
	data->isPositionAbsolute = 0;
	resetPhysicsObject(&data->physics);
	resetPhysicsObject(&data->randomization);
	data->physics.mPosition.z = ENEMY_SHOT_POSITION_Z;	
	data->basePosition = makePosition(0, 0, 0);
	data->col = makeCollisionCirc(makePosition(0, 0, 0), 0);
	data->color = COLOR_WHITE;

	while(this->pointers.cur != NULL){
		char word[100];
		this->pointers.cur = getNextWord(this->pointers.cur, word);
			
		if(!strcmp("TYPE", word)){
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->shotType);
		} else if(!strcmp("POSITION", word)){
			this->pointers.cur = getNextScriptDouble(this->pointers.cur, &data->physics.mPosition.x);	
			this->pointers.cur = getNextScriptDouble(this->pointers.cur, &data->physics.mPosition.y);	
		} else if(!strcmp("VELOCITY", word)){
			this->pointers.cur = getNextScriptDouble(this->pointers.cur, &data->physics.mVelocity.x);	
			this->pointers.cur = getNextScriptDouble(this->pointers.cur, &data->physics.mVelocity.y);
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
		} else if(!strcmp("POSITION_ABSOLUTE", word)){
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &data->isPositionAbsolute);
		} else if(!strcmp("COLOR", word)){
			this->pointers.cur = getNextWord(this->pointers.cur, word);
			data->color = stringToShotColor(word);
		} else {
			logError("Unable to parse word");
			logErrorString(word);
			abortSystem();	
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

void removeShotScriptShot(void* this, int shotID, int strength){
	(void) this;
	(void) strength;
	removeEnemyShot(shotID);
}

ScriptResult updateShotScript(script * this){
	ShotScriptData* data = this->data;

	PhysicsObject physics;
	resetPhysicsObject(&physics);

	Position p = data->physics.mPosition;
	if(!data->isPositionAbsolute) p = vecAdd(data->basePosition, p);
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

	Color color = data->color;
	if(color == -1){
		color = getRandomColor();
	}

	addition = variatePosition(data->randomization.mVelocity);
	physics.mVelocity = vecAdd(physics.mVelocity, addition);
	physics.mVelocity = vecAdd(physics.mVelocity, data->physics.mVelocity);

	addition = variatePosition(data->randomization.mAcceleration);
	physics.mAcceleration = vecAdd(data->physics.mVelocity, addition);
	
	addEnemyShotCirc((void*)this, data->strength, data->col, data->shotType, physics, color, removeShotScriptShot);

	return SCRIPT_RESULT_END;
}

ScriptDrawingData getShotScriptDrawingData(script * this){
	ScriptDrawingData ret;
	ret.size = 0;
	return ret;

}

void updateShotScriptBasePosition(script * this, Position p){
	ShotScriptData* data = this->data;
	data->basePosition = p;
	data->basePosition.z = 0;
}
