#ifndef SCRIPTBASE_H
#define SCRIPTBASE_H

#include <tari/drawing.h>

typedef struct script_internal script;

typedef struct{
	TextureData texture;
	Position pos;
	Rectangle texturePosition;
} SingleScriptDrawingData;

typedef struct{
	int size;
	SingleScriptDrawingData data[10000];
} ScriptDrawingData;

#endif
