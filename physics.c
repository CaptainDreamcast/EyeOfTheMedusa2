#include "physics.h"

#include <tari/physics.h>

static PhysicsData* gData;

void setupPhysics(PhysicsData* pData){
	resetPhysics();
	gData = pData;
}

void updatePhysics(PhysicsData* pData){
	(void)pData;
}

void shutdownPhysics(PhysicsData* pData){
	(void)pData;
}
