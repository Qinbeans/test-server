CC = clang++
BUILD = bin
TARG := $(BUILD)/server
CFLAG := -std=c++17 -Ofast  -I include
LFLAG := -lpthread
OBJ = $(patsubst src/%.cpp,$(BUILD)/%.o,$(wildcard src/*.cpp))

ifeq ($(OS),Windows_NT)
	CC := clang++
	TARG := $(TARG).exe
	LFLAG += -lenet -fopenmp -lwinmm -lws2_32
else
	ifeq ($(OS), Linux)
		LFLAG += -L lib/Linux
	else
		LFLAG += -fopenmp
	endif
endif

all: $(TARG) $(OBJ)

$(TARG):$(OBJ)
	$(CC) -o $(TARG) $(OBJ) $(LFLAG)

$(OBJ): bin/%.o:src/%.cpp
	$(CC) -c $(CFLAG) $< -o $@

clean:
	rm $(BUILD)/*.o
	rm $(TARG)
