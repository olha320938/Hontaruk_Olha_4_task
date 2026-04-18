#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>
using namespace std;
// підраховуємо кількість слів у реченні(крапка в кінці не рахується як слово)
int countWords(string sentence) {
	string temp = sentence;
	if (!temp.empty() && temp.back() == '.')
		temp.pop_back();
	stringstream ss(temp);// передаємо рядок без крапки
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
	// зчитуємо файл рядок за рядком(кожен рдок = одне речення)
	while (getline(inputFile, currentLine)) {
		if (currentLine.empty()) continue;

		int wordsInCurrent = countWords(currentLine);
		cout << "Речення: \"" << currentLine << "\" — Слів: " << wordsInCurrent << endl;
		// запам'ятовуємо речення з найбільшою кількістю слів
		if (wordsInCurrent > maxWords) {
			maxWords = wordsInCurrent;
			longestSentence = currentLine;
		}
	}
	// записуємо результат у вихідний файл
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

