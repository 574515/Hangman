#include "header.h"
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <vector>
#include <sstream>

int getCategoryIndex(Categories* usedCats, std::string& filePath) {
	int i;
	for (i = 0; i < gh.getNumberOfCategories(); i++) if (usedCats[i].getFilePath().compare(filePath) == 0) return i;
	return (-1);
}

std::string getCategoryName(const std::string& str) {
	std::size_t found, lastIndex;
	std::string rawName;
	unsigned int i;
	found = str.find_last_of("/\\");
	lastIndex = str.substr(found + 1).find_last_of(".");
	rawName = str.substr(found + 1).substr(0, lastIndex);
	for (i = 0; i < rawName.size(); i++) {
		if (rawName[i] == '_' || rawName[i] == '.') rawName[i] = ' ';
		if (i == 0 || isspace(rawName[i - 1])) rawName[i] = toupper(rawName[i]);
		else rawName[i] = tolower(rawName[i]);
	}
	return rawName;
}

int getCategorySize(std::string filePath) {
	std::ifstream filePtr(filePath);
	std::string word;
	int size = 0;
	while (std::getline(filePtr, word)) size++;
	filePtr.close();
	return size;
}

void getRandomWord(std::string& filePath, Categories* usedCats) {	
	std::vector<std::string> wordList;
	std::string getWord, word;
	int index = getCategoryIndex(usedCats, filePath);
	if (index != (-1)) handler.setIndex(index);
	std::ifstream filePtr(filePath);
	while (std::getline(filePtr, word)) wordList.push_back(word);
	filePtr.close();
	handler.setSecret(wordList[usedCats[handler.getIndex()].getValuesOfIndexArray(usedCats[handler.getIndex()].getSavedPosition())]);
	usedCats[handler.getIndex()].setSavedPosition(usedCats[handler.getIndex()].getSavedPosition() + 1);
}

void initArrayOfIndex(Categories cat) {	
	cat.setIndexArray(new int[cat.getSize()]);
	for (int j = 0; j < cat.getSize(); j++) cat.setValuesToIndexArray(j, j);
	randomizeArray(cat.getIndexArray(), cat.getSize());
}

void initSetCategories(Categories* cats) {
	int i, j, k = 0;
	for (const auto& entry : std::filesystem::directory_iterator(gh.getFullPath()))
		cats[k++] = Categories(getCategoryName(entry.path().string()), entry.path().string());
	for (i = 0; i < gh.getNumberOfCategories(); i++) {
		cats[i].setSize(getCategorySize(cats[i].getFilePath()));
		cats[i].setSavedPosition(0);
		cats[i].setPlayedGames(0);
		cats[i].setWonGames(0);
		cats[i].setLostGames(0);
		cats[i].setIndexArray(new int[cats[i].getSize()]);
		for (j = 0; j < cats[i].getSize(); j++) cats[i].setValuesToIndexArray(j, j);
		randomizeArray(cats[i].getIndexArray(), cats[i].getSize());
	}
}

void randomizeArray(int* arr, int n) {
	int i, temp;
	for (i = n - 1; i > 0; i--) {
		temp = rand() % (i + 1);
		swap(&arr[i], &arr[temp]);
	}
}

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}