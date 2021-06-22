CC = clang++
BUILD = bin
TARG := $(BUILD)
DATE = $(shell date '+%Y-%m-%d' )
FILE := test-$(DATE)
CFLAG := -std=c++17 -Ofast  -I include
LFLAG := -lpthread
OBJ = $(patsubst src/%.cpp,$(BUILD)/%.o,$(wildcard src/*.cpp))

ifeq ($(OS),Windows_NT)
	PLAT = Windows
	LFLAG += -lenet -fopenmp -lwinmm -lws2_32
else
	OS = $(shell uname -s)
	ifeq ($(OS), Linux)
		PLAT = Linux
		LFLAG += -lenet -L lib/Linux
	else
		PLAT = Mac
		CFLAG += -I /opt/homebrew/include
		LFLAG += -lenet -L /opt/homebrew/lib
	endif
endif

OBJ = $(patsubst src/%.cpp,$(BUILD)/$(PLAT)/%.o,$(wildcard src/*.cpp))
TARG := $(TARG)/$(PLAT)/$(FILE)

all: $(TARG) $(OBJ)

$(TARG):$(OBJ)
	$(CC) -o $(TARG) $(OBJ) $(LFLAG)

$(OBJ): bin/$(PLAT)/%.o:src/%.cpp
	$(CC) -c $(CFLAG) $< -o $@

clean:
	rm $(BUILD)/$(PLAT)/*.o
	rm $(TARG)

init:
	mkdir $(BUILD)/$(PLAT)