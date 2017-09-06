CXXFLAGS = -std=c++14 -g
CC = clang++

catan-board: board/Board.cpp player/Player.cpp
	$(CC) $(CXXFLAGS) board/Board.cpp board/SetupBoard.cpp board/BoardUtils.cpp board/TestBoard.cpp player/Player.cpp -o bin/Board

catan-player: board/Board.cpp player/Player.cpp board/SetupBoard.cpp
	$(CC) $(CXXFLAGX) board/Board.cpp board/SetupBoard.cpp player/Player.cpp -o bin/PlayerBoard
