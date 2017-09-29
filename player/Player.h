#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>

#include "../board/Board.h"

using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

enum Move {BUILD_SETT, BUILD_ROAD, BUILD_CITY, BUY_DEV,
    OFFER_TRADE, USE_PORT, PLAY_DEV, END_TURN};
enum MoveState {MOVE_ENDTURN, MOVE_UNSUCCESSFUL, MOVE_SUCCESS};

class Player {
    protected:
        Player(int color, Board &b, string name);
        int resHand[5];
        vector<int> devHand;
        vector<int> settlements;
        int numRoads;
        int numSettlements;
        int numCities;
        int color;
        int score;
        string name;
        Board &board;
    private:
        // Setup data implementations
        virtual int getFirstSett() = 0;
        virtual int getFirstRoad() = 0;
        virtual int getSecondSett() = 0;
        virtual int getSecondRoad() = 0;

        // Move data implementations
        virtual Move getNextMove() = 0;
        virtual int getMoveSettlement() = 0;
        virtual int getMoveRoad() = 0;
        virtual int getMoveCity() = 0;
        virtual void getTradeRate(int (&rate)[2][2]) = 0;
        virtual int getMoveDev() = 0;
        virtual int getRobberMove() = 0;
        virtual vector<int> robberDiscardCards() = 0;

        // Call subclass methods for move data
        MoveState moveDoMove();
        bool moveBuildSettlement();
        bool moveBuildRoad();
        bool moveBuildCity();
        bool moveBuyDev();
        bool movePlayDev();
        bool moveOfferTrade();
        bool moveUsePort();

        // Player-player interactions
        bool offerTrade(int tradeRate[2][2]);
        int stealResource();
        int stealMonopoly(int res);


    public:
        // Setup
        void placeFirstPair();
        void placeSecondPair();

        // Turn
        void collectFromRoll(int roll); // Collect resources on die roll
        void startTurn();
        void moveRobber();

        // Misc.
        int getScore(); // Return # of Victory Points
        string getName(); // Return name
        int getColor();
};

#endif
