#ifndef CLASSES_H
#define CLASSES_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdlib.h>

class Admin {
private:
	std::string Password;
protected:
	std::string Username;
public:
	Admin() {}

	void setUsername(std::string username) { Admin::Username = username; }
	std::string getUsername() { return Admin::Username; }

	void setPassword(std::string password) { Admin::Password = password; }
	std::string getPassword() { return Admin::Password; }

	friend std::ostream& operator << (std::ostream& out, const Admin& obj) {
		out << obj.Username << "\n" << obj.Password << std::endl;
		return out;
	}
	friend std::istream& operator >> (std::istream& in, Admin& obj) {
		in >> obj.Username;
		in >> obj.Password;
		return in;
	}

	~Admin() {}
};

class Categories {
private:
	int* IndexArray;
	int SavedPosition, Size, Played, Won, Lost;
	std::string Name, FilePath;
public:
	Categories() {
		Categories::Name = "";
		Categories::FilePath = "";
		Categories::IndexArray = NULL;
		Categories::SavedPosition = Categories::Size = Categories::Played = Categories::Won = Categories::Lost = 0;
	}
	Categories(std::string name, std::string filePath) {
		Categories::Name = name;
		Categories::FilePath = filePath;
		Categories::IndexArray = NULL;
		Categories::SavedPosition = Categories::Size = Categories::Played = Categories::Won = Categories::Lost = 0;
	}

	void setValuesToIndexArray(int value, int position) { Categories::IndexArray[position] = value; }
	int getValuesOfIndexArray(int position) { return Categories::IndexArray[position]; }

	void deleteArray(int* arr) { delete[] arr; }

	void setIndexArray(int* indxArr) { Categories::IndexArray = indxArr; }
	int* getIndexArray(void) { return Categories::IndexArray; }

	void setSavedPosition(int savPos) { Categories::SavedPosition = savPos; }
	int getSavedPosition(void) { return Categories::SavedPosition; }

	void setSize(int size) { Categories::Size = size; }
	int getSize(void) { return Categories::Size; }

	void setName(std::string name) { Categories::Name = name; }
	std::string getName(void) { return Categories::Name; }

	void setFilePath(std::string filePath) { Categories::FilePath = filePath; }
	std::string getFilePath(void) { return Categories::FilePath; }

	void setPlayedGames(int played) { Categories::Played = played; }
	int getPlayedGames(void) { return Categories::Played; }

	void setWonGames(int won) { Categories::Won = won; }
	int getWonGames(void) { return Categories::Won; }

	void setLostGames(int lost) { Categories::Lost = lost; }
	int getLostGames(void) { return Categories::Lost; }

	~Categories() {}
};

class Encrypter {
private:
	int Key;
	std::string PlainWord, EncryptedWord, DecryptedWord;
public:
	Encrypter(int key) { Encrypter::Key = key; }

	void encrypt() {
		Encrypter::EncryptedWord.clear();
		for (char c : Encrypter::PlainWord) Encrypter::EncryptedWord.push_back((char)(c + Encrypter::Key));
	}
	std::string getEnryptedWord() { return Encrypter::EncryptedWord; }

	void decrypt() {
		Encrypter::DecryptedWord.clear();
		for (char c : Encrypter::EncryptedWord) Encrypter::DecryptedWord.push_back((char)(c - Encrypter::Key));
	}
	std::string getDecryptedWord() { return Encrypter::DecryptedWord; }

	void setKey(int key) { Encrypter::Key = key; }
	void setPlainWord(std::string plainWord) { Encrypter::PlainWord = plainWord; }

	~Encrypter() {}
};

class GameHandler {
private:
	int NumberOfCategories, Played, Won, Lost, Hints;
	std::string FullPath, InitPath;
	bool IsWon, HintsAllowed, EvaluateHintsUsed;
	void initNumberOfFiles(std::string& path) {
		int counter = 0;
		for (const auto& entry : std::filesystem::directory_iterator(path)) counter++;
		GameHandler::NumberOfCategories = counter;
	}
public:
	GameHandler(std::string filePath) {
		GameHandler::FullPath = filePath;
		GameHandler::initNumberOfFiles(GameHandler::FullPath);
		GameHandler::Played = GameHandler::Won = GameHandler::Lost = GameHandler::Hints = 0;
	}

	void setNumberOfCategories(int num) { GameHandler::NumberOfCategories = num; }
	int getNumberOfCategories() {
		GameHandler::initNumberOfFiles(GameHandler::FullPath);
		return GameHandler::NumberOfCategories;
	}

	void setNumberOfGames(int played) { GameHandler::Played = played; }
	int getNumberOfGames() { return GameHandler::Played; }

	void setNumberOfWon(int won) { GameHandler::Won = won; }
	int getNumberOfWon() { return GameHandler::Won; }

	void setNumberOfLost(int lost) { GameHandler::Lost = lost; }
	int getNumberOfLost() { return GameHandler::Lost; }

	void setNumberOfHints(int hints) { GameHandler::Hints = hints; }
	int getNumberOfHints() {
		if (GameHandler::Hints < 3) return GameHandler::Hints;
		else return 3;
	}

	void setFullPath(std::string fullPath) { GameHandler::FullPath = fullPath; }
	std::string getFullPath() { return GameHandler::FullPath; }

	void setInitPath(std::string initPath) { GameHandler::InitPath = initPath; }
	std::string getInitPath() { return GameHandler::InitPath; }

	void setIsWon(bool state) { GameHandler::IsWon = state; }
	bool getIsWon() { return GameHandler::IsWon; }

	void setHintsAllowed(bool state) { GameHandler::HintsAllowed = state; }
	bool getHintsAllowed() { return GameHandler::HintsAllowed; }

	void setEvaluateHintsUsed(bool state) { GameHandler::EvaluateHintsUsed = state; }
	bool getEvaluateHintsUsed() { return GameHandler::EvaluateHintsUsed; }

	~GameHandler() { GameHandler::FullPath.clear(); GameHandler::InitPath.clear(); }
};
extern GameHandler gh;

class Hangman {
private:
	int State;
	void drawHangman() {
		switch (State) {
		case 0: std::cout << " _____ \n| /   |\n|/     \n|      \n|      \n|______" << std::endl;
			break;
		case 1: std::cout << " _____ \n| /   |\n|/    O\n|      \n|      \n|______" << std::endl;
			break;
		case 2: std::cout << " _____  \n| /   | \n|/  \\ O\n|       \n|       \n|______ " << std::endl;
			break;
		case 3: std::cout << " _____    \n| /   |   \n|/  \\ O /\n|         \n|         \n|______   " << std::endl;
			break;
		case 4: std::cout << " _____    \n| /   |   \n|/  \\ O /\n|     |   \n|         \n|______   " << std::endl;
			break;
		case 5: std::cout << " _____    \n| /   |   \n|/  \\ O /\n|     |   \n|    /    \n|______   " << std::endl;
			break;
		case 6: std::cout << " _____    \n| /   |   \n|/  \\ O /  YOU HAVE\n|     |    BEEN HANGED.\n|    / \\   YOU LOST!\n|______   " << std::endl;
			break;
		default: break;
		}
	}
public:
	Hangman() {
		State = 0;
	}

	void hangSomeone() { Hangman::drawHangman(); }

	void setState(int state) { Hangman::State = state; }
	int getState() { return Hangman::State; }

	~Hangman() {}
};

class User {
private:
	std::string Username;
	bool IsSet;
	int Played, Won, Lost;
public:
	User() {
		User::IsSet = false;
		User::Played = User::Won = User::Lost = 0;
	}

	void setCurrentUser(std::string uName) { User::Username = uName; }
	std::string getCurrentUser() { return User::Username; }

	void setIsSet(bool state) { User::IsSet = state; }
	bool getIsSet() { return User::IsSet; }

	~User() {}
};
extern User currUser;

class WordHandler {
private:
	std::string GuessedLetters, Secret;
	int Index, Size;
	char* GuessingWord;
public:
	WordHandler() {
		WordHandler::GuessingWord = NULL;
		WordHandler::Index = WordHandler::Size = 0;
	}

	void setGuessedLetter(char letter) { WordHandler::GuessedLetters.push_back(letter); }
	void reSetGuessedLetter() { WordHandler::GuessedLetters.clear(); }
	std::string getGuessedLetters() { return WordHandler::GuessedLetters; }

	void setSecret(std::string secret) { WordHandler::Secret = secret; }
	void reSetSecret() { WordHandler::Secret.clear(); }
	std::string getSecret() { return WordHandler::Secret; }

	void setIndex(int indx) { WordHandler::Index = indx; };
	int getIndex() { return WordHandler::Index; }

	void setSize(int size) { WordHandler::Size = size; }
	int getSize() { return WordHandler::Size; }

	void setValuesToGuessingWord(char value, int position) { WordHandler::GuessingWord[position] = value; }
	char getValuesOfGuessingWord(int position) { return WordHandler::GuessingWord[position]; }

	void deleteArray(char* arr) { delete[] arr; }

	void setGuessingWord(char* arr) { WordHandler::GuessingWord = arr; }
	char* getGuessingWord(void) { return WordHandler::GuessingWord; }

	char getGuessingWordLetterOnIndex(int index) { return WordHandler::GuessingWord[index]; }

	~WordHandler() {}
};
extern WordHandler handler;

#endif // CLASSES_H