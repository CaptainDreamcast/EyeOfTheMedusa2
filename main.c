#include <kos.h> 

KOS_INIT_FLAGS(INIT_DEFAULT);

#include <tari/log.h>
#include <tari/file.h>
#include <tari/pvr.h>
#include <tari/physics.h>
#include <tari/framerateselectscreen.h>
#include <tari/drawing.h>

#include "game.h"

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

uint32_t useRomDisk = 1;

#define DEVELOP

void exitGame() {
#ifdef DEVELOP
  arch_exit();
#else
  arch_menu();
#endif
}

void setMainFileSystem() {
	setFileSystem("/rd");
}

int main() {

  initiatePVR();
  initPhysics();
  initFileSystem();
  initDrawing();
  setMainFileSystem();

  log("Check framerate");
  setFont("$/rd/fonts/dolmexica.hdr", "$/rd/fonts/dolmexica.pkg");
  FramerateSelectReturnType framerateReturnType = selectFramerate();
  if (framerateReturnType == FRAMERATE_SCREEN_RETURN_ABORT) {
    exitGame();
  }

  log("Begin game routine");
  startGame();


  exitGame();


  return (1);
}
