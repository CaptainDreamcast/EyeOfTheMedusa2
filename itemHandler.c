#include "itemHandler.h"

#include "collision.h"
#include "shotHandler.h"

static void removeItemCB(void* this, int shotID, int strength){
	removeItem(shotID);
}

void addPowerItems(Position p, int amount){
	Vector3D variance = makePosition(10, 10, 0);
	Velocity v = makePosition(-1, 0, 0);

	while(amount--){
		Position varianceC = variatePosition(variance);
		Position f = vecAdd(p, varianceC);
		CollisionCirc col = makeCollisionCirc(makePosition(8, 8, 8), 6);
		PhysicsObject physics;
		physics.mPosition = f;
		physics.mVelocity = v;
		physics.mAcceleration = makePosition(0,0,0);
		addPowerItem(NULL, col, ITEM_TYPE_POWER, physics, removeItemCB);
	}
}
