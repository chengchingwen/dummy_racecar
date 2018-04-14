MAKEFLAGS += -rR

OBJDIR = bin
SRCDIR = src
INCDIR = header
TESTDIR = test

CC = gcc
CFLAG = -I$(INCDIR) -Wall -g

TARGET = state car attribute map control game display
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

# test_%:  $(TESTDIR)/test_%.c $(OBJDIR)/%.o
# 	$(CC) $(CFLAG) $^ -o $@
# 	@./$@
# 	@rm ./$@

$(TEST): % : $(TESTDIR)/%.c $(OBJ)
	$(CC) $(CFLAG) -o $@  $^
	@./$@
	@rm ./$@

.PHONY: clean
clean:
	-@rm -f $(OBJDIR)/*



