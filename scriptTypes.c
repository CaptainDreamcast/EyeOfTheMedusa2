#include "scriptTypes.h"

struct script_type GameScript = {
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
