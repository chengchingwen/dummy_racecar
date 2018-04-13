MAKEFLAGS += -rR

OBJDIR = bin
SRCDIR = src
INCDIR = header

CC = gcc
CFLAG = -I$(INCDIR) -Wall -g

TARGET = state car attribute map control game display
OBJ = $(TARGET:%=$(OBJDIR)/%.o)
SRC = $(TARGET:%=$(SRCDIR)/%.c)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $<

all: $(OBJ)
	@echo $(OBJ)

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

test_%:  test/test_%.c $(OBJDIR)/%.o
	$(CC) $(CFLAG) $^ -o $@
	@./$@
	@rm ./$@

test_car: test/test_car.c $(OBJDIR)/car.o $(OBJDIR)/attribute.o $(OBJDIR)/control.o
	$(CC) $(CFLAG) -o $@  $^
	@./$@
	@rm ./$@

test_map: test/test_map.c $(OBJ)
	$(CC) $(CFLAG) -o $@  $^
	@./$@
	@rm ./$@

test_game: test/test_game.c $(OBJ)
	$(CC) $(CFLAG) -o $@  $^
	@./$@
	@rm ./$@

.PHONY: clean
clean:
	-@rm -f $(OBJDIR)/*



