#include "header.h"
#include <stdio.h>

void addNewCategory() {
	std::string fileName;
	bool validate;
	do {
		std::cout << "Enter file name [without extension]: ";
		std::cin >> fileName;
		validate = isValid(fileName);
	} while (!validate);
	std::ofstream outFile(gh.getFullPath() + "\\" + fileName + ".txt");
	outFile.close();	
	return;
}

Admin adminLogIn() {
	Admin admin;
	Encrypter crypter(42);
	std::ifstream fPtr("admins.txt");
	if (!fPtr.good()) {
		clearScreen();
		std::cout << "This is first run. No " << RED << "admin" << RES << " account avaliable. Proceed to create one.";
		if (_getch() <= 0) exit(EXIT_FAILURE);
		admin = registerNewAdmin(crypter);
		crypter.~Encrypter();
		return admin;
	}
	else {
		std::string username, password;
		std::ifstream in("admins.txt");
		bool eval = false;
		in >> admin;
		in.close();
		do {
			username = getUsernameInput();
			eval = doesAdminExists(username);
			if (!eval) {
				std::cout << "\nNo admin with username " << RED << "[" << username << "]" << RES << "." << std::endl;
				if (_getch() <= 0) exit(EXIT_FAILURE);
			}
		} while (!eval);
		eval = false;
		do {
			clearScreen();
			std::cout << "Enter username: " << username << std::endl;
			std::cout << "Enter password: ";
			password = getPasswordInput();
			crypter.setPlainWord(password);
			crypter.encrypt();
			eval = isPassowrdGood(admin, crypter);
			if (!eval) {
				std::cout << "\nPassword for " << RED << "[" << username << "]" << RES << " is incorrect." << std::endl;
				if (_getch() <= 0) exit(EXIT_FAILURE);
			}
		} while (!eval);
		crypter.~Encrypter();
		return admin;
	}
}

void printCategories(Categories* cats) {
	clearScreen();
	int i;
	for (i = 0; i < gh.getNumberOfCategories(); i++) {
		std::cout << BLUE << "[Category no: " << (i + 1) << "]" << RES << std::endl;
		std::cout << BLUE << "Name: " << RES << cats[i].getName() << std::endl;
		std::cout << BLUE << "File path: " << RES << cats[i].getFilePath() << std::endl;
		std::cout << BLUE << "Size: " << RES << cats[i].getSize() << "\n" << std::endl;
	}
	std::cout << BLUE << "There is [" << gh.getNumberOfCategories() << "] cateogries total." << RES << std::endl;
	if (_getch() <= 0) exit(EXIT_FAILURE);
	return;
}

void printCurrentPath() {
	clearScreen();
	std::cout << "Currently set path: " << YLW << "[" << gh.getFullPath() << "]" << RES << "." << std::endl;
	if (_getch() <= 0) exit(EXIT_FAILURE);
	return;
}

void removeCategory() {
	std::string tempFileName, fileName, answer;
	bool eval;
	do {
		clearScreen();
		std::cout << "Enter file name [without extension]: ";
		std::cin >> tempFileName;
		eval = doesExistsInDirectory(tempFileName);
		if (!eval) {
			std::cout << "File named " << tempFileName << ".txt does not exist." << std::endl;
			if (_getch() <= 0) exit(EXIT_FAILURE);
		}
	} while (!eval);
	if (fileDeletionPrompt(tempFileName)) {
		for (char c : gh.getFullPath()) fileName.push_back(c);
		fileName.append("\\" + tempFileName + ".txt");
		std::remove(fileName.c_str()) == 0 ?
			std::cout << "\nFile " << RED << tempFileName << ".txt" << RES << " deleted!" << std::endl :
			std::cout << "\nError!" << std::endl;
	}
	else std::cout << "\nFile " << RED << tempFileName << ".txt" << RES << " will not be deleted." << std::endl;
	if (_getch() <= 0) exit(EXIT_FAILURE);
	return;
}

void setNewPath() {
	std::string stringPath;
	bool eval;
	do {
		clearScreen();
		std::cout << "Old path: " << YLW << "[" << gh.getFullPath() << "]" << RES << std::endl;
		std::cout << "Enter new path: ";
		std::cin >> stringPath;
		eval = doesDirExists(stringPath);
		if (!eval) {
			std::cout << "Directory does not exist." << std::endl;
			if (_getch() <= 0) exit(EXIT_FAILURE);
		}
	} while (!eval);	
	gh.setInitPath(gh.getFullPath());
	gh.setFullPath(stringPath);
	std::cout << "\nNew path set: " << GRN << "[" << gh.getFullPath() << "]" << RES << "." << std::endl;
	if (_getch() <= 0) exit(EXIT_FAILURE);
	return;
}