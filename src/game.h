#include "tetromino.h"

#ifndef GAME_H
#define GAME_H

class Game {
public:
    Game():tetromino(){};
    void render();
    void updateState();
    void trasformTetromino(int);
    int level = 0;
    bool isGameOver = false;
    bool shouldExit = false;
    void resetGame();  // New method to reset the game

private:
    bool collideWithTetrominoes();
    bool isRowCompleted(int);
    void deleteRow(int);
    void updateScore();
    bool checkGameOver();
    void renderGameOver();
    int handleGameOverInput();  // New method to handle game over menu input
    void clearBoard();          // New method to clear the board
    int board[20][10] = {};
    int completedRows = 0;
    int score = 0;
    Tetromino tetromino;
    Tetromino nextTetromino;
};

#endif