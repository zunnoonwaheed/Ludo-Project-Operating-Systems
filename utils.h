#ifndef UTILS_H
#define UTILS_H

#include "ludo.h"
#include <string>

void inputnames();  // Function to input player names
void verifyInput(); // Function to verify input and clear invalid input
std::string idToColor(int id); // Function to get color by player ID
int getIndexByTurn(int turn);  // Function to get index based on player's turn
bool isFinalWay(int turn, int choice, int index); // Check if the player is on the final way
void moveOnFinalWay(Player players[MAX_PLAYER][4], int turn, int choice); // Move player on the final way

#endif // UTILS_H

