#ifndef STAGESCRIPT_H
#define STAGESCRIPT_H

#include "scriptBase.h"

script* loadStageScript(char* path);
void unloadStageScript(script * this);
void updateStageScript(script * this, double deltaTime);
ScriptDrawingData getStageScriptDrawingData(script * this);

#endif
