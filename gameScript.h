#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include "script.h"

script* loadGameScript(char* path);
void unloadGameScript(script * this);
ScriptResult updateGameScript(script * this);
ScriptDrawingData getGameScriptDrawingData(script * this);

#endif
