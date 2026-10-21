# simple recursive build, all .cpp under this folder -> main.exe
CXX      := g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -g

SRCS := $(shell find . -name '*.cpp')     # finds main.cpp and class .cpp in subfolders
OBJS := $(SRCS:.cpp=.o)
TARGET := main.exe

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
