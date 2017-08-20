#include <cstddef>
#include <iostream>
#include <stdio.h>


#include "Board.h"

using namespace std;

const int NUM_TILES = 19;
const int NUM_CORNERS = 54;
const int NUM_EDGES = 72;
const int CORNERS_PER_TILE = 6;

Tile tiles[NUM_TILES];
Corner corners[NUM_CORNERS];
Edge edges[NUM_EDGES];


void initBoard() {

  int nextEdge = 0;

  // Loop through each tile on the board
  for(int t = 0; t < NUM_TILES; t++) {
    Tile tile = tiles[t];
    // Loop through the corners of each tile
    for(int e = 0; e < CORNERS_PER_TILE; e++) {
      int adj = TILE_ADJACENCIES[t][e];
      // Assign the adjacent tile
      if(adj < 0) tile.adjTiles[e] = NULL;
      else tile.adjTiles[e] = &tiles[adj];
    }
  }
}

void printTiles(char tc[19], int ti[19]) {
    for(int i = 0; i < 19; i++) {
      if(i == 0 || i == 16) cout << "\n     ";
      else if(i == 3 || i == 12) cout << "\n   ";
      else if(i == 7)cout << "\n";

      if(tc[i])printf("[%c%02d]", tc[i], ti[i]);
      else printf("[ %1d ]", i);
    }
    cout << "\n\n";
}

void inputBoard() {
  char tc[19] = {0};
  int ti[19] {0};

  for(int i = 0; i < 19; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod][1-12] -> ";
    cin >> tc[i] >> ti[i];
  }
}

void inputBoard(const int possibilitiesSpiral[18]) {
  char tc[19] = {0};
  int ti[19] {0};

  for(int i = 0; i < 19; i++) {
    printTiles(tc, ti);
    cout << "Please specify tile " << i << ": [blwgod]-> ";
    cin >> tc[i];
    cout << endl;
  }

  int c = 0;
  for(int i = 0; i < 19; i++){
      if(tc[TILES_SPIRAL[i]] == 'd')continue;
      ti[TILES_SPIRAL[i]] = possibilitiesSpiral[c++];
  }

  cout << "Game board with assigned possibilities: " << endl;
  printTiles(tc, ti);

}

int main(){
  inputBoard(TILE_POSSIBILITIES);
  return 0;
}
