#include "enemyScript.h"

#include <tari/animation.h>

#include "shotScript.h"

#define ENEMY_POSITION_Z 5

typedef struct{
	Animation animation;
	TextureData textures[10];
	PhysicsObject physics;

	int shotAmount;
	script* shotTypes[10];

	Duration duration;	
	Duration now;

} EnemyScriptData;

void loadEnemyTexture(script* this, EnemyScriptData* data){
	char path[100];
	this->pointers.cur = getNextWord(this->pointers.cur, path);


	data->textures[data->animation.mFrameAmount] = loadTexturePKG(path);
	data->animation.mFrameAmount++;
}

void loadEnemyShot(script* this, EnemyScriptData* data){
	char scriptName[100];
	this->pointers.cur = getNextWord(this->pointers.cur, scriptName);
	char path[100];
	getScriptPath(path, scriptName);

	int shot = data->shotAmount;
	data->shotAmount++;
	data->shotTypes[shot] = loadShotScript(path);
}


void loadEnemyAssets(script* this, EnemyScriptData* data){
	this->pointers.cur = this->pointers.loadStart;


	resetAnimation(&data->animation);
	resetPhysicsObject(&data->physics);
	data->physics.mPosition.z = ENEMY_POSITION_Z;	

	data->shotAmount = 0;	

	while(this->pointers.cur != NULL){
		char word[100];
		this->pointers.cur = getNextWord(this->pointers.cur, word);
			
		if(!strcmp("TEXTURE", word)){
			loadEnemyTexture(this, data);
		} else if(!strcmp("DURATION", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);
			data->animation.mDuration = v;
		} else if(!strcmp("POSITION", word)){
			int v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);
			data->physics.mPosition.x = v;
			this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);
			data->physics.mPosition.y = v;
		}else if(!strcmp("SHOT", word)){
			loadEnemyShot(this, data);
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
	for(i = 0; i < data->animation.mFrameAmount; i++){
		unloadTexture(data->textures[i]);
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
	this->pointers.cur = getNextWord(this->pointers.cur, word);
	if(!strcmp("SHOT", word)){
		int shotType;
		this->pointers.cur = getNextScriptInteger(this->pointers.cur, &shotType);
		int amount;
		this->pointers.cur = getNextScriptInteger(this->pointers.cur, &amount);
		
	
		int i;
		for(i = 0; i < amount; i++) {
			updateShotScriptBasePosition(data->shotTypes[shotType], data->physics.mPosition);
			data->shotTypes[shotType]->func.update(data->shotTypes[shotType]);
		}
	}		

	this->pointers.cur = toNextInstruction(this->pointers.cur, this->pointers.mainEnd);
}

ScriptResult updateEnemyScript(script * this){
	EnemyScriptData* data = this->data;

	animate(&data->animation);
	handlePhysics(&data->physics);

	if(isEnemyWaiting(this, data)) return SCRIPT_RESULT_CONTINUE;
	
	int isScriptOver = this->pointers.cur == NULL;
	if(isScriptOver) return SCRIPT_RESULT_CONTINUE;

	readNextEnemyInstruction(this, data);

	return SCRIPT_RESULT_CONTINUE;

}


ScriptDrawingData getEnemyScriptDrawingData(script * this){
	ScriptDrawingData ret;
	EnemyScriptData* data = this->data;

	TextureData texture = data->textures[data->animation.mFrame];
	Rectangle rect = makeRectangle(0, 0, texture.mTextureSize.x, texture.mTextureSize.y);

	drawSprite(texture, data->physics.mPosition, rect);

	return ret;
}




