#include <cstddef>
#include <iostream>
#include <cstdio>
#include <vector>

#include "../board/Board.h"

using namespace std;

#ifndef PLAYER_H
#define PLAYER_H

enum Move {BUILD_SETT, BUILD_ROAD, BUILD_CITY, BUY_DEV, OFFER_TRADE, USE_PORT, PLAY_DEV, END_TURN };

enum MoveState {MOVE_UNSUCCESSFUL, MOVE_SUCCESS};

struct PairedMove {
    Move move;
    int parameter;
    MoveState state;
    PairedMove(Move m, int p): move(m), parameter(p), state(MOVE_SUCCESS) {}
};

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
        virtual PairedMove getNextMove() = 0;
        virtual int getRobberMove() = 0;
        virtual vector<int> robberDiscardCards() = 0;

        // Call subclass methods for move data
        PairedMove moveDoMove();
        bool moveBuildSettlement(int c);
        bool moveBuildRoad(int edge);
        bool moveBuildCity(int c);
        bool moveBuyDev();
        bool movePlayDev(int dev);
        bool moveOfferTrade(int rate);
        bool moveUsePort(int rate);

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
        void moveRobber();

        // Misc.
        int getScore(); // Return # of Victory Points
        string getName(); // Return name
        int getColor();
};

#endif
