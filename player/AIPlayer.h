#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

using namespace std;

class AIPlayer: public Player {
    private:
        // Implemented from Player
        void inputName();

        vector<double> AIPlayer::calculateWeights(Board &board);
};

#endif
