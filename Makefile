MAKEFLAGS += -rR

OBJDIR = bin
SRCDIR = src
INCDIR = header
TESTDIR = test

ARCH ?= PC

CC = gcc
CFLAG = -I$(INCDIR) -std=gnu99

ifdef DEBUG
	CFLAG += -g
endif

ifeq ($(ARCH), ARM)
	CFLAG += -D ARM
	CC= arm-unknown-linux-gnu-gcc -L /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/lib/ -I /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/include/ -I /home/lab616/microtime/linux/include/
endif



TARGET = state car attribute map control game display sockop sh
OBJ = $(TARGET:%=$(OBJDIR)/%.o)
SRC = $(TARGET:%=$(SRCDIR)/%.c)
TEST = $(TARGET:%=test_%)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $<

all: $(OBJ)
	@echo $(OBJ)

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

$(TEST): % : $(TESTDIR)/%.c $(OBJ)
	$(CC) $(CFLAG) -o $@  $^
	@./$@
	@rm ./$@

server: $(SRCDIR)/raced.c $(OBJ)
	$(CC) $(CFLAG) -o $@ $^

client: $(SRCDIR)/racec.c $(OBJ)
	$(CC) $(CFLAG) -o $@ $^

.PHONY: clean
clean:
	-@rm -f $(OBJDIR)/*
	# -@rm -f client server


