#include "BoardUtils.h"
#include "Board.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

extern Tile tiles[NUM_TILES];
extern Corner corners[NUM_TILES];
extern Edge edges[NUM_EDGES];
extern Port ports[NUM_PORTS];

/**
 * Returns the corner connected to the edge
 * that is not *c
 */
int getOtherCorner(int e, int c) {
    int front = e.getAdjCorners().front();
    if (front == c) return e.getAdjCorners().back();
    else return front;
}

/**
 * Returns a list of tiles with a certain number
 * If the tile has the robber, it isn't included
 */
vector<int> getTiles(int num) {
    vector<int> matches;
    for(Tile &t : tiles) {
        if(t.num == num && !t.getRobber)
            matches.push_back(&t);
    }
}

/**
 * Returns the list of occupied settlements adjacent
 * to a tile
 */
vector<int> getSettlements(int tile) {
    vector<int> matches;
    for (int c : tiles[tile].getAdjCorners())
      if (corners[c].getSettlement() != Corner::NO_SETTLEMENT)
        matches.push_back(c);
    return matches;
}

/**
 * Returns a list of all of the ports owned by a player
 */
vector<int> portsOwned(int player) {
    vector<int> ports;
    for(Corner &c : corners) {
      if(c.getSettlement() == player && c.getPort() != 0)
        ports.push_back(c.getPort().getIndex());
    }
    return ports;
}

/**
 * Checks if a settlement (or settlement spot)
 * is >= 2 spots away from other settlements
 */
bool isTwoAway(int settlement) {
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
bool adjOwnRoad(int settlement, int player) {
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
bool adjOwnProperty(int road, int player) {
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
void printBoard() {
    cout << endl << color(-2) << endl;
    string out[9];
    printStartRow(out, 0);
    printTile(&tiles[0], out, 1,1,0,1);
    printTile(&tiles[1], out, 0,1,0,1);
    printTile(&tiles[2], out, 0,1,0,1);
    printRows(out);
    printStartRow(out, 1);
    printTile(&tiles[3], out, 1,1,0,1);
    printTile(&tiles[4], out, 0,1,0,1);
    printTile(&tiles[5], out, 0,1,0,1);
    printTile(&tiles[6], out, 0,1,0,1);
    printRows(out);
    printStartRow(out, 2);
    printTile(&tiles[7], out, 1,1,1,1);
    printTile(&tiles[8], out, 0,1,1,1);
    printTile(&tiles[9], out, 0,1,1,1);
    printTile(&tiles[10], out, 0,1,1,1);
    printTile(&tiles[11], out, 0,1,1,1);
    printRows(out);
    printStartRow(out, 3);
    printTile(&tiles[12], out, 1,0,1,1);
    printTile(&tiles[13], out, 0,0,1,1);
    printTile(&tiles[14], out, 0,0,1,1);
    printTile(&tiles[15], out, 0,0,1,1);
    printRows(out);
    printStartRow(out, 4);
    printTile(&tiles[16], out, 1,0,1,1);
    printTile(&tiles[17], out, 0,0,1,1);
    printTile(&tiles[18], out, 0,0,1,1);
    printRows(out);
    cout << endl << color(0) << endl;
}

/**
 * Prints the correct amount of spaces per row, to pad
 * the left side of the board when printing
 *
 * Spaces are appended to each string in out[]
 */
void printStartRow(string (&out)[9], int row) {
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
void printRows(string (&out)[9]) {
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
string roadColor(int tile, int r) {
  return color(tile.getAdjEdges()[r].getRoad());
}

/**
 * Returns the color associated with a settlement,
 * at t[c], in ANSI escape string format
 */
string settColor(int tile, int c) {
  return color(tile.getAdjCorners()[c].getSettlement());
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
void printTile(int tile, string (&out)[9], bool l, bool u, bool d, bool r) {
    string s13(13, ' ');
    string s9(9, ' ');
    string s6(6, ' ');
    string s5(5, ' ');
    string s4(4, ' ');
    string s3(3, ' ');
    string s2(2, ' ');
    string s(1, ' ');
    string clr = color(-2);

    if(l && u) {
        out[0] += s;
        out[1] += s;
        out[2] += settColor(tiles[tile],5) + "S" + clr;
    } if(l) {
        string cr5 = roadColor(tiles[tile],5);
        out[3] += cr5 + "|" + clr;
        out[4] += cr5 + "|" + clr;
        out[5] += cr5 + "|" + clr;
    } if(l && d) {
        out[6] += settColor(tiles[tile],4) + "S" + clr;
        out[7] += s;
        out[8] += s;
    }
    if(u) {
        out[0] += s4 + roadColor(tiles[tile],0) + "_ " +
            settColor(tiles[tile],0) + "S" + roadColor(tiles[tile],1) + " _" + s4 + clr;
        out[1] += s + roadColor(tiles[tile],0) + "_--" + s5 + roadColor(tiles[tile],1) + "--_" + s + clr;
        out[2] += s13;
    }

    printTileMiddle(tiles[tile], out);

    if(d) {
        out[6] += s + roadColor(tiles[tile],4) + "_" + s9 + roadColor(tiles[tile],3) + "_" + s + clr;
        out[7] += s2 + roadColor(tiles[tile],4) + "--_" + s3 + roadColor(tiles[tile],3) + "_--" + s2 + clr;
        out[8] += s6 + settColor(tiles[tile],3) + "S" + s6 + clr;
    }

    if(r && u) {
        out[0] += s;
        out[1] += s;
        out[2] += settColor(tiles[tile],1) + "S" + clr;
    } if(r) {
        string cr2 = roadColor(tiles[tile],2);
        out[3] += cr2 + "|" + clr;
        out[4] += cr2 + "|" + clr;
        out[5] += cr2 + "|" + clr;
    } if(r && d) {
        out[6] += settColor(tiles[tile],2) + "S" + clr;
        out[7] += s;
        out[8] += s;
    }
}

/**
* Returns the ANSI escape code string to print
* the color associated with each player
*/
string color(int player) {
    switch(player) {
        case 1: // Red
            return "\033[31;1m";
        case 2: // Blue
            return "\033[36;1m";
        case 3: // White
            return "\033[37;1m";
        case 4: // Orange (yellow)
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
void printTileMiddle(int tile, string (&out)[9]) {
    string clr = color(0);
    string red = "\033[31m";
    string green = "\033[32m";
    string yellow = "\033[33m";
    string grey = "\033[30;1m";

    char buff[10];
    sprintf(buff, "%02d", tiles[tile].getNum());
    string n = buff;


    switch(tiles[tile].getResource()) {
        case 0:
            out[3] += red + "___|___|___|_" + clr;
            out[4] += red + "_|___" + n + "__|___" + clr;
            out[5] += red + "___|___|___|_" + clr;
            return;
        case 1:
            out[3] += green + " /\\ /\\    /\\ " + clr;
            out[4] += green + " /\\ /\\ " + n + " /\\ " + clr;
            out[5] += green + " || ||    || " + clr;
            return;
        case 2:
            out[3] += green + "  " + n + "   _--_  " + clr;
            out[4] += green + "  _--_-    -_" + clr;
            out[5] += green + "_-    -_     " + clr;
            return;
        case 3:
            out[3] += yellow + "    \"        " + clr;
            out[4] += yellow + "   \\|/  " + n + "   " + clr;
            out[5] += yellow + "    |        " + clr;
            return;
        case 4:
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