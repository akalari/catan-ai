#ifndef BOARDUTILS_H
#define BOARDUTILS_H

#include "Board.h"
#include <vector>
#include <string>

using namespace std;

int getOtherCorner(int e, int c);
vector<int> getSettlements(int number);
vector<int> portsOwned(int player);

bool isTwoAway(int settlement);
bool adjOwnRoad(int settlement, int player);
bool adjOwnProperty(int road, int player);

void printBoard();
void printStartRow(string (&out)[9], int row);
void printRows(string (&out)[9]);
void printTile(int tile, string (&out)[9], bool l, bool u, bool d, bool r);
string color(int player);
void printTileMiddle(int tile, string(&out)[9]);

#endif
