#ifndef COLLISION_H
#define COLLISION_H

#include <tari/physics.h>
#include <tari/collision.h>
#include <tari/animation.h>
#include <tari/texture.h>

typedef enum  {
	COLLISION_ENEMY_SHOT,
	COLLISION_ENEMY,
	COLLISION_PLAYER,
	COLLISION_PLAYER_SHOT
} CollisionType;

typedef enum  {
	COLLISION_OBJECT_CIRC,
	COLLISION_OBJECT_RECT,
} CollisionObjectType;

typedef void (*collisionHitCB)(int shotID, CollisionType otherShotType);

typedef struct CollisionElement_internal{
	int id;
	int type;
	void* data;
	collisionHitCB hitCB;
	
	struct CollisionElement_internal* next;
	struct CollisionElement_internal* prev;

} CollisionElement; 

typedef struct {
	int size;
	CollisionElement* first;
	CollisionElement* last;
} CollisionList;

typedef struct{
	CollisionList playerShots;
	CollisionList enemyShots;
	CollisionList enemies;
	CollisionList player;
} CollisionData;

void setupCollision(CollisionData* cData);
void updateCollision(CollisionData* cData);

int addPlayerCirc(CollisionObjectCirc* col, collisionHitCB hitCB);
int addPlayerShotRect(CollisionObjectRect* col, PhysicsObject* physics, Animation* animation, TextureData* textures, collisionHitCB hitCB);
int addPlayerShotCirc(CollisionObjectCirc* col, PhysicsObject* physics, Animation* animation, TextureData* textures, collisionHitCB hitCB);
int addEnemyShotCirc(CollisionObjectCirc* col, int enemyShotType, PhysicsObject physics, collisionHitCB hitCB);
void removePlayerShot(int shotID);
void removeEnemyShot(int shotID);

#endif
