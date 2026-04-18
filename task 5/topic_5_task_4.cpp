#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using namespace std;
// структура для опису холодильника
struct Fridge {
	string name;
	double volume;
	string hasFreezer;
	string energyClass;
	double price;
};
// додавання нового холодильника у файл
void addFridge(string filename) {
	ofstream fout(filename, ios::app);
	if (!fout.is_open()) return;
	Fridge f;
	cin.ignore(10000, '\n');
	cout << "\n--- Додавання холодильника ---" << endl;

	cout << "1. Назва холодильника (одним словом): ";
	cin >> f.name;

	cout << "2. Об'єм (л): ";
	cin >> f.volume;

	cout << "3. Морозилка (є/немає): ";
	cin >> f.hasFreezer;

	cout << "4. Клас енергоефективності (В, A, A+ ...): ";
	cin >> f.energyClass;

	cout << "5. Ціна (грн): ";
	cin >> f.price;
	// записуємо холодильник у файл
	fout << f.name << " " << f.volume << " " << f.hasFreezer << " " << f.energyClass << " " << f.price << endl;
	fout.close();

	cout << "\nЗБЕРЕЖЕНО: " << f.name << " | Клас: " << f.energyClass << " | Ціна: " << f.price << " грн" << endl;
}
// виведення всіх холодильників з файлу
void showAll(string filename) {
	ifstream fin(filename);
	if (!fin.is_open()) {
		cout << "База порожня." << endl;
		return;
	}
	Fridge f;
	bool found = false;
	cout << "\n--- СПИСОК УСІХ ХОЛОДИЛЬНИКІВ ---" << endl;
	while (fin >> f.name >> f.volume >> f.hasFreezer >> f.energyClass >> f.price) {
		cout << "Назва: " << f.name << " | Клас: " << f.energyClass << " | Ціна: " << f.price << " грн" << endl;
		found = true;
	}
	if (!found) cout << "Записи не знайдені або пошкоджені." << endl;
	fin.close();
}
// показати холодильники з класом В і кращі
void showHighEfficiency(string filename) {
	ifstream fin(filename);
	if (!fin.is_open()) return;
	Fridge f;
	bool found = false;
	cout << "\n--- ХОЛОДИЛЬНИКИ КЛАСУ B та кращі(A, A+...) ---" << endl;
	while (fin >> f.name >> f.volume >> f.hasFreezer >> f.energyClass >> f.price) {
		if (f.energyClass == "A" ||  f.energyClass == "B" ||
			f.energyClass == "A+" || f.energyClass == "A++" ||
			f.energyClass == "A+++") {
			cout << "Модель: " << f.name << " | Клас: " << f.energyClass << " | Ціна: " << f.price << " грн" << endl;
			found = true;
		}
	}
	if (!found) cout << "Таких холодильників не знайдено." << endl;
	fin.close();
}

int main()
{
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	string filename = "fridges_db.txt";
	int choice;

	while (true) {
		cout << "\n--- МЕНЮ ---" << endl;
		cout << "1. Додати холодильник" << endl;
		cout << "2. Показати всі" << endl;
		cout << "3. Показати найкращі (B і кращі)" << endl;
		cout << "0. Вихід" << endl;
		cout << "Ваш вибір: ";
		// перевірка коректності введення
		if (!(cin >> choice)) {
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Помилка! Введіть цифру." << endl;
			continue;
		}

		if (choice == 0) break;

		switch (choice) {
		case 1:
			addFridge(filename);
			break;
		case 2:
			showAll(filename);
			break;
		case 3:
			showHighEfficiency(filename);
			break;
		default:
			cout << "Немає такого пункту." << endl;
			break;
		}
	}
	return 0;
}