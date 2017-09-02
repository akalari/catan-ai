#ifndef SETUPBOARD_H
#define SETUPBOARD_H

#include "Board.h"

void loadBoard(Board &b, char tc[NUM_TILES], int ti[NUM_TILES]);
void randomBoard(Board &b, const int possibilitiesSpiral[18]);
void inputBoard(Board &b);
void inputBoard(Board &b, const int possibilitiesSpiral[18]);
int getElement(char tc);
void printTiles(char tc[NUM_TILES], int ti[NUM_TILES]);

#endif
