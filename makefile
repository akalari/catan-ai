CXXFLAGS = -std=c++11 -g
CC = g++

catan-board: board/Board.cpp board/SetupBoard.cpp
	$(CC) $(CXXFLAGS) board/Board.cpp board/SetupBoard.cpp -o bin/Board
