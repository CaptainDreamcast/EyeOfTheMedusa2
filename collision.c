#include "collision.h"

#include "shotHandler.h"

#include <tari/log.h>
#include <tari/drawing.h>
#define DEBUG_Z 1

static CollisionData* gData;

void initList(CollisionList* list){
	list->size = 0;
	list->first = NULL;
	list->last = NULL;
}

static void loadDebugTextures(CollisionData* cData){
	cData->debug.collisionCircTexture = loadTexturePKG("/debug/collision_circ.pkg");
	cData->debug.collisionRectTexture = loadTexturePKG("/debug/collision_rect.pkg");
}

void setupCollision(CollisionData* cData){
	initList(&cData->playerShots);
	initList(&cData->enemyShots);
	initList(&cData->enemies);
	initList(&cData->player);
	loadDebugTextures(cData);
	gData = cData;
}

void addToCollisionList(CollisionList* list, CollisionElement* obj){
	CollisionElement* prevLast = list->last;
	list->last = obj;
	
	if(prevLast == NULL) list->first = obj;
	else {
		prevLast->next = obj;
		obj->prev = prevLast;
	}

	list->size++;
}

int addToCollisionListObj(CollisionList* list, void* col, CollisionType type, collisionHitCB hitCB){
	CollisionElement* obj = malloc(sizeof(CollisionElement));
	obj->id = getNextShotId();
	obj->type = type;
	obj->hitCB = hitCB;
	obj->data = col;
	obj->prev = NULL;
	obj->next = NULL;
	addToCollisionList(list, obj);
	return obj->id;
}
int addToCollisionListRect(CollisionList* list, CollisionObjectRect* col, CollisionType type, collisionHitCB hitCB){
	return addToCollisionListObj(list, col, type, hitCB);
}
int addToCollisionListCirc(CollisionList* list, CollisionObjectCirc* col, CollisionType type, collisionHitCB hitCB){
	return addToCollisionListObj(list, col, type, hitCB);
}

void removeFromCollisionList(CollisionList* list, int shotID){
	int left = list->size;
	CollisionElement* cur = list->first;

	int i;
	for(i = 0; i < left; i++){
		if(cur->id == shotID){
			if(cur->prev != NULL) cur->prev->next = cur->next;
			if(cur->next != NULL) cur->next->prev = cur->prev;
			if(cur->prev == NULL) list->first = cur->next;
			if(cur->next == NULL) list->last = cur->prev;
			free(cur->data);
			free(cur);
			return;
		}
		cur = cur->next;
	}
}

void compareObjectsAndHandleCollision(CollisionElement* obj1, CollisionElement* obj2){
	int isCollision = 0;
	CollisionObjectCirc* colCirc1 = NULL;
	CollisionObjectRect* colRect1 = NULL;
	CollisionObjectCirc* colCirc2 = NULL;
	CollisionObjectRect* colRect2 = NULL;
	
	if(obj1->type == COLLISION_OBJECT_CIRC){
		colCirc1 = (CollisionObjectCirc*)obj1->data;
	} else {
		colRect1 = (CollisionObjectRect*)obj1->data;
	}

	if(obj2->type == COLLISION_OBJECT_CIRC){
		colCirc2 = (CollisionObjectCirc*)obj2->data;
	} else {
		colRect2 = (CollisionObjectRect*)obj2->data;
	}

	if(obj1->type == COLLISION_OBJECT_CIRC && obj2->type == COLLISION_OBJECT_CIRC){
		isCollision = checkCollisionObjectCirc(*colCirc1, *colCirc2);
	} else if(obj1->type == COLLISION_OBJECT_CIRC && obj2->type == COLLISION_OBJECT_RECT){
		isCollision = checkCollisionObjectCircRect(*colCirc1, *colRect2);
	} else if(obj1->type == COLLISION_OBJECT_RECT && obj2->type == COLLISION_OBJECT_CIRC){
		isCollision = checkCollisionObjectCircRect(*colCirc2, *colRect1);
	} else {
		isCollision = checkCollisionObjectRect(*colRect1, *colRect2);
	}

	if(!isCollision) return;

	obj1->hitCB(obj1->id, obj2->type);
	obj2->hitCB(obj2->id, obj1->type);
}

void compareCollisionListToObject(CollisionList* list, CollisionElement* obj){
	int left = list->size;
	CollisionElement* cur = list->first;

	int i;
	for(i = 0; i < left; i++){
		compareObjectsAndHandleCollision(cur, obj);
		cur = cur->next;
	}
}

void compareCollisionLists(CollisionList* list1, CollisionList* list2){
	int left = list1->size;
	CollisionElement* cur = list1->first;

	int i;
	for(i = 0; i < left; i++){
		compareCollisionListToObject(list2, cur);
		cur = cur->next;
	}
}

