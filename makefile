# OpenAI - 00000000A
CXX ?= g++
CXXFLAGS ?= -O3 -std=c++17 -Wall -Wextra

maze_bb: maze_bb.cc
	$(CXX) $(CXXFLAGS) maze_bb.cc -o maze_bb

clean:
	rm -f maze_bb
