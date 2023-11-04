#include <iostream>
#include <fstream>
#include "board.h"

using namespace std;

void resetBoard(unsigned char *board) {
  memset(board, 0, 25);
  assignPiece(board, 27, 1);
  assignPiece(board, 28, 2);
  assignPiece(board, 35, 2);
  assignPiece(board, 36, 1);
  board[16] = 0b10000000;
  string limit;
  cout << "Time limit (1-127 seconds): ";
  cin >> limit;
  board[16] |= stoi(limit) <= 0 ? 5 : stoi(limit);
  return;
}

void crash(const string &err, const unsigned char *board, const unsigned char &erroredPiece) {
  if (erroredPiece >= 64) {
    cout << "\n\n" << err << (int) erroredPiece << "\n\n\n";
  } else {
    cout << "\n\n" << err << "\n\n\n";
  }
  cout << "Save Board To File? Y/N: ";
  char c;
  cin >> c;
  if (c == 'Y' || c == 'y') {
    string fileName;
    cout << "Name of file to save board to: ";
    cin >> fileName;
    saveBoard(board, fileName);
  }
  terminate();
  return;
}

void printBoard(const unsigned char *board) {
  unsigned char piece;
  unsigned char b[16];
  bool nl = false;
  memcpy(b, board, 16*sizeof(unsigned char));
  cout << "\n   0  1  2  3  4  5  6  7\n0 ";
  for (unsigned char i = 0; i < 16; i++) {
    piece = 0b11000000;
    for (unsigned char j = 0; j < 4; j++) {
      switch ((b[i] & piece) >> (6 - (2*j))) {
        // https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
        case 0:
          if (isLegalMove(board, (i*4)+j)) {
            cout << "\033[1;42m X \033[0m"; // blank spaces (potential-moves)
          } else {
            cout << "\033[1;42m - \033[0m"; // blank spaces (non-potential-moves)
          }
          break;
        case 1:
          cout << "\033[1;47m O \033[0m"; // white spaces
          break;
        case 2:
          cout << "\033[1;40m O \033[0m"; // black spaces
          break;
        default:
          crash("Unknown Piece Type: ", board, ((b[i] & piece) >> (6 - (2*j))));
          return;
      }
      piece >>= 2;
    }
    if (nl) {
      cout << "\033[0m\n\033[m";
      if ((i >> 1)+1 < 8) {
        cout << (i >> 1)+1 << " ";
      }
    }
    nl = !nl;
  }
  cout << "\n";
  cout << ((board[16] & 0b10000000) ? "Black\'s Turn: \033[1;40m O \033[0m" : "White\'s Turn: \033[1;47m O \033[0m");
  cout << "\n\n";
  return;
}

void printBoardDebug(const unsigned char *board) {
  printBoard(board);
  // for (int i = 0; i < 64; i++) {
  //   if (i % 8 == 0) {
  //     cout << "\n";
  //   }
  //   cout << (int) getPieceAtPos(board, i) << " ";
  // }
  // cout << "\n\n";
  for (int i = 0; i < 8; i++) {
    cout << ((board[16] & (0b0000001 << (7-i))) ? "1" : "0") << " ";
  }
  cout << "\n";
  cout << ((board[16] & 0b10000000) ? "Black\'s Turn\n" : "White\'s Turn\n");
  cout << "Time Limit: " << (int) (board[16] & 0b01111111) << "\n";
}

void buildFromFile(unsigned char *board, const string &fileName) {
  ifstream file;
  file.open(fileName);
  char c;
  cout << "\n\nBuilding Board...\n\n";
  for (int i = 0; i < 64; i++) {
    c = file.get();
    cout << c;
    if (c == '\n') {
      i--;
    } else {
      cout << " ";
      assignPiece(board, i, c-'0');
    }
  }
  file.get(); // skip newline
  board[16] = (file.get()-'0' ? 0b10000000 : 0b00000000);
  file.get(); // skip newline
  char digit = file.get();
  unsigned char x = 0;
  while (digit != '\n' && digit != EOF) {
    x = (x*10) + (digit & 0b00001111);
    digit = file.get();
  }
  board[16] |= (x & 0b01111111);
  cout << "\n\nBuild Complete\n\n\n";
  return;
}

