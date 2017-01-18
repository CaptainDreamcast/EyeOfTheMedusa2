#ifndef ENEMYSCRIPT_H
#define ENEMYSCRIPT_H

#include "scriptBase.h"

script* loadEnemyScript(char* path);
void unloadEnemyScript(script * this);
void updateEnemyScript(script * this, double deltaTime);
ScriptDrawingData getEnemyScriptDrawingData(script * this);

#endif
