#include <iostream>
#include <string>
#include <vector>

#include "Board.h"

using namespace std;

/**
 * Returns the corner connected to the edge
 * that is not *c
 */
int Board::getOtherCorner(int e, int c) {
    int front = edges[e].getAdjCorners().front();
    if (front == c) return edges[e].getAdjCorners().back();
    else return front;
}

/**
 * Returns a list of tiles with a certain number
 * If the tile has the robber, it isn't included
 */
vector<int> Board::getMatchingTiles(int num) {
    vector<int> matches;
    for(int i = 0; i < NUM_TILES; i++) {
        if(tiles[i].getNum() == num && !tiles[i].getRobber())
            matches.push_back(i);
    }
    return matches;
}

/**
 * Returns the list of occupied settlements adjacent
 * to a tile
 */
vector<int> Board::getSettlements(int tile) { 
    vector<int> matches;
    for (int c : tiles[tile].getAdjCorners())
      if (corners[c].getSettlement() != Corner::NO_SETTLEMENT)
        matches.push_back(c);
    return matches;
}

/**
 * Returns a list of all of the ports owned by a player
 */
vector<int> Board::portsOwned(int player) {
    vector<int> ports;
    for(Corner &c : corners) {
      if(c.getSettlement() == player && c.getPort() != 0)
        ports.push_back(c.getPort());
    }
    return ports;
}

/**
 * Checks if a settlement (or settlement spot)
 * is >= 2 spots away from other settlements
 */
bool Board::isTwoAway(int settlement) {
  // Check all adjacent edges
  for (int e : corners[settlement].getAdjEdges()) {
    int c = getOtherCorner(e, settlement);
    // If there is a settlement, return false
    if (corners[c].getSettlement() != Corner::NO_SETTLEMENT)
      return false;
  }
  return true;
}

/**
 * Checks if a new settlement is adjacent to a road
 * occupied by the player
 */
bool Board::adjOwnRoad(int settlement, int player) {
  for (int e : corners[settlement].getAdjEdges()) {
    if(edges[e].getRoad() != Edge::NONE && edges[e].getRoad() == player)
      return true;
  }
  return false;
}

/**
 * Checks if a new road is adjacent to a road or settlement occupied
 * by the player
 */
bool Board::adjOwnProperty(int road, int player) {
  for (int c : edges[road].getAdjCorners()) {
    if (corners[c].getSettlement() == player)
      return true;
    for (int e : corners[c].getAdjEdges())
      if (edges[e].getRoad() == player && e != road)
        return true;
  }
  return false;
}

/**
 * Prints the game board in its current state
 * Different segments of each tile are printed, based on its location
 * (see printTile() for more details)
 */
void Board::printBoard() {
    cout << endl << printColor(-2) << endl;
    string out[9];
    printStartRow(out, 0);
    printTile(0, out, 1,1,0,1);
    printTile(1, out, 0,1,0,1);
    printTile(2, out, 0,1,0,1);
    printRows(out);
    printStartRow(out, 1);
    printTile(3, out, 1,1,0,1);
    printTile(4, out, 0,1,0,1);
    printTile(5, out, 0,1,0,1);
    printTile(6, out, 0,1,0,1);
    printRows(out);
    printStartRow(out, 2);
    printTile(7, out, 1,1,1,1);
    printTile(8, out, 0,1,1,1);
    printTile(9, out, 0,1,1,1);
    printTile(10, out, 0,1,1,1);
    printTile(11, out, 0,1,1,1);
    printRows(out);
    printStartRow(out, 3);
    printTile(12, out, 1,0,1,1);
    printTile(13, out, 0,0,1,1);
    printTile(14, out, 0,0,1,1);
    printTile(15, out, 0,0,1,1);
    printRows(out);
    printStartRow(out, 4);
    printTile(16, out, 1,0,1,1);
    printTile(17, out, 0,0,1,1);
    printTile(18, out, 0,0,1,1);
    printRows(out);
    cout << endl << printColor(-1) << endl;
}

/**
 * Prints the correct amount of spaces per row, to pad
 * the left side of the board when printing
 *
 * Spaces are appended to each string in out[]
 */
void Board::printStartRow(string (&out)[9], int row) {
    int s = 2;
    if(row != 2)s += 7;
    if(row == 0 || row == 4)s += 7;

    for(int i=0; i < 9; i++) {
        out[i] += std::string(s, ' ');
    }
}

/**
 * Outputs the string array representing a row of tiles
 * once the array has been populated
 *
 * The strings in the out[] are cleared following printing
 */
void Board::printRows(string (&out)[9]) {
    for(int s=0; s < 9; s++){
        // Lines with less than 20 characters shouldn't be printed,
        // as they are left empty due to overlap between tiles
        if(out[s].length() > 20)
            cout << out[s] << endl;
        // Clear string
        out[s].clear();
    }
}

/**
 * Returns the color associated with a road,
 * at t[r], in ANSI escape string format
 */
string Board::roadColor(int tile, int r) {
  return printColor(edges[tiles[tile].getAdjEdges()[r]].getRoad());
}

/**
 * Returns the color associated with a settlement,
 * at t[c], in ANSI escape string format
 */
string Board::settColor(int tile, int c) {
  return printColor(corners[tiles[tile].getAdjCorners()[c]].getSettlement());
}


/**
 * Prints the desired segments of a tile to each of 9 strings
 * (one string is used per line, as each tile is 9 lines tall)
 *
 * Segments are used to ensure that overlapping regions
 * are only printed once per pair of tiles (see printBoard())
 *
 * segments:
 *     u
 *    l r
 *     d
 */
void Board::printTile(int tile, string (&out)[9], bool l, bool u, bool d, bool r) {
    string s13(13, ' ');
    string s9(9, ' ');
    string s6(6, ' ');
    string s5(5, ' ');
    string s4(4, ' ');
    string s3(3, ' ');
    string s2(2, ' ');
    string s(1, ' ');
    string clr = printColor(-2);

    if(l && u) {
        out[0] += s;
        out[1] += s;
        out[2] += settColor(tile,5) + "S" + clr;
    } if(l) {
        string cr5 = roadColor(tile,5);
        out[3] += cr5 + "|" + clr;
        out[4] += cr5 + "|" + clr;
        out[5] += cr5 + "|" + clr;
    } if(l && d) {
        out[6] += settColor(tile,4) + "S" + clr;
        out[7] += s;
        out[8] += s;
    }
    if(u) {
        out[0] += s4 + roadColor(tile,0) + "_ " +
            settColor(tile,0) + "S" + roadColor(tile,1) + " _" + s4 + clr;
        out[1] += s + roadColor(tile,0) + "_--" + s5 + roadColor(tile,1) + "--_" + s + clr;
        out[2] += s13;
    }

    printTileMiddle(tile, out);

    if(d) {
        out[6] += s + roadColor(tile,4) + "_" + s9 + roadColor(tile,3) + "_" + s + clr;
        out[7] += s2 + roadColor(tile,4) + "--_" + s3 + roadColor(tile,3) + "_--" + s2 + clr;
        out[8] += s6 + settColor(tile,3) + "S" + s6 + clr;
    }

    if(r && u) {
        out[0] += s;
        out[1] += s;
        out[2] += settColor(tile,1) + "S" + clr;
    } if(r) {
        string cr2 = roadColor(tile,2);
        out[3] += cr2 + "|" + clr;
        out[4] += cr2 + "|" + clr;
        out[5] += cr2 + "|" + clr;
    } if(r && d) {
        out[6] += settColor(tile,2) + "S" + clr;
        out[7] += s;
        out[8] += s;
    }
}

/**
* Returns the ANSI escape code string to print
* the color associated with each player
*/
string Board::printColor(int player) {
    switch(player) {
        case 0: // Red
            return "\033[31;1m";
        case 1: // Blue
            return "\033[36;1m";
        case 2: // White
            return "\033[37;1m";
        case 3: // Orange (yellow)
            return "\033[33;1m";
        case -2: // Magenta (neutral)
            return "\033[0m\033[35m";
        default:
            return "\033[0m";
    }
}

/**
 * Prints ASCII art representing the middle of each tile
 * depending on the tile's resource type
 */
void Board::printTileMiddle(int tile, string (&out)[9]) {
    string clr = printColor(-2);
    string red = "\033[31m";
    string green = "\033[32m";
    string yellow = "\033[33m";
    string grey = "\033[30;1m";

    char buff[10];
    sprintf(buff, "%02d", tiles[tile].getNum());
    string n = buff;


    switch(tiles[tile].getResource()) {
        case BRICK:
            out[3] += red + "___|___|___|_" + clr;
            out[4] += red + "_|___" + n + "__|___" + clr;
            out[5] += red + "___|___|___|_" + clr;
            return;
        case LUMBER:
            out[3] += green + " /\\ /\\    /\\ " + clr;
            out[4] += green + " /\\ /\\ " + n + " /\\ " + clr;
            out[5] += green + " || ||    || " + clr;
            return;
        case WOOL:
            out[3] += green + "  " + n + "   _--_  " + clr;
            out[4] += green + "  _--_-    -_" + clr;
            out[5] += green + "_-    -_     " + clr;
            return;
        case GRAIN:
            out[3] += yellow + "    \"        " + clr;
            out[4] += yellow + "   \\|/  " + n + "   " + clr;
            out[5] += yellow + "    |        " + clr;
            return;
        case ORE:
            out[3] += grey + "  /\\/\\ /\\    " + clr;
            out[4] += grey + " /  \\ /" + n + "\\   " + clr;
            out[5] += grey + "/    /    \\  " + clr;
            return;
        default:
            out[3] += yellow + "   {}      @ " + clr;
            out[4] += yellow + "--_{}  _{}--_" + clr;
            out[5] += yellow + "   {}_- {}   " + clr;
            return;
    }
}
