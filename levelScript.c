#include "levelScript.h"

#include <stdlib.h>

#include <tari/log.h>
#include <tari/system.h>
#include <tari/math.h>

#include "scriptTypes.h"

typedef struct{

	int cur;

} LevelScriptData;

void loadSubScripts(script* ret){
	int i = 0;
	ret->pointers.cur = ret->pointers.mainStart;
	while(ret->pointers.cur != NULL){
		char identifier[100];
		char scriptName[100];	

		ret->pointers.cur = getNextMedusaWord(ret->pointers.cur, identifier);
		ret->pointers.cur = getNextMedusaWord(ret->pointers.cur, scriptName);

		char path[100];
		getScriptPath(path, scriptName);

		if(!strcmp(identifier, "STAGE")){
			ret->subScripts[i] = StageScript.load(path);
		} else if(!strcmp(identifier, "SECTION")){
			ret->subScripts[i] = SectionScript.load(path);
		} else if(!strcmp(identifier, "BOSS")){
			logError("Not implemented");
			abortSystem();
		} else {
			logError("Unidentified Script Token");
			logErrorString(identifier);
		}	

		ret->pointers.cur = toNextInstruction(ret->pointers.cur, ret->pointers.mainEnd);
		i++;
	}

	ret->subScriptAmount = i;	
}

script* loadLevelScript(char* path){
	script* ret = malloc(sizeof(script));
	ret->func.load = loadLevelScript;
	ret->func.unload = unloadLevelScript;
	ret->func.update = updateLevelScript;
	ret->func.getScriptDrawingData = getLevelScriptDrawingData;

	initScriptData(ret, path);

	loadSubScripts(ret);

	ret->data = malloc(sizeof(LevelScriptData));
	LevelScriptData* data = ret->data;
	data->cur = 0;

	return ret;
}

void unloadLevelScript(script * this){
	free(this->data);
	destroyScript(this);
}

ScriptResult updateLevelScript(script * this){
	LevelScriptData* data = this->data;
	
	ScriptResult isSubScriptOver = this->subScripts[data->cur]->func.update(this->subScripts[data->cur]);
	if(isSubScriptOver == SCRIPT_RESULT_CONTINUE) return SCRIPT_RESULT_CONTINUE;

	data->cur++;
	int isOver = data->cur >= this->subScriptAmount;
	data->cur = min(data->cur, this->subScriptAmount-1);
	if(isOver) return SCRIPT_RESULT_END;
	
	return SCRIPT_RESULT_CONTINUE;
}


ScriptDrawingData getLevelScriptDrawingData(script * this){
	LevelScriptData* data = this->data;
	return this->subScripts[data->cur]->func.getScriptDrawingData(this->subScripts[data->cur]);
}
