PROJECT = planets
UNITS = view phyics planets main

OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(UNITS)))

TARGET = LINUX

DEBUG_FLAGS ?= #-O0 -g

ifeq ($(TARGET),LINUX)
$(info Building for linux...)
CC = gcc
OUT_EXTENSION = .out
OBJDIR = obj
OUT_DIR = bin/linux
LDLIBS = -lGL -lc -lm -lpthread -ldl -lrt
CFLAGS = -Wall -lraylib $(LDLIBS) -lX11 -lc $(DEBUG_FLAGS)
else
$(info Building for windows...)
CC = x86_64-w64-mingw32-gcc
OUT_EXTENSION = .exe
OBJDIR = obj_win
OUT_DIR = bin/win
LDLIBS = -static-libgcc -lopengl32 -lgdi32 -lwinmm -static -lpthread -I/usr/local/include/
OBJECTS += $(OBJDIR)/libraylib.a
CFLAGS = -Wall $(LDLIBS)
endif

INCLUDE_DIR = include
SOURCE_DIR	= src

run: all
	./$(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION)

all: $(OBJECTS)
	mkdir -p $(OUT_DIR)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION)

$(OBJDIR)/%.o: src/%.c
	mkdir -p $(OBJDIR)/
	$(CC) -c $< -o $@ -I$(INCLUDE_DIR) $(CFLAGS) 

clean: 
	rm -f $(OUT_DIR)/$(PROJECT)$(OUT_EXTENSION)
	rm -f $(OBJDIR)/*.o