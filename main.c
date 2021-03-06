

#include <tari/log.h>
#include <tari/file.h>
#include <tari/pvr.h>
#include <tari/physics.h>
#include <tari/framerateselectscreen.h>
#include <tari/drawing.h>
#include <tari/system.h>
#include <tari/wrapper.h>

#include "titlescreen.h"

#ifdef DREAMCAST
#include <kos.h> 

KOS_INIT_FLAGS(INIT_DEFAULT);

extern uint8 romdisk[];
KOS_INIT_ROMDISK(romdisk);

uint32_t useRomDisk = 1;
#endif

void exitGame() {
#ifdef DEVELOP
  abortSystem();
#else
  returnToMenu();
#endif
}

static void setMainFileSystem() {
	setFileSystem("/rd");
}



int main(int argc, char** argv) {

	setGameName("EYE OF THE MEDUSA: ULTIMATE FRONTIER");

	initTariWrapperWithDefaultFlags();
	setMainFileSystem();

	logg("Check framerate");
	setFont("$/rd/fonts/dolmexica.hdr", "$/rd/fonts/dolmexica.pkg");
	FramerateSelectReturnType framerateReturnType = selectFramerate();
	if (framerateReturnType == FRAMERATE_SCREEN_RETURN_ABORT) {
		exitGame();
	}

	logg("Begin game routine");
	startScreenHandling(&TitleScreen);


	exitGame();


	return (0);
}

int wmain(int argc, char** argv) {
	return main(argc, argv);
}
