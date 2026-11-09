CXX ?= g++
CXXFLAGS += -c -Wall -Wextra
LDFLAGS += -lgmpxx -lgmp -lsodium

SRC = $(wildcard *.cpp)
HDR = $(wildcard *.h)
OBJ = $(SRC:.cpp=.o)

all: Release

Debug: CXXFLAGS += -g
Debug: main.exe

Release: main.exe

main.exe: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp $(HDR)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJ) main.exe
