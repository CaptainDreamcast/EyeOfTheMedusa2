#ifndef PHYSICS_H
#define PHYSICS_H

typedef struct{
	int mDummy;
} PhysicsData;

void setupPhysics(PhysicsData* pData);
void updatePhysics(PhysicsData* pData);
void shutdownPhysics(PhysicsData* pData);

#endif
