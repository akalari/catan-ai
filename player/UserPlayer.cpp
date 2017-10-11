#include <iostream>
#include <string>
#include <regex>

#include "UserPlayer.h"

using namespace std;

UserPlayer::UserPlayer(int color, Board &b):
    Player(color, b, inputName(color))
{}

/**
 * Returns the corner index for placing the first settlement
 */
int UserPlayer::getFirstSett() {
    board.printBoard(true);
    int t = -1;
    int c = -1;
    cout << name << ", please enter your first settlement:" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    cout << endl;
    cout << "   _ 0 _   " << endl;
    cout << " 5       1 " << endl;
    cout << " |       | " << endl;
    cout << " 4 _   _ 2 " << endl;
    cout << "     3     ";
    do {
        cout << "Enter a corner [0-5] -> ";
        cin >> c;
    } while(c < 0 || c > 5);

    return board.getTiles()[t].getAdjCorners()[c];
}

/**
 *  Returns the edge index for placing the first road
 */
int UserPlayer::getFirstRoad() {
    board.printBoard(true);
    int t = -1;
    int e = -1;
    cout << name << ", please enter your first road:" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    cout << "        *        " << endl;
    cout << "     0     1     " << endl;
    cout << " *             * " << endl;
    cout << "                 " << endl;
    cout << " 5             2 " << endl;
    cout << "                 " << endl;
    cout << " *             * " << endl;
    cout << "     4     3     " << endl;
    cout << "        *        ";
    do {
        cout << "Enter an edge [0-5] -> ";
        cin >> e;
    } while(e < 0 || e > 5);

    return board.getTiles()[t].getAdjEdges()[e];
}

/**
 * Returns the corner index for placing the second settlement
 */
int UserPlayer::getSecondSett() {
    board.printBoard(true);
    int t = -1;
    int c = -1;
    cout << name << ", please enter your second settlement:" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    cout << endl;
    cout << "   _ 0 _   " << endl;
    cout << " 5       1 " << endl;
    cout << " |       | " << endl;
    cout << " 4 _   _ 2 " << endl;
    cout << "     3     ";
    do {
        cout << "Enter a corner [0-5] -> ";
        cin >> c;
    } while(c < 0 || c > 5);

    return board.getTiles()[t].getAdjCorners()[c];
}

/**
 * Returns the edge index for placing the second road
 */
int UserPlayer::getSecondRoad() {
    board.printBoard(true);
    int t = -1;
    int e = -1;
    cout << name << ", please enter your second road:" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    cout << endl;
    cout << "       _ * _       " << endl;
    cout << "   _ 0       1 _   " << endl;
    cout << " *               * " << endl;
    cout << " |               | " << endl;
    cout << " 5               2 " << endl;
    cout << " |               | " << endl;
    cout << " * _           _ * " << endl;
    cout << "     4 _   _ 3     " << endl;
    cout << "         *         ";
    do {
        cout << "Enter an edge [0-5] -> ";
        cin >> e;
    } while(e < 0 || e > 5);

    return board.getTiles()[t].getAdjEdges()[e];
}

/**
 * Returns the next move type that the player should take
 */
PairedMove UserPlayer::getNextMove() {
    regex valid_move("sett|road|city|buydv|trade|port|pdev|end");
    string input = "";
    while(regex_match(input, valid_move)) {
        cout << name << ", what's your next move? ['help'|moveType] -> " << endl;
        if(name == "help") {
            cout << "----- Move Input Help -----" << endl;
            cout << "sett  > build a settlement" << endl;
            cout << "road  > build a road" << endl;
            cout << "city  > build a city" << endl;
            cout << "buydv > buy a dev card" << endl;
            cout << "pdev  > play a dev card" << endl;
            cout << "trade > make a trade" << endl;
            cout << "port  > use a port" << endl;
            cout << "end   > end your turn" << endl;
            cout << "---------------------------" << endl;
            cout << endl;
        }
    }

    if(input == "sett") return PairedMove(BUILD_SETT, getMoveSettlement());
    if(input == "road") return PairedMove(BUILD_ROAD, getMoveRoad());
    if(input == "city") return PairedMove(BUILD_CITY, getMoveCity());
    if(input == "buydv") return PairedMove(BUY_DEV, 0);
    if(input == "pdev") return PairedMove(PLAY_DEV, getMoveDev());
    if(input == "trade") return PairedMove(OFFER_TRADE, getTradeRate());
    if(input == "port") return PairedMove(USE_PORT, getTradeRate());
    return PairedMove(END_TURN, 0);
}

/**
 * Returns the location of the settlement to build
 */
int UserPlayer::getMoveSettlement() {
    board.printBoard(true);
    int t = -1;
    int c = -1;
    cout << name << ", please enter your next settlement:" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    cout << endl;
    cout << "   _ 0 _   " << endl;
    cout << " 5       1 " << endl;
    cout << " |       | " << endl;
    cout << " 4 _   _ 2 " << endl;
    cout << "     3     ";
    do {
        cout << "Enter a corner [0-5] -> ";
        cin >> c;
    } while(c < 0 || c > 5);

    return board.getTiles()[t].getAdjCorners()[c];
}

/**
 * Returns the location of the road to build
 */
int UserPlayer::getMoveRoad() {
    board.printBoard(true);
    int t = -1;
    int e = -1;
    cout << name << ", please enter your first road:" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    cout << endl;
    cout << endl;
    cout << "       _ * _       " << endl;
    cout << "   _ 0       1 _   " << endl;
    cout << " *               * " << endl;
    cout << " |               | " << endl;
    cout << " 5               2 " << endl;
    cout << " |               | " << endl;
    cout << " * _           _ * " << endl;
    cout << "     4 _   _ 3     " << endl;
    cout << "         *         ";
    do {
        cout << "Enter an edge [0-5] -> ";
        cin >> e;
    } while(e < 0 || e > 5);

    return board.getTiles()[t].getAdjEdges()[e];
}

/**
 * Returns the location of the settlement to upgrade to a city
 */
int UserPlayer::getMoveCity() {
    board.printBoard(true);
    int t = -1;
    int c = -1;
    cout << name << ", please specify the settlement to upgrade:" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    cout << endl;
    cout << "   _ 0 _   " << endl;
    cout << " 5       1 " << endl;
    cout << " |       | " << endl;
    cout << " 4 _   _ 2 " << endl;
    cout << "     3     ";
    do {
        cout << "Enter a corner [0-5] -> ";
        cin >> c;
    } while(c < 0 || c > 5);

    return board.getTiles()[t].getAdjCorners()[c];
}

/**
 * Returns the rate to trade with either the port,
 * bank, or another player
 * @return a reference to a 2-D array, format:
 * {{giveRes, giveAmt}, {recvRes, RecvAmt}}
 */
void UserPlayer::getTradeRate(int (&rate)[2][2]) {
    rate[0][0] = 0;
    rate[0][1] = 0;
    rate[1][0] = 0;
    rate[1][1] = 0;
}

/**
 * Returns the dev card to play
 */
int UserPlayer::getMoveDev() {
    return 0;
}

/**
 * Returns the location (tile) to move the robber
 */
int UserPlayer::getRobberMove() {
    board.printBoard(true);
    int t = -1;
    cout << name << ", which tile should the robber be placed on?" << endl;
    do {
        cout << "Enter a tile [0-18] -> ";
        cin >> t;
    } while(t < 0 || t > 18);
    return t;
}

/**
 * Returns half of the deck to discard,
 * if the player has more than 7 cards
 */
vector<int> UserPlayer::robberDiscardCards() {
    return vector<int>();
}

/**
 * Inputs the name of the player
 */
string UserPlayer::inputName(int color) {
    string n;
    cout << "My color is " << COLOR_STRINGS[color] << ". What is my name? ";
    cin >> n;

    string clr = "\033[0m";
    switch(color) {
        case 0: // Red
            return "\033[31;1m" + n + clr;
        case 1: // Blue
            return "\033[36;1m" + n + clr;
        case 2: // White
            return "\033[37;1m" + n + clr;
        case 3: // Orange (yellow)
            return "\033[33;1m" + n + clr;
    }

    return n;
}
