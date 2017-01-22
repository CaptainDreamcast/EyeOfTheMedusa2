#ifndef SHOTHANDLER_H
#define SHOTHANDLER_H

#include <tari/physics.h>
#include <tari/animation.h>
#include <tari/texture.h>

void setupShotHandling();
void updateShotHandling();
void drawShotHandling();
PhysicsObject* addToShotHandling(int shotID, PhysicsObject physics, Animation animation, TextureData* textures);
PhysicsObject* addToShotHandlingType(int shotID, PhysicsObject physics, int type);
void removeFromShotHandling(int shotID);
int getNextShotId ();

#endif
