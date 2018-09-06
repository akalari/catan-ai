#ifndef BOARDUTILS_H
#define BOARDUTILS_H

#include <string> // std::string
#include <vector> // std::vector
#include "Board.h"

//general utils
string writeBoard(Board &b);
void writeBoards(string filename, vector<Board> &boards);
void readBoard(string text, Board &b);
void readBoards(string filename, vector<Board> &boards);
void randomBoards(int n, vector<Board> &boards);

#endif
