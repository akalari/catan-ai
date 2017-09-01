#ifndef BOARDUTILS_H
#define BOARDUTILS_H

#include "Board.h"
#include <vector>

Corner* getOtherCorner(Edge* e, Corner* c);
std::vector<Corner*> getSettlements(int number);
std::vector<Port*> portsOwned(int player);

bool isTwoAway(Corner* settlement);
bool adjOwnRoad(Corner* settlement, int player);
bool adjOwnProperty(Edge* road, int player);

void printBoard();
void printStartRow(std::string (&out)[9], int row);
void printRows(std::string (&out)[9]);
void printTile(Tile *t, std::string (&out)[9], bool l, bool u, bool d, bool r);
std::string color(int player);
void printTileMiddle(Tile *t, std::string(&out)[9]);

#endif
