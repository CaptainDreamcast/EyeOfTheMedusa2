#include "collision.h"

#include "shotHandler.h"

CollisionData* gData;

void initList(CollisionList* list){
	list->size = 0;
	list->first = NULL;
	list->last = NULL;
}

void setupCollision(CollisionData* cData){
	initList(&cData->playerShots);
	initList(&cData->enemyShots);
	initList(&cData->enemies);
	initList(&cData->player);
	cData->idCounter = 0;
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
	obj->id = gData->idCounter++;
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
		isCollision = checkCollisionObjectCircRect(*colCirc2, *colRect1);
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

int addPlayerShotRect(CollisionObjectRect* col, PhysicsObject* physics, Animation* animation, TextureData* textures, collisionHitCB hitCB){
	int shotID = addToCollisionListRect(&gData->playerShots, col, COLLISION_PLAYER_SHOT, hitCB);
	addToShotHandling(shotID, *physics, *animation, textures);
	return shotID;
}

int addPlayerShotCirc(CollisionObjectCirc* col, PhysicsObject* physics, Animation* animation, TextureData* textures, collisionHitCB hitCB){
	int shotID = addToCollisionListCirc(&gData->playerShots, col, COLLISION_PLAYER_SHOT, hitCB);
	addToShotHandling(shotID, *physics, *animation, textures);
	return shotID;
}

int addToCollisionListRect(CollisionList* list, CollisionObjectRect* rect, CollisionType type, collisionHitCB hitCB);


void removePlayerShot(int shotID){
	removeFromCollisionList(&gData->playerShots, shotID);
	removeFromShotHandling(shotID);
}