void saveBoard(const unsigned char *board, const string &fileName) {
  ofstream file;
  file.open(fileName);
  unsigned char piece;
  unsigned char b[16];
  bool nl = false;
  memcpy(b, board, 16*sizeof(unsigned char));
  for (unsigned char i = 0; i < 16; i++) {
    piece = 0b11000000;
    for (unsigned char j = 0; j < 4; j++) {
      switch ((b[i] & piece) >> (6 - (2*j))) {
        case 0:
          file << "0"; // blank spaces
          break;
        case 1:
          file << "1"; // white spaces
          break;
        case 2:
          file << "2"; // black spaces
          break;
        default:
          file << "-"; // unknown piece
          break;
      }
      piece >>= 2;
    }
    if (nl) {file << "\n";}
    nl = !nl;
  }
  file << (board[6] & 0b10000000 ? "1" : "0") << "\n";
  file << (int) (board[16] & 0b01111111) << "\n";
  file.close();
}

void getLegalMoves(const unsigned char *board, unsigned char *moves) {
  memset(moves, 0, 9);
  for (int byte = 0; byte < 8; byte++) {
    for (int bit = 0; bit < 8; bit++) {
      if (board[17+byte] & (0b00000001 << (7-bit))) {
        if (isLegalMove(board, (8*byte) + bit)) {
          moves[byte] |= (0b00000001 << (7-bit));
          moves[8]++;
        }
      }
    }
  }
  return;
}

void switchTurn(unsigned char *board) {
  board[16] ^= 0b10000000;
}

unsigned char getPieceAtPos(const unsigned char *board, unsigned char position) {
  return (board[position >> 2] >> (2*(3 - (position % 4)))) & 0b00000011;
}

unsigned char getPieceAtRowCol(const unsigned char *board, unsigned char row, unsigned char col) {
  return getPieceAtPos(board, (row*8) + col);
  // return (board[(row >> 1) + (col < 4 ? 0 : 1)] >> (2*(3-(col % 4)))) & 0b00000011;
}

