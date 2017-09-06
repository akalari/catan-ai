#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include "../board/Board.h"

using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

enum Move {BUILD_SETT, BUILD_ROAD, BUILD_CITY, BUY_DEV,
    OFFER_TRADE, USE_PORT, PLAY_DEV};

class Player {
    protected:
        vector<int> resHand;
        vector<int> devHand;
        vector<int> settlements;
        int numRoads;
        int numSettlements;
        int numCities;
        int color;
        int score;
        string name;
        Board board;
    private:
        // Implemented by subclasses
        virtual void inputName() = 0;

        virtual Move getNextMove() = 0;
        virtual int getMoveSettlement() = 0;
        virtual int getMoveRoad() = 0;
        virtual int getMoveCity() = 0;
        virtual int getTradeRate()[2][2] = 0;
        virtual int getMoveDev() = 0;

        // Call subclass methods for move data
        bool moveBuildSettlement();
        bool moveBuildRoad();
        bool moveBuildCity();
        bool moveBuyDev();
        bool moveOfferTrade();
        bool moveUsePort();
        bool movePlayDev();
        void moveEndTurn();

        void robberDiscardCards();
        void robberMove();

        // Player-player interactions
        bool offerTrade(int tradeRate[2][2]);
        int stealResource();
        int stealMonopoly(int res);
    public:
        // Setup
        Player(int color, Board b);
        void placeFirstSettlement();
        void placeSecondSettlement();

        // Turn
        void collectFromRoll(int roll); // Collect resources on die roll
        void startTurn();
        void moveRobber();

        // Misc.
        int getScore(); // Return # of Victory Points
        string getName(); // Return name
        int getColor();
};

    // // OLD
    // void addResCard(int e);
    // bool takeResCard(int e);
    // void addDevCard(int e);
    // bool takeDevCard(int e);
    // void inputRoad();
    // void placeRoad(int e);
    // int getColor();
    // vector<int> &getSettlements();
    // void inputSettlement(bool isFirstSettlement);
    // void placeSettlement(int c, bool isFirstSettlement);
    // void placeCity(int c);
    // void inputCity();
    // string getName();
    // string toString();
    // int bestCornerDP(Board &board, vector<double> resWeights, double probWeights);
    // vector<double> calculateWeights(Board &board);
};

#endif
