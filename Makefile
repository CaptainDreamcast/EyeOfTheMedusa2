TARGET = 1ST_READ
OBJS = main.o game.o gamescreen.o scriptHandler.o player.o collision.o physics.o userInterface.o gameScript.o script.o levelScript.o \
scriptTypes.o stageScript.o sectionScript.o enemyScript.o shotScript.o shotHandler.o gameStateLogic.o itemHandler.o titlescreen.o \
gameoverscreen.o congratulationsscreen.o \
romdisk.o
OPTFLAGS=-O3 -fomit-frame-pointer -fno-delayed-branch -DDREAMCAST -Wall -Werror
KOS_CFLAGS+= $(OPTFLAGS)
KOS_ROMDISK_DIR = romdisk_boot

all: clean build_images $(TARGET).elf

build_images:
	$(KOS_BASE)/utils/kmgenc/kmgenc -a4 $(wildcard assets/sprites/*.png)
	tools/KOMPRESSOR/kompressor $(wildcard assets/sprites/*.kmg)
	$(KOS_BASE)/utils/kmgenc/kmgenc -a4 $(wildcard assets/debug/*.png)
	tools/KOMPRESSOR/kompressor $(wildcard assets/debug/*.kmg)
	mkdir romdisk_boot
	mkdir romdisk_boot/sprites
	cp assets/sprites/*.pkg romdisk_boot/sprites
	mkdir romdisk_boot/fonts
	cp assets/fonts/* romdisk_boot/fonts
	mkdir romdisk_boot/debug
	cp assets/debug/*.pkg romdisk_boot/debug
	mkdir romdisk_boot/scripts
	cp assets/scripts/*.txt romdisk_boot/scripts
	
	

clean:
	-rm -f $(TARGET).elf $(OBJS)
	-rm -f $(TARGET).BIN
	-rm -f assets/sprites/*.pkg
	-rm -f assets/debug/*.pkg
	-rm -r -f romdisk_boot
	-rm -f romdisk.img

$(TARGET).elf: $(OBJS) 
	$(KOS_CC) $(KOS_CFLAGS) -I${KOS_BASE}/../extensions/include $(KOS_LDFLAGS) \
	-o $(TARGET).elf $(KOS_START) \
	$(OBJS) -lkmg $(OPTIONAL_LIBS) -lm -ltremor -ltari $(OBJEXTRA) $(KOS_LIBS)
	$(KOS_OBJCOPY) -O binary $(TARGET).elf $(TARGET).BIN

include $(KOS_BASE)/Makefile.rules
