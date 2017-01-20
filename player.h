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
	PhysicsObject physics;
	CollisionObjectCirc col;
	Animation animation;
	TextureData textures[10];	
} PlayerShotFire;

typedef struct {
	Duration now;
	Duration duration;
	PlayerShotType currentType;
	int fireLevel;
	PlayerShotFire fire[4];
} PlayerShotData;

typedef struct{
	int amount;
	int active;
	int shotID;
	PhysicsObject physics;
	CollisionObjectRect col;
	Animation animation;
	TextureData textures[10];
} BombData;

typedef struct{
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

#endif