void updateCollision(CollisionData* cData){
	compareCollisionLists(&gData->playerShots, &gData->enemies);
	compareCollisionLists(&gData->enemyShots, &gData->player);
}

int addPlayerCirc(CollisionObjectCirc* col, collisionHitCB hitCB){
	int shotID = addToCollisionListCirc(&gData->player, col, COLLISION_PLAYER, hitCB);
	return shotID;
}

int addPlayerShotRect(CollisionRect col, PhysicsObject physics, Animation animation, TextureData* textures, collisionHitCB hitCB){
	CollisionObjectRect* colObj = malloc(sizeof(CollisionObjectRect));
	(*colObj) = makeCollisionObjectRect(col.mTopLeft, col.mBottomRight, NULL);
	int shotID = addToCollisionListRect(&gData->playerShots, colObj, COLLISION_PLAYER_SHOT, hitCB);
	PhysicsObject* p2 = addToShotHandling(shotID, physics, animation, textures);
	colObj->mPhysics = p2;
	return shotID;
}

int addPlayerShotCirc(CollisionCirc col, PhysicsObject physics, Animation animation, TextureData* textures, collisionHitCB hitCB){
	debugLog("Adding player shot");
	CollisionObjectCirc* colObj = malloc(sizeof(CollisionObjectCirc));
	(*colObj) = makeCollisionObjectCirc(col.mCenter, col.mRadius, NULL);
	int shotID = addToCollisionListCirc(&gData->playerShots, colObj, COLLISION_PLAYER_SHOT, hitCB);
	PhysicsObject* p2 = addToShotHandling(shotID, physics, animation, textures);
	colObj->mPhysics = p2;

	debugInteger(shotID);
	debugDouble(p2->mPosition.x); debugDouble(p2->mPosition.y); debugDouble(p2->mPosition.z);

	return shotID;
}

int addEnemyShotCirc(CollisionCirc col, int enemyShotType, PhysicsObject physics, collisionHitCB hitCB){
	CollisionObjectCirc* colObj = malloc(sizeof(CollisionObjectCirc));
	(*colObj) = makeCollisionObjectCirc(col.mCenter, col.mRadius, NULL);
	int shotID = addToCollisionListCirc(&gData->enemyShots, colObj, COLLISION_ENEMY_SHOT, hitCB);
	PhysicsObject* p2 = addToShotHandlingType(shotID, physics, enemyShotType);
	colObj->mPhysics = p2;
	return shotID;
}

void removePlayerShot(int shotID){
	removeFromCollisionList(&gData->playerShots, shotID);
	removeFromShotHandling(shotID);
}

void removeEnemyShot(int shotID){
	removeFromCollisionList(&gData->enemyShots, shotID);
	removeFromShotHandling(shotID);
}

void drawColCirc(CollisionObjectCirc* obj){
	double r = obj->mCol.mRadius;
	Position position = vecAdd(obj->mCol.mCenter, obj->mPhysics->mPosition);
	position.z = DEBUG_Z;
	position = vecAdd(position, makePosition(-r, -r, 0));

	scaleDrawing(r / 8.0, position);
	drawSprite(gData->debug.collisionCircTexture, position, makeRectangleFromTexture(gData->debug.collisionCircTexture));
	setDrawingParametersToIdentity();
	
}

void drawColRect(CollisionObjectRect* obj){
	double dx = obj->mCol.mBottomRight.x -  obj->mCol.mTopLeft.x;
	double dy = obj->mCol.mBottomRight.y -  obj->mCol.mTopLeft.y;
	
	Position position = vecAdd(obj->mCol.mTopLeft, obj->mPhysics->mPosition);
	position.z = DEBUG_Z;
	
	Vector3D scale = makePosition(16.0 / dx, 16.0 / dy, 1);
	scaleDrawing3D(scale, position);

	drawSprite(gData->debug.collisionRectTexture, position, makeRectangleFromTexture(gData->debug.collisionRectTexture));
	setDrawingParametersToIdentity();
	
}


void drawCollisionElement(CollisionElement* obj1){
	CollisionObjectCirc* colCirc1 = NULL;
	CollisionObjectRect* colRect1 = NULL;
	if(obj1->type == COLLISION_OBJECT_CIRC){
		colCirc1 = (CollisionObjectCirc*)obj1->data;
		drawColCirc(colCirc1);
	} else {
		colRect1 = (CollisionObjectRect*)obj1->data;
		drawColRect(colRect1);
	}
}

void drawCollisionList(CollisionList* list){
	int left = list->size;
	CollisionElement* cur = list->first;

	int i;
	for(i = 0; i < left; i++){
		drawCollisionElement(cur);
		cur = cur->next;
	}
}

void drawCollisions(CollisionData* cData){
	drawCollisionList(&gData->player);
	drawCollisionList(&gData->playerShots);
	drawCollisionList(&gData->enemies);
	drawCollisionList(&gData->enemyShots);
}
