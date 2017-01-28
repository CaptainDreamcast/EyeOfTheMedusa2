#ifndef COLLISION_H
#define COLLISION_H

#include <tari/physics.h>
#include <tari/collision.h>
#include <tari/animation.h>
#include <tari/texture.h>
#include <tari/drawing.h>

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

typedef void (*collisionHitCB)(void* this, int shotID, int strength);

typedef struct CollisionElement_internal{
	int id;
	CollisionObjectType type;
	void* data;
	void* caller;
	int strength;
	collisionHitCB hitCB;
	int isScheduledForDelete;
	int isOwningData;
	
	struct CollisionElement_internal* next;
	struct CollisionElement_internal* prev;

} CollisionElement; 

typedef struct {
	int size;
	CollisionElement* first;
	CollisionElement* last;
} CollisionList;

typedef struct {
	TextureData collisionCircTexture;
	TextureData collisionRectTexture;

} CollisionDebugData;

typedef struct{
	CollisionList playerShots;
	CollisionList enemyShots;
	CollisionList enemies;
	CollisionList player;
	CollisionList playerCollection;
	CollisionList items;
	CollisionDebugData debug;
	int isPlayerFocused;
} CollisionData;

void setupCollision(CollisionData* cData);
void shutdownCollision(CollisionData* cData);
void updateCollision(CollisionData* cData);

int addPlayerCirc(void* caller, CollisionObjectCirc* col, collisionHitCB hitCB);
int addPlayerCollectCirc(void* caller, CollisionObjectCirc* col, collisionHitCB hitCB);

int addEnemyCirc(void* caller, CollisionObjectCirc* col, collisionHitCB hitCB);
int addPlayerShotRect(void* caller, int strength, CollisionRect col, PhysicsObject physics, Animation animation, TextureData* textures, collisionHitCB hitCB);
int addPlayerShotCirc(void* caller, int strength, CollisionCirc col, PhysicsObject physics, Animation animation, TextureData* textures, collisionHitCB hitCB);
int addEnemyShotCirc(void* caller, int strength, CollisionCirc col, int enemyShotType, PhysicsObject physics, Color color, collisionHitCB hitCB);
int addPowerItem(void* caller, CollisionCirc col, int itemType, PhysicsObject physics, collisionHitCB hitCB);

void removePlayerShot(int shotID);
void removeEnemyShot(int shotID);
void removeItem(int shotID);
void removeEnemy(int shotID);
void removePlayer(int shotID, int shotIDCollection);
void removeAllEnemyShots();

void setDoNotDrawPlayerFocus();
void setDrawPlayerFocus();
void drawCollisions(CollisionData* cData);

#endif
