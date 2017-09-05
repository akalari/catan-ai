#ifndef BOARDUTILS_H
#define BOARDUTILS_H

#include <string> // std::string
#include <vector> // std::vector
#include "Board.h"

//general utils
void writeBoards(string filename, vector<Board> &boards);
void loadBoards(string filename, vector<Board> &boards);
void randomBoards(int n, vector<Board> &boards);

#endif
