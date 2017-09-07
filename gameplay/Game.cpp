#include <cstdio>
#include <cstddef>
#include <cstdlib> // std::rand, std::srand
#include <ctime> // time
#include <string>
#include <iostream>
#include <vector>

#include "../board/Board.h"
#include "../board/SetupBoard.h"
#include "../elements/Elements.h"
#include "../player/Player.h"
#include "Game.h"

using namespace std;

int main () {
  srand((int) time(0));

  // Before gameplay starts
  Player players[4] = initPlayers();
  // Setup the board (previously in SetupBoard.cpp main)
  initBoard();
  inputBoard(TILE_POSSIBILITIES);

  // Place first round of settlements/roads
  for (int i = 0; i < players.sizeOf(); i++) {
    cout << "Turn: " << players[i].toString << endl;
    cout << "Enter the tile number to place a settlement: ";
    int tile; cin >> int tile;
    cout << "Enter the corner of the settlement: ";
    int compass; cin >> int compass;

    // Place the settlement
    placeSettlement(true);
    // TODO: PLACE THE FIRST ROADS
  }

  return 0;
}

Player[] initPlayers() {
  // Creates an array of players
  Player play[4] = {
    new Player(Color::RED, "RED"),
    new Player(Color::BLUE, "BLUE"),
    new Player(Color::WHITE, "WHITE"),
    new Player(Color::ORANGE, "ORANGE")
  };

  // Randomly shuffles the array
  random_shuffle(play, play + 4);

  // Prints order
  cout << "This is the order of play:" << endl;
  for (int i = 0; i < play.sizeOf(); i++)
    cout << i << ". " << play[i].toString() << endl;

  return play;
}
