all:
	g++ --std=c++11 -Wall -o bin/GameOfLife main.cpp

debug:
	g++ --std=c++11 -Wall -ggdb -o bin/GameOfLife main.cpp