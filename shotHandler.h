#ifndef SHOTHANDLER_H
#define SHOTHANDLER_H
	void setupShotHandling();
	void updateShotHandling();
	void addToShotHandling(int shotID, PhysicsObject* physics, Animation* animation, TextureData* textures);
	void removeFromShotHandling(int shotID);
#endif
