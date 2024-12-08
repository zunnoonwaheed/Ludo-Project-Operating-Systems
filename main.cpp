#include "ludo.h"
#include "board.h"
#include "utils.h"
#include "menu.h"
#include "play.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "gui_helper.h"


using namespace std;

// Function to reset the game state
void resetAll() {
    // Reset global variables
    sem_destroy(&sem);
    sem_init(&sem, 0, 1);

    position = 1;
    winning_position = 1;

    // Reset board and game state arrays
    for (int i = 0; i < CELL_NUMBER; i++) {
        cells[i] = Cell();
    }

    for (int i = 0; i < MAX_PLAYER; i++) {
        for (int j = 0; j < 4; j++) {
            players[i][j] = Player();
            houses[i][j] = Cell();
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = ' ';
        }
    }

    // Reset dynamic arrays if allocated
    if (isfinished != nullptr) {
        delete[] isfinished;
        isfinished = new bool[MAX_PLAYER]();
    }

    if (diceroll != nullptr) {
        delete[] diceroll;
        diceroll = new int[3]();
    }

    cout << GREEN << "Game state reset successfully." << RESET << endl;
}


// Splash Screen Implementation
void splashScreen() {
    clearScreen();

    // Colorful border
    horizontalLine('*', 80, MAGENTA);

    // Enhanced Ludo ASCII Art
    string ludoArt = R"(
    ██╗     ██╗   ██╗██████╗  ██████╗ 
    ██║     ██║   ██║██╔══██╗██╔═══██╗
    ██║     ██║   ██║██║  ██║██║   ██║
    ██║     ██║   ██║██║  ██║██║   ██║
    ███████╗╚██████╔╝██████╔╝╚██████╔╝
    ╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ 
    )";

    string gameArt = R"(
     ██████╗  █████╗ ███╗   ███╗███████╗██████╗ ███████╗
    ██╔════╝ ██╔══██╗████╗ ████║██╔════╝██╔══██╗██╔════╝
    ██║  ███╗███████║██╔████╔██║█████╗  ██████╔╝███████╗
    ██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ██╔══██╗╚════██║
    ╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗██║  ██║███████║
     ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═╝╚══════╝
    )";

    // Center and color the ASCII arts with safe color application
    centerText(ludoArt);
    centerText(gameArt);

    // Fancy welcome banner
    horizontalLine('=', 80, CYAN);
    centerText("🎲 WELCOME TO THE ULTIMATE LUDO ADVENTURE! 🎲", RED);
    horizontalLine('=', 80, CYAN);

    // Loading sequence with creative messages
    vector<pair<string, string>> loadingMessages = {
        {YELLOW, "🎲 Preparing Epic Battlefield... 🎲"},
        {GREEN, "🏁 Assembling Legendary Game Tokens... 🏁"},
        {BLUE, "🧩 Generating Mystical Player Zones... 🧩"},
        {MAGENTA, "🌟 Charging Cosmic Dice Power... 🌟"}
    };

    for (const auto& [color, msg] : loadingMessages) {
        centerText(msg);
        this_thread::sleep_for(chrono::milliseconds(600));
    }

    // Final activation message
    horizontalLine('-', 80, GREEN);
    centerText("✨ LUDO ARENA ACTIVATED! BATTLE COMMENCES! ✨", RED);
    horizontalLine('-', 80, GREEN);

    // Wait before clearing screen
    this_thread::sleep_for(chrono::seconds(3));
    clearScreen();
}

// Global Variable Initialization
sem_t sem;
Cell cells[CELL_NUMBER];
Cell houses[MAX_PLAYER][4];
char board[SIZE][SIZE];
int n_players = -1;
int n_teams = -1;
Player players[MAX_PLAYER][4];
int position = 1;
int winning_position = 1;
int n_tokens = -1;
bool *isfinished = nullptr;
int *diceroll = nullptr;
int teams[MAX_TEAM][2];
bool gameOver = false;

int main() {

    // Memory allocation with error checking
    try {
        isfinished = new bool[MAX_PLAYER]();  // Zero-initialize
        diceroll = new int[3]();              // Zero-initialize
    } catch (const std::bad_alloc& e) {
        cerr << "Memory allocation failed: " << e.what() << endl;
        return 1;
    }

    // Semaphore and random seed initialization
    sem_init(&sem, 0, 1);
    srand(time(NULL));

    // Splash Screen
    splashScreen();



    while (!gameOver) { 
        resetAll();
            // Initialize the board
        initBoard();
        int choice = -1;
        menu(choice);


        if (choice == 1) {
          play();
          display();
        } else if (choice == 2) {
          Teamplay();
        
        } else if (choice == 3) {
          cout << endl << RED << "Stopping the game..." << RESET << endl;
          break;
        }
        
          if (gameOver) {
        break;  // Exit the game loop
    }

    }

    

    // Safe memory cleanup
    delete[] isfinished;
    delete[] diceroll;

    return 0;
}
