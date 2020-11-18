#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <thread>
#include <vector>
int *A, *B;
std::vector<int> str;

void Add(int i)
{
    str.push_back(i);
}

void Thread(int iThread, int iTN, int *Arr, int *Brr, int size)
{
    for (int i = iThread; i < size; i += iTN)
    {
        int a = Arr[i];
        int b = Brr[i];
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
//        std::string out = std::to_string(i) + " " + std::to_string(Arr[i]) + " " + std::to_string(Brr[i]) + " " + std::to_string(a) + "\n";
//        std::cout << out;
        if(a == 1)
        {
            std::thread th(Add, i);
            th.join();
        }
    }
}

int main()
{
    unsigned int seed = 84;
    srand(seed);
    int size_of_array = (int)(rand() % 1000) + 1000;
    std::cout << "size = " << size_of_array << "\n";
    int X[size_of_array], Y[size_of_array];
    A = X;
    B = Y;
    for (int i = 0; i < size_of_array+5; ++i)
    {
        A[i] = (int)(rand() % 10000);
        B[i] = (int)(rand() % 10000);
    }
    int n = 4;
    std::cout << n << " concurrent threads are supported.\n";
    std::cout << "Enter the number of threads n (n >= 1 && n <=" << n << ")\n";
    std::cin >> n;
    if (n < 1 || n > 4)
        n = 4;
    std::thread *thr[n];
    for (int i = 0; i < n; i++)
    {
        thr[i] = new std::thread{Thread, i, n, A, B, size_of_array};
    }

    for (int i = 0; i < n; ++i)
    {
        thr[i]->join();
    }
    std::cout << str.size() << "\n";
    for (int i = 0; i < str.size(); ++i)
    {
        std::cout << str[i] << "\t";
    }
    return 0;
}
