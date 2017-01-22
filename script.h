#ifndef SCRIPT_H
#define SCRIPT_H

#include <tari/texture.h>
#include <tari/file.h>

#include "scriptBase.h"

struct script_type {
	script* (*load)(char* path);
	void (*unload)(script * this);
	ScriptResult (*update)(script * this);
	ScriptDrawingData (*getScriptDrawingData)(script * this);
};

typedef struct{
	char* loadStart;
	char* loadEnd;

	char* mainStart;
	char* mainEnd;

	char* cur;

} ScriptPointers;

typedef struct script_internal {
	struct script_type  func;	

	int subScriptAmount;
	struct script_internal* subScripts[100];
	
	ScriptPointers pointers;
	Buffer raw;	

	void* data;
} script;

void initScriptData(script* s, char* path);

char* toNextInstruction(char* scriptPointer, char* blockEnd);
char* getNextWord(char* scriptPointer, char* word);
char* getNextScriptInteger(char* scriptPointer, int* v);


void getScriptPath(char* outPath, char* scriptName);

void destroyScript(script* s);

#endif
