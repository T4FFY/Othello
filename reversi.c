//
// Author: Zafina Mahmood
//
#include <stdio.h>

#include "lab8part2.h"

void startingBoard(char board[][26], int n) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      board[row][col] = 'U';

      if ((col == (n / 2) - 1 && row == (n / 2) - 1) ||
          (col == (n / 2) && row == (n / 2)))
        board[row][col] = 'W';
      if ((col == (n / 2) && row == (n / 2) - 1) ||
          (col == (n / 2) - 1 && row == (n / 2)))
        board[row][col] = 'B';
    }
  }
}

void printBoard(char board[][26], int n) {
  // Top Row
  printf("  ");
  for (int i = 0; i < n; i++) {
    printf("%c", 'a' + i);
  }
  printf("\n");

  // Side row and main board
  for (int i = 0; i < n; i++) {
    printf("%c ", 'a' + i);

    for (int j = 0; j < n; j++) {
      printf("%c", board[i][j]);
    }
    printf("\n");
  }
}
bool positionInBounds(int n, int row, int col) {
  if (row < n && col < n && row >= 0 && col >= 0) {
    return true;
  } else {
    return false;
  }
}
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  // Check legality in direction
  bool keepChecking = true;
  int tempRow = 0, tempCol = 0;

  for (int i = 1; keepChecking; i++) {
    tempRow = row + (deltaRow * i);
    tempCol = col + (deltaCol * i);

    // Check if in bounds
    if (!(positionInBounds(n, tempRow, tempCol))) {
      return false;
    }
    // Check for continious line
    if (board[tempRow][tempCol] == colour) {
      keepChecking = true;
    } else if (board[tempRow][tempCol] == 'U') {
      return false;
    } else if (i == 1 && board[tempRow][tempCol] != colour) {
      return false;
    } else {
      return true;
    }
  }
  return false;
}

bool validMove(char board[][26], int n, int *deltaRow, int *deltaCol,
               char *pInput, char oppColour, char colour) {
  bool valid = false;
  int row = pInput[0] - 'a', col = pInput[1] - 'a';

  // Check if valid move
  for (int i = 0; i < 8; i++) {
    if (board[row][col] == 'U' &&
        checkLegalInDirection(board, n, row, col, oppColour, deltaRow[i],
                              deltaCol[i])) {
      valid = true;

      // Update board
      bool keepGoing = true;
      for (int j = 1; keepGoing; j++) {
        int tempRow = row + (j * deltaRow[i]);
        int tempCol = col + (j * deltaCol[i]);

        if (board[tempRow][tempCol] == oppColour) {
          board[tempRow][tempCol] = colour;
        } else {
          keepGoing = false;
        }
      }
    }
  }
  board[row][col] = colour;
  return valid;
}

int moveScore(char board[][26], int n, int row, int col, int *deltaRow,
              int *deltaCol, char colour, char oppColour) {
  bool keepGoing = true;
  int tempRow, tempCol, score = 0;

  n--;

  if ((row == 0 && col == 0) || (row == 0 && col == n) ||
      (row == n && col == 0) || (row == n && col == n)) {
    score = 1;
  } else if ((row == 0 && col != 1 && col != n - 1) ||
             (row == n && col != 1 && col != n - 1) ||
             (col == 0 && row != 1 && row != n - 1) ||
             (col == n && row != 1 && row != n - 1) || (row == 1 && col == 1) ||
             (row == 1 && col == n - 1) || (row == n - 1 && col == 1) ||
             (row == n - 1 && col == n - 1)) {
    score = 4;
  } else if (row == 1 || row == n - 1 || col == 1 || col == n - 1) {
    score = 3;
  } else {
    score = 2;
  }
  return score;
}

bool possibleMoves(char board[][26], int n, char oppColour, int *deltaRow,
                   int *deltaCol) {
  bool possibleMove = false;

  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        bool directionFound = false;
        // Check Directions
        for (int k = 0; k < 8 && !directionFound; k++) {
          // Check if the direction has black piece
          if (checkLegalInDirection(board, n, row, col, oppColour, deltaRow[k],
                                    deltaCol[k])) {
            directionFound = true;
            possibleMove = true;
          }
        }
      }
    }
  }

  return possibleMove;
}

