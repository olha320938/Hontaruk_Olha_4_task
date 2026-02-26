
#include <iostream>
#include <Windows.h>
#include <cmath>
using namespace std;

double factorial(int n)
{
    double result = 1;
    for (int i = 1; i <= n; i++)
    {
        result *= i;
    }
    return result;
}

double productArray(int arr[], int size)
{
    double product = 1;
    for (int i = 0; i < size; i++)
    {
        product *= arr[i];
    }
    return product;
}

int main()
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    int n = 3;
    int x[7] = { 5, 6, 7, 3, 9, 8, 4 };

    double num1 = factorial(5 * n - 4) + 8;
    double den1 = factorial(2 * n + 1);
    double part1 = num1 / den1;

    double totalP = productArray(x, 7);
    double num2 = factorial(n) + totalP;
    double den2 = pow(totalP, 1.0 / 3.0);
    double part2 = num2 / den2;

    double z = part1 + part2;
    cout << "Результат обчислення за формулою: Z =" << z << endl;
}
