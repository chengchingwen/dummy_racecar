MAKEFLAGS += -rR

OBJDIR = bin
SRCDIR = src
INCDIR = header

CC = gcc
CFLAG = -I$(INCDIR) -Wall

TARGET = state car attribute
OBJ = $(TARGET:%=$(OBJDIR)/%.o)
SRC = $(TARGET:%=$(SRCDIR)/%.c)
# TEST = $(TARGET:%=test/test_%.c)
# TBIN = $(TEST:test/%.c=%)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAG) -o $@ -c $<

all: $(OBJ)
	$(CC) bin/car.o test/test_car.c

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

test_%:  test/test_%.c $(OBJDIR)/%.o
	$(CC) $(CFLAG) $^ -o $@
	./$@
	rm ./$@

test_car: test/test_car.c $(OBJ)
	$(CC) $(CFLAG) -o $@  $^
	@./$@
	@rm ./$@

.PHONY: clean
clean:
	-@rm -f $(OBJDIR)/*



