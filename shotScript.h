#ifndef SHOT_SCRIPT_H
#define SHOT_SCRIPT_H

#include "script.h"

script* loadShotScript(char* path);
void unloadShotScript(script * this);
ScriptResult updateShotScript(script * this);
ScriptDrawingData getShotScriptDrawingData(script * this); 
void updateShotScriptBasePosition(script * this, Position p);


#endif
