#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

using namespace std;

class AIPlayer: public Player {
    private:
        // Implemented from Player
        string inputName();
        Move getNextMove();
        int getMoveSettlement();
        int getMoveRoad();
        int getMoveCity();
        int getTradeRate()[2][2];
        int getMoveDev();
        int getRobberMove();
        vector<int> robberDiscardCards();

        vector<double> AIPlayer::calculateWeights(Board &board);
    public:
        AIPlayer(int color, Board b)
};

#endif
