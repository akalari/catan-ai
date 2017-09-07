#ifndef USERPLAYER_H
#define USERPLAYER_H

#include "Player.h"

using namespace std;

class UserPlayer: public Player {
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

    public:
        UserPlayer(int color, Board b);
};

#endif
