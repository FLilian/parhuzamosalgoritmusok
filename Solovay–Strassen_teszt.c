#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>

// Segédfüggvény az a^b mod n számításához
int modularExponentiation(int a, int b, int n) {
    int result = 1;
    a = a % n;

    while (b > 0) {
        if (b & 1) {
            result = (result * a) % n;
        }

        a = (a * a) % n;
        b = b >> 1;
    }

    return result;
}

// Segédfüggvény az Euler phi függvény kiszámításához
int eulerPhi(int n) {
    int result = n;

    for (int p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            while (n % p == 0) {
                n = n / p;
            }

            result = result - (result / p);
        }
    }

    if (n > 1) {
        result = result - (result / n);
    }

    return result;
}

// Solovay-Strassen prímteszt
bool solovayStrassenTest(int number, int iterations) {
    if (number < 2) return false;
    if (number != 2 && number % 2 == 0) return false;

    for (int i = 0; i < iterations; ++i) {
        int a = rand() % (number - 2) + 2; // Véletlenszerű a választása [2, number-1] között

        int x = modularExponentiation(a, (number - 1) / 2, number);

        if (x != 1 && x != number - 1) {
            return false; // Nem prím
        }

        int jacobiSymbol = (x == 1) ? 1 : -1;

        if (jacobiSymbol != (modularExponentiation(a, (number - 1) / 2, number) % number)) {
            return false; // Nem prím
        }
    }

    return true; // Valószínűleg prím
}

int main() {
    int number = 100; // Tesztelendő szám
    int iterations = 10; // Iterációk száma

    bool isPrime = true;

#pragma omp parallel for shared(isPrime) num_threads(4)
    for (int i = 2; i <= number; ++i) {
        if (!solovayStrassenTest(i, iterations)) {
#pragma omp critical
            {
                isPrime = false;
            }
        }
    }

    if (isPrime) {
        printf("%d is prime.\n", number);
    }
    else {
        printf("%d is not prime.\n", number);
    }

    return 0;
}

