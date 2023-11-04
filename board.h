
#include <cstring>
#include <iostream>

using namespace std;

// reset board to default state
void resetBoard(unsigned char *board);

// print board to console
void printBoard(const unsigned char *board);

void printBoardDebug(const unsigned char *board);

// function called to terminate program
void crash(const string &err, const unsigned char *board, const unsigned char &erroredPiece = 64);

// input starting board from file
void buildFromFile(unsigned char *board, const string &fileName);

// save current board to file
void saveBoard(const unsigned char *board, const string &fileName);

// stores the set of all legal moves given the state of the board in moves
void getLegalMoves(const unsigned char *board, unsigned char *moves);

// print all legal moves
void printLegalMoves(const unsigned char *board);

// returns the piece on the board at a given positon
unsigned char getPieceAtPos(const unsigned char *board, unsigned char position);

// returns the piece on the board at a given row and collumn (more userfriendly)
unsigned char getPieceAtRowCol(const unsigned char *board, unsigned char row, unsigned char col);

// returns whether a given position is a legal move for the current players turn
bool isLegalMove(const unsigned char *board, unsigned char position);

// switches whose turn it is
void switchTurn(unsigned char *board);

// assigns a spot on the board a piece and updates the potential moves array
void assignPiece(unsigned char *board, const unsigned char &position, const unsigned char &piece);

// inverts the bit in potential moves array
void updatePotentialMoves(unsigned char *board, const unsigned char &position);

// prints the bits of the char out
void printBinaryByte(unsigned char byte);

// prints the bits of the char out
void printBinaryByte(char byte);

// prints the potential moves given a board
void printPotentialMoves(const unsigned char *b);

// updates board with the move made
void makeMove(unsigned char *board, const unsigned char &position);

// returns the number of legal moves for whoever's turn it is
unsigned char getLegalMoveCount(const unsigned char *board);

// flips a piece without updating the potential moves
void flipPiece(unsigned char *board, const unsigned char &position);

// prints the results of a game
void printWinner(const unsigned char *board);


// counts the number of pieces on the board
unsigned char countBlack(const unsigned char *board);
unsigned char countWhite(const unsigned char *board);