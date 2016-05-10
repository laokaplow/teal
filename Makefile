.DEFAULT_GOAL = all
.PHONY: all clean

CXX := g++
CXXFLAGS := -std=c++14 -Wall -Werror

SRCS := $(wildcard src/*.cpp)
HDRS := $(wildcard src/*.h)

all: $(SRCS) $(HDRS)
	$(COMPILE) $(SRCS) -o teal

clean:
	rm -rf teal
