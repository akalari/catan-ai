#include <cstddef>
#include <iostream>
#include <string>

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

int main(){
  return 0;
}
