#include "board.h"  // Include the header for board functions
#include <iostream>
//#include <SFML/Graphics.hpp>
using namespace std;


void display() {
    // Clear screen for a clean display
    cout << "\033[2J\033[1;1H";

    // Decorative header
    cout << WHITE << string(50, '=') << RESET << endl;
    cout << YELLOW << 
    R"(
    ██╗     ██╗   ██╗██████╗  ██████╗ 
    ██║     ██║   ██║██╔══██╗██╔═══██╗
    ██║     ██║   ██║██║  ██║██║   ██║
    ██║     ██║   ██║██║  ██║██║   ██║
    ███████╗╚██████╔╝██████╔╝╚██████╔╝
    ╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ 
    " GAME RESULTS " 
    )" << RESET << endl;
    cout << WHITE << string(60, '=') << RESET << endl;

    // Game Statistics
    cout << WHITE << "🎲 Game Statistics:" << RESET << endl;
    cout << "Total Tokens per Player: " << GREEN << n_tokens << RESET << endl;
    cout << "Total Players: " << GREEN << n_players << RESET << endl;
    
    // Detailed Player Performance
    cout << "\n" << WHITE << "📊 Player Performance Breakdown:" << RESET << endl;
    
    // Sort players by position for more meaningful display
    vector<pair<int, int>> player_ranks;
    for (int i = 0; i < n_players; i++) {
        player_ranks.push_back({players[i][0].position, i});
    }
    sort(player_ranks.begin(), player_ranks.end());

    // Color codes for different ranks
    const string rank_colors[] = {
        YELLOW,  // 1st
        WHITE,   // 2nd
        "\033[38;5;208m"  // 3rd (bronze-like)
    };

    // Detailed player statistics
    for (auto& rank : player_ranks) {
        int i = rank.second;
        string rank_display;
        string color = RESET;

        if (players[i][0].position == 1) {
            rank_display = "🥇 1st Place";
            color = rank_colors[0];
        } else if (players[i][0].position == 2) {
            rank_display = "🥈 2nd Place";
            color = rank_colors[1];
        } else if (players[i][0].position == 3) {
            rank_display = "🥉 3rd Place";
            color = rank_colors[2];
        } else if (players[i][0].position == -1) {
            rank_display = "❌ Eliminated";
            color = RED;
        } else {
            rank_display = "🏁 Finished";
            color = GREEN;
        }

        cout << color 
             << "Player " << i + 1 << " (" << players[i][0].name << "):" << endl;
        
        cout << "  🏆 Final Position: " << rank_display << endl;
        
        // Performance metrics
        cout << "  💥 Tokens Eliminated: " 
             << RED << players[i][0].removed << RESET << endl;
        
        cout << "  🔥 Hits: " << CYAN << players[i][0].hits << RESET << endl; // Added hits display
        // Completion status
        if (players[i][0].position != -1) {
            cout << "  🚩 Game Completion Status: " 
                 << (players[i][0].position <= n_players ? "Completed" : "Ongoing") 
                 << RESET << endl;
        }
        
        cout << RESET << endl;
    }

    // Cancelled Threads Section
    cout << WHITE << "🚫 Cancelled Threads:" << RESET << endl;
    bool has_cancelled_threads = false;
    for (int i = 0; i < n_players; i++) {
        if (players[i][0].position == -1) {
            cout << RED 
                 << "  🔴 Player " << i + 1 << " (" << players[i][0].name 
                 << "): Thread Cancelled" << RESET << endl;
            has_cancelled_threads = true;
        }
    }
    
    if (!has_cancelled_threads) {
        cout << GREEN << "  ✅ No Threads Cancelled" << RESET << endl;
    }

    // Final Game Over Display
    cout << "\n" << WHITE << string(60, '=') << RESET << endl;
    cout << YELLOW << 
    R"(
    ███████╗███╗   ██╗██████╗ 
    ██╔════╝████╗  ██║██╔══██╗
    █████╗  ██╔██╗ ██║██║  ██║
    ██╔══╝  ██║╚██╗██║██║  ██║
    ███████╗██║ ╚████║██████╔╝
    ╚══════╝╚═╝  ╚═══╝╚═════╝ 
    )" << RESET << endl;

    cout << WHITE << string(60, '=') << RESET << endl;

    // Optional: Sound or visual alert (platform-dependent)
    cout << "\a";  // Audible alert (console bell)
    
        // Wait for user input to restart the game
    cout << endl << GREEN << "Press Enter to restart the game..." << RESET;
    cin.ignore();       // Ignore any remaining input in the buffer
    cin.get();
    
}


