#ifndef SECTIONSCRIPT_H
#define SECTIONSCRIPT_H

#include "scriptBase.h"

script* loadSectionScript(char* path);
void unloadSectionScript(script * this);
void updateSectionScript(script * this, double deltaTime);
ScriptDrawingData getSectionScriptDrawingData(script * this);

#endif
