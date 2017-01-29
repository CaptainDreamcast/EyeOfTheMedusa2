#ifndef SHOTHANDLER_H
#define SHOTHANDLER_H

#include <tari/physics.h>
#include <tari/animation.h>
#include <tari/texture.h>
#include <tari/drawing.h>

#define ITEM_TYPE_POWER 1

void setupShotHandling();
void updateShotHandling();
void drawShotHandling();
void shutdownShotHandling();
PhysicsObject* addToShotHandling(int shotID, PhysicsObject physics, Animation animation, TextureData* textures, Color color);
PhysicsObject* addToShotHandlingType(int shotID, PhysicsObject physics, int type, Color color);
void removeFromShotHandling(int shotID);
int getNextShotId ();

#endif