void displayTeam() {
    // Clear screen for a clean display
    cout << "\033[2J\033[1;1H";


     cout << WHITE << string(50, '=') << RESET << endl;
        cout << YELLOW << 
        R"(
    ████████╗███████╗ █████╗ ███╗   ███╗██████╗ ██╗      █████╗ ██╗   ██╗
    ╚══██╔══╝██╔════╝██╔══██╗████╗ ████║██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝
       ██║   █████╗  ███████║██╔████╔██║██████╔╝██║     ███████║ ╚████╔╝ 
       ██║   ██╔══╝  ██╔══██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══██║  ╚██╔╝  
       ██║   ███████╗██║  ██║██║ ╚═╝ ██║██║     ███████╗██║  ██║   ██║   
       ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   
    )" << RESET << endl;
        cout << WHITE << string(50, '=') << RESET << endl;
    

    // Game Summary
    cout << WHITE << "🎲 Game Summary:" << RESET << endl;

    // Display winning team details
    for (int team = 0; team < n_teams; team++) {
        int teamScore = 0;
        for (int i = 0; i < n_players; i++) {
            if (players[i][0].team_member == team) {
                teamScore += players[i][0].position;
            }
        }

        // Show the winning team and players
        if (teamScore == winning_position) {
            cout << GREEN << "🏆 Winning Team " << team + 1 << " 🏆" << RESET << endl;
            cout << "   Players in Team " << team + 1 << ":" << endl;
            for (int i = 0; i < n_players; i++) {
                if (players[i][0].team_member == team) {
                    cout << "   - Player " << i + 1 << ": " << players[i][0].name << endl;
                }
            }
        }
    }

    // Show cancelled threads (if any)
    cout << WHITE << "🚫 Cancelled Threads:" << RESET << endl;
    bool hasCancelled = false;
    for (int i = 0; i < n_players; i++) {
        if (players[i][0].position == -1) {
            cout << RED << "  🔴 Player " << i + 1 << " (" << players[i][0].name << "): Thread Cancelled" << RESET << endl;
            hasCancelled = true;
        }
    }

    if (!hasCancelled) {
        cout << GREEN << "  ✅ No Threads Cancelled" << RESET << endl;
    }

    // Show token count and hit rates
    cout << WHITE << "🎯 Tokens and Hit Rates:" << RESET << endl;
    for (int team = 0; team < n_teams; team++) {
        int teamTokens = 0;
        int teamHits = 0;
        for (int i = 0; i < n_players; i++) {
            if (players[i][0].team_member == team) {
                teamTokens += players[i][0].removed;
                teamHits += players[i][0].hits;
            }
        }
        cout << "   Team " << team + 1 << ":" << endl;
        cout << "     Total Tokens: " << teamTokens << endl;
        cout << "     Total Hits: " << teamHits << endl;
    }

    // Final Game Over Display
    cout << "\n" << WHITE << string(60, '=') << RESET << endl;
    cout << YELLOW << 
    R"(
    ███████╗███╗   ██╗██████╗ 
    ██╔════╝████╗  ██║██╔══██╗
    █████╗  ██╔██╗ ██║██║  ██║
    ██╔══╝  ██║╚██╗██║██║  ██║
    ███████╗██║ ╚████║██████╔╝
    ╚══════╝╚═╝  ╚═══╝╚═════╝ 
    )" << RESET << endl;

    cout << WHITE << string(60, '=') << RESET << endl;

    // Optional: Sound or visual alert (platform-dependent)
    cout << "\a";  // Audible alert (console bell)

    // Wait for user input to restart the game
    cout << endl << GREEN << "Press Enter to restart the game..." << RESET;
    cin.ignore();       // Ignore any remaining input in the buffer
    cin.get();

}




