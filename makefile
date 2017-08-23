CXXFLAGS = -std=c++11 -g
CC = g++

board: board/Board.cpp
	$(CC) $(CXXFLAGS) board/Board.cpp -o bin/Board
