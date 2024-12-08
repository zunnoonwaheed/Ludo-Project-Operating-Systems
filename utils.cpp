#include "ludo.h"
#include <limits>
#include <iostream>
#include "utils.h"
#include "gui_helper.h"


void inputnames()
{
    string Name;

    for (int i = 0; i < n_players; i++) {
        // Typewriter-style delayed text prompt
        printWithDelay("Enter Player " + to_string(i + 1) + "'s Name (Without spaces): ", 50);

        cin >> Name; // Capture player name input
        players[i][0].name = Name; // Assign the name to the appropriate player object
    }
}


void verifyInput()
{
    if (!cin)
    {
        cout << "ERROR - Enter a valid number";

        cin.clear();

        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

string idToColor(int id)
{
    switch (id)
    {
    case 0:
        return RED;
    case 1:
        return GREEN;
    case 2:
        return CYAN;
    case 3:
        return YELLOW;
    default:
        return "";
    }
    return "";
}

int getIndexByTurn(int turn)
{
    return turn * 13;
}

bool isFinalWay(int turn,int choice, int index)
{
    switch (turn)
    {
    case 0:
        if (index == 50 && players[turn][choice-1].removed >0)
            return true; 
        return false;

    case 1:
        if (index == 11 && players[turn][choice-1].removed >0)
            return true;
        return false;
    case 2:
        if (index == 24 && players[turn][choice-1].removed >0)
            return true;
        return false;
    case 3:
        if (index == 37 && players[turn][choice-1].removed >0)
            return true;
        return false;
    }
    return false;
}

void moveOnFinalWay(Player players[MAX_PLAYER][4], int turn, int choice)
{
    switch (turn)
    {
    case 0:
        players[turn][choice - 1].x += 1;
        break;
    case 2:
        players[turn][choice - 1].x -= 1;
        break;
    case 1:
        players[turn][choice - 1].y += 1;
        break;
    case 3:
        players[turn][choice - 1].y -= 1;
        break;
    }
}
