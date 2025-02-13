PROJECT = planets_pp
UNITS = main
UNITS += game celestial celestialview worldvector

COMPILER = g++
SOURCE_EXTENSION = .cpp

BUILD_DIR = build


OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(UNITS)))

TARGET ?= LINUX
#TARGET = BUILD_ON_WINDOWS
#TARGET = BUILD_FOR_WINDOWS

RAYLIB_INSTALL_DIR = c://raylib// #for on-windows-builds


#DEBUG_FLAGS ?= #-O0 -g
DEBUG_FLAGS = -g

ifeq ($(TARGET),LINUX)
$(info Building for linux...)
CC = $(COMPILER)
OUT_EXTENSION = .out
OBJDIR = $(BUILD_DIR)/obj
OUT_DIR = $(BUILD_DIR)/bin/linux
LDLIBS = -lGL -lc -lm -lpthread -ldl -lrt -lm
CFLAGS = -Wall -lraylib $(LDLIBS) -lX11 -lc $(DEBUG_FLAGS)
else ifeq ($(TARGET),BUILD_ON_WINDOWS)
$(info Building ON windows FOR windows...)
CC = x86_64-w64-mingw32-$(COMPILER)
OUT_EXTENSION = .exe
OBJDIR = $(BUILD_DIR)/obj_win
OUT_DIR = $(BUILD_DIR)/bin/win
LDLIBS = -I$(strip $(RAYLIB_INSTALL_DIR))/include// -static-libgcc -lopengl32 -lgdi32 -lwinmm -static -lpthread -lm
OBJECTS += $(strip $(RAYLIB_INSTALL_DIR))/lib//libraylib.a
CFLAGS = -Wall $(LDLIBS)
else ifeq ($(TARGET),BUILD_FOR_WINDOWS)
$(info Building FOR windows ON LINUX ...)
CC = x86_64-w64-mingw32-$(COMPILER)
OUT_EXTENSION = .exe
OBJDIR = $(BUILD_DIR)/obj_win
OUT_DIR = $(BUILD_DIR)/bin/win
LDLIBS = -I/usr/local/include/ -static-libgcc -lopengl32 -lgdi32 -lwinmm -static -lpthread -lm
OBJECTS += $(OBJDIR)/libraylib.a
CFLAGS = -Wall $(LDLIBS)
else
$(error No valid TARGET is selected!!)
endif

INCLUDE_DIR = include
SOURCE_DIR	= src


all: $(OBJECTS)
	mkdir -p $(OUT_DIR)
	$(CC) $(OBJECTS) -o $(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION) $(CFLAGS)

run: all
	./$(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION) systems/tauri2000.csv

$(OBJDIR)/%.o: src/%$(SOURCE_EXTENSION)
	mkdir -p $(OBJDIR)/
	$(CC) -c $< -o $@ -I$(INCLUDE_DIR) $(CFLAGS) 

clean: 
	rm -f $(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION)
	rm -f $(OBJDIR)/*.o
