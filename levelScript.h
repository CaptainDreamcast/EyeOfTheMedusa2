#ifndef LEVELSCRIPT_H
#define LEVELSCRIPT_H

#include "script.h"

script* loadLevelScript(char* path);
void unloadLevelScript(script * this);
ScriptResult updateLevelScript(script * this);
ScriptDrawingData getLevelScriptDrawingData(script * this);

#endif
