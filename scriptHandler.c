#include "scriptHandler.h"

#include <tari/log.h>
#include <tari/framerate.h>
#include <tari/file.h>
#include <tari/system.h>

#include "script.h"
#include "scriptTypes.h"
#include "gameStateLogic.h"

script* mainScript;

int isGameScript(char* path){
	char* file = getPureFileName(path);
	return file[0] == 'G';
}

int isLevelScript(char* path){
	char* file = getPureFileName(path);
	return file[0] == 'L';
}

int isStageScript(char* path){
	char* file = getPureFileName(path);
	return file[0] == 'S' && file[1] == 'T';
}

int isSectionScript(char* path){
	char* file = getPureFileName(path);
	return file[0] == 'S' && file[1] == 'E';
}

void setupScripts(ScriptData* sData, char* mainScriptPath){
	if(isGameScript(mainScriptPath)){
		mainScript = GameScript.load(mainScriptPath);		
	} else if(isLevelScript(mainScriptPath)){
		mainScript = LevelScript.load(mainScriptPath);		
	} else if(isStageScript(mainScriptPath)){
		mainScript = StageScript.load(mainScriptPath);		
	} else if(isSectionScript(mainScriptPath)){
		mainScript = SectionScript.load(mainScriptPath);		
	} else {
		logError("Unable to identify script type");
		logErrorString(mainScriptPath);
		abortSystem();
	}

}
void updateScripts(ScriptData* sData){
	ScriptResult res = mainScript->func.update(mainScript);
	if(res == SCRIPT_RESULT_END) {
		setGameWon();
	}
}

void drawScripts(ScriptData* sData){
	mainScript->func.getScriptDrawingData(mainScript);
}


