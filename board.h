#ifndef BOARD_H
#define BOARD_H

#include "ludo.h"
#include <algorithm> // Required for sort
#define WHITE   "\033[37m"
// Function declarations
void display(); // Function to display the game results
void displayTeam();

void initBoard(); // Function to initialize the game board
void initGame(); // Function to initialize the game state
// board.h
//void renderBoard(sf::RenderWindow &window, const char board[SIZE][SIZE], const Player players[MAX_PLAYER][4]);
//void renderPaths(sf::RenderWindow &window, const Player players[MAX_PLAYER][4]);
//void renderTokens(sf::RenderWindow& window, const Player players[MAX_PLAYER][4]);

#endif // BOARD_H

