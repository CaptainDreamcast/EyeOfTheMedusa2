#ifndef PLAYER_H
#define PLAYER_H

#include <tari/physics.h>
#include <tari/collision.h>

typedef struct{
	PhysicsObject physics;
	CollisionObjectCirc col;
	int isFocused;
	
} PlayerData;

void setupPlayer(PlayerData* pData);
void updatePlayer(PlayerData* pData);
void drawPlayer(PlayerData* pData);

#endif
