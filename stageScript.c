#ifndef STAGESCRIPT_H
#define STAGESCRIPT_H

#include "script.h"

script* loadStageScript(char* path){
	return NULL;
}

void unloadStageScript(script * this){

}

ScriptResult updateStageScript(script * this){
	return SCRIPT_RESULT_END;
}

ScriptDrawingData getStageScriptDrawingData(script * this){
	ScriptDrawingData d;
	d.size = 0;
	return d;
}

#endif
