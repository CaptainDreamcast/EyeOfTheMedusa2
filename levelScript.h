#ifndef LEVELSCRIPT_H
#define LEVELSCRIPT_H

#include "scriptBase.h"

script* loadLevelScript(char* path);
void unloadLevelScript(script * this);
void updateLevelScript(script * this, double deltaTime);
ScriptDrawingData getLevelScriptDrawingData(script * this);

#endif
