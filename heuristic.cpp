#include <limits.h>
#include <cmath>
#include "heuristic.h"
#include "board.h"
#include "weights.h"

int heuristic(unsigned char *board) {
    int h1 = heuristic1(board);
    if (h1 > (INT_MAX-64) || h1 < (INT_MIN+64)) {
        return h1;
    }
    int h2 = heuristic2(board);
    return (H1_WEIGHT*h1 + H2_WEIGHT*h2) >> SHIFT;
}

int heuristic1(unsigned char *board) {
    int totalPieces = 0;
    int totalWhite = 0;
    int totalBlack = 0;
    for (int i = 0; i < 64; i++) {
        switch (getPieceAtPos(board, i)) {
            case 1:
                totalWhite++;
                break;
            case 2:
                totalWhite++;
                break;
            default:
                break;
        }
        totalPieces = totalBlack + totalWhite;
    }
    // return totalBlack-totalWhite;
    int diff = 0; // blackEval - whiteEval
    int bl = 0; // black weight
    int wh = 0; // white weight
    unsigned char turn = (board[16] & 0b10000000) ? 2 : 1;
    for (int i = 0; i < 64; i++) {
        switch (getPieceAtPos(board, i)) {
            case 1:
                wh += weight(board, i, totalPieces, turn);
                continue;
            case 2:
                bl += weight(board, i, totalPieces, turn);
                continue;
            default:
                continue;
        }
    }
    int moveDiff1 = getLegalMoveCount(board);
    switchTurn(board);
    int moveDiff2 = getLegalMoveCount(board);
    switchTurn(board);
    if ((moveDiff1 == 0 && moveDiff2 == 0) || totalPieces >= ENDGAME) {
        if (totalBlack > totalWhite) {
            return (INT_MAX - 64) + (totalBlack - totalWhite);
        } else if (totalBlack < totalWhite) {
            return (INT_MIN + 64) + (totalWhite - totalBlack);
        } else {
            return 0;
        }
    }
    int moveDiff = ((moveDiff1 - moveDiff2) << 9) / (moveDiff1 + moveDiff2);
    if (!(board[16] & 0b10000000)) {
        moveDiff *= -1;
    }
    diff += (5*(bl - wh)) >> 2;
    diff += ((97 - totalPieces) >> 2)*moveDiff;
    diff += (moveDiff >> 1)*MOVES_WEIGHT;
    diff += totalBlack*PIECE_DIFF_WEIGHT;
    diff -= totalWhite*PIECE_DIFF_WEIGHT;
    return (diff >> 1);
}

int heuristic2(unsigned char *board) {
    int weightBL = 0;
    int countBl = 0;
    int movesBl = 0;
    int weightWh = 0;
    int countWh = 0;
    int movesWh = 0;
    int total = 0;
    for (int i = 0; i < 64; i++) {
        switch (getPieceAtPos(board, i)) {
            case 1:
                countWh++;
                weightWh += spaces[i];
                break;
            case 2:
                countBl++;
                weightBL += spaces[i];
                break;
            default:
                break;
        }
    }
    if (weightBL + weightWh != 0) {
        total += ((weightBL - weightWh) << 7) / (weightBL + weightWh);
    }
    movesBl = getLegalMoveCount(board);
    switchTurn(board);
    movesWh = getLegalMoveCount(board);
    switchTurn(board);
    if ((movesBl == 0 && movesWh == 0) || countBl + countWh >= ENDGAME) {
        if (countBl > countWh) {
            return (INT_MAX - 64) + (countBl - countWh);
        } else if (countBl < countWh) {
            return (INT_MIN + 64) + (countWh - countBl);
        } else {
            return 0;
        }
    };
    if (!(board[16] & 0b10000000)) {
        swap(movesBl, movesWh);
    }
    total += MOVES_WEIGHT*(((movesBl - movesWh) << 8) / (movesBl + movesWh));
    total += PIECE_DIFF_WEIGHT*(countBl - countWh);
    return (total << 1);
}

int weight(const unsigned char *board, const unsigned char &position, const int &piecesOnBoard, const unsigned char &turn) {
    if (piecesOnBoard < ENDGAME) {
        unsigned char posMod8 = position & 0b00000111;
        if (position == 0 || position == 7 || position == 56 || position == 63) {
            return CORNER;
        } else if (position == 1 || position == 8 || position == 9) { // around top left corner
            if (getPieceAtPos(board, 0) == turn) {
                return NEAR_CORNER_POSITIVE;
            } else {
                return NEAR_CORNER_NEGATIVE;
            }
        } else if (position == 6 || position == 14 || position == 15) { // around top right corner
            if (getPieceAtPos(board, 7) == turn) {
                return NEAR_CORNER_POSITIVE;
            } else {
                return NEAR_CORNER_NEGATIVE;
            }
        } else if (position == 48 || position == 49 || position == 57) { // around bottom left
            if (getPieceAtPos(board, 56) == turn) {
                return NEAR_CORNER_POSITIVE;
            } else {
                return NEAR_CORNER_NEGATIVE;
            }
        } else if (position == 54 || position == 55 || position == 62) { // around bottom right
            if (getPieceAtPos(board, 63) == turn) {
                return NEAR_CORNER_POSITIVE;
            } else {
                return NEAR_CORNER_NEGATIVE;
            }
        } else if (position <= 6) { // top edge
            if (getPieceAtPos(board, 0) == turn || getPieceAtPos(board, 7) == turn) {
                return EDGE_POSITIVE;
            }
            return EDGE_NEGATIVE;
        } else if (posMod8 == 0) { // left edge
            if (getPieceAtPos(board, 0) == turn || getPieceAtPos(board, 56) == turn) {
                return EDGE_POSITIVE;
            }
            return EDGE_NEGATIVE;
        } else if (position >= 56) { // bottom edge
            if (getPieceAtPos(board, 56) == turn || getPieceAtPos(board, 63) == turn) {
                return EDGE_POSITIVE;
            }
            return EDGE_NEGATIVE;
        } else if (posMod8 == 7) { // right edge
            if (getPieceAtPos(board, 7) == turn || getPieceAtPos(board, 63) == turn) {
                return EDGE_POSITIVE;
            }
            return EDGE_NEGATIVE;
        } else if (position <= 17) { // top large inner band
            return LARGE_INNER_BAND;
        } else if (posMod8 == 1) { // left large inner band
            return LARGE_INNER_BAND;
        } else if (position >= 46) { // bottom large inner band
            return LARGE_INNER_BAND;
        } else if (posMod8 == 6) { // right large inner band
            return LARGE_INNER_BAND;
        } else if (position != 27 && position != 28 && position != 35 && position != 36) { // small inner band
            return SMALL_INNER_BAND;
        }
        return CENTER;
    }
    return 0;
}