#include "script.h"

#include <tari/log.h>
#include <tari/file.h>

void initScriptData(script* ret, char* path){
	debugLog("Initialize Script.");
	ret->raw = fileToBuffer(path);
	ret->pointers.cur = (char*)ret->raw.mData;	
	ret->subScriptAmount = 0;

	ret->pointers.loadStart = strchr(ret->pointers.cur, '{')+1;
	ret->pointers.loadEnd = strchr(ret->pointers.loadStart, '}');
	ret->pointers.mainStart = strchr(ret->pointers.loadEnd, '{')+1;
	ret->pointers.mainEnd = strchr(ret->pointers.mainStart, '}');
}

char* toNextInstruction(char* pointer, char* end){
	pointer = strchr(pointer, '\n')+1;
	if(pointer > end) pointer = NULL;
	
	if(pointer != NULL){
		char testEnd[100];
		getNextWord(pointer, testEnd);
		if(testEnd[0] == '}') pointer = NULL;
	}

	return pointer;
}

char* getNextWord(char* scriptPointer, char* word){
	debugLog("Reading next word");
	int positionsRead;	
	sscanf(scriptPointer, "%s%n", word, &positionsRead);
	scriptPointer += positionsRead;
	debugInteger(positionsRead);
	debugString(word);
	
	return scriptPointer;
}

char* getNextScriptInteger(char* scriptPointer, int* v){
	debugLog("Reading next Integer");
	int positionsRead;	
	sscanf(scriptPointer, "%d%n", v, &positionsRead);
	scriptPointer += positionsRead;
	debugInteger(positionsRead);
	debugInteger(v);
	
	return scriptPointer;
}

char* getNextScriptDouble(char* scriptPointer, double* v){
	debugLog("Reading next Double");
	int positionsRead;	
	sscanf(scriptPointer, "%lf%n", v, &positionsRead);
	scriptPointer += positionsRead;
	debugInteger(positionsRead);
	debugDouble(v);
	
	return scriptPointer;
}

void getScriptPath(char* path, char* scriptName){
	sprintf(path, "/scripts/%s.txt", scriptName);
}

void destroyScript(script * this){
	int i;
	for(i = 0; i < this->subScriptAmount; i++){
		this->subScripts[i]->func.unload(this->subScripts[i]);
	}
	freeBuffer(this->raw);
	free(this);
}

