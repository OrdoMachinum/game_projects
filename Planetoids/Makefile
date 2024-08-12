PROJECT = planets
UNITS = view phyics planets main


CC = gcc
#CC = /usr/bin/x86_64-w64-mingw32-gcc-win32 


DEBUG_FLAGS ?= #-O0 -g

#CFLAGS = -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lc $(DEBUG_FLAGS) --static
CFLAGS = -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lc $(DEBUG_FLAGS) --static

OUT_EXTENSION = .out
OBJDIR = obj
INCLUDE_DIR = include
SOURCE_DIR	= src

OBJECTS = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(UNITS)))

all: $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $(PROJECT)$(OUT_EXTENSION)

$(OBJDIR)/%.o: src/%.c
	mkdir -p $(OBJDIR)/
	$(CC) -c $< -o $@ -I$(INCLUDE_DIR) $(CFLAGS) 

clean: 
	rm $(OBJDIR)/*.o
	rm $(PROJECT)$(OUT_EXTENSION)