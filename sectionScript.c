#include "sectionScript.h"

#include <tari/animation.h>

#include <tari/log.h>
#include <tari/system.h>

#include "enemyScript.h"

typedef struct{
	int enemyAmount;
	int isEnemyAlive[10];
	script* subEnemies[10];
	int curEnemy;

	Duration now;
	Duration duration;

} SectionScriptData;

void loadSectionAssets(script* this, SectionScriptData* data){
		debugLog("Begin loading section script assets.");

		data->enemyAmount = 0;
		memset(data->isEnemyAlive, 0, sizeof data->isEnemyAlive);
		data->curEnemy = 0;
		data->now = 0;
		data->duration = 0;

		this->pointers.cur = this->pointers.mainStart;

		

		while(this->pointers.cur != NULL){
			char word[100];
			this->pointers.cur = getNextWord(this->pointers.cur, word);
			if(!strcmp("ENEMY", word)){
				char scriptName[100];
				char path[100];
				getNextWord(this->pointers.cur, scriptName);
				getScriptPath(path, scriptName);

				int enemy = data->enemyAmount;
				data->enemyAmount++;
				data->subEnemies[enemy] = loadEnemyScript(path);
			} else if(!strcmp("WAIT", word)){
				// no action
			} else {
				logError("Unable to parse nect section word!");
				logErrorString(word);
				abortSystem();
			}				
			
			this->pointers.cur = toNextInstruction(this->pointers.cur, this->pointers.mainEnd);
		}
}

script* loadSectionScript(char* path){
	debugLog("Begin loading section script.");
	debugString(path);
	script* ret = malloc(sizeof(script));
	ret->func.load = loadSectionScript;
	ret->func.unload = unloadSectionScript;
	ret->func.update = updateSectionScript;
	ret->func.getScriptDrawingData = getSectionScriptDrawingData;

	initScriptData(ret, path);

	ret->data = malloc(sizeof(SectionScriptData));
	SectionScriptData* data = ret->data;
	loadSectionAssets(ret, data);

	ret->pointers.cur = ret->pointers.mainStart;
	
	debugLog("Finished loading section script.");

	return ret;
}

void unloadSectionEnemies(script * this, SectionScriptData* data){
	int i;	
	for(i = 0; i < data->enemyAmount; i++){
		data->subEnemies[i]->func.unload(data->subEnemies[i]);
	}
}

void unloadSectionScript(script * this){
	SectionScriptData* data = this->data;
	unloadSectionEnemies(this, data);
	free(this->data);
	destroyScript(this);
}

int checkIfSectionDead(script* this, SectionScriptData* data){
	int i;
	for(i = 0; i < data->enemyAmount; i++){
		if(data->isEnemyAlive[i] != 2) return 0;
	}

	return 1;
}

void updateActiveSectionEnemies(script* this, SectionScriptData* data){

	int i;
	for(i = 0; i < data->enemyAmount; i++){
		if(data->isEnemyAlive[i] != 1) continue;
		ScriptResult isEnemyAlive = data->subEnemies[i]->func.update(data->subEnemies[i]);
		if(isEnemyAlive == SCRIPT_RESULT_END) data->isEnemyAlive[i] = 2;
	}
}

int checkIfSectionScriptOver(script* this){
	return this->pointers.cur == NULL;
}

int isSectionWaiting(script* this, SectionScriptData* data){
	if(data->duration == 0) return 0;

	int isOver = handleDurationAndCheckIfOver(&data->now, data->duration);
	if(isOver){
		data->now = 0;
		data->duration = 0;
		return 0;
	}

	return 1;
}

void readNextSectionInstruction(script* this, SectionScriptData* data){
	char word[100];
	this->pointers.cur = getNextWord(this->pointers.cur, word);
	if(!strcmp("ENEMY", word)){
		data->isEnemyAlive[data->curEnemy] = 1;
		vitalizeEnemy(data->subEnemies[data->curEnemy]);
		data->curEnemy++;
	} else if(!strcmp("WAIT", word)){
		int v;
		this->pointers.cur = getNextScriptInteger(this->pointers.cur, &v);
		data->duration = v;
		data->now = 0;
	}		

	this->pointers.cur = toNextInstruction(this->pointers.cur, this->pointers.mainEnd);
}

ScriptResult updateSectionScript(script * this){
	SectionScriptData* data = this->data;

	debugLog("Start updating section script.");
	if(checkIfSectionDead(this, data)) return SCRIPT_RESULT_END;

	debugLog("Section script not dead.");

	updateActiveSectionEnemies(this, data);

	if(isSectionWaiting(this, data)) return SCRIPT_RESULT_CONTINUE;
	
	debugLog("Section script not waiting.");

	int isScriptOver = checkIfSectionScriptOver(this);
	if(isScriptOver) return SCRIPT_RESULT_CONTINUE;

	debugLog("Section script not over.");

	readNextSectionInstruction(this, data);

	return SCRIPT_RESULT_CONTINUE;
}

ScriptDrawingData getSectionScriptDrawingData(script * this){
	ScriptDrawingData ret;
	SectionScriptData* data = this->data;

	int i;
	for(i = 0; i < data->enemyAmount; i++){
		if(data->isEnemyAlive[i] == 1) ret = data->subEnemies[i]->func.getScriptDrawingData(data->subEnemies[i]);
	}

	return ret;
}
