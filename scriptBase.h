#ifndef SCRIPTBASE_H
#define SCRIPTBASE_H

#include <tari/drawing.h>

typedef struct script_internal script;

typedef enum {
	SCRIPT_RESULT_CONTINUE,
	SCRIPT_RESULT_END,
} ScriptResult;

typedef struct{
	TextureData texture;
	Position pos;
	Rectangle texturePosition;
} SingleScriptDrawingData;

typedef struct{
	int size;
} ScriptDrawingData;

#endif
