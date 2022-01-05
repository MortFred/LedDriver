SRC_DIR = ./src
TEST_DIR = ./tests
IDIR = ./include
# IDIR += include/util
ODIR = $(SRC_DIR)/obj
LDIR =./lib

CC=g++
CPPFLAGS=-I$(IDIR) -I$(IDIR)/util
LIBS=-lm

_DEPS = LedDriver.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = LedDriver.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CPPFLAGS)

test:
	make -C $(TEST_DIR)

test_clean:
	make -C $(TEST_DIR) clean

LedDriver: $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LIBS)

all: test LedDriver 

.PHONY: clean

clean: test_clean
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 