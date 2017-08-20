#ifndef TILE_H
#define TILE_H
#include "Corner.h"
#include "Edge.h"


struct Tile {
  Corner *adjSettlements[6];
  Edge *adjEdges[6];
  Tile *adjTiles[6];
};

#endif
