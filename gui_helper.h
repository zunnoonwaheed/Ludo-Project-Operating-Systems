#ifndef GUI_HELPER_H
#define GUI_HELPER_H

#include <iostream>
#include "ludo.h"
#include <string>
#include <chrono>
#include <thread>
#include <algorithm> 





// Function Declarations
void clearScreen();
void printWithDelay(const std::string& text, int delay = 50);
void centerText(const std::string& text, const std::string& color = RESET);
void horizontalLine(char character = '=', int length = 80, const std::string& color = RESET);

#endif // GUI_HELPER_H

