#include <iostream>
#include <vector>
#include <cmath>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

// Miller-Rabin prímteszt
bool millerRabinTest(int number, int iterations) {
    if (number < 2)
        return false;

    if (number != 2 && number % 2 == 0)
        return false;

    int d = number - 1;
    while (d % 2 == 0)
        d /= 2;

    for (int i = 0; i < iterations; ++i) {
        int a = rand() % (number - 2) + 2;
        int x = static_cast<int>(std::pow(a, d)) % number;

        if (x == 1 || x == number - 1)
            continue;

        bool isProbablePrime = false;
        while (d != number - 1) {
            x = (x * x) % number;
            d *= 2;

            if (x == 1)
                return false;

            if (x == number - 1) {
                isProbablePrime = true;
                break;
            }
        }

        if (!isProbablePrime)
            return false;
    }

    return true;
}

int main() {
    std::vector<int> numbers = { 123457, 123459, 123461, 123463 };  // Tesztelendő számok
    int iterations = 10;  // Iterációk száma

    tbb::parallel_for(tbb::blocked_range<size_t>(0, numbers.size()), [&](const tbb::blocked_range<size_t>& range) {
        for (size_t i = range.begin(); i != range.end(); ++i) {
            bool isPrime = millerRabinTest(numbers[i], iterations);

            if (isPrime)
                std::cout << numbers[i] << " is prime." << std::endl;
            else
                std::cout << numbers[i] << " is not prime." << std::endl;
        }
        });

    return 0;
}