void initBoard()
{
    char newBoard[SIZE][SIZE] = {
        {'r', 'r', 'r', 'r', 'r', 'r', 'O', 'O', 'O', 'g', 'g', 'g', 'g', 'g', 'g'},
        {'r', ' ', ' ', ' ', ' ', 'r', 'O', 'g', 'g', 'g', ' ', ' ', ' ', ' ', 'g'},
        {'r', ' ', 'r', 'r', ' ', 'r', 'g', 'g', 'O', 'g', ' ', 'g', 'g', ' ', 'g'},
        {'r', ' ', 'r', 'r', ' ', 'r', 'O', 'g', 'O', 'g', ' ', 'g', 'g', ' ', 'g'},
        {'r', ' ', ' ', ' ', ' ', 'r', 'O', 'g', 'O', 'g', ' ', ' ', ' ', ' ', 'g'},
        {'r', 'r', 'r', 'r', 'r', 'r', 'O', 'g', 'O', 'g', 'g', 'g', 'g', 'g', 'g'},
        {'O', 'r', 'O', 'O', 'O', 'O', ' ', ' ', ' ', 'O', 'O', 'O', 'b', 'O', 'O'},
        {'O', 'r', 'r', 'r', 'r', 'r', ' ', 'H', ' ', 'b', 'b', 'b', 'b', 'b', 'O'},
        {'O', 'O', 'r', 'O', 'O', 'O', ' ', ' ', ' ', 'O', 'O', 'O', 'O', 'b', 'O'},
        {'y', 'y', 'y', 'y', 'y', 'y', 'O', 'y', 'O', 'b', 'b', 'b', 'b', 'b', 'b'},
        {'y', ' ', ' ', ' ', ' ', 'y', 'O', 'y', 'O', 'b', ' ', ' ', ' ', ' ', 'b'},
        {'y', ' ', 'y', 'y', ' ', 'y', 'O', 'y', 'O', 'b', ' ', 'b', 'b', ' ', 'b'},
        {'y', ' ', 'y', 'y', ' ', 'y', 'O', 'y', 'y', 'b', ' ', 'b', 'b', ' ', 'b'},
        {'y', ' ', ' ', ' ', ' ', 'y', 'y', 'y', 'O', 'b', ' ', ' ', ' ', ' ', 'b'},
        {'y', 'y', 'y', 'y', 'y', 'y', 'O', 'O', 'O', 'b', 'b', 'b', 'b', 'b', 'b'},
    };

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            board[i][j] = newBoard[i][j];
        }
    }
}

void initGame()
{
    int newCells[CELL_NUMBER][2] = {
        {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5},
        {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}, {0, 7},
        {0, 8}, {1, 8}, {2, 8}, {3, 8}, {4, 8}, {5, 8},
        {6, 9}, {6, 10}, {6, 11}, {6, 12}, {6, 13}, {6, 14}, {7, 14},
        {8, 14}, {8, 13}, {8, 12}, {8, 11}, {8, 10}, {8, 9},
        {9, 8}, {10, 8}, {11, 8}, {12, 8}, {13, 8}, {14, 8}, {14, 7},
        {14, 6}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6},
        {8, 5}, {8, 4}, {8, 3}, {8, 2}, {8, 1}, {8, 0}, {7, 0}, {6, 0}};

    int newHouses[MAX_PLAYER][4][2] = {
        {{2, 2}, {2, 3}, {3, 3}, {3, 2}},
        {{2, 11}, {2, 12}, {3, 12}, {3, 11}},
        {{11, 11}, {11, 12}, {12, 12}, {12, 11}},
        {{11, 2}, {11, 3}, {12, 3}, {12, 2}},
    };

    for (int i = 0; i < CELL_NUMBER; i++)
    {
        cells[i].y = newCells[i][0];
        cells[i].x = newCells[i][1];
        cells[i].value = 0;
    }

    for (int i = 0; i < MAX_PLAYER; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            houses[i][j].y = newHouses[i][j][0];
            houses[i][j].x = newHouses[i][j][1];
            houses[i][j].value = (j + 1) + (i)*10;
        }
    }

    for (int i = 0; i < MAX_PLAYER; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            players[i][j].x = houses[i][j].x;
            players[i][j].y = houses[i][j].y;
            players[i][j].index = -1;
            players[i][j].team = i;
            players[i][j].team_member = i / 2;  // Assuming 2 players per team
            players[i][j].id = j + 1;
            players[i][j].removed = 0;
            players[i][j].position = n_players;
            players[i][j].counter = 0;
            players[i][j].ingame = (i < n_players);
            players[i][j].hits = 0;   
        }
    }
}


