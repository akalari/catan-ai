#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include "../board/Board.h"

using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

class Player {
  private:
    vector<int> resHand;
    vector<int> devHand;
    vector<int> settlements;
    int numRoads;
    int numSettlements;
    int numCities;
    int color;
    int score;
    string name;
    string color_string;
  public:
    void addResCard(int e);
    bool takeResCard(int e);
    void addDevCard(int e);
    bool takeDevCard(int e);
    void inputRoad();
    void placeRoad(int e);
    int getColor();
    vector<int> &getSettlements();
    void inputSettlement(bool isFirstSettlement);
    void placeSettlement(int c, bool isFirstSettlement);
    void placeCity(int c);
    void inputCity();
    string getName();
    int getScore();
    string toString();
    int bestCornerDP(Board &board, vector<double> resWeights, double probWeights);
    vector<double> calculateWeights(Board &board);
    Player(int c, string string_color);
};

#endif
