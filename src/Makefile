TARGET = eboot
OBJS = ./pbp/main.o ./pbp/kexploit.o

INCDIR = ./include
LIBDIR = ./lib

BUILD_PRX = 1
PSP_FW_VERSION = 100
EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = iypcwc
PSP_EBOOT_ICON = ./pbp/ICON0.PNG

USE_PSPSDK_LIBC = 1
USE_PSPSDK_LIBS = 1
CFLAGS = -Ofast -w
LIBS = -lpspexploit -lpsprtc

PSPSDK = $(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak