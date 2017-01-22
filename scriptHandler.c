#include "scriptHandler.h"

#include <tari/log.h>
#include <tari/framerate.h>
#include <tari/file.h>

#include "script.h"
#include "scriptTypes.h"

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
	}

}
void updateScripts(ScriptData* sData){
	mainScript->func.update(mainScript);
}

void drawScripts(ScriptData* sData){
	ScriptDrawingData dData = mainScript->func.getScriptDrawingData(mainScript);
	int i;
	for(i = 0; i < dData.size; i++){
		drawSprite(dData.data[i].texture, dData.data[i].pos, dData.data[i].texturePosition);
	}
}


