CXXFLAGS = -std=c++14 -g
CC = clang++

BOARDFILES = board/Board.cpp board/BoardUtils.cpp board/SetupBoard.cpp

catan-board: $(BOARDFILES) board/TestBoard.cpp
	$(CC) $(CXXFLAGS) $(BOARDFILES) board/TestBoard.cpp -o bin/Board

PLAYERFILES = player/Player.cpp player/AIPlayer.cpp player/UserPlayer.cpp

catan-player: $(BOARDFILES) $(PLAYERFILES) player/TestPlayer.cpp
	$(CC) $(CXXFLAGS) $(BOARDFILES) $(PLAYERFILES) player/TestPlayer.cpp -o bin/Player

GAMEFILES = $(BOARDFILES) $(PLAYERFILES) gameplay/Game.cpp

catan-game: $(GAMEFILES)
	$(CC) $(CXXFLAGS) $(GAMEFILES) -o bin/Game
