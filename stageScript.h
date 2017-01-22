#ifndef STAGESCRIPT_H
#define STAGESCRIPT_H

#include "scriptBase.h"

script* loadStageScript(char* path);
void unloadStageScript(script * this);
ScriptResult updateStageScript(script * this);
ScriptDrawingData getStageScriptDrawingData(script * this);

#endif
