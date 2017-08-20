#ifndef CORNER_H
#define CORNER_H
#include "Tile.h"
#include "Edge.h"

struct Tile;

struct Corner {
  Tile *adjTiles; //variable length: {1, 2, 3}
  Edge *adjEdges; //variable length: {2, 3}
  Corner() {}
};

#endif