bool isLegalMove(const unsigned char *board, unsigned char position) {
  if (getPieceAtPos(board, position) != 0) {
    return 0;
  }

  unsigned char c = 0;
  unsigned char turn = (board[16] >> 7) + 1;
  unsigned char op = 3 - turn;
  unsigned char newPos;
  unsigned char posMod_8 = position & 0b00000111;

  // 1 2 3
  // 4   5
  // 6 7 8

  /*
    8 different directions
    Check if immediate neighbor is opponent's color
      - if not skip to next direction
      - if yes update position to neighbor and continue to loop
    Loop until non opponent's color
      - If empty space return false
      - Else return true
  */

  // 1 (top left)
  // position - 9 = new position
  // edges are: position < 9 and position % 8 = 0
  if (position >= 9 && posMod_8 != 0 && getPieceAtPos(board, position-9) == op) {
    newPos = position - 9;
    while (newPos >= 9  && (newPos & 0b00000111) != 0 && getPieceAtPos(board, newPos) == op) {
      newPos -= 9;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }

  // 2 (top middle)
  // position - 8 = new position
  // edges are: position < 8
  if (position >= 8 && getPieceAtPos(board, position-8) == op) {
    newPos = position - 8;
    while (newPos >= 8 && getPieceAtPos(board, newPos) == op) {
      newPos -= 8;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }

  // 3 (top right)
  // position - 7 = new position
  // edges are: position < 8 and position % 8 = 7
  if (position >= 8 && posMod_8 != 7 && getPieceAtPos(board, position-7) == op) {
    newPos = position - 7;
    while (newPos >= 8  && (newPos & 0b00000111) != 7 && getPieceAtPos(board, newPos) == op) {
      newPos -= 7;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }

  // 4 (left)
  // position - 1 = new position
  // edges are: position % 8 = 0
  if (posMod_8 != 0 && getPieceAtPos(board, position-1) == op) {
    newPos = position - 1;
    while ((newPos & 0b00000111) != 0 && getPieceAtPos(board, newPos) == op) {
      newPos -= 1;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }

  // 5 (right)
  // position + 1 = new position
  // edges are: position % 8 = 7
  if (posMod_8 != 7 && getPieceAtPos(board, position+1) == op) {
    newPos = position + 1;
    while ((newPos & 0b00000111) != 7 && getPieceAtPos(board, newPos) == op) {
      newPos += 1;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }

  // 6 (bottom left)
  // position + 7 = new position
  // edges are: position > 55 and position % 8 = 0
  if (position <= 55 && posMod_8 != 0 && getPieceAtPos(board, position+7) == op) {
    newPos = position + 7;
    while (newPos <= 55  && (newPos & 0b00000111) != 0 && getPieceAtPos(board, newPos) == op) {
      newPos += 7;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }

  // 7 (bottom middle)
  // position + 8 = new position
  // edges are: position > 55
  if (position <= 55 && getPieceAtPos(board, position+8) == op) {
    newPos = position + 8;
    while (newPos <= 55 && getPieceAtPos(board, newPos) == op) {
      newPos += 8;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }

  // 8 (bottom right)
  // position + 9 = new position
  // edges are: position > 55 and position % 8 = 7
  if (position <= 55 && posMod_8 != 7 && getPieceAtPos(board, position+9) == op) {
    newPos = position + 9;
    while (newPos <= 55  && (newPos & 0b00000111) != 7 && getPieceAtPos(board, newPos) == op) {
      newPos += 9;
    }
    if (getPieceAtPos(board, newPos) == turn) {
      return true;
    }
  }
  return false;
}

void printLegalMoves(const unsigned char *board) {
  unsigned char *moves = (unsigned char*) calloc(8, sizeof(unsigned char));
  getLegalMoves(board, moves);
  for (int i = 0; i < 64; i++) {
    int iMod_8 = i & 0b00000111;
    if (iMod_8 == 0) {
      cout << '\n';
    }
    cout << (moves[i >> 3] & ((0b00000001 << (7-iMod_8))) ? "1" : "0") << " ";
  }
  cout << (int) moves[9];
  free(moves);
  cout << "\n\n";
}

void assignPiece(unsigned char *board, const unsigned char &position, const unsigned char &piece) {
  board[position >> 2] &= ~(0b00000011 << (2*(3 - (position & 0b00000011)))); // set piece to 0
  board[position >> 2] |= (piece << (2*(3 - (position & 0b00000011)))); // assign piece
  unsigned char posMod_8 = position & 0b00000111;

  // center
  updatePotentialMoves(board, position);

  // top left
  if (position >= 9 && posMod_8 != 0 && getPieceAtPos(board, (unsigned char) position-9) == 0) {
    updatePotentialMoves(board, (unsigned char) position-9);
  }

  // top middle
  if (position >= 8 && getPieceAtPos(board, (unsigned char) position-8) == 0) {
    updatePotentialMoves(board, (unsigned char) position-8);
  }

  // top right
  if (position >= 8 && posMod_8 != 7 && getPieceAtPos(board, (unsigned char) position-7) == 0) {
    updatePotentialMoves(board, (unsigned char) position-7);
  }

  // left
  if (posMod_8 != 0 && getPieceAtPos(board, (unsigned char) position-1) == 0) {
    updatePotentialMoves(board, (unsigned char) position-1);
  }

  // right
  if (posMod_8 != 7 && getPieceAtPos(board, (unsigned char) position+1) == 0) {
    updatePotentialMoves(board, (unsigned char) position+1);
  }

  // bottom left
  if (position <= 55 && posMod_8 != 0 && getPieceAtPos(board, (unsigned char) position+7) == 0) {
    updatePotentialMoves(board, (unsigned char) position+7);
  }

  // bottom middle
  if (position <= 55 && getPieceAtPos(board, (unsigned char) position+8) == 0) {
    updatePotentialMoves(board, (unsigned char) position+8);
  }
  
  // bottom right
  if (position <= 55 && posMod_8 != 7 && getPieceAtPos(board, (unsigned char) position+9) == 0) {
    updatePotentialMoves(board, (unsigned char) position+9);
  }
}

void updatePotentialMoves(unsigned char *board, const unsigned char &position) {
  if (getPieceAtPos(board, position) == 0) {
      board[17 + (position >> 3)] |= (0b00000001 << (7-(position & 0b00000111)));
  } else {
    if (getPieceAtPos(board, position) != 0) {
      board[17 + (position >> 3)] &= ~(0b00000001 << (7-(position & 0b00000111)));
    }
  }
}

void printBinaryByte(unsigned char byte) {
  for (int i = 0; i < 8; i++) {
    cout << ((byte & (0b10000000 >> i)) >> (7-i)) << " ";
  }
}

void printBinaryByte(char byte) {
    printBinaryByte((unsigned char) byte);
}

void printPotentialMoves(const unsigned char *b) {
  for (int i = 0; i < 8; i++) {
    printBinaryByte(b[17+i]);
    cout << "\n";
  }
  cout << "\n";
}

void flipPiece(unsigned char *board, const unsigned char &position) {
  unsigned char piece = getPieceAtPos(board, position);
  board[position >> 2] &= ~(0b00000011 << (2*(3 - (position & 0b00000011)))); // set piece to 0
  board[position >> 2] |= ((3-piece) << (2*(3 - (position & 0b00000011)))); // assign piece
}

unsigned char getLegalMoveCount(const unsigned char *board) {
  unsigned char count = 0;
  for (int byte = 0; byte < 8; byte++) {
    for (int bit = 0; bit < 8; bit++) {
      if (board[17+byte] & (0b00000001 << (7-bit))) {
        if (isLegalMove(board, (8*byte) + bit)) {
          count++;
        }
      }
    }
  }
  return count;
}

void makeMove(unsigned char *board, const unsigned char &position) {
  if (position >= 64) {
    crash("Bad Position (Attempted To Make NULL Move): ", board, position);
    return;
  } else {
    unsigned char turn = (board[16] >> 7) + 1;
    unsigned char op = 3 - turn;
    unsigned char newPos;
    unsigned char posMod_8 = position & 0b00000111;

    // 0
    assignPiece(board, position, turn);

    // 1 2 3
    // 4 0 5
    // 6 7 8

    // 1 (top left)
    // position - 9 = new position
    // edges are: position < 9 and position % 8 = 0
    if (position >= 9 && (posMod_8) != 0 && getPieceAtPos(board, position-9) == op) {
      newPos = position - 9;
      while (newPos >= 9  && (newPos & 0b00000111) != 0 && getPieceAtPos(board, newPos) == op) {
        newPos -= 9;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos += 9;
        do {
          flipPiece(board, newPos);
          newPos += 9;
        } while (newPos != position);
      }
    }

    // 2 (top middle)
    // position - 8 = new position
    // edges are: position < 8
    if (position >= 8 && getPieceAtPos(board, position-8) == op) {
      newPos = position - 8;
      while (newPos >= 8 && getPieceAtPos(board, newPos) == op) {
        newPos -= 8;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos += 8;
        do {
          flipPiece(board, newPos);
          newPos += 8;
        } while (newPos != position);
      }
    }

    // 3 (top right)
    // position - 7 = new position
    // edges are: position < 8 and position % 8 = 7
    if (position >= 8 && posMod_8 != 7 && getPieceAtPos(board, position-7) == op) {
      newPos = position - 7;
      while (newPos >= 8  && (newPos & 0b00000111) != 7 && getPieceAtPos(board, newPos) == op) {
        newPos -= 7;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos += 7;
        do {
          flipPiece(board, newPos);
          newPos += 7;
        } while (newPos != position);
      }
    }

    // 4 (left)
    // position - 1 = new position
    // edges are: position % 8 = 0
    if (posMod_8 != 0 && getPieceAtPos(board, position-1) == op) {
      newPos = position - 1;
      while ((newPos & 0b00000111) != 0 && getPieceAtPos(board, newPos) == op) {
        newPos -= 1;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos += 1;
        do {
          flipPiece(board, newPos);
          newPos += 1;
        } while (newPos != position);
      }
    }

    // 5 (right)
    // position + 1 = new position
    // edges are: position % 8 = 7
    if (posMod_8 != 7 && getPieceAtPos(board, position+1) == op) {
      newPos = position + 1;
      while ((newPos & 0b00000111) != 7 && getPieceAtPos(board, newPos) == op) {
        newPos += 1;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos -= 1;
        do {
          flipPiece(board, newPos);
          newPos -= 1;
        } while (newPos != position);
      }
    }

    // 6 (bottom left)
    // position + 7 = new position
    // edges are: position > 55 and position % 8 = 0
    if (position <= 55 && posMod_8 != 0 && getPieceAtPos(board, position+7) == op) {
      newPos = position + 7;
      while (newPos <= 55  && (newPos & 0b00000111) != 0 && getPieceAtPos(board, newPos) == op) {
        newPos += 7;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos -= 7;
        do {
          flipPiece(board, newPos);
          newPos -= 7;
        } while (newPos != position);
      }
    }

    // 7 (bottom middle)
    // position + 8 = new position
    // edges are: position > 55
    if (position <= 55 && getPieceAtPos(board, position+8) == op) {
      newPos = position + 8;
      while (newPos <= 55 && getPieceAtPos(board, newPos) == op) {
        newPos += 8;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos -= 8;
        do {
          flipPiece(board, newPos);
          newPos -= 8;
        } while (newPos != position);
      }
    }

    // 8 (bottom right)
    // position + 9 = new position
    // edges are: position > 55 and position % 8 = 7
    if (position <= 55 && posMod_8 != 7 && getPieceAtPos(board, position+9) == op) {
      newPos = position + 9;
      while (newPos <= 55  && (newPos & 0b00000111) != 7 && getPieceAtPos(board, newPos) == op) {
        newPos += 9;
      }
      if (getPieceAtPos(board, newPos) == turn) {
        newPos -= 9;
        do {
          flipPiece(board, newPos);
          newPos -= 9;
        } while (newPos != position);
      }
    }
  }
  switchTurn(board);
  if (getLegalMoveCount(board) == 0) {
    switchTurn(board);
  }
  return;
}

void printWinner(const unsigned char *board) {
  unsigned char W_count = 0;
  unsigned char B_count = 0;
  unsigned char piece;
  bool nl = false;
  cout << "\n   0  1  2  3  4  5  6  7\n0 ";
  for (unsigned char i = 0; i < 16; i++) {
    piece = 0b11000000;
    for (unsigned char j = 0; j < 4; j++) {
      switch ((board[i] & piece) >> (6 - (2*j))) {
        // https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
        case 0:
          if (isLegalMove(board, (i*4)+j)) {
            cout << "\033[1;42m X \033[0m"; // blank spaces (potential-moves)
          } else {
            cout << "\033[1;42m - \033[0m"; // blank spaces (non-potential-moves)
          }
          break;
        case 1:
          cout << "\033[1;47m O \033[0m"; // white spaces
          W_count++;
          break;
        case 2:
          cout << "\033[1;40m O \033[0m"; // black spaces
          B_count++;
          break;
        default:
          crash("Unknown Piece Type: ", board, ((board[i] & piece) >> (6 - (2*j))));
          return;
      }
      piece >>= 2;
    }
    if (nl) {
      cout << "\033[0m\n\033[m";
      if ((i >> 1)+1 < 8) {
        cout << (i >> 1)+1 << " ";
      }
    }
    nl = !nl;
  }
  cout << "\n";
  if (B_count > W_count) {
    cout << "Black [\033[1;40m O \033[0m] Wins: " << (int)  B_count << " To " << (int)  W_count << "!";
  } else if (W_count > B_count) {
    cout << "White [\033[1;47m O \033[0m] Wins: " << (int)  W_count << " To " << (int)  B_count << "!";
  } else {
    cout << "The Game Ended In A Tie: " << (int) B_count << " (Black) To " << (int) W_count << " (White) !";
  }
  cout << "\n\n";
  return;
}

unsigned char countBlack(const unsigned char *board) {
  int count = 0;
  for (int i = 0; i < 64; i++) {
      if (getPieceAtPos(board, i) == 2) {
          count++;
      }
  }
  return count;
}

unsigned char countWhite(const unsigned char *board) {
  int count = 0;
  for (int i = 0; i < 64; i++) {
      if (getPieceAtPos(board, i) == 1) {
          count++;
      }
  }
  return count;
}