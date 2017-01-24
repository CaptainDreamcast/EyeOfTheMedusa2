#ifndef PLAYER_H
#define PLAYER_H

#include <tari/physics.h>
#include <tari/collision.h>
#include <tari/animation.h>
#include <tari/texture.h>
#include <tari/drawing.h>

typedef enum {
	PLAYER_SHOT_FIRE
} PlayerShotType;

typedef struct {
	int strength;
	PhysicsObject physics;
	CollisionCirc col;
	Animation animation;
	TextureData textures[10];	
	Duration duration[4];
} PlayerShotFire;

typedef struct {
	Duration now;
	PlayerShotType currentType;
	int fireLevel;
	PlayerShotFire fire;
	int laserLevel;
	int homingLevel;
} PlayerShotData;

typedef struct{
	int amount;
	int active;
	int shotID;
	PhysicsObject physics;
	CollisionRect col;
	Animation animation;
	TextureData textures[10];
} BombData;

typedef struct{
	int lifeAmount;	
	
	PhysicsObject physics;
	CollisionObjectCirc col;
	int isFocused;
	BombData bomb;
	PlayerShotData shots;
	int collisionID;

	Animation animation;
	TextureData textures[10];
	Rectangle texturePosition;
} PlayerData;

void setupPlayer(PlayerData* pData);
void updatePlayer(PlayerData* pData);
void drawPlayer(PlayerData* pData);

Position getPlayerPosition();

#endif
