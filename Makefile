.DEFAULT_GOAL = all
.PHONY: all clean demo

CXX := g++
CXXFLAGS := -std=c++14 -Wall -Werror
COMPILE = $(CXX) $(CXXFLAGS) -fmax-errors=1

SRCS := $(wildcard src/*.cpp)
HDRS := $(wildcard src/*.h)

all: $(SRCS) $(HDRS)
	$(COMPILE) $(SRCS) -o teal

demo: teal
	# demos/maze/mazegen.py | tee maze1.txt
	cat demos/maze/maze1.txt | ./teal demos/maze/solver.tl

clean:
	rm -rf teal
