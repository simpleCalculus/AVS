#include <omp.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <string>
#include <vector>

const int SIZE = 1500;

using namespace std;

int main() {
    std::cout << "size of arrays " << SIZE << "\n";
    int X[SIZE + 5], Y[SIZE + 5]; // создаем массивы для A и B
    std::vector<bool> ans(SIZE + 5); // Массив ответов
    unsigned int seed = 55; // начальная значение для генератора случайных чисел
    srand(seed);
    
    // Заполняем массивы случайнимы числами
    for (int j = 0; j < SIZE; ++j)
    {
        X[j] = (int)(rand() % 10000);
        Y[j] = (int)(rand() % 10000);
    }

    cout << "Enter the number of threads ";
    int i, nthreads;
    std::cin >> nthreads; // количество желаемых потоков
    if (nthreads <= 0) // Если пользователь ввел отрицательно число, количество потоков булет равно 4
        nthreads = 4;
    clock_t clock_timer = clock();
    double wall_timer = omp_get_wtime();
    omp_set_num_threads(nthreads);
    #pragma omp parallel for
    for (i = 0; i < SIZE; i++)
    {
        int a = X[i];
        int b = Y[i];
        // Пока a != b, вычтем из большого маленького
        while (a != b)
        {
            if (a > b)
            {
                long tmp = a;
                a = b;
                b = tmp;
            }
            b = b - a;
        }
        ans[i] = (b == 1); // Если b == 1, это означает что gcd(X[i], Y[i]) = 1
    }
    std::cout << " time on clock(): " <<
        (double)((double)clock() - clock_timer) / CLOCKS_PER_SEC
        << " time on wall: " << omp_get_wtime() - wall_timer << "\n";

    for (i = 0; i < SIZE; ++i)
    {
        // Выводим все индксы i для которых НОД(X[i], Y[i]) = 1 
        if (ans[i])
            std::cout << "For i = " << i << "  gcd(" << X[i] << "," << Y[i] << ") = 1" << "\n";
    }
    return 0;
}
