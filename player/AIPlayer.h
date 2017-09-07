#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

using namespace std;

class AIPlayer: public Player {
    private:
        // Implemented from Player
        void inputName();

        vector<double> calculateWeights(Board &board);
        int bestCornerDP
          (Board &board, vector<double> resWeights, double probWeights);

        int chooseBestRoad(Corner corn);
};

#endif
