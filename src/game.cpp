#include <ncurses.h>
#include "game.h"
#include "tetromino.h"

void Game::render() {
    int colorRendered;

    // render main board
    tetromino.updateBoard();
    WINDOW* gameWindow = newwin(22, 22, 1, 2);
    start_color();
    wborder(gameWindow, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_S1,
            ACS_ULCORNER, ACS_URCORNER, ACS_S1, ACS_S1);
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            colorRendered = board[i][j] + tetromino.board[i+4][j];
            wattron(gameWindow, COLOR_PAIR(colorRendered));
            mvwprintw(gameWindow, i + 1, j * 2 + 1, "  ");
            wattroff(gameWindow, COLOR_PAIR(colorRendered));
        }
    }
    wrefresh(gameWindow);

    // render score and level
    WINDOW* statsWindow = newwin(4, 19, 1, 30);
    box(statsWindow, 0, 0);
    mvwprintw(statsWindow, 1, 2, "level: %d", level + 1);
    mvwprintw(statsWindow, 2, 2, "score: %d", score);
    wrefresh(statsWindow);

    // render next tetromino
    WINDOW* nextWindow = newwin(7, 15, 7, 32);
    box(nextWindow, 0, 0);
    for (int i = 1; i < 5; ++i) {
        for (int j = 3; j < 7; ++j) {
            colorRendered = nextTetromino.board[i][j];
            wattron(nextWindow, COLOR_PAIR(colorRendered));
            mvwprintw(nextWindow, i + 1, (j - 1) * 2, "  ");
            wattroff(nextWindow, COLOR_PAIR(colorRendered));
        }
    }
    wrefresh(nextWindow);

    // render game over screen if game is over
    if (isGameOver) {
        renderGameOver();
    }
}

void Game::renderGameOver() {
    WINDOW* gameOverWindow = newwin(9, 30, 8, 7);
    box(gameOverWindow, 0, 0);
    
    // Game Over title
    mvwprintw(gameOverWindow, 2, 10, "GAME OVER!");
    mvwprintw(gameOverWindow, 3, 9, "Final Score: %d", score);
    
    // Menu options
    mvwprintw(gameOverWindow, 5, 4, "Press 'r' to Play Again");
    mvwprintw(gameOverWindow, 6, 4, "Press 'q' to Quit");
    
    wrefresh(gameOverWindow);
}

int Game::handleGameOverInput() {
    nodelay(stdscr, FALSE);  // Wait for input
    while (true) {
        int ch = getch();
        if (ch == 'r' || ch == 'R') {
            nodelay(stdscr, TRUE);  // Restore non-blocking input
            return 1;  // Play again
        }
        if (ch == 'q' || ch == 'Q') {
            nodelay(stdscr, TRUE);
            return 0;  // Quit
        }
    }
}

void Game::resetGame() {
   // Clear the entire screen to remove the game over window
    clear();
    refresh();
    
    clearBoard();
    score = 0;
    level = 0;
    completedRows = 0;
    isGameOver = false;
    tetromino = Tetromino();
    nextTetromino = Tetromino();
}

void Game::clearBoard() {
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            board[i][j] = 0;
        }
    }
}

void Game::updateState() {
    if (isGameOver) {
        int choice = handleGameOverInput();
        if (choice == 1) {
            resetGame();  // Play again
        } else {
            shouldExit = true;  // Exit game
        }
        return;
    }

    // Check for game over condition
    if (checkGameOver()) {
        isGameOver = true;
        return;
    }

    // check collisions with the bottom border
    bool collide = !tetromino.moveDown();
    // check collisions with other tetrominoes
    if (collideWithTetrominoes()) {
        tetromino.moveUp();
        collide = true;
    }

    // fix tetromino, update score and spawn a new tetromino
    if (collide) {
        tetromino.updateBoard();
        for (int i = 0; i < 20; ++i) {
            for (int j = 0; j < 10; ++j) {
                if (board[i][j] == 0) {
                    board[i][j] = tetromino.board[i+4][j];
                }
            }
        }
        updateScore();
        tetromino = nextTetromino;
        nextTetromino = Tetromino();
    }
}

void Game::updateScore() {
    int rowCleared = 0;
    for (int i = 0; i < 20; ++i) {
        if (isRowCompleted(i)) {
            deleteRow(i);
            rowCleared += 1;
        }
    }

    // Original Nintendo scoring system
    switch (rowCleared) {
        case 1:
            score += 40 * (level + 1);
            break;
        case 2:
            score += 100 * (level + 1);
            break;
        case 3:
            score += 300 * (level + 1);
            break;
        case 4:
            score += 1200 * (level + 1);
            break;
    };

    // level up
    if (completedRows % 10 > 9 && level < 9) level += 1;
}

bool Game::isRowCompleted(int row) {
    for (int j = 0; j < 10; ++j) {
        if (board[row][j] == 0) return false;
    }
    completedRows += 1;
    return true;
}

void Game::deleteRow(int row) {
    for (int i = row; i > 0; --i) {
        for (int j = 0; j < 10; ++j) {
           board[i][j] = board[i - 1][j];
        }
    }
    for (int j = 0; j < 10; ++j) {
        board[0][j] = 0;
    }
}

bool Game::collideWithTetrominoes() {
    tetromino.updateBoard();
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (board[i][j] != 0 && tetromino.board[i+4][j] != 0) {
                return true;
            }
        }
    }
    return false;
}

bool Game::checkGameOver() {
    // Check if any blocks in the top row
    for (int j = 0; j < 10; ++j) {
        if (board[0][j] != 0) {
            return true;
        }
    }
    
    // Check if new tetromino collides immediately upon spawning
    if (collideWithTetrominoes()) {
        return true;
    }
    
    return false;
}

void Game::trasformTetromino(int key) {
    switch (key) {
        case KEY_UP:
            tetromino.rotate();
            if (collideWithTetrominoes()) tetromino.rotate(true);
            break;
        case KEY_RIGHT:
            tetromino.moveRight();
            if (collideWithTetrominoes()) tetromino.moveLeft();
            break;
        case KEY_LEFT:
            tetromino.moveLeft();
            if (collideWithTetrominoes()) tetromino.moveRight();
            break;
        case KEY_DOWN:
            tetromino.moveDown();
            if (collideWithTetrominoes()) tetromino.moveUp();
            break;
    }
}