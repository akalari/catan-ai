#include <iostream> // std::cout

#include "Board.h"
#include "BoardUtils.h"
#include "SetupBoard.h"

using namespace std;

int main() {
    cout << "Testing Board" << endl;
    initBoard();
    randomBoard(TILE_POSSIBILITIES);
    printBoard();
    return 0;
}
