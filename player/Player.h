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
    int numRoads = 15;
    int numSettlements = 5;
    int numCities = 4;
    int color = -1;
    int score = 0;
    string name = NULL;
    string color_string;
  public:
    void addResCard();
    void takeResCard();
    void addDevCard();
    void takeDevCard();
    void placeRoad();
    void placeRoad(Edge e);
    void placeSettlement();
    void placeSettlement(Corner c, bool isFirstSettlement);
    void placeCity();
    void placeCity(Corner c);
    string getName();
    int getScore();
    Player () {}
};

#endif
