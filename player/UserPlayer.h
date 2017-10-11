#ifndef USERPLAYER_H
#define USERPLAYER_H

#include "Player.h"
#include "../board/Board.h"

using namespace std;

class UserPlayer: public Player {
    public:
        UserPlayer(int color, Board &b);
    private:
        // Setup: Implemented from Player
        int getFirstSett() override;
        int getFirstRoad() override;
        int getSecondSett() override;
        int getSecondRoad() override;

        // Moves: Implemented from Player
        PairedMove getNextMove() override;

        int getMoveSettlement();
        int getMoveRoad();
        int getMoveCity();
        int getTradeRate();
        int getMoveDev();

        int getRobberMove() override;
        vector<int> robberDiscardCards() override;

        //helper methods
        string inputName(int color);
};

#endif
