#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <string>
#include "Player.h"

using namespace std;

class AIPlayer: public Player {
    private:
        // AI_NAMES{"@Sherlock", "@AlphaCatan", "@DeepGreen", "@realSlimShady"},
        static const string AI_NAMES[4];
        string getName(int color);
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

        vector<int> possibleCities();
        vector<int> possibleSettlements();
        vector<int> possibleRoads();

        vector<double> calculateWeights(Board &board);
        int bestCornerDP(Board &board, vector<double> resWeights, double probWeights);
    public:
        AIPlayer(int color, Board &b);
        vector<PairedMove> getPossibleMoves() override;
};

#endif
