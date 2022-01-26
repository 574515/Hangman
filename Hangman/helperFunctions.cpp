#include "header.h"
#include <regex>

enum IN {
	IN_BACK = 8,
	IN_RET = 13
};

bool check(char guess) {
	bool flag = false;
	for (int i = 0; i < handler.getSize(); i++) {
		if (handler.getSecret()[i] == guess) {
			handler.setValuesToGuessingWord(guess, i);
			flag = true;
		}
		else if (handler.getSecret()[i] == (guess - 32)) {
			handler.setValuesToGuessingWord(toupper(guess), i);
			flag = true;
		}
		else if (handler.getSecret()[i] == (guess + 32)) {
			handler.setValuesToGuessingWord(tolower(guess), i);
			flag = true;
		}
	}
	return flag;
}

void clearScreen() {
#ifdef WINDOWS
	std::system("cls");
#else
	std::system("clear");
#endif
}

bool doesAdminExists(std::string& username) {
	std::ifstream fPtr("admins.txt");
	if (!fPtr.good()) exit(EXIT_FAILURE);
	std::string readFile;
	while (getline(fPtr, readFile)) if (fPtr.good() && strcmp(readFile.c_str(), username.c_str()) == 0) return true;
	return false;
}

bool doesDirExists(std::string& path) {
	try {
		std::filesystem::path pathObj(path);
		if (std::filesystem::exists(pathObj) && std::filesystem::is_directory(pathObj)) return true;
	}
	catch (std::filesystem::filesystem_error & e) { std::cerr << e.what() << std::endl; }
	return false;
}

bool doesExistsInDirectory(std::string& fileName) {
	bool exists = false;
	std::ifstream check(gh.getFullPath() + '\\' + fileName + ".txt");
	if (check.good()) exists = true;
	check.close();
	return exists;
}

bool fileDeletionPrompt(std::string& fileName) {
	std::string answer;
	bool decision = false;
	do {
		clearScreen();
		std::cout << "You are about to delete " << RED << fileName << ".txt" << RES << ". It is irreversible.\nAre you sure [yes / no]: ";
		std::cin >> answer;
		if (answer.compare("yes") == 0) decision = true;
		else if (answer.compare("no") == 0) decision = false;
	} while ((answer.compare("yes") != 0) && (answer.compare("no") != 0));
	return decision;
}

char getHint() {
	int i, index;
	do {
		index = (rand() % ((handler.getSize() - 1) - 0 + 1)) + 0;
	} while (handler.getGuessingWordLetterOnIndex(index) != '*');
	gh.setNumberOfHints(gh.getNumberOfHints() + 1);
	for (i = 0; i < handler.getSize(); i++)
		if (handler.getSecret()[index] == handler.getSecret()[i])
			handler.setValuesToGuessingWord(handler.getSecret()[index], i);
	return handler.getGuessingWordLetterOnIndex(index);
}

std::string getPasswordInput() {
	std::string password = "";
	const char helpChar = '*';
	char chInput;
	while (true) {
		chInput = _getch();
		if (chInput == IN::IN_RET) {
			std::cout << std::endl;
			return password;
		}
		else if (chInput == IN::IN_BACK && password.length() != 0) {
			password.pop_back();
			std::cout << "\b \b";
			continue;
		}
		else if (chInput == IN::IN_BACK && password.length() == 0) continue;
		password.push_back(chInput);
		std::cout << helpChar;
	}
	return password;
}

std::string getUsernameInput() {
	bool validate;
	std::string username;
	do {
		clearScreen();
		std::cout << "Enter username: ";
		std::cin >> username;
		validate = handleInput(username);
		if (!validate) username.clear();
	} while (!validate);
	return username;
}

bool handleInput(std::string& user) {
	int lengthCheck;
	bool validate, areAllDigits;
	lengthCheck = isLongEnough(user);
	validate = isValid(user);
	areAllDigits = isAllDigits(user);
	if (lengthCheck == (-1)) {
		std::cout << "\nUsername has to be at least 4 characters long." << std::endl;
		if (_getch() <= 0) exit(EXIT_FAILURE);
		return false;
	}
	else if (lengthCheck == 1) {
		std::cout << "\nUsername has to be shorter than 16 characters." << std::endl;
		if (_getch() <= 0) exit(EXIT_FAILURE);
		return false;
	}
	else if (!validate) {
		std::cout << "\nNo special characters allowed." << std::endl;
		if (_getch() <= 0) exit(EXIT_FAILURE);
		return false;
	}
	else if (areAllDigits) {
		std::cout << "\nUsername has to have at least one letter." << std::endl;
		if (_getch() <= 0) exit(EXIT_FAILURE);
		return false;
	}
	return true;
}

bool isAllDigits(std::string& user) {
	for (char& c : user) if (isalpha(c)) return false;
	return true;
}

bool isAlreadyIn(char guess) {
	bool flag = false;
	for (char c : handler.getGuessedLetters()) {
		if (c == guess) {
			flag = true;
			break;
		}
	}
	return flag;
}