/*
void renderBoard(sf::RenderWindow &window, const char board[SIZE][SIZE], const Player players[MAX_PLAYER][4]) {
    // Create a background rectangle to give the board a defined background
    sf::RectangleShape background(sf::Vector2f(SIZE * CELL_SIZE, SIZE * CELL_SIZE));
    background.setFillColor(sf::Color(255, 255, 255)); // White background for clean appearance
    window.draw(background);

    // Highlight path and home for each player
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(col * CELL_SIZE, row * CELL_SIZE);
            
            // Set cell color based on board content
            switch (board[row][col]) {
                case 'O': 
                    cell.setFillColor(sf::Color(230, 230, 230)); // Light gray for open spaces
                    break;
                case 'r': 
                    cell.setFillColor(sf::Color(255, 0, 0)); // Red for one type of cell (houses)
                    break;
                case 'g': 
                    cell.setFillColor(sf::Color(0, 255, 0)); // Green for another cell type
                    break;
                case 'b': 
                    cell.setFillColor(sf::Color(0, 0, 255)); // Blue for another type
                    break;
                case 'y':
                    cell.setFillColor(sf::Color(255, 255, 0)); // Yellow for different cell
                    break;
                default:
                    cell.setFillColor(sf::Color(255, 255, 255)); // Default to white for uninitialized or empty spaces
                    break;
            }

            // Highlight home sections with a colored border
            if ((row == 7 && col >= 6 && col <= 10) || (row == 6 && col == 6)) { // Home section for player 1
                cell.setOutlineThickness(3);
                cell.setOutlineColor(sf::Color(255, 0, 0)); // Red border for player 1's home area
            } else if ((row == 7 && col >= 11 && col <= 14) || (row == 6 && col == 11)) { // Home section for player 2
                cell.setOutlineThickness(3);
                cell.setOutlineColor(sf::Color(0, 255, 0)); // Green border for player 2's home area
            } else if ((row == 12 && col >= 11 && col <= 14) || (row == 13 && col == 11)) { // Home section for player 3
                cell.setOutlineThickness(3);
                cell.setOutlineColor(sf::Color(0, 0, 255)); // Blue border for player 3's home area
            } else if ((row == 12 && col >= 6 && col <= 10) || (row == 13 && col == 6)) { // Home section for player 4
                cell.setOutlineThickness(3);
                cell.setOutlineColor(sf::Color(255, 255, 0)); // Yellow border for player 4's home area
            }

            // Add a subtle outline to all cells
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color(200, 100, 250)); // Light gray border for all cells
            window.draw(cell);
        }
    }
}

void renderTokens(sf::RenderWindow &window, const Player players[MAX_PLAYER][4]) {
    for (int i = 0; i < MAX_PLAYER; i++) {
        for (int j = 0; j < 4; j++) {
            if (players[i][j].ingame) {
                sf::CircleShape token(CELL_SIZE / 2.5f); // Smaller token size for better fit
                token.setPosition(players[i][j].x * CELL_SIZE + CELL_SIZE / 4, 
                                  players[i][j].y * CELL_SIZE + CELL_SIZE / 4); // Center the token in cell

                // Set color of the token based on player team
                switch (i) {
                    case 0:
                        token.setFillColor(sf::Color(255, 0, 0)); // Red player
                        break;
                    case 1:
                        token.setFillColor(sf::Color(0, 255, 0)); // Green player
                        break;
                    case 2:
                        token.setFillColor(sf::Color(0, 0, 255)); // Blue player
                        break;
                    case 3:
                        token.setFillColor(sf::Color(255, 255, 0)); // Yellow player
                        break;
                }

                // Add some effects to the token for realism
                token.setOutlineThickness(2);
                token.setOutlineColor(sf::Color(0, 0, 0)); // Black border for visibility
                window.draw(token); // Draw the token
            }
        }
    }

    // Render path highlights for each player (adjust logic as per game path)
    for (int i = 0; i < MAX_PLAYER; i++) {
        for (int j = 0; j < 4; j++) {
            if (players[i][j].ingame) {
                sf::RectangleShape pathHighlight(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                pathHighlight.setPosition(players[i][j].x * CELL_SIZE, players[i][j].y * CELL_SIZE);

                // Highlight the path with the color matching the player's team
                switch (i) {
                    case 0:
                        pathHighlight.setFillColor(sf::Color(255, 0, 0, 100)); // Red path with transparency
                        break;
                    case 1:
                        pathHighlight.setFillColor(sf::Color(0, 255, 0, 100)); // Green path with transparency
                        break;
                    case 2:
                        pathHighlight.setFillColor(sf::Color(0, 0, 255, 100)); // Blue path with transparency
                        break;
                    case 3:
                        pathHighlight.setFillColor(sf::Color(255, 255, 0, 100)); // Yellow path with transparency
                        break;
                }

                window.draw(pathHighlight); // Draw the path highlight
            }
        }
    }
}
*/
