#ifndef LUDO_H
#define LUDO_H

//#include <SFML/Graphics.hpp>
//#include <SFML/Window.hpp>
//#include <SFML/Audio.hpp>

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <limits>
#include <string>
#include <cstring>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <thread>    // For this_thread
using namespace std;

//-----------------------------------------------------------------------------
// Macros for Colors
//-----------------------------------------------------------------------------
#define RESET "\u001B[0m"
#define RED "\u001B[31m"
#define GREEN "\u001B[32m"
#define YELLOW "\u001B[33m"
#define BLUE "\u001B[34m"
#define CYAN "\u001B[36m"
#define MAGENTA "\033[1;35m"
#define BRIGHT_YELLOW  "\033[1;93m"
#define DARK_ORANGE "\033[38;5;214m"  // Dark orange color in 256-color mode


//-----------------------------------------------------------------------------
// Game Configurations
//-----------------------------------------------------------------------------
#define SIZE 15
#define CELL_NUMBER 52
#define MAX_PLAYER 4
#define MAX_DE 6
#define MAX_TEAM 2

#define CELL_SIZE 50.0f // Size of each cell in pixels
//-----------------------------------------------------------------------------
// Semaphore for Synchronization
//-----------------------------------------------------------------------------
extern sem_t sem;

//-----------------------------------------------------------------------------
// Class Definitions
//-----------------------------------------------------------------------------
class Cell {
public:
    int x;
    int y;
    int value;
};

class Player {
public:
    int x;
    int y;
    int team;
    int index;
    int id;
    int removed;   // killed opponent?
    int position;  // winning position
    bool token_in_house; // tokens in house   
    bool ingame; // is playing game?
    int counter;
    string name;
    int hits;
    int team_member;
};

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
extern Cell cells[CELL_NUMBER];
extern Cell houses[MAX_PLAYER][4];
extern char board[SIZE][SIZE]; // Grid
extern int n_players;       // No of players
extern int n_teams;
extern Player players[MAX_PLAYER][4]; // Players
extern int position;
extern int winning_position;
extern int n_tokens;
extern bool *isfinished;
extern int *diceroll;

extern int teams[MAX_TEAM][2];
extern bool gameOver;
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void display();
void initBoard();
void initGame();
void menu(int &);
int play();
void verifyInput();
string idToColor(int);
int getIndexByTurn(int);
bool isFinalWay(int, int, int);
void moveOnFinalWay(Player[MAX_PLAYER][4], int, int);
void displayCurrent();
bool isblock(int, int);
bool is_safepoint(int);
void killtoken(int, int);
int choicevalidity(int, int, int);
bool isanymovepossible(int, int);
void movetoken(int, int, int);
void *createMasterThread(int, int);
void *Masterthread(void *);
void *PlayerThread(void *);
void inputnames(); // Declare inputnames function
void makeTeams(); // Team setup function

#endif // LUDO_H

