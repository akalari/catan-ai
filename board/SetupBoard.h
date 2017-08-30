#ifndef SETUPBOARD_H
#define SETUPBOARD_H

#include "Board.h"

void printTiles(char tc[NUM_TILES], int ti[NUM_TILES]);
void randomBoard(const int possibilitiesSpiral[18]);
void inputBoard();
void inputBoard(const int possibilitiesSpiral[18]);
void loadBoard(char tc[NUM_TILES], int ti[NUM_TILES]);
int getElement(char tc);

#endif
