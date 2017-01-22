#include "script.h"

void initScriptData(script* ret, char* path){
	ret->raw = fileToBuffer(path);
	ret->pointers.cur = (char*)ret->raw.data;	
	
	ret->pointers.loadStart = strchr(ret->pointers.cur, '{')+1;
	ret->pointers.loadEnd = strchr(ret->pointers.loadStart, '}');

	ret->pointers.loadStart = strchr(ret->pointers.loadEnd, '{')+1;
	ret->pointers.loadEnd = strchr(ret->pointers.mainStart, '}');
}

char* toNextInstruction(char* pointer, char* end){
	pointer = strchr(pointer, ';')+1;
	if(pointer > end) pointer = NULL;
	return pointer;
}

char* getNextWord(char* scriptPointer, char* word){
	int positionsRead;	
	sscanf(scriptPointer, "%s%n", word, &positionsRead);
	scriptPointer += positionsRead;
	return scriptPointer;
}

char* getNextScriptInteger(char* scriptPointer, int* v){
	int positionsRead;	
	sscanf(scriptPointer, "%d%n", v, &positionsRead);
	scriptPointer += positionsRead;
	return scriptPointer;
}

void getScriptPath(char* path, char* scriptName){
	sprintf(path, "/rd/scripts/%s.txt", scriptName);
}

void destroyScript(script * this){
	int i;
	for(i = 0; i < this->subScriptAmount; i++){
		this->subScripts[i]->func.unload(this->subScripts[i]);
	}
	freeBuffer(this->raw);
	free(this);
}

