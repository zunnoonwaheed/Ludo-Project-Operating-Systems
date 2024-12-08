#include "gui_helper.h"

// Screen Clearing Function
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Delayed Text Printing
void printWithDelay(const std::string& text, int delay) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

// Centered Text Printing with Color
void centerText(const std::string& text, const std::string& color) {
    int terminalWidth = 80;
    int textLength = text.length();
    int padding = (terminalWidth - textLength) / 2;

    // Ensure padding is not negative
    padding = std::max(0, padding);

    std::cout << std::string(padding, ' ') << color << text << RESET << std::endl;
}

// Horizontal Line with Color
void horizontalLine(char character, int length, const std::string& color) {
    std::cout << color << std::string(length, character) << RESET << std::endl;
}

