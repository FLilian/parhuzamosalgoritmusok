#include <stdio.h>
#include <stdlib.h>

// Gyors hatványozás moduláris aritmetikával
unsigned long long fastModExp(unsigned long long base, unsigned long long exp, unsigned long long mod) {
    unsigned long long result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp = exp / 2;
    }

    return result;
}

// Jacobi-szimbólum kiszámítása
int jacobiSymbol(unsigned long long a, unsigned long long n) {
    if (a == 0) {
        return 0;
    }
    if (a == 1) {
        return 1;
    }

    int result = 1;
    while (a != 0) {
        while (a % 2 == 0) {
            a /= 2;
            unsigned long long nmod8 = n % 8;
            if (nmod8 == 3 || nmod8 == 5) {
                result = -result;
            }
        }
        unsigned long long temp = a;
        a = n % temp;
        n = temp;
        if (a > n / 2) {
            a = a - n;
        }
        if (a % 4 == n % 4 && a % 4 == 3) {
            result = -result;
        }
    }

    if (n == 1) {
        return result;
    }
    return 0;
}

// Solovay-Strassen prímszámteszt
int solovayStrassen(unsigned long long n, int k) {
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }

    for (int i = 0; i < k; i++) {
        unsigned long long a = 2 + rand() % (n - 3);
        unsigned long long x = fastModExp(a, (n - 1) / 2, n);

        if (x != 1 && x != n - 1 && jacobiSymbol(a, n) != x % n) {
            return 0;
        }
    }

    return 1;
}

int main() {
    unsigned long long number;
    int iterations;
    printf("Adj meg egy szamot: ");
    scanf_s("%llu", &number);
    printf("Adj meg egy iteracios szamot: ");
    scanf_s("%d", &iterations);

    if (solovayStrassen(number, iterations)) {
        printf("%llu primszam.\n", number);
    }
    else {
        printf("%llu nem primszam.\n", number);
    }

    return 0;
}
