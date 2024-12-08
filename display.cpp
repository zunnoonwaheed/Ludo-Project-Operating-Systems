#include "display.h"
#include <iostream>
#include <string>
#include <thread>    // For this_thread
#include <chrono>    // For chrono

using namespace std;

void displayCurrent() {
    // Clear screen for a fresh display
    cout << "\033[2J\033[1;1H";
    
    // Add a decorative border
    cout << WHITE << string(50, '=') << RESET << endl;
    cout << WHITE << "           🎲 LUDO GAME ROUND 🎲" << RESET << endl;
    cout << WHITE << string(50, '=') << RESET << endl;

    char currentBoard[SIZE][SIZE];

    // Copy the board
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            currentBoard[i][j] = board[i][j];
        }
    }

    // Place player tokens
    for (int i = 0; i < MAX_PLAYER; i++) {
        for (int j = 0; j < 4; j++) {
            currentBoard[players[i][j].y][players[i][j].x] = '0' + players[i][j].id;
        }
    }

    // Enhanced board display
    for (int i = 0; i < SIZE; i++) {
        cout << "  ";
        for (int j = 0; j < SIZE; j++) {
            string s = "";
            s = s + currentBoard[i][j] + " ";

            // Colorful and dynamic board rendering
            switch (currentBoard[i][j]) {
            case 'r': // Red home
                s = "■ ";
                s = RED + s + RESET;
                break;
            case 'g': // Green home
                s = "■ ";
                s = GREEN + s + RESET;
                break;
            case 'b': // Blue home
                s = "■ ";
                s = BLUE + s + RESET;
                break;
            case 'y': // Yellow home
                s = "■ ";
                s = YELLOW + s + RESET;
                break;
            case 'O': // Neutral zones
                s = "◊ ";
                s = WHITE + s + RESET;
                break;
            case 'H': // Neutral zones
                s = "H ";
                s = DARK_ORANGE + s + RESET;
                break;
            case '1':
            case '2':
            case '3':
            case '4':
                s = "";
                s = s + currentBoard[i][j] + " ";
                for (int a = 0; a < MAX_PLAYER; a++) {
                    for (int b = 0; b < 4; b++) {
                        if (i == players[a][b].y && j == players[a][b].x) {
                            // More vibrant token colors
                            switch (players[a][b].team) {
                         
                         case 0:
                                s = RED + s + RESET;
                                break;
                            case 1:
                                s = GREEN + s + RESET;
                                break;
                            case 2:
                                s = BLUE + s + RESET;
                                break;
                            case 3:
                                s = YELLOW + s + RESET;
                                break;
                            }
                        }
                    }
                }
                break;
            default:
                s = ". ";
                s = MAGENTA + s + RESET;
            }
            cout << s;
        }
        cout << endl;
    }

    // Add some game stats or current player information
    cout << endl;
    cout << WHITE << "Current Game Status:" << RESET << endl;
// Displaying team and player information based on n_teams
if (n_teams > 1) {
    for (int i = 0; i < n_teams; i++) {
        // Assign color for the team
        string teamColor;
        switch (i) {
            case 0: teamColor = RED; break;      // Team 1 - Red
            case 1: teamColor = GREEN; break;    // Team 2 - Green
            case 2: teamColor = BLUE; break;     // Team 3 - Blue
            case 3: teamColor = YELLOW; break;   // Team 4 - Yellow
            default: teamColor = WHITE; break;   // Default to white for other teams
        }

        // Display team header with team color
        cout << teamColor << "\nTeam " << i + 1 << " Players:\n" << RESET;
        
        // Display players in the team with their respective colors
        for (int j = 0; j < 2; j++) {  // Assuming 2 players per team
            int playerIndex = i * 2 + j;
            teams[i][j] = playerIndex;  // Assign player to team

            // Display each player's information with team color
            cout << teamColor << "Player " << playerIndex + 1 << ": " 
                 << players[playerIndex][0].name << RESET << endl;
            players[playerIndex][0].team_member = i;
            cout << "  - Team: " << i + 1 << ", Name: " 
                 << players[playerIndex][0].name << ", team_member: " 
                 << players[playerIndex][0].team_member << RESET << endl;
        }
    }
} else {
    // Display only individual players if there's only 1 team
    for (int i = 0; i < MAX_PLAYER; i++) {
        string playerColor;
        switch (i) {
            case 0: playerColor = RED; break;
            case 1: playerColor = GREEN; break;
            case 2: playerColor = BLUE; break;
            case 3: playerColor = YELLOW; break;
        }
        // Display individual players with their respective colors
        cout << playerColor << "Player " << i + 1 << ": " 
             << players[i][0].name << RESET << endl;
    }
}


    // Optional: Add a subtle flashing effect
    this_thread::sleep_for(chrono::seconds(1));
}

