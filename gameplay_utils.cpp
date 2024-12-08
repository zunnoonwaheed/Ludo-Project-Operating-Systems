#include "gameplay_utils.h"
#include "display.h"

void *PlayerThread(void *arg) {
    sem_wait(&sem);
    int turn = *(int *) arg;
    int j = 0;
    int result;
    for (int i = 0; i < 3; i++) {
        diceroll[i] = 0;
    }
    displayCurrent();
    int choice = -1;
    if (isfinished[turn] != 1) {
        cout << endl << ("🕹️ It's up to player " + idToColor(turn)) << (turn + 1) << " to play." << RESET << endl;
        j = 0;
        do { // again turn on case of 6
            result = (rand() % MAX_DE) + 1;
            cout << "🎲 Launch of the dice ... Result : " << result << endl;
            diceroll[j] = result;
            j++;
            if (diceroll[2] == 6) {
                cout << "Result : 6,6,6 so, Passing the turn\n";
                
                for (int i = 0; i < 3; i++) {
                    diceroll[i] = 0;
                }
                createMasterThread(turn, choice);
                sem_post(&sem);
                break;
            }
                
            // Share dice roll with the partner if they've finished all their pieces
            if (players[turn][0].ingame == false && result == 6) {
                // Player has completed all pieces and can control the partner's pieces
                int partner = -1;
                for (int i = 0; i < n_players; i++)
                {
                    if (i != turn && 
                        players[i][0].team_member == players[turn][0].team_member)
                    {
                        partner = i;
                        break;
                    }
                }
                
                if (partner != -1)
                {
                         usleep(5000000);
                    cout << "Player " << turn + 1 << " shares their roll with Player " << partner + 1 << endl;
                    shareDiceRollWithPartner(turn, partner, diceroll);
                    break;
                }
            }
        } while (result == 6);
    }
    
    /*
    
        bool movepossible = false;
    
  // Calculate the sum of the dice rolls
    int dice_sum = 0;
    for (int i = 0; i < 3; i++) {
        dice_sum += diceroll[i];
    }
    
            for (int i = 0; i < n_tokens; i++) {
            if (players[turn][i].index != -1 && players[turn][i].index != 106) {
                int targetIndex = players[turn][i].index + dice_sum;

 
  if (targetIndex <= 106) {
            movepossible = true;
            break;  // If one valid move is found, no need to continue checking other tokens
        }
 
            }
        }
    

bool is_any_movepossible = true;  

  if(movepossible){
  
    is_any_movepossible  = isanymovepossible(turn, diceroll[0]);
    if (!is_any_movepossible && players[turn][0].ingame == true) {
        players[turn][0].counter++;
    } else {
        players[turn][0].counter = 0;
    }
    
  }
  
  */
    j = 0;

   bool is_any_movepossible = isanymovepossible(turn, diceroll);
    
    if (!is_any_movepossible && players[turn][0].ingame == true) {
        players[turn][0].counter++;
    } else {
        players[turn][0].counter = 0;
    }

    createMasterThread(turn, choice);

    if (!is_any_movepossible) {
        sem_post(&sem);
        return NULL;
    }
    
    
while (is_any_movepossible && diceroll[j] != 0 && j < 3) {
    cout << "🎲 Which piece do you want to move a throw of " << diceroll[j] << endl << endl;
    cout << ">";
    int choice;
    cin >> choice;
    verifyInput();

    int c = 0;
    int validity = choicevalidity(diceroll[j], turn, choice);

    // Handle invalid choices or overshooting
    while ((validity == 0 || (c < 4 && isblock(turn, choice))) && validity != -1) {
        cout << "Enter valid token: ";
        cin >> choice;
        verifyInput();
        validity = choicevalidity(diceroll[j], turn, choice);
        c++;  // Prevent infinite loops in case of unexpected situations
    }

    // If the move overshoots or no valid move is possible
    if (validity == -1) {
        cout << RED << "Move overshoots the final position or invalid choice for all tokens. Skipping this turn." << RESET << endl;
        break; // Exit the loop and skip the turn
    }

    if (c != 4 && validity == 1) {
        movetoken(turn, choice, diceroll[j]);
        createMasterThread(turn, choice);
    }

    j++;
}
    
    sem_post(&sem);
    return NULL;
}


