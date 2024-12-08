#ifndef GAMEPLAY_UTILS_H
#define GAMEPLAY_UTILS_H

#include "ludo.h"


void *PlayerThread(void *arg);
void *createMasterThread(int turn = 0, int choice = -1);
void *Masterthread(void *args);
bool isblock(int turn, int choice);
bool is_safepoint(int index);
bool isanymovepossible(int turn, int diceroll[]);
void movetoken(int turn, int choice, int result);
int choicevalidity(int res, int turn, int choice);
void shareDiceRollWithPartner(int turn, int partner, int diceroll[]);
bool isValidMovePossible(int turn, int diceRoll);
#endif // GAMEPLAY_UTILS_H

