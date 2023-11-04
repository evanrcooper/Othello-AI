// how many pieces should be on the board before maximizing number of pieces on the board
#define ENDGAME 58

#define H1_WEIGHT 5
#define H2_WEIGHT 2

// produces more ties which are handled randomly
// positive values produce more randomness and negativve values reduce randomness
#define RANDOMNESS_FACTOR -1

#define DIV_DIFF 1

// divisor log 2
int SHIFT = 1 + RANDOMNESS_FACTOR + floor(log2(H1_WEIGHT+H2_WEIGHT-DIV_DIFF));

#define MOVES_WEIGHT 5

#define PIECE_DIFF_WEIGHT 6

// weights for certain parts of the board (heuristic1)
#define CORNER 3993
#define NEAR_CORNER_POSITIVE -623
#define NEAR_CORNER_NEGATIVE -2996
#define EDGE_POSITIVE 235
#define EDGE_NEGATIVE -1497
#define CENTER -123
#define LARGE_INNER_BAND -149
#define SMALL_INNER_BAND 128

// weights for each space (heuristic2)
// https://github.com/kartikkukreja/blog-codes/blob/master/src/Heuristic%20Function%20for%20Reversi%20(Othello).cpp
int spaces[64] = {  30, -12, 11, 8, 8, 11, -12, 30,
                    -12, -19, -4, 1, 1, -4, -19, -12,
                    11, -4, 2, 2, 2, 2, -4, 11,
                    8, 1, 2, -3, -3, 2, 1, 8,
                    8, 1, 2, -3, -3, 2, 1, 8,
                    11, -4, 2, 2, 2, 2, -4, 11,
                    -12, -19, -4, 1, 1, -4, -19, -12,
                    30, -12, 11, 8, 8, 11, -12, 30,
                };