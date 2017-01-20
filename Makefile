TARGET = 1ST_READ
OBJS = main.o game.o gamescreen.o scriptHandler.o player.o collision.o
OPTFLAGS=-O3 -fomit-frame-pointer -fno-delayed-branch -DDREAMCAST -Wall -Werror
KOS_CFLAGS+= $(OPTFLAGS)
KOS_ROMDISK_DIR = romdisk_boot

all: clean build_images $(TARGET).elf

build_images: 
	
clean:
	-rm -f $(TARGET).elf $(OBJS)
	-rm -f $(TARGET).BIN
	-rm -f romdisk.img

$(TARGET).elf: $(OBJS) 
	$(KOS_CC) $(KOS_CFLAGS) -I${KOS_BASE}/../extensions/include $(KOS_LDFLAGS) \
	-o $(TARGET).elf $(KOS_START) \
	$(OBJS) -lkmg $(OPTIONAL_LIBS) -lm -ltremor -ltari $(OBJEXTRA) $(KOS_LIBS)
	$(KOS_OBJCOPY) -O binary $(TARGET).elf $(TARGET).BIN

include $(KOS_BASE)/Makefile.rules
