MAKEFLAGS += -rR

OBJDIR = bin
SRCDIR = src
INCDIR = header
TESTDIR = test

#CC = gcc
CFLAG = -I$(INCDIR) -D ARM -std=c99
CC= arm-unknown-linux-gnu-gcc -L /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/lib/ -I /opt/arm-unknown-linux-gnu/arm-unknown-linux-gnu/include/ -I /home/lab616/microtime/linux/include/

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

arm: main.c $(OBJ)
	$(CC) $(CFLAG) -D ARM -o main $^

hello: hello.c
	$(CC) -o $@ $< 

.PHONY: clean
clean:
	-@rm -f $(OBJDIR)/*



