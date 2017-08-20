#ifndef EDGE_H
#define EDGE_H
#include "Corner.h"

struct Corner;

struct Edge {
  Corner *adjCorners[2];
};

#endif
