#ifndef COLLISION_H
#define COLLISION_H

#include <tari/physics.h>


typedef struct CollisionElement_internal{
	int id;
	int type;
	void* data;
	
	struct CollisionElement_internal* next;
	struct CollisionElement_internal* prev;

} CollisionElement; 

typedef struct {
	CollisionElement* first;
	CollisionElement* last;
} CollisionList;

typedef struct{
	CollisionList playerShots;
	CollisionList enemyShots;
	CollisionList enemies;
	CollisionObjectCirc player;
} CollisionData;

void setupCollision(CollisionData* cData);
void updateCollision(CollisionData* cData);

int addPlayerShotRect(CollisionObjectRect* col, PhysicsObject* physics, Animation* animation, TextureData* textures, void (*hitCB)(int shotID));
int addPlayerShotCirc(CollisionObjectCirc* col, PhysicsObject* physics, Animation* animation, TextureData* textures, void (*hitCB)(int shotID));
void removePlayerShot(int shotID);

#endif
