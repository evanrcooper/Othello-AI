#include "heuristic.h"

using namespace std;

// return type of alphabeta search
struct valMovePair {
  int val;
  unsigned char move;
};

// alpha beta evaluater
unsigned char minimaxAlphaBeta(unsigned char *board, const unsigned char &depth, bool *timeUp, const unsigned long long &X);

// alpha beta search (black is maximizer)
struct valMovePair minimaxValue(unsigned char *board, const unsigned char &depth, int alpha, int beta, bool *timeUp, const unsigned long long &X);

// iterative deepening alpha beta search
unsigned char iterativeDeepening(unsigned char *board);