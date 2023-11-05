// how many pieces should be on the board before maximizing number of pieces on the board
#define ENDGAME 58
#define MIDGAME 56

#define H1_WEIGHT 7
#define H2_WEIGHT 6
#define H3_WEIGHT 3

// produces more ties which are handled randomly
// positive values produce more randomness and negativve values reduce randomness
#define RANDOMNESS_FACTOR -1

#define DIV_DIFF 2

// divisor log 2
int SHIFT = 1 + RANDOMNESS_FACTOR + floor(log2(H1_WEIGHT+H2_WEIGHT+H3_WEIGHT-DIV_DIFF));

#define MOVES_WEIGHT 6

#define PIECE_DIFF_WEIGHT 5

// weights for certain parts of the board (heuristic1)
#define CORNER 6993
#define NEAR_CORNER_POSITIVE -4996
#define NEAR_CORNER_NEGATIVE -5996
#define EDGE_POSITIVE -200
#define EDGE_NEGATIVE -1497
#define CENTER -223
#define LARGE_INNER_BAND -649
#define SMALL_INNER_BAND -78

// #define CORNER_M 20
// #define NEAR_CORNER_POSITIVE_M 9
// #define NEAR_CORNER_NEGATIVE_M -29
// #define EDGE_POSITIVE_M 8
// #define EDGE_NEGATIVE_M -5
// #define CENTER_M 3
// #define LARGE_INNER_BAND_M 3
// #define SMALL_INNER_BAND_M 4


// weights for each space (heuristic2)
// https://github.com/kartikkukreja/blog-codes/blob/master/src/Heuristic%20Function%20for%20Reversi%20(Othello).cpp
int spaces[64] = {  30, -12, 11, 8, 8, 11, -12, 30,
                    -12, -29, -4, 1, 1, -4, -29, -12,
                    11, -4, 2, 2, 2, 2, -4, 11,
                    8, 1, 2, -3, -3, 2, 1, 8,
                    8, 1, 2, -3, -3, 2, 1, 8,
                    11, -4, 2, 2, 2, 2, -4, 11,
                    -12, -29, -4, 1, 1, -4, -29, -12,
                    30, -12, 11, 8, 8, 11, -12, 30,
                };