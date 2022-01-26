#include "header.h"

std::string categoryChoice(int choice, Categories* cats) {
	std::string filePath = "na";
	int i;
	if (choice == (-1)) filePath = "exit";
	if (choice == (-2)) filePath = "catSB";
	if (choice == 0) filePath = cats[(rand() % ((gh.getNumberOfCategories() - 1) - 0 + 1)) + 0].getFilePath();
	if (choice > 0 && choice <= gh.getNumberOfCategories()) {
		for (i = 0; i < gh.getNumberOfCategories(); i++) {
			if ((choice - 1) == i) {
				if (cats[i].getSavedPosition() >= cats[i].getSize()) filePath = "full";
				else filePath = cats[i].getFilePath();
				break;
			}
		}
	}
	return filePath;
}

void endGame(Hangman hangman, Categories* usedCats) {
	clearScreen();
	gh.setNumberOfGames(gh.getNumberOfGames() + 1);
	usedCats[handler.getIndex()].setPlayedGames(usedCats[handler.getIndex()].getPlayedGames() + 1);
	printScore(usedCats[handler.getIndex()]);
	hangman.hangSomeone();
	std::cout << "\n\t";
	gh.getIsWon() ? std::cout << handler.getSecret() << "\n\tGreat job!" << std::endl :
		std::cout << "Word was [" << handler.getSecret() << "]." << std::endl;
	hangman.~Hangman();
	playAgain(usedCats);
}

char getGuessInput(Hangman hangman, Categories currentCat) {
	char guess;
	clearScreen();
	printScore(currentCat);
	hangman.hangSomeone();
	for (int i = 0; i < handler.getGuessingWord()[i] != '\0'; i++) {
		i == 0 ? std::cout << "\n\t" << handler.getGuessingWord()[i] : std::cout << handler.getGuessingWord()[i];
	}
	if(gh.getHintsAllowed()){
		gh.getNumberOfHints() < 3 ? std::cout << "\n\nYou can use " << (3 - gh.getNumberOfHints()) << " more hint(s)." :
			std::cout << "\n\nYou can not use any more hints.";
		std::cout << "\nUsed letters: " << handler.getGuessedLetters();
		std::cout << "\nLength: [" << handler.getSize() << "]\n" << std::endl;
		gh.getNumberOfHints() < 3 ? std::cout << "[1] Hint" << std::endl :
			std::cout << "[1] " << RED << "Hint" << RES << std::endl;
		std::cout << "[2] Give up\n[3] Exit\n[a - z] Guess\n" << std::endl;
	}
	else {
		std::cout << "\n\nUsed letters: " << handler.getGuessedLetters();
		std::cout << "\nLength: [" << handler.getSize() << "]\n\n[1] Give up\n[2] Exit\n[a - z] Guess\n" << std::endl;
	}
	std::cout << "Enter: ";
	std::cin >> guess;
	return guess;
}

std::string pickCategory(Categories* cats) {
	std::string filePath, answer;
	int i, choice;
	do {
		clearScreen();
		if (currUser.getIsSet()) std::cout << "\tCategories [" << PUR << currUser.getCurrentUser() << RES << "]\n\n";
		else std::cout << "\tCategories\n\n";
		for (i = 0; i < gh.getNumberOfCategories(); i++) {
			if (i != 0 && (i % 3) == 0) std::cout << "\n";
			if (cats[i].getSavedPosition() >= cats[i].getSize()) std::cout << "[" << (i + 1) << "] " << RED << cats[i].getName() << RES << "\t";
			else std::cout << "[" << (i + 1) << "] " << YLW << cats[i].getName() << RES << "\t";
		}
		std::cout << "\n\n[0] Random \t[-1] Exit\n[-2] Category Scoreboard\n\nChoice: ";
		std::cin >> choice;
		filePath = categoryChoice(choice, cats);
		if (filePath.compare("exit") == 0) exit(EXIT_SUCCESS);
		else if (filePath.compare("catSB") == 0) scoreBoard(cats);
		else if (filePath.compare("full") == 0) {
			std::cout << "\nU ovoj kategoriji nema vise pojmova.\n";
			if (_getch() <= 0) exit(EXIT_FAILURE);
		}
		else if (filePath.compare("na") == 0) {
			std::cout << "\nNeispravan izbor.\n";
			if (_getch() <= 0) exit(EXIT_FAILURE);
		}
		filePath.clear();
		choice = (-3);
	} while (choice < (-2) || choice > gh.getNumberOfCategories());
	return filePath;
}

void play(Categories* usedCats) {
	char guess;
	bool test;
	std::string filePath;
	filePath = pickCategory(usedCats);
	if (filePath.empty()) exit(EXIT_FAILURE);
	getRandomWord(filePath, usedCats);
	handler.setIndex(getCategoryIndex(usedCats, filePath));
	initArrayOfIndex(usedCats[handler.getIndex()]);
	gh.setEvaluateHintsUsed(false);
	handler.setSize(handler.getSecret().size());
	handler.setGuessingWord(new char[handler.getSize()]);
	handler.getSize() < 6 ? gh.setHintsAllowed(false) : gh.setHintsAllowed(true);
	setWord();
	Hangman hangman;
	gh.setNumberOfHints(0);
	while (true) {
		guess = getGuessInput(hangman, usedCats[handler.getIndex()]);
		if (gh.getHintsAllowed()) {
			if (guess == 49 && gh.getNumberOfHints() < 3) {
				passGuessToGuessed(getHint());
				gh.setEvaluateHintsUsed(false);
				test = check(guess);
				gh.setIsWon(isWon());
			}
			if (guess != 49) gh.setEvaluateHintsUsed(true);
			if (guess == 50) {
				gh.setNumberOfLost(gh.getNumberOfLost() + 1);
				usedCats[handler.getIndex()].setLostGames(usedCats[handler.getIndex()].getLostGames() + 1);
				gh.setIsWon(false);
				endGame(hangman, usedCats);
				break;
			}
			if (guess == 51) break;
		}
		else {
			gh.setEvaluateHintsUsed(true);
			if (guess == 49) {
				gh.setNumberOfLost(gh.getNumberOfLost() + 1);
				usedCats[handler.getIndex()].setLostGames(usedCats[handler.getIndex()].getLostGames() + 1);
				gh.setIsWon(false);
				endGame(hangman, usedCats);
				break;
			}
			else if (guess == 50) break;
		}
		if (gh.getEvaluateHintsUsed()) {
			passGuessToGuessed(guess);
			test = check(guess);
			if (!test) hangman.setState(hangman.getState() + 1);
			gh.setIsWon(isWon());
			if (gh.getIsWon()) {
				gh.setNumberOfWon(gh.getNumberOfWon() + 1);
				usedCats[handler.getIndex()].setWonGames(usedCats[handler.getIndex()].getWonGames() + 1);
				endGame(hangman, usedCats);
				break;
			}
			else if (hangman.getState() == 6 && !gh.getIsWon()) {
				gh.setNumberOfLost(gh.getNumberOfLost() + 1);
				usedCats[handler.getIndex()].setLostGames(usedCats[handler.getIndex()].getLostGames() + 1);
				endGame(hangman, usedCats);
				break;
			}
		}
	}
	delete[] handler.getGuessingWord();
	handler.~WordHandler();
	hangman.~Hangman();
	exit(EXIT_SUCCESS);
}