PROJECT = planets
UNITS = view phyics planets main

OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(UNITS)))

TARGET = LINUX
#TARGET = BUILD_ON_WINDOWS

DEBUG_FLAGS ?= #-O0 -g

ifeq ($(TARGET),LINUX)
$(info Building for linux...)
CC = gcc
OUT_EXTENSION = .out
OBJDIR = obj
OUT_DIR = bin/linux
LDLIBS = -lGL -lc -lm -lpthread -ldl -lrt -lm
CFLAGS = -Wall -lraylib $(LDLIBS) -lX11 -lc $(DEBUG_FLAGS)
endif

ifeq ($(TARGET),BUILD_ON_WINDOWS)
$(info Building ON windows FOR windows...)
CC = x86_64-w64-mingw32-gcc
OUT_EXTENSION = .exe
OBJDIR = obj_win
OUT_DIR = bin/win
LDLIBS = -Ic://raylib//include// -static-libgcc -lopengl32 -lgdi32 -lwinmm -static -lpthread -lm
OBJECTS += c://raylib//lib//libraylib.a
CFLAGS = -Wall $(LDLIBS)
else
$(info Building FOR windows ON LINUX ...)
CC = x86_64-w64-mingw32-gcc
OUT_EXTENSION = .exe
OBJDIR = obj_win
OUT_DIR = bin/win
LDLIBS = -I/usr/local/include/ -static-libgcc -lopengl32 -lgdi32 -lwinmm -static -lpthread -lm
OBJECTS += $(OBJDIR)/libraylib.a
CFLAGS = -Wall $(LDLIBS)
endif

INCLUDE_DIR = include
SOURCE_DIR	= src

run: all
	./$(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION)

all: $(OBJECTS)
	mkdir -p $(OUT_DIR)
	$(CC) $(OBJECTS) -o $(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION) $(CFLAGS)

$(OBJDIR)/%.o: src/%.c
	mkdir -p $(OBJDIR)/
	$(CC) -c $< -o $@ -I$(INCLUDE_DIR) $(CFLAGS) 

clean: 
	rm -f $(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION)
	rm -f $(OBJDIR)/*.o