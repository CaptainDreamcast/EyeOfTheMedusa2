#include "physics.h"

#include <tari/physics.h>

PhysicsData* gData;

void setupPhysics(PhysicsData* pData){
	resetPhysics();
	gData = pData;
}

void updatePhysics(PhysicsData* pData){
	(void)pData;
}
