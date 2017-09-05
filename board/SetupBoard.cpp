#include <iostream> // std::cout
#include <array> // std::array
#include <algorithm> // std::shuffle
#include <random> // std::default_random_engine

#include "SetupBoard.h"
#include "Board.h"
#include "../elements/Elements.h"

using namespace std;

/**
 * Loads user-input tile data into the Board
 */
void loadBoard(Board &b, char tc[NUM_TILES], int ti[NUM_TILES]) {
    for(int t = 0; t < NUM_TILES; t++) {
        Tile &tile = b.getTiles()[t];
        tile.setResource(getElement(tc[t]));
        tile.setNum(ti[t]);
    }
}

/**
 * Generates the board by randomly shuffling tiles
 * Uses the possibilitiesSpiral that is provided
 */
void randomBoard(Board &b, const int possibilitiesSpiral[18]) {
    array<char,19> tc {'b','b','b','o','o','o','l','l','l','l',
        'w','w','w','w','g','g','g','g','d'};
    int ti[NUM_TILES] {0};

    random_device rng;
    mt19937 urng(rng());
    shuffle(tc.begin(), tc.end(), urng);

    int c =0;
    for (int i = 0; i < NUM_TILES; i++) {
      if(tc[TILES_SPIRAL[i]] == 'd')continue;
      ti[TILES_SPIRAL[i]] = possibilitiesSpiral[c++];
    }

    loadBoard(b, tc.data(), ti);
}

/**
 * Allows the user to input the values for the board resources and probabilities
 */
void inputBoard(Board &b) {
 char tc[NUM_TILES] = {0};
  int ti[NUM_TILES] {0};

  for(int i = 0; i < NUM_TILES; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod][1-12] -> ";
    cin >> tc[i] >> ti[i];
  }

  loadBoard(b, tc, ti);
}

/**
 * Allows the user to input the values for the board resources only
 */
void inputBoard(Board &b, const int possibilitiesSpiral[18]) {
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

  loadBoard(b, tc, ti);
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
 * Prints out the tiles
 */
void printTiles(char tc[NUM_TILES], int ti[NUM_TILES]) {

    for(int i = 0; i < NUM_TILES; i++) {
      if(i == 0 || i == 16) cout << "\n     ";
      else if(i == 3 || i == 12) cout << "\n   ";
      else if(i == 7) cout << "\n";
      if(tc[i]) printf("[%c%02d]", tc[i], ti[i]);
      else printf("[ %1d ]", i);
    }

    cout << "\n\n";
}
