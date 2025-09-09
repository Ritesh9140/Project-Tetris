# Project Tetris

A terminal-based Tetris game implemented in C++ using the ncurses library. This project provides a classic Tetris gaming experience directly in your terminal.

## Description

This is a simple yet functional implementation of the classic Tetris game. The game runs in a terminal environment and uses ncurses for rendering the graphical elements.

Features:
- Classic Tetris gameplay
- Score tracking
- Level progression
- Next tetromino preview
- Game over screen with restart option

## Requirements

- C++ compiler (g++ recommended)
- ncurses library

## Compilation

To compile the project, run the following command in the project root directory:

```bash
g++ -lncurses src/main.cpp src/tetromino.cpp src/game.cpp -o tetris
```

Alternatively, you can use the provided compilation command in compile.txt:

```bash
cat compile.txt | bash
```

## How to Run

After compilation, run the game using:

```bash
./tetris
```

## Controls

- **Left Arrow**: Move tetromino left
- **Right Arrow**: Move tetromino right
- **Down Arrow**: Move tetromino down (soft drop)
- **Up Arrow**: Rotate tetromino
- **Other keys may be available based on implementation**

## Project Structure

- `src/main.cpp`: The entry point of the program
- `src/game.cpp`, `src/game.h`: Game class implementation
- `src/tetromino.cpp`, `src/tetromino.h`: Tetromino class implementation
- `src/tetrominoes.h`: Definitions of tetromino shapes
- `tetris`: The compiled executable
- `compile.txt`: Compilation command

## How to Play

1. Run the game
2. Use arrow keys to control the falling tetromino
3. Complete rows to score points
4. The game speeds up as you progress through levels
5. The game ends when the stack of tetrominoes reaches the top of the playing field

## License

This project is provided as-is with no explicit license.

## Acknowledgments

This project is a classic implementation of Tetris using ncurses for terminal graphics.
