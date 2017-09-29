#ifndef AIPLAYER_H
#define AIPLAYER_H

#include "Player.h"

using namespace std;

class AIPlayer: public Player {
    private:
        const string AINames[4] = {"@Sherlock", "@AlphaCatan", "@DeepGreen", "@realSlimShady"};

        // Setup: Implemented from Player
        int getFirstSett() override;
        int getFirstRoad() override;
        int getSecondSett() override;
        int getSecondRoad() override;

        // Moves: Implemented from Player
        Move getNextMove() override;
        int getMoveSettlement() override;
        int getMoveRoad() override;
        int getMoveCity() override;
        void getTradeRate(int (&rate)[2][2]) override;
        int getMoveDev() override;
        int getRobberMove() override;
        vector<int> robberDiscardCards() override;

        vector<double> calculateWeights(Board &board);
        int bestCornerDP(Board &board, vector<double> resWeights, double probWeights);
    public:
        AIPlayer(int color, Board &b);
};

#endif
