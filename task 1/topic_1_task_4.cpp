#include <iostream>
#include <ctime>
#include <chrono>
#include <future>
#include <iomanip>
#include <Windows.h>

using namespace std;
using namespace std::chrono;

// --- ДОПОМІЖНІ ФУНКЦІЇ ---

// Вивід масиву (якщо великий — скорочений)
void printArray(int arr[], int size) {
	if (size <= 20) {
		for (int i = 0; i < size; i++) cout << arr[i] << " ";
	}
	else {
		for (int i = 0; i < 3; i++) cout << arr[i] << " ";
		cout << "... ";
		for (int i = size - 3; i < size; i++) cout << arr[i] << " ";
	}
	cout << endl;
}

// Заповнення випадковими числами
void fillArray(int arr[], int size) {
	for (int i = 0; i < size; i++) {
		arr[i] = rand() % 100000;
	}
}

// Копіювання масиву
void copyArray(int src[], int dst[], int size) {
	for (int i = 0; i < size; i++) {
		dst[i] = src[i];
	}
}

// --- АЛГОРИТМИ СОРТУВАННЯ ---

// Сортування вибором: знаходимо мінімум і ставимо на місце
void selectionSort(int arr[], int size) {
	for (int i = 0; i < size - 1; i++) {
		int min_idx = i;
		for (int j = i + 1; j < size; j++) {
			if (arr[j] < arr[min_idx]) min_idx = j;
		}
		if (min_idx != i) {
			int temp = arr[i];
			arr[i] = arr[min_idx];
			arr[min_idx] = temp;
		}
	}
}

// Сортування вставкою: кожен елемент вставляємо на правильне місце
void insertionSort(int arr[], int size) {
	for (int i = 1; i < size; i++) {
		int key = arr[i];
		int j = i - 1;
		while (j >= 0 && arr[j] > key) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

// Сортування Шелла: вставка з кроком gap, що зменшується вдвічі
void shellSort(int arr[], int size) {
	for (int gap = size / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < size; i++) {
			int key = arr[i];
			int j = i;
			while (j >= gap && arr[j - gap] > key) {
				arr[j] = arr[j - gap];
				j -= gap;
			}
			arr[j] = key;
		}
	}
}

// --- ПОШУК ---

int binarySearch(int arr[], int size, int target) {
	int left = 0, right = size - 1;
	while (left <= right) {
		int mid = left + (right - left) / 2;
		if (arr[mid] == target) return mid;
		if (arr[mid] < target) left = mid + 1;
		else right = mid - 1;
	}
	return -1;
}

// --- ВИМІРЮВАННЯ ЧАСУ ---

// Приймає вказівник на функцію сортування — це простий спосіб передати
// "яке сортування використовувати" без складних шаблонів.
double measureTime(void (*sortFunc)(int[], int), int original[], int size) {
	int* arr = new int[size];
	copyArray(original, arr, size);
	auto start = high_resolution_clock::now();
	sortFunc(arr, size);
	auto end = high_resolution_clock::now();
	delete[] arr;
	return duration<double, milli>(end - start).count();
}

// --- АСИНХРОННИЙ ЗАПУСК ---

// Кожен метод отримує свою окрему копію масиву,
// щоб паралельне виконання було коректним
void runAsync(int original[], int size) {
	// Робимо три окремі копії — кожен потік працює зі своїм масивом
	int* copy1 = new int[size];
	int* copy2 = new int[size];
	int* copy3 = new int[size];
	copyArray(original, copy1, size);
	copyArray(original, copy2, size);
	copyArray(original, copy3, size);

	// Запускаємо три сортування паралельно (кожне у своєму потоці)
	auto f1 = async(launch::async, [copy1, size]() {
		auto start = high_resolution_clock::now();
		selectionSort(copy1, size);
		auto end = high_resolution_clock::now();
		return duration<double, milli>(end - start).count();
		});
	auto f2 = async(launch::async, [copy2, size]() {
		auto start = high_resolution_clock::now();
		insertionSort(copy2, size);
		auto end = high_resolution_clock::now();
		return duration<double, milli>(end - start).count();
		});
	auto f3 = async(launch::async, [copy3, size]() {
		auto start = high_resolution_clock::now();
		shellSort(copy3, size);
		auto end = high_resolution_clock::now();
		return duration<double, milli>(end - start).count();
		});

	double tSel = f1.get();
	double tIns = f2.get();
	double tShl = f3.get();

	delete[] copy1;
	delete[] copy2;
	delete[] copy3;

	cout << left << setw(10) << size << " | "
		<< setw(12) << fixed << setprecision(3) << tSel << " | "
		<< setw(12) << tIns << " | "
		<< setw(12) << tShl << endl;
}

int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);
	srand(time(0));

	int sizes[] = { 12, 125, 1500, 7000, 35000, 170000 };
	int numSizes = 6;

	// Функція для друку шапки таблиці
	auto printHeader = []() {
		cout << "\n" << left << setw(10) << "Розмір (n)" << " | "
			<< setw(12) << "Selection" << " | "
			<< setw(12) << "Insertion" << " | "
			<< setw(12) << "Shell (ms)" << endl;
		cout << string(55, '-') << endl;
		};

	// 1. Демонстрація
	cout << "=== ДЕМОНСТРАЦІЯ (n=12) ===" << endl;
	int demo[12];
	fillArray(demo, 12);
	cout << "До сортування: "; 
	printArray(demo, 12);

	shellSort(demo, 12);
	cout << "Після сортування: "; 
	printArray(demo, 12);

	int target;
	cout << "\nВведіть число для пошуку: "; 
	cin >> target;
	int result = binarySearch(demo, 12, target);
	if (result != -1) cout << "Знайдено на позиції: " << result << endl;
	else cout << "Число не знайдено." << endl;

	// 2. Синхронний режим
	cout << "\n--- СИНХРОННЕ СОРТУВАННЯ (5 запусків) ---" << endl;
	for (int r = 0; r < 5; r++) {
		cout << "\nЗапуск #" << r + 1;
		printHeader();
		for (int s = 0; s < numSizes; s++) {
			int n = sizes[s];
			int* original = new int[n];
			fillArray(original, n);

			double tSel = measureTime(selectionSort, original, n);
			double tIns = measureTime(insertionSort, original, n);
			double tShl = measureTime(shellSort, original, n);

			cout << left << setw(10) << n << " | "
				<< setw(12) << fixed << setprecision(3) << tSel << " | "
				<< setw(12) << tIns << " | "
				<< setw(12) << tShl << endl;

			delete[] original;
		}
	}

	// 3. Асинхронний режим
	cout << "\n--- АСИНХРОННЕ СОРТУВАННЯ (5 запусків) ---" << endl;
	for (int r = 0; r < 5; r++) {
		cout << "\nЗапуск #" << r + 1;
		printHeader();
		for (int s = 0; s < numSizes; s++) {
			int n = sizes[s];
			int* original = new int[n];
			fillArray(original, n);
			runAsync(original, n);
			delete[] original;
		}
	}

	return 0;
}