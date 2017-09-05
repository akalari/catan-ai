#include "Board.h"

#ifndef TESTBOARD_H
#define TESTBOARD_H

void placeFirstSettDP(Board &b, int player);
void placeSecondSettDP(Board &b, int player);
int bestCornerDP(Board &b, double (&resourceWeights)[5]);

#endif