void checkWinner(char board[][26], int n) {
  int countW = 0, countB = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'W') {
        countW++;
      } else if (board[row][col] == 'B') {
        countB++;
      } else {
      }
    }
  }

  if (countW > countB) {
    printf("W player wins.");
  } else if (countB > countW) {
    printf("B player wins.");
  } else {
    printf("Tie");
  }
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  int deltaRow[] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int deltaCol[] = {0, 1, 1, 1, 0, -1, -1, -1};
  char oppTurn;
  int score, bestScore = 10;

  // Assign Colours
  if (turn == 'W') {
    oppTurn = 'B';
  } else {
    oppTurn = 'W';
  }

  // Turn is the colour your trying to find the best move for
  for (int rowA = 0; rowA < n; rowA++) {
    for (int colA = 0; colA < n; colA++) {
      if (board[rowA][colA] == 'U') {
        bool directionFound = false;
        // Check Directions
        for (int k = 0; k < 8 && !directionFound; k++) {
          // Check if the direction has black piece
          if (checkLegalInDirection(board, n, rowA, colA, oppTurn, deltaRow[k],
                                    deltaCol[k])) {
            directionFound = true;

            // Compute score
            score = moveScore(board, n, rowA, colA, deltaRow, deltaCol, turn,
                              oppTurn);

            // Compare score
            if (score < bestScore) {
              *row = rowA;
              *col = colA;
              bestScore = score;
            }
          }
        }
      }
    }
  }
  return 0;
}

void gamePlay(char board[][26], int n, char compColour, int *deltaRow,
              int *deltaCol) {
  bool gameOver = false, playerMoves = true, compMoves = true;
  char turn, playerColour, input[3] = "00", *pInput = input, winner;
  int score, highestScore = 0;
  int bestRow = 0, bestCol = 0;

  // Assigning Colours and turns
  if (compColour == 'B') {
    turn = 'C';
    playerColour = 'W';
  } else {
    turn = 'P';
    playerColour = 'B';
  }

  while (gameOver == false) {
    if (turn == 'P' && playerMoves) {
      printf("Enter move for colour %c (RowCol): ", playerColour);
      scanf("%s", input);

      if (validMove(board, n, deltaRow, deltaCol, pInput, compColour,
                    playerColour)) {
        printBoard(board, n);
      } else {
        printf("Invalid move.\n");
        printf("%c player wins.", compColour);
        gameOver = true;
      }

      turn = 'C';
    } else if (turn == 'C' && compMoves) {
      makeMove(board, n, compColour, &bestRow, &bestCol);
      input[0] = bestRow + 'a';
      input[1] = bestCol + 'a';

      printf("Computer places %c at %s.\n", compColour, input);
      validMove(board, n, deltaRow, deltaCol, pInput, playerColour, compColour);
      printBoard(board, n);

      turn = 'P';
      highestScore = 0;
    } else {
    }
    playerMoves = possibleMoves(board, n, compColour, deltaRow, deltaCol);
    compMoves = possibleMoves(board, n, playerColour, deltaRow, deltaCol);
    // check status
    if (!playerMoves && !compMoves) {
      checkWinner(board, n);
      gameOver = true;
    } else if (!playerMoves && turn == 'P') {
      printf("%c player has no valid move. \n", playerColour);
      turn = 'C';
    } else if (!compMoves && turn == 'C') {
      printf("%c player has no valid move.\n", compColour);
      turn = 'P';
    } else {
    }

    playerMoves = possibleMoves(board, n, compColour, deltaRow, deltaCol);
    compMoves = possibleMoves(board, n, playerColour, deltaRow, deltaCol);
  }
}

int main(void) {
  // Input
  int n;
  printf("Enter the board dimension: ");
  scanf("%d", &n);

  // Create Starting Board
  char board[26][26];
  startingBoard(board, n);

  // Decide Computer Colour
  char compColour;
  printf("Computer plays (B/W): ");
  scanf(" %c", &compColour);

  // Possible Moves
  int deltaRow[] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int deltaCol[] = {0, 1, 1, 1, 0, -1, -1, -1};

  // Start gameplay
  printBoard(board, n);
  gamePlay(board, n, compColour, deltaRow, deltaCol);

  return 0;
}