int isLongEnough(std::string& user) {
	if (user.length() < 4) return (-1);
	else if (user.length() > 16) return 1;
	return 0;
}

bool isPassowrdGood(Admin admin, Encrypter crypter) {
	if (admin.getPassword().compare(crypter.getEnryptedWord()) == 0) return true;
	return false;
}

bool isValid(std::string name) {
	const std::regex pattern("^[a-zA-Z0-9]*$");
	if (std::regex_match(name, pattern)) return true;
	return false;
}

bool isWon() {
	int i = 0;
	for (char c : handler.getSecret()) if (c != handler.getValuesOfGuessingWord(i++)) return false;
	return true;
}

void passGuessToGuessed(char guess) {
	bool evaluate = false;
	for (char c : handler.getGuessedLetters()) if (c == guess) evaluate = true;
	if (!evaluate) {
		handler.setGuessedLetter(guess);
		handler.setGuessedLetter(' ');
	}
}

void playAgain(Categories* usedCats) {
	std::string answer;
	bool correct = false;
	do {
		std::cout << "\nWould you like to play again [yes / no]: ";
		std::cin.clear();
		std::cin >> answer;
		if (answer.compare("yes") == 0) {
			correct = true;
			reSetHandler();
			play(usedCats);
		}
		else if (answer.compare("no") == 0) {
			int i;
			if (gh.getFullPath().compare(gh.getInitPath()) == 0) {
				for (i = 0; i < gh.getNumberOfCategories(); i++) usedCats[i].deleteArray(usedCats[i].getIndexArray());
				delete[] usedCats;
			}
			gh.~GameHandler();
			exit(EXIT_SUCCESS);
		}
		else correct = false;
	} while (!correct);
}

void printScore(Categories cat) {
	std::cout << "\tUser: [" << PUR << currUser.getCurrentUser() << RES << "]" << std::endl;
	std::cout << "\tP: [" << BLUE << gh.getNumberOfGames() << RES << "]\t";
	std::cout << "W: [" << GRN << gh.getNumberOfWon() << RES << "]\t";
	std::cout << "L: [" << RED << gh.getNumberOfLost() << RES << "]" << std::endl;
	std::cout << "\tCategory: [" << PUR << cat.getName() << RES << "]\n" << std::endl;
}

Admin registerNewAdmin(Encrypter crypter) {
	Admin admin;
	std::string password;
	admin.setUsername(getUsernameInput());
	do {
		clearScreen();
		std::cout << "Enter password: ";
		password = getPasswordInput();
	} while (password.empty() || password.size() < 1 || password.size() > 16);
	crypter.setPlainWord(password);
	crypter.encrypt();
	admin.setPassword(crypter.getEnryptedWord());
	std::ofstream out("admins.txt");
	out << admin;
	out.close();
	return admin;
}

void reSetHandler() {
	handler.reSetGuessedLetter();
	handler.reSetSecret();
	handler.setIndex(0);
	handler.setSize(0);
	handler.deleteArray(handler.getGuessingWord());
}

void scoreBoard(Categories* cats) {
	clearScreen();
	int i;
	for (i = 0; i < gh.getNumberOfCategories(); i++) {
		std::cout << "[" << (i + 1) << "]\t";
		std::cout << "P: [" << BLUE << cats[i].getPlayedGames() << RES << "]\t";
		std::cout << "W: [" << GRN << cats[i].getWonGames() << RES << "]\t";
		std::cout << "L: [" << RED << cats[i].getLostGames() << RES << "]\t\t";
		if (cats[i].getSavedPosition() >= cats[i].getSize()) std::cout << RED << cats[i].getName() << RES << std::endl;
		else std::cout << YLW << cats[i].getName() << RES << std::endl;
	}
	std::cout << "\n[TOTAL]\tP: [" << BLUE << gh.getNumberOfGames() << RES << "] \t";
	std::cout << "W: [" << GRN << gh.getNumberOfWon() << RES << "]\t";
	std::cout << "L: [" << RED << gh.getNumberOfLost() << RES << "]" << std::endl;
	if (_getch() <= 0) exit(EXIT_FAILURE);
	return;
}

void setUser() {
	std::string user;
	bool validate;
	do {
		clearScreen();
		std::cout << "Enter name [4, 16]: ";
		std::cin >> user;
		validate = handleInput(user); // xx
		if (!validate) user.clear();
	} while (!validate);
	currUser.setCurrentUser(user);
	currUser.setIsSet(true);
}

void setWord() {
	for (int i = 0; i < handler.getSize(); i++) {
		if ((handler.getSecret()[i] >= 65 && handler.getSecret()[i] <= 90)
			|| (handler.getSecret()[i] >= 97 && handler.getSecret()[i] <= 122))
			handler.setValuesToGuessingWord('*', i);
		else handler.setValuesToGuessingWord(handler.getSecret()[i], i);
	}
}