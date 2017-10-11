#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

#include "../board/Board.h"
#include "../board/SetupBoard.h"

#include "Player.h"
#include "UserPlayer.h"
#include "AIPlayer.h"

using namespace std;

int main() {
    cout << "Testing Player" << endl;
    srand((unsigned)time(0));

    Board b = Board();

    randomBoard(b, TILE_POSSIBILITIES);

    // Player* gamePlayers[4];
    // for(int p = 0; p < 4; p++) {
    //     gamePlayers[p] = new AIPlayer(p, b);
    // }
    //
    // random_device rng;
    // mt19937 urng(rng());
    // shuffle(begin(gamePlayers), end(gamePlayers), urng);
    //
    // cout << "Let's play catan! Here's our order: " << endl;
    // for(int p = 0; p < 4; p++) cout << gamePlayers[p]->getName() << " ";
    // cout << endl;
    //
    // for(Player* p:gamePlayers) p->placeFirstPair();
    // for(int p = 3; p > 0; p--) gamePlayers[p]->placeSecondPair();

    b.printBoard();
    return 0;
}
