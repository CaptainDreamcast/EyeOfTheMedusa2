#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <tari/wrapper.h>

#include "scriptHandler.h"
#include "player.h"
#include "collision.h"
#include "physics.h"
#include "userInterface.h"

typedef enum {
	GAMESCREEN_RETURN_SUCCESS,
	GAMESCREEN_RETURN_FAILURE,
	GAMESCREEN_RETURN_CONTINUE,
	GAMESCREEN_RETURN_ABORT,
} GameScreenReturnType;

typedef struct {
	PhysicsData physics;
	CollisionData collision;
	PlayerData player;
	ScriptData scripts;
	UserInterfaceData userInterface;
	int winFlag;
	int gameOverFlag;
} GameScreenData;

extern Screen GameScreen;

void setMainScriptPath(char* tMainScriptPath);
#endif
