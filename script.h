#ifndef SCRIPT_H
#define SCRIPT_H

#include <tari/texture.h>

#include "scriptBase.h"
#include "gameScript.h"
#include "levelScript.h"
#include "stageScript.h"
#include "sectionScript.h"
#include "enemyScript.h"

struct script_type {
	script* (*load)(char* path);
	void (*unload)(script * this);
	void (*update)(script * this, double deltaTime);
	ScriptDrawingData (*getScriptDrawingData)(script * this);
} GameScript = {
	.load = loadGameScript,
	.unload = unloadGameScript,
	.update = updateGameScript,
	.getScriptDrawingData = getGameScriptDrawingData
}, LevelScript = {
	.load = loadLevelScript,
	.unload = unloadLevelScript,
	.update = updateLevelScript,
	.getScriptDrawingData = getLevelScriptDrawingData
}, StageScript = {
	.load = loadStageScript,
	.unload = unloadStageScript,
	.update = updateStageScript,
	.getScriptDrawingData = getStageScriptDrawingData
}, SectionScript = {
	.load = loadSectionScript,
	.unload = unloadSectionScript,
	.update = updateSectionScript,
	.getScriptDrawingData = getSectionScriptDrawingData
}, EnemyScript = {
	.load = loadEnemyScript,
	.unload = unloadEnemyScript,
	.update = updateEnemyScript,
	.getScriptDrawingData = getEnemyScriptDrawingData
}, ShotScript = {
	.load = loadEnemyScript,
	.unload = unloadEnemyScript,
	.update = updateEnemyScript,
	.getScriptDrawingData = getEnemyScriptDrawingData 
};

typedef struct script_internal {
	struct script_type  func;
} script;

#endif
