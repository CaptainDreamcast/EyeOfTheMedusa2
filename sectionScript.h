#ifndef SECTIONSCRIPT_H
#define SECTIONSCRIPT_H

#include "script.h"

script* loadSectionScript(char* path);
void unloadSectionScript(script * this);
ScriptResult updateSectionScript(script * this);
ScriptDrawingData getSectionScriptDrawingData(script * this);

#endif