bool isblock(int turn, int choice)
{
    int c = 0;
    for (int i = 0; i < MAX_PLAYER; i++)
    {
        if (i != turn) // Don't check the current player (turn) against themselves
        {
            for (int j = 0; j < n_tokens; j++)
            {
                if (players[i][j].x == players[turn][choice - 1].x && players[i][j].y == players[turn][choice - 1].y && players[i][j].team_member == players[turn][choice - 1].team_member) // Ensure not on the same team
                {
                    if (is_safepoint(players[i][j].index)) // Check if it is not a safe point
                    {
                        c++;
                    }
                    if (c >= 2) // If two or more blocks are detected, return true
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false; // If no block is detected
}


void shareDiceRollWithPartner(int turn, int partner, int diceroll[]) {
    // Check for three 6s - skip turn
    if (diceroll[0] == 6 && diceroll[1] == 6 && diceroll[2] == 6) {
        cout << "Three 6s rolled for Player " << partner + 1 << ". Turn skipped!" << endl;
        return;
    }

    bool anyMovePossible = false;
    for (int rollIndex = 0; rollIndex < 3; rollIndex++) { // Iterate over dice results
        anyMovePossible = false; // Reset for each roll
        vector<int> movableTokens;
        vector<int> openableTokens;

        // Special handling for 6: Check for openable tokens
        if (diceroll[rollIndex] == 6) {
            // Find tokens that can be opened
            for (int token = 0; token < n_tokens; token++) {
                if (players[partner][token].index == -1) {
                    openableTokens.push_back(token);
                }
            }

            // If openable tokens exist, ask partner which token to open
            if (!openableTokens.empty()) {
                cout << "Player " << partner + 1 << ", you can open a new token with this 6." << endl;
                cout << "Available tokens to open:" << endl;
                for (size_t i = 0; i < openableTokens.size(); i++) {
                    cout << "[" << i + 1 << "] Token " << openableTokens[i] + 1 << endl;
                }

                int choice;
                do {
                    cout << "Enter your choice (1-" << openableTokens.size() << "): ";
                    cin >> choice;
                } while (choice < 1 || choice > (int)openableTokens.size());

                // Open the chosen token
                int chosenToken = openableTokens[choice - 1];
                movetoken(partner, chosenToken + 1, 6);
                anyMovePossible = true;
                continue;  // Move to next roll
            }
        }

        // Regular move logic for other rolls or if no token was opened
        for (int token = 0; token < n_tokens; token++) {
            // Check if token is in play and can move
            if (players[partner][token].index != -1 && 
                players[partner][token].index != 106) {
                int targetIndex = players[partner][token].index + diceroll[rollIndex];
                
                if (targetIndex <= 106) {
                    movableTokens.push_back(token);
                    anyMovePossible = true;
                }
            }
        }

        if (anyMovePossible) {
            int chosenToken;
            if (movableTokens.size() == 1) {
                // If only one token can move, select it automatically
                chosenToken = movableTokens[0];
                cout << "Player " << partner + 1 << " has only one token to move: Token " << chosenToken + 1 << endl;
                movetoken(partner, chosenToken + 1, diceroll[rollIndex]);
            } else if (!movableTokens.empty()) {
                // If multiple tokens can move, prompt the player for input
                cout << "Player " << partner + 1 << ", choose a token to move for dice roll " << diceroll[rollIndex] << ": ";
                for (size_t i = 0; i < movableTokens.size(); i++) {
                    cout << "[" << i + 1 << "] Token " << movableTokens[i] + 1 << " ";
                }
                cout << endl;
                
                int choice;
                do {
                    cout << "Enter your choice (1-" << movableTokens.size() << "): ";
                    cin >> choice;
                } while (choice < 1 || choice > (int)movableTokens.size());
                
                chosenToken = movableTokens[choice - 1];
                // Move the chosen token
                movetoken(partner, chosenToken + 1, diceroll[rollIndex]);
            }
        } else {
            // No valid moves for this roll
            cout << "Player " << partner + 1 << " has no valid moves for roll: " << diceroll[rollIndex] << endl;
            usleep(5000000); // Pause for a moment
        }
    }
}

/*
// Master thread for managing game logic, team checks, block checks, and win condition
void *Masterthread(void *args)
{
    int turn = (*(pair<int, int> *)args).first;
    int choice = (*(pair<int, int> *)args).second;

    // Check if both players in the team have moved all pieces to home
    bool teamWon = true;
    for (int i = 0; i < n_tokens; i++)
    {
        if (players[turn][i].index != 106) // Not in home triangle
        {
            teamWon = false;
            break;
        }
    }

    if (teamWon)
    {
        // Check if the other team member has completed their pieces as well
        int partner = (turn + 1) % 2; // Assuming two players per team
        bool partnerWon = true;
        for (int i = 0; i < n_tokens; i++)
        {
            if (players[partner][i].index != 106)
            {
                partnerWon = false;
                break;
            }
        }

        if (partnerWon)
        {
            cout << YELLOW << "🏆 Player " << idToColor(turn) << turn + 1 << " and Player " << partner + 1 << " have won the game! 🏆" << RESET << endl;
            usleep(5000000);
            isfinished[turn] = 1;
            isfinished[partner] = 1;
            return NULL;
        }
    }

    // Check for inactivity (if player has not progressed for 20 turns)
    if (players[turn][0].counter >= 20)
    {
        cout << RED << "❌ GAME OVER for Player " << turn + 1
             << "! (20 turns without progress)" << RESET << endl;

        cout << "🚫  Cancelled thread ID: " << pthread_self() << endl;
        players[turn][0].ingame = false;
        players[turn][0].position = -1;
        position++;
        isfinished[turn] = 1;
    }

for (int i = 0; i < MAX_PLAYER; i++) {
    if (i != turn) {
        for (int j = 0; j < n_tokens; j++) {
            if (players[i][j].x == players[turn][choice - 1].x &&
                players[i][j].y == players[turn][choice - 1].y &&
                players[i][j].team_member != players[turn][choice - 1].team_member &&
                (!is_safepoint(players[i][j].index))) {
                
                cout << "Player " << i + 1 << " (Team " << players[i][j].team_member
                     << ") was hit by Player " << turn + 1
                     << " (Team " << players[turn][choice - 1].team_member << ")" << endl;
                     
                          // Optional: Increase the pause time to 1 second
        usleep(5000000); // 1 second pause

                cout << RED << "💥 Token Eliminated! Player "
                     << i + 1 << "'s token sent back to base." << RESET << endl;

                players[i][j].index = -1;
                players[i][j].x = houses[i][j].x;
                players[i][j].y = houses[i][j].y;
                for (int k = 0; k < n_tokens; k++) {
                    players[turn][k].removed++;
                }
            }
        }
    }
}


    // Block Check: Ensure the selected move is not blocked by the opponent's pieces
    if (isblock(turn, choice))
    {
        cout << RED << "🚫 Player " << turn + 1 << "'s piece is blocked by a team member's token!" << RESET << endl;
        usleep(5000000);
        return NULL;
    }

    // Check if a player has completed all their pieces and can share the roll
    if (players[turn][0].ingame == true)
    {
        bool canShare = true;
        for (int i = 0; i < n_tokens; i++)
        {
            if (players[turn][i].index != 106)
            {
                canShare = false;
                break;
            }
        }

        if (canShare)
        {
            // Share roll with partner if this player has completed all pieces
            int partner = (turn + 1) % 2;
            cout << "Player " << turn + 1 << " can now share their roll with Player " << partner + 1 << endl;
            usleep(5000000);
            shareDiceRollWithPartner(turn, partner, diceroll);
        }
    }

    // Verifying Win, Assigning Position & Removing
    int count = 0;
    for (int j = 0; j < n_tokens; j++)
    {
        if (players[turn][j].index == 106) // All pieces in home triangle
        {
            count++;
        }
    }
    if (count == n_tokens && players[turn][0].ingame == true)
    {
        cout << YELLOW << "🏆 Player " << idToColor(turn)
             << turn + 1 << " finished at position: "
             << winning_position << "! 🏆" << RESET << endl;
        usleep(5000000);
        players[turn][0].ingame = false;
        players[turn][0].position = winning_position;
        cout << "Player " << idToColor(turn) << " finished at position: " << winning_position << endl;
        winning_position++;
        position++;
        isfinished[turn] = 1;
    }

    return NULL;
}

*/

void *Masterthread(void *args)
{
    int turn = (*(pair<int, int> *)args).first;
    int choice = (*(pair<int, int> *)args).second;

    // Check if the current player has finished all their tokens
    bool playerFinished = true;
    for (int i = 0; i < n_tokens; i++)
    {
        if (players[turn][i].index != 106) // Not in home triangle
        {
            playerFinished = false;
            break;
        }
    }

if (n_teams > 1 && playerFinished) {
    // Find partner
    int partner = -1;
    for (int i = 0; i < n_players; i++) {
        if (i != turn && players[i][0].team_member == players[turn][0].team_member) {
            partner = i;
            break;
        }
    }

    if (partner != -1) {
        // Check if the partner has also finished
        bool partnerFinished = true;
        for (int i = 0; i < n_tokens; i++) {
            if (players[partner][i].index != 106) {  // Not finished
                partnerFinished = false;
                break;
            }
        }

        if (partnerFinished && playerFinished) {  // Both players must be finished
            // Both players finished, declare the team winner
            cout << YELLOW << "🏆 Team " << players[turn][0].team_member
                 << " (Players " << turn + 1 << " and " << partner + 1 << ") have won the game! 🏆" << RESET << endl;
           

            // Mark both players as finished
            isfinished[turn] = isfinished[partner] = 1;
            players[turn][0].ingame = players[partner][0].ingame = false;
            players[turn][0].position = players[partner][0].position = winning_position++;
            position += 2;
            gameOver = true; // Mark the game as over
            usleep(5000000);
            displayTeam();
            pthread_exit(NULL);
            return NULL; // End the game after declaring the winner
        }
    }
}


    // Dice Roll Sharing (only for team play)
    if (n_teams > 1 && playerFinished)
    {
        int partner = -1;
        for (int i = 0; i < n_players; i++)
        {
            if (i != turn && players[i][0].team_member == players[turn][0].team_member)
            {
                partner = i;
                break;
            }
        }

        if (partner != -1)
        {
            cout << "Player " << turn + 1 << " can now share their roll with Player " << partner + 1 << endl;
            usleep(5000000);
            shareDiceRollWithPartner(turn, partner, diceroll);
        }
    }


    // Single-Player Win Condition (if only one team or no teams)
    if (n_teams <= 1 && playerFinished)
    {
        cout << YELLOW << "🏆 Player " << idToColor(turn)
             << turn + 1 << " finished at position: "
             << winning_position << "! 🏆" << RESET << endl;
        usleep(5000000);
        players[turn][0].ingame = false;
        players[turn][0].position = winning_position;
        cout << "Player " << idToColor(turn) << " finished at position: " << winning_position << endl;
        winning_position++;
        position++;
        isfinished[turn] = 1;
        gameOver = true;  // Game is over
    }

    // Inactivity Check
    if (players[turn][0].counter >= 20)
    {
             usleep(5000000);
        cout << RED << "❌ GAME OVER for Player " << turn + 1
             << "! (20 turns without progress)" << RESET << endl;
        players[turn][0].ingame = false;
        players[turn][0].position = -1;
        position++;
        isfinished[turn] = 1;
    }
    
   
    if (gameOver) {
        return NULL; // Stop the thread if the game is over
    }
    
    
for (int i = 0; i < MAX_PLAYER; i++) {
    if (i != turn) {
        for (int j = 0; j < n_tokens; j++) {
            if (players[i][j].x == players[turn][choice - 1].x &&
                players[i][j].y == players[turn][choice - 1].y &&
                players[i][j].team_member != players[turn][choice - 1].team_member &&
                (!is_safepoint(players[i][j].index))) {
                
                cout << "Player " << i + 1 << " (Team " << players[i][j].team_member
                     << ") was hit by Player " << turn + 1
                     << " (Team " << players[turn][choice - 1].team_member << ")" << endl;
                     
                          // Optional: Increase the pause time to 1 second
   
                 players[i][j].hits++; // Increase the hit count for the player
                cout << RED << "💥 Token Eliminated! Player "
                     << i + 1 << "'s token sent back to base." << RESET << endl;

                
                players[i][j].index = -1;
                players[i][j].x = houses[i][j].x;
                players[i][j].y = houses[i][j].y;
                for (int k = 0; k < n_tokens; k++) {
                    players[turn][k].removed++;
                }
                     usleep(5000000); // 1 second pause
            }
        }
    }
}



    // Optional: Continue the rest of the logic as before (token elimination, block check, etc.)
    
    // Block Check
    if (isblock(turn, choice))
    {
        cout << RED << "🚫 Player " << turn + 1 << "'s piece is blocked!" << RESET << endl;
        return NULL;
    }
    
    
   
    


    return NULL;
}



// Master thread creation function
void *createMasterThread(int turn, int choice)
{
    pthread_t master;
    pair<int, int> args = make_pair(turn, choice);
    pthread_create(&master, NULL, Masterthread, (void *)&args);
    void *status;
    pthread_join(master, &status);
    return status;
}



bool is_safepoint(int index)
{
    if((index == 0)||(index == 8)||(index == 13)||(index == 21)||(index == 26)||(index == 34)|| (index == 39) || (index == 47))
    {
        return true;
    }
    return false;
}

bool isanymovepossible(int turn, int diceroll[]) {
    // If player is finished, no move possible
    if (isfinished[turn] == 1) {
        return false;
    }

    // Check if any token is at the starting position
    bool hasTokenAtStart = false;
    for (int i = 0; i < n_tokens; i++) {
        if (players[turn][i].index == -1) {
            hasTokenAtStart = true;
        }
    }

    // If any dice roll is 6 and there's a token at the starting position
    for (int i = 0; i < 3; i++) {
        if (diceroll[i] == 6 && hasTokenAtStart) {
            return true;
        }
    }

    // Check moves for each dice roll
    for (int rollIndex = 0; rollIndex < 3; rollIndex++) {
        if (diceroll[rollIndex] == 0) continue;  // Skip empty rolls

        // Check for any token that can move
        bool movePossibleForRoll = false;
        for (int i = 0; i < n_tokens; i++) {
            // Skip tokens that are not in play
            if (players[turn][i].index == -1 || players[turn][i].index == 106) continue;

            // Calculate target index
            int currentIndex = players[turn][i].index;
            int targetIndex = currentIndex + diceroll[rollIndex];

            // Strict check for final position
            if (targetIndex <= 106) {
                // Ensure the move doesn't overshoot the final position
                if (targetIndex == 106 || targetIndex < 106) {
                    movePossibleForRoll = true;
                    break;
                }
            }
        }

        // If no move is possible for this roll, return false
        if (!movePossibleForRoll) {
            return false;
        }
    }

    // All rolls have a possible move
    return true;
}


void movetoken(int turn, int choice, int result)
{
    if (players[turn][choice - 1].index == -1 && result == 6)
        {
            players[turn][choice - 1].index = getIndexByTurn(turn);
            players[turn][choice - 1].x = cells[getIndexByTurn(turn)].x;
            players[turn][choice - 1].y = cells[getIndexByTurn(turn)].y;
        }
        else if (players[turn][choice - 1].index != -1)
        {
            int step = result;
            while (step > 0)
            {
                if (isFinalWay(turn, choice, players[turn][choice - 1].index))
                {
                    players[turn][choice - 1].index = 100;
                }
                if (players[turn][choice - 1].index < CELL_NUMBER)
                {
                    players[turn][choice - 1].index = (players[turn][choice - 1].index + 1) % CELL_NUMBER;
                    players[turn][choice - 1].x = cells[players[turn][choice - 1].index].x;
                    players[turn][choice - 1].y = cells[players[turn][choice - 1].index].y;
                }
                else
                {
                    players[turn][choice - 1].index = players[turn][choice - 1].index + 1;
                    moveOnFinalWay(players, turn, choice);
                    if (players[turn][choice - 1].x == 5 && players[turn][choice - 1].y == 5)
                    {
                        displayCurrent();
                    }
                }
                step--;
            }
        }
}

int choicevalidity(int res, int turn, int choice) {
    if (choice < 1 || choice > 4 || choice > n_tokens) {
        return 0; // Invalid token selection
    }

    int tokenIndex = choice - 1;

    // Check if token has already finished
    if (players[turn][tokenIndex].index == 106) {
        return 0; // Token has already completed the game
    }

    // Special case: Token is at starting position (-1)
    if (players[turn][tokenIndex].index == -1) {
        if (res != 6) {
            return 0; // Token can only be moved out with a roll of 6
        }
        return 1; // Valid move
    }

    int currentIndex = players[turn][tokenIndex].index;
    int targetIndex = currentIndex + res;

    // Skip move if it overshoots the final position
    if (targetIndex > 106) {
        return -1; // Overshooting the final position
    }

    return 1; // Valid move
}

