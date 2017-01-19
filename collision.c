#include "collision.h"

CollisionData* gData;

void setupCollision(CollisionData* cData){
	gData = cData;
}

void updateCollision(CollisionData* cData){
	
}

int addPlayerShotRect(CollisionObjectRect* col, PhysicsObject* physics, Animation* animation, TextureData* textures, void (*hitCB)(int shotID)){
	shotID = addToCollisionList(col, hitCB);
	addToShotHandling(sphysics, animation, textures);
}

int addPlayerShotCirc(CollisionObjectCirc* col, PhysicsObject* physics, Animation* animation, TextureData* textures, void (*hitCB)(int shotID)){
	addToCollisionList(&gData->playerShots, hitCB, asdadsd)
}

void removePlayerShot(int shotID){
	removeFromCollisionList(&gData->playerShots, shotID);
	removeFromShotHandling(shotID);
}
