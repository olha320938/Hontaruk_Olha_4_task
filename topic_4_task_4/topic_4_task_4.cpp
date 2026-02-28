
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>
using namespace std;

int countWords(string sentence) {
	stringstream ss(sentence);
	string word;
	int count = 0;
	while (ss >> word) {
		count++;
	}
	return count;
}

int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	ifstream inputFile("input.txt");
	ofstream outputFile("output.txt");

	if (!inputFile.is_open()) {
		cout << "Помилка! Не вдалося відкрити файл input.txt" << endl;
		return 1;
	}
	string currentLine;
	string longestSentence = "";
	int maxWords = 0;

	cout << "Аналіз тексту..." << endl;

	while (getline(inputFile, currentLine)) {
		if (currentLine.empty()) continue;

		int wordsInCurrent = countWords(currentLine);
		cout << "Речення: \"" << currentLine << "\" — Слів: " << wordsInCurrent << endl;

		if (wordsInCurrent > maxWords) {
			maxWords = wordsInCurrent;
			longestSentence = currentLine;
		}
	}
	if (maxWords > 0) {
		outputFile << "Найдовше речення (кількість слів: " << maxWords << "):\n";
		outputFile << longestSentence << endl;
		cout << "\nРезультат записано в output.txt" << endl;
	}
	else {
		cout << "Файл порожній." << endl;
	}

	inputFile.close();
	outputFile.close();

	return 0;
}

