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
    OFFER_TRADE, USE_PORT, PLAY_DEV, END_TURN};
enum MoveState {END_TURN, MOVE_UNSUCCESSFUL, MOVE_SUCCESS};

class Player {
    protected:
        Player(int color, Board b);
        int resHand[5];
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
        // Setup data implementations
        virtual string inputName() = 0;
        virtual int getFirstSett() = 0;
        virtual int getFirstRoad() = 0;
        virtual int getSecondSett() = 0;
        virtual int getSecondRoad() = 0;

        // Move data implementations
        virtual Move getNextMove() = 0;
        virtual int getMoveSettlement() = 0;
        virtual int getMoveRoad() = 0;
        virtual int getMoveCity() = 0;
        virtual int getTradeRate()[2][2] = 0;
        virtual int getMoveDev() = 0;
        virtual int getRobberMove() = 0;
        virtual vector<int> robberDiscardCards() = 0;

        // Call subclass methods for move data
        bool moveDoMove();
        bool moveBuildSettlement();
        bool moveBuildRoad();
        bool moveBuildCity();
        bool moveBuyDev();
        bool moveOfferTrade();
        bool moveUsePort();
        bool movePlayDev();

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
