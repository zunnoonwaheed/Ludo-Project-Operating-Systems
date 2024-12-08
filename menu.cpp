#include "ludo.h"
#include "utils.h"
#include "menu.h"
#include "gui_helper.h"


void drawLudoLogo() {
    string ludoLogo = R"(
    ██╗     ██╗   ██╗██████╗  ██████╗ 
    ██║     ██║   ██║██╔══██╗██╔═══██╗
    ██║     ██║   ██║██║  ██║██║   ██║
    ██║     ██║   ██║██║  ██║██║   ██║
    ███████╗╚██████╔╝██████╔╝╚██████╔╝
    ╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ 
    )";
    centerText(ludoLogo, MAGENTA);
}


  void TeamdrawLudoLogo() {
       string ludoLogo  =  R"(
    ████████╗███████╗ █████╗ ███╗   ███╗██████╗ ██╗      █████╗ ██╗   ██╗
    ╚══██╔══╝██╔════╝██╔══██╗████╗ ████║██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝
       ██║   █████╗  ███████║██╔████╔██║██████╔╝██║     ███████║ ╚████╔╝ 
       ██║   ██╔══╝  ██╔══██║██║╚██╔╝██║██╔═══╝ ██║     ██╔══██║  ╚██╔╝  
       ██║   ███████╗██║  ██║██║ ╚═╝ ██║██║     ███████╗██║  ██║   ██║   
       ╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   
    )";
    centerText(ludoLogo, MAGENTA);
}


void flashingText(const string& text, int flashes = 3, int delay = 300) {
    for (int i = 0; i < flashes; ++i) {
        // Alternate between colors
        if (i % 2 == 0) {
            cout << "\r" << YELLOW << text << RESET << flush;
        } else {
            cout << "\r" << GREEN << text << RESET << flush;
        }
        this_thread::sleep_for(chrono::milliseconds(delay));
        cout << "\r" << string(text.length(), ' ') << "\r" << flush;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }
    // Final display
    cout << "\r" << BRIGHT_YELLOW << text << RESET << endl;
}

void menu(int &choice) {
    while (true) {
        // Clear screen and draw decorative elements
        clearScreen();
        
        // Draw Ludo Logo
        drawLudoLogo();
        
        // Decorative lines
        horizontalLine('=', 80, CYAN);
        
        // Menu Title with Flashing Effect
        flashingText("🎲 LUDO GAME MENU 🎲", 3, 300);
        
        // Decorative lines
        horizontalLine('-', 80, GREEN);
        
        // Menu Options with Colorful Design
        vector<string> menuOptions = {
        "1. START GAME 🚀",
        "2. MAKE TEAMS 👥",
        "3. QUIT GAME 🏳️"
        };

        // Display Menu Options
        for (const auto& option : menuOptions) {
            centerText(option, YELLOW);
        }
        
        // Decorative lines
        horizontalLine('-', 80, RED);
        
        // Prompt for Choice
        cout << BRIGHT_YELLOW;
        centerText("Enter your choice (1-2): ");
        cout << RESET;
        
        // Input with error handling
        cout << GREEN << "> " << RESET;
        cin >> choice;
        verifyInput();
        
        // Validate main menu choice
        if (choice >= 1 && choice <= 2) {
            break;
        }
        
        // Invalid choice handling with flashing error
        centerText("INVALID CHOICE! Please try again.", RED);
        this_thread::sleep_for(chrono::seconds(1));
    }
    
    // Player and Token Configuration
    if (choice == 1) {
        // Player Number Selection
        while (true) {
            clearScreen();
            drawLudoLogo();
            horizontalLine('=', 80, CYAN);
            centerText("PLAYER CONFIGURATION", MAGENTA);
            horizontalLine('-', 80, GREEN);
            
            cout << YELLOW;
            centerText("Select Number of Players (2-4)");
            cout << RESET;
            
            cout << GREEN << "> " << RESET;
            cin >> n_players;
            verifyInput();
            
            if (n_players >= 2 && n_players <= 4) {
                break;
            }
            
            centerText("INVALID PLAYERS! Choose between 2-4.", RED);
            this_thread::sleep_for(chrono::seconds(1));
        }
        
        // Token Number Selection
        while (true) {
            clearScreen();
            drawLudoLogo();
            horizontalLine('=', 80, CYAN);
            centerText("TOKEN CONFIGURATION", MAGENTA);
            horizontalLine('-', 80, GREEN);
            
            cout << YELLOW;
            centerText("Select Number of Tokens (1-4)");
            cout << RESET;
            
            cout << GREEN << "> " << RESET;
            cin >> n_tokens;
            verifyInput();
            
            if (n_tokens >= 1 && n_tokens <= 4) {
                break;
            }
            
            centerText("INVALID TOKENS! Choose between 1-4.", RED);
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    else if (choice == 2) {
      
       
        n_players = 4; // Fixed for team play

        // Token Number Selection
        while (true) {
            clearScreen();
             TeamdrawLudoLogo();
            horizontalLine('=', 80, CYAN);
            centerText("TOKEN CONFIGURATION", MAGENTA);
            horizontalLine('-', 80, GREEN);

            cout << YELLOW;
            centerText("Select Number of Tokens (1-4)");
            cout << RESET;

            cout << GREEN << "> " << RESET;
            cin >> n_tokens;
            verifyInput();

            if (n_tokens >= 1 && n_tokens <= 4) {
                break;
            }

            centerText("INVALID TOKENS! Choose between 1-4.", RED);
            this_thread::sleep_for(chrono::seconds(1));
            }
        
        
        
      
    }
}
