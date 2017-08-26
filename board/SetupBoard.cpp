#include <iostream>
#include <cstdio>

#include "Board.h"
#include "../elements/Elements.h"

using namespace std;

extern Tile tiles[NUM_TILES];

int main() {
    initBoard();
    inputBoard(TILE_POSSIBILITIES);
    return 0;
}

/**
 * Loads user-input tile data into the Board
 */
void loadBoard(char tc[NUM_TILES], int ti[NUM_TILES]) {
    for(int t = 0; t < NUM_TILES; t++) {
        Tile &tile = tiles[t];
        tile.resource = getElement(tc[t]);
        tile.num = ti[t];
    }
}

/**
 * Return the element linked to the user-input char
 */
int getElement(char tc) {
    switch(tc) {
        case 'b': return BRICK;
        case 'l': return LUMBER;
        case 'w': return WOOL;
        case 'g': return GRAIN;
        case 'o': return ORE;
        case 'd': return -1; // Desert
        default: return -2;
    }
}

/**
 * Allows the user to input the values for the board resources and probabilities
 */
void inputBoard() {
  char tc[NUM_TILES] = {0};
  int ti[NUM_TILES] {0};

  for(int i = 0; i < NUM_TILES; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod][1-12] -> ";
    cin >> tc[i] >> ti[i];
  }

  loadBoard(tc, ti);
}

/**
 * Allows the user to input the values for the board resources only
 */
void inputBoard(const int possibilitiesSpiral[18]) {
  char tc[NUM_TILES] = {0};
  int ti[NUM_TILES] {0};

  for(int i = 0; i < NUM_TILES; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod]-> ";
    cin >> tc[i];
    cout << endl;
  }

  int c = 0;
  for(int i = 0; i < NUM_TILES; i++){
      if(tc[TILES_SPIRAL[i]] == 'd')continue;
      ti[TILES_SPIRAL[i]] = possibilitiesSpiral[c++];
  }

  cout << "Game board with assigned possibilities: " << endl;
  printTiles(tc, ti);
  loadBoard(tc, ti);
}

/**
 * Prints out the tiles
 */
void printTiles(char tc[NUM_TILES], int ti[NUM_TILES]) {

    for(int i = 0; i < NUM_TILES; i++) {
      if(i == 0 || i == 16) cout << "\n     ";
      else if(i == 3 || i == 12) cout << "\n   ";
      else if(i == 7)cout << "\n";
      if(tc[i])printf("[%c%02d]", tc[i], ti[i]);
      else printf("[ %1d ]", i);
    }

    cout << "\n\n";
}
