#include "ludo.h"
#include "board.h"  // Assuming you have board logic in a separate file
#include "utils.h"
#include "play.h"
#include <algorithm> 
#include <set>
#include "gameplay_utils.h"
#include "gui_helper.h"

int play() {
    initGame();
    inputnames();
  
    bool *visited = new bool[n_players];
    int counter = 0;  // Sequential turn counter
    int *turn = new int;
    srand(time(NULL));  // Seed the random number generator

    // Initial setup
    for (int i = 0; i < n_players; i++) {
        isfinished[i] = 0;
        visited[i] = 0;
        if (i < 3) {
            diceroll[i] = 0;
        }
    }
    
    for (int i = 0; i < n_players; i++) {
    players[i][0].team_member = i; // Assign each player to their own team
    cout << "Player " << i + 1 << " is their own team in single-player mode.\n";
}


    cout << "Game Started" << endl;
    pthread_t Playthreads[MAX_PLAYER];
    
    while (position != n_players) {
        // Ensure every player completes their turn in sequence
        for (int i = 0; i < n_players; i++) {
            *turn = i;  // Ensure players take turns sequentially

            // Create a thread for each player's turn
            pthread_create(&Playthreads[i], NULL, PlayerThread, turn);
            
            // Check if it's time to reset turn flags (after all players completed their turns)
            if (counter == n_players) {
                counter = 0;
                for (int j = 0; j < n_players; j++) {
                    visited[j] = 0;  // Reset turn visited flags
                }
            }
            
            // Wait for the current player's turn to complete before moving on to the next
            pthread_join(Playthreads[i], NULL);
            counter++;
        }
    }

    return 0;
}


void makeTeams() {
    cout << "Enter the number of players per team (2-4): ";
    cin >> n_players;
    verifyInput();

    if (n_players < 2 || n_players > MAX_PLAYER) {
        cout << "Invalid number of players! Must be between 2 and 4." << endl;
        return;
    }

    cout << "Configuring tokens for each player in teams...\n";
    for (int team = 0; team < MAX_TEAM; ++team) {
        cout << "Team " << (team + 1) << ":\n";
        for (int player = 0; player < n_players; ++player) {
            cout << "Enter Player ID for Team " << (team + 1) << ", Player " << (player + 1) << ": ";
            cin >> teams[team][player];
            verifyInput();
            players[teams[team][player]][player].team = team + 1;

    
                players[teams[team][player]][player].counter = n_tokens;
                players[teams[team][player]][player].ingame = true;
            
        }
    }
    cout << "Teams and tokens configured successfully!" << endl;
}



void Teamplay() {
    cout << "\nStarting Team Play Mode\n";

    // Initialize game for team play
    initGame();

    cout << "Enter the number of teams (2-4): ";
    cin >> n_teams;
    while (n_teams < 2 || n_teams > MAX_TEAM) {
        cout << "Invalid number of teams. Enter a number between 2 and 4: ";
        cin >> n_teams;
    }

    n_players = n_teams * 2; // Assume each team has 2 players for simplicity

string Name;
for (int i = 0; i < n_players; i++) {
    printWithDelay("Enter Player " + to_string(i + 1) + "'s Name (Without spaces): ", 50);
    cin >> Name;
    players[i][0].name = Name;  // Assign name to player
  
}

// Displaying players in teams
for (int i = 0; i < n_teams; i++) {
    cout << "\nTeam " << i + 1 << " Players:\n";
    for (int j = 0; j < 2; j++) {  // Assuming 2 players per team
        int playerIndex = i * 2 + j;
        teams[i][j] = playerIndex;  // Assign player to team
        
        players[playerIndex][0].team_member = i;
        
    }
    

}

    
    // Create a list for player order
    vector<int> playerOrder(n_players);
    for (int i = 0; i < n_players; i++) {
        playerOrder[i] = i;
    }

    srand(time(NULL));  // Seed the random number generator

    // Initialize game state
    for (int i = 0; i < n_players; i++) {
        isfinished[i] = false;
        if (i < 3) {
            diceroll[i] = 0;
        }
    }

    cout << "Team Play Game Started\n";
    pthread_t Playthreads[MAX_PLAYER];

    while (position != n_teams) { // Game loop until all positions are filled
        // Shuffle player order for this round
        random_shuffle(playerOrder.begin(), playerOrder.end());

        // Each player takes their turn sequentially in this round
        for (int roundPlayerIndex = 0; roundPlayerIndex < n_players; roundPlayerIndex++) {
            int turn = playerOrder[roundPlayerIndex];

            // Skip if player has already finished
            if (isfinished[turn]) {
                continue;
            }

            // Create and join threads for the current player's turn
            pthread_create(&Playthreads[turn], NULL, PlayerThread, &turn);
            pthread_join(Playthreads[turn], NULL); // Wait for the player's turn to complete

            // Optional: Add a small delay between turns for readability
            usleep(500000); // 500 milliseconds
        }
    }

    cout << "Team Play Mode Ended\n";
}

