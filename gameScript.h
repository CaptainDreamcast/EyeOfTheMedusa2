#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include "scriptBase.h"

script* loadGameScript(char* path);
void unloadGameScript(script * this);
void updateGameScript(script * this, double deltaTime);
ScriptDrawingData getGameScriptDrawingData(script * this);

#endif
