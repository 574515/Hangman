#ifndef HEADER_H
#define HEADER_H
#define _CRTDBG_MAP_ALLOC
#include "classes.h"
#include <conio.h>
#include <crtdbg.h>

// colors
constexpr auto RED = "\x1B[31m";
constexpr auto BLUE = "\x1B[34m";
constexpr auto GRN = "\x1B[32m";
constexpr auto PUR = "\033[0;35m";
constexpr auto YLW = "\033[0;33m";
constexpr auto RES = "\033[0m";

// adminPanel.cpp
void addNewCategory();
Admin adminLogIn();
void printCategories(Categories*);
void printCurrentPath();
void removeCategory();
void setNewPath();

// categoryFunctions.cpp
int getCategoryIndex(Categories*, std::string&);
std::string getCategoryName(const std::string&);
int getCategorySize(std::string&);
void getRandomWord(std::string&, Categories*);
void initArrayOfIndex(Categories);
void initSetCategories(Categories*);
void randomizeArray(int*, int);
void swap(int*, int*);

// helperFunctions.cpp
bool check(char);
void clearScreen();
bool doesAdminExists(std::string&);
bool doesDirExists(std::string&);
bool doesExistsInDirectory(std::string&);
bool fileDeletionPrompt(std::string&);
char getHint();
std::string getPasswordInput();
std::string getUsernameInput();
bool handleInput(std::string&);
bool isAllDigits(std::string&);
bool isAlreadyIn(char);
int isLongEnough(std::string&);
bool isPassowrdGood(Admin, Encrypter);
bool isValid(std::string);
bool isWon();
void passGuessToGuessed(char);
void playAgain(Categories*);
void printScore(Categories);
Admin registerNewAdmin(Encrypter);
void reSetHandler();
void scoreBoard(Categories*);
void setUser();
void setWord();

// menus.cpp
void adminPanel(Admin, Categories*);
void initialMenu();

// playFunctions.cpp
std::string categoryChoice(int, Categories*);
void endGame(Hangman, Categories*);
char getGuessInput(Hangman, Categories);
std::string pickCategory(Categories*);
void play(Categories*);

#endif // HEADER_H