#include "header.h"
#include <time.h>

GameHandler gh("C:\\Users\\User\\Projects\\C++\\Hangman2ndTry\\Hangman2ndTry\\words");

int main() {

	srand((unsigned)time(NULL));

	initialMenu();

	return 0;
}