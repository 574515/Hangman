#include "header.h"

WordHandler handler;
User currUser;

void adminPanel(Admin admin, Categories* cats) {
	int choice;
	while (true) {
		do {
			clearScreen();
			std::cout << "\tLogged as: " << RED << "[" << admin.getUsername() << "]" << RES << std::endl;
			std::cout << "\n[1] List categories\n[2] Add category\n[3] Delete category" << std::endl;
			std::cout << "[4] Print current words path\n[5] Change words path\n[6] Log off\n\nChoice: ";
			std::cin >> choice;
		} while (choice < 1 || choice > 6);
		switch (choice) {
		case 1: {
			printCategories(cats);
			break;
		}
		case 2: {
			addNewCategory();
			break;
		}
		case 3: {
			removeCategory();
			break;
		}
		case 4: {
			printCurrentPath();
			break;
		}
		case 5: {
			setNewPath();
			break;
		}
		case 6: {
			admin.~Admin();
			initialMenu();
			break;
		}
		}
	}
}

void initialMenu() {
	int i, choice;
	Categories* cats = new Categories[gh.getNumberOfCategories()];
	initSetCategories(cats);
	do {
		clearScreen();
		std::cout << "\n\tHANGMAN GAME\n\n[1] Play the game\n[2] Admin panel\n[3] Exit\n\nChoice: ";
		std::cin >> choice;
	} while (choice < 1 || choice > 3);
	switch (choice) {
	case 1: {
		setUser();
		play(cats);
		break;
	}
	case 2: {
		adminPanel(adminLogIn(), cats);
		break;
	}
	case 3: {
		if (gh.getFullPath().compare(gh.getInitPath()) == 0) {
			for (i = 0; i < gh.getNumberOfCategories(); i++) cats[i].deleteArray(cats[i].getIndexArray());
			delete[] cats;
		}
		gh.~GameHandler();
		_CrtDumpMemoryLeaks();
		exit(EXIT_SUCCESS);
	}
	}
}