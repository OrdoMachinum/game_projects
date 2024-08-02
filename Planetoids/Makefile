PROJECT = planets
UNITS = view solsystem planets main

CC = gcc

DEBUG_FLAGS ?= #-O0 -g

CFLAGS = -Wall -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 $(DEBUG_FLAGS)

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