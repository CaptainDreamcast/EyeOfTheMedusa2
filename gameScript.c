#include "gameScript.h"

#include <tari/file.h>

#include "levelScript.h"



int loadNextLevel(script* ret){
	if(ret->pointers.cur == NULL) return 0;

	char dummy[100];
	char levelName[100];

	sscanf(ret->pointers.cur , "%s %s", dummy, levelName);
	ret->pointers.cur = toNextInstruction(ret->pointers.cur, ret->pointers.mainEnd);

	char path[100];
	getScriptPath(path, levelName);
	ret->subScripts[0]->func.unload(ret->subScripts[0]);
	ret->subScripts[0] = loadLevelScript(path);
	ret->subScriptAmount = 1;

	return 1;
}

script* loadGameScript(char* path){
	script* ret = malloc(sizeof(script));
	ret->func.load = loadGameScript;
	ret->func.unload = unloadGameScript;
	ret->func.update = updateGameScript;
	ret->func.getScriptDrawingData = getGameScriptDrawingData;

	initScriptData(ret, path);	
	
	ret->pointers.cur = ret->pointers.mainStart;
	loadNextLevel(ret);
	return ret;
}

void unloadGameScript(script * this){
	
	destroyScript(this);
}

ScriptResult updateGameScript(script * this){

	ScriptResult isLevelOver = this->subScripts[0]->func.update(this->subScripts[0]);
	if(isLevelOver == SCRIPT_RESULT_END){
		int hasNextLevel = loadNextLevel(this);
		if(!hasNextLevel) return SCRIPT_RESULT_END;
	}

	return SCRIPT_RESULT_CONTINUE;
}

ScriptDrawingData getGameScriptDrawingData(script * this){
	return this->subScripts[0]->func.getScriptDrawingData(this->subScripts[0]);
}

