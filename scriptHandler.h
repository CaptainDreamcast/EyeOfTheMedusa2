#ifndef SCRIPTHANDLER_H
#define SCRIPTHANDLER_H

typedef struct{
	int mDummy;
} ScriptData;

void setupScripts(ScriptData* sData, char* mainScriptPath);
void shutdownScripts(ScriptData* sData);
void updateScripts(ScriptData* sData);
void drawScripts(ScriptData* sData);

#endif
