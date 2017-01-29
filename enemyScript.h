#ifndef ENEMYSCRIPT_H
#define ENEMYSCRIPT_H

#include "script.h"

script* loadEnemyScript(char* path);
void unloadEnemyScript(script * this);
ScriptResult updateEnemyScript(script * this);
ScriptDrawingData getEnemyScriptDrawingData(script * this);
void vitalizeEnemy(script* this);

#endif
