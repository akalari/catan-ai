CXXFLAGS = -std=c++14 -g
CC = clang++

BOARDFILES = board/Board.cpp board/BoardUtils.cpp board/SetupBoard.cpp

catan-board: $(BOARDFILES) board/TestBoard.cpp
	$(CC) $(CXXFLAGS) $(BOARDFILES) board/TestBoard.cpp -o bin/Board

PLAYERFILES = player/Player.cpp

catan-player: $(BOARDFILES) $(PLAYERFILES)
	$(CC) $(CXXFLAGX) $(BOARDFILES) $(PLAYERFILES) -o bin/PlayerBoard

GAMEFILES = $(BOARDFILES) $(PLAYERFILES) gameplay/Game.cpp

catan-game: $(GAMEFILES)
	$(CC) $(CXXFLAGX) $(GAMEFILES) -o bin/Game