/*
NOTES
  each pair of bits in board[0-15] is a spot on the board
  from top left to bottom right
  001122334455667788
  9900112233 etc...
  
  first bit in board[16] represents turn (1 for black, 0 for white)
  last seven bits of board[16] represents the time limit in seconds (max 127)

  store a list of all possible spaces to occupy and every time a piece
  is added check its adjecent pieces and if its blank then add it to the list
  that way it only has to check some spaces for legal moves

  use wZerbra to check competetiveness
*/

#include <limits.h>
#include <iostream>
#include <fstream>
// #include <stdlib.h>
#include <time.h>  
#include "board.h"
#include "alphabeta.h"

using namespace std;

bool getPlayerType(const char &player);

// main
int main() {
  srand (time(NULL));
  // time_t start, end;
  unsigned char b[25];
  resetBoard(b);
  bool players[2]; // true if AI {Black, White}
  char player;
  cout << "Will Player 1 (Black) Be An AI (Y/N): ";
  cin >> player;
  players[0] = getPlayerType(player);
  cout << "Will Player 2 (White) Be An AI (Y/N): ";
  cin >> player;
  players[1] = getPlayerType(player);
  char loadFromFile;
  cout << "Load Board From File (Y/N): ";
  cin >> loadFromFile;
  if (loadFromFile == 'Y' || loadFromFile == 'y') {
    string buildFileName;
    cout << "File Name: ";
    cin >> buildFileName;
    buildFromFile(b, buildFileName);
  }
  // char printHeuristicChar;
  // cout << "Print Heuristic Changes (Y/N): ";
  // cin >> printHeuristicChar;
  // bool printHeuristic;
  // if (printHeuristicChar == 'Y' || printHeuristicChar == 'y') {
  //   printHeuristic = true;
  // } else {
  //   printHeuristic = false;
  // }
  do {
    printBoard(b);
    if (players[1-((b[16] & 0b10000000) ? 1 : 0)]) {
      cout << "Computing next move...\n\n";
      // int heuristicBefore = heuristic(b);
      makeMove(b, iterativeDeepening(b));
      // if (printHeuristic) {
      //   cout << "Heuristic Before: " << heuristicBefore << "\n";
      //   cout << "Heuristic Change: " << heuristic(b) - heuristicBefore << "\n";
      // }
    } else {
      int count = 0;
      unsigned char moves[9];
      unsigned char moveSelection[moves[8]];
      getLegalMoves(b, moves);
      for (int byte = 0; byte < 8; byte++) {
        for (int bit = 0; bit < 8; bit++) {
          if (moves[byte] & (0b10000000 >> bit)) {
            moveSelection[count] = (8*byte)+bit;
            cout << "Move #" << ++count << ": (Row: " << byte << ", Collumn: " << bit << ")\n";
          }
        }
      }
      int selection;
      cout << ((b[16] & 0b10000000) ? "Black's" : "White's") << " Move: ";
      cin >> selection;
      while (selection > count || selection < 0) {
        cout << "Inavlid Move... New Move: ";
        cin >> selection;
      }
      // int heuristicBefore = heuristic(b);
      makeMove(b, moveSelection[selection-1]);
      // if (printHeuristic) {
      //   cout << "Heuristic Before: " << heuristicBefore << "\n";
      //   cout << "Heuristic Change: " << heuristic(b) - heuristicBefore << "\n";
      // }
    }
  } while (getLegalMoveCount(b) > 0);
  printWinner(b);
  return 0;
}

bool getPlayerType(const char &player) {
  switch (player) {
    case 'Y':
      return true;
    case 'y':
      return true;
    case 'N':
      return false;
    case 'n':
      return false;
    default:
      return false;
  }
}