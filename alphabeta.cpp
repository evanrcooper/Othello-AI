#include <limits.h>
#include "alphabeta.h"
#include "board.h"
#include "heuristic.h"
#include "timeKeep.h"

using namespace std;

struct valMovePair minimaxValue(unsigned char *board, const unsigned char &depth, int alpha, int beta, bool *timeUp, const unsigned long long &X) {
  // https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-4-alpha-beta-pruning/

  // heuristic needs to be black_eval - white_eval
  
  // function minimax(node, depth, isMaximizingPlayer, alpha, beta):
  
  // get legalMoveCount to check if node is a leaf node
  unsigned char moves[9];
  getLegalMoves(board, moves);
  
  if (depth == 0 || moves[8] == 0) {
    //   if node is a leaf node :
    //       return value of the node
    return (struct valMovePair) {heuristic(board), 64};
  }

  // if (moves[8] == 1) { // if only 1 move avaialable skip the search
  //   for (unsigned char byte = 0; byte < 8; byte++) {
  //     for (unsigned char bit = 0; bit < 8; bit++) {
  //       if (moves[byte] & (0b10000000 >> bit)) {
  //         return (struct valMovePair) {heuristic(board), static_cast<unsigned char>((byte<<3)+bit)};
  //       }
  //     }
  //   }
  //   // return (struct valMovePair) {0, 64}; // couldn't find the legal move
  // }

  if (millisecondsSinceX(X) > (((board[16] & 0b011111111)*1000) - 10)) {
    //   if time is up
    //      exit
    *timeUp = true;
    return (struct valMovePair) {0, 64};
  }
  if (board[16] & 0b10000000) { // if black's turn (max)
    //   if isMaximizingPlayer :
    //       bestVal = -INFINITY 
    //       for each child node :
    //           value = minimax(node, depth+1, false, alpha, beta)
    //           bestVal = max( bestVal, value) 
    //           alpha = max( alpha, bestVal)
    //           if beta <= alpha:
    //               break
    //       return bestVal
    struct valMovePair bestVal = {INT_MIN, 64};
    for (unsigned char byte = 0; byte < 8; byte++) {
      for (unsigned char bit = 0; bit < 8; bit++) {
        if (moves[byte] & (0b10000000 >> bit)) {
          unsigned char tempBoard[25];
          copy(board, board+25, tempBoard);
          makeMove(tempBoard, ((byte << 3) + bit));
          struct valMovePair curVal = minimaxValue(tempBoard, depth-1, alpha, beta, timeUp, X);
          curVal.move = (byte<<3) + bit;
          if (bestVal.val < curVal.val && curVal.move < 64) {
            bestVal.move = curVal.move;
            bestVal.val = curVal.val;
          }  else if (bestVal.val == curVal.val && curVal.move < 64 && timeSinceEpochMillisec() & 1) {
            bestVal.move = curVal.move;
            bestVal.val = curVal.val;
          }
          alpha = max(alpha, bestVal.val);
          if (beta <= alpha) {
            bit = 8;
            byte = 8;
          }
        }
      }
    }
    return bestVal;
  } else { // white's turn (min)
    //   else :
    //       bestVal = +INFINITY 
    //       for each child node :
    //           value = minimax(node, depth+1, true, alpha, beta)
    //           bestVal = min( bestVal, value) 
    //           beta = min( beta, bestVal)
    //           if beta <= alpha:
    //               break
    //       return bestVal
    struct valMovePair bestVal = {INT_MAX, 64};
    for (unsigned char byte = 0; byte < 8; byte++) {
      for (unsigned char bit = 0; bit < 8; bit++) {
        if (moves[byte] & (0b10000000 >> bit)) {
          unsigned char tempBoard[25];
          copy(board, board+25, tempBoard);
          makeMove(tempBoard, ((byte << 3) + bit));
          struct valMovePair curVal = minimaxValue(tempBoard, depth-1, alpha, beta, timeUp, X);
          curVal.move = (byte<<3) + bit;
          if (bestVal.val > curVal.val && curVal.move < 64) {
            bestVal.move = curVal.move;
            bestVal.val = curVal.val;
          } else if (bestVal.val == curVal.val && curVal.move < 64 && timeSinceEpochMillisec() & 1) {
            bestVal.move = curVal.move;
            bestVal.val = curVal.val;
          }
          beta = min(beta, bestVal.val);
          if (beta <= alpha) {
            bit = 8;
            byte = 8;
          }
        }
      }
    }
    return bestVal;
  }
}

unsigned char minimaxAlphaBeta(unsigned char *board, const unsigned char &depth, bool *timeUp, const unsigned long long &X) {
  return minimaxValue(board, depth, INT_MIN, INT_MAX, timeUp, X).move;
}

unsigned char iterativeDeepening(unsigned char *board) {
  int i = 1;
  unsigned char tempMove;
  unsigned char curBestMove = 64;
  bool timeUp = false;
  int maxTimeMillis = 1000*(board[16] & 0b01111111);
  unsigned long long startTime = timeSinceEpochMillisec();
  unsigned char moves[9];
  getLegalMoves(board, moves);
  if (moves[8] == 1) {
    for (unsigned char byte = 0; byte < 8; byte++) {
      for (unsigned char bit = 0; bit < 8; bit++) {
        if (moves[byte] & (0b10000000 >> bit)) {
          cout << "Depth Searched: 0 (Only One Legal Move)\n";
          cout << "Time Taken By AI (seconds): " << static_cast<double>(millisecondsSinceX(startTime))/1000 << "\n";
          return (byte << 3) + bit;
        }
      }
    }
    return 64;
  }
  while ((millisecondsSinceX(startTime) < (maxTimeMillis >> 1)) && (!timeUp)) {
    tempMove = minimaxAlphaBeta(board, i, &timeUp, startTime);
    if (timeUp) {
      i++;
      break;
    }
    if (tempMove != 64) {
      curBestMove = tempMove;
    }
    i++;
  }
  cout << "Depth Searched: " << i-1 << "\n";
  cout << "Time Taken By AI (seconds): " << static_cast<double>(millisecondsSinceX(startTime))/1000 << "\n";
  return curBestMove;
}
