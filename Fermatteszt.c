#include <stdio.h>

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

// Fermat-prímteszt
int fermatTest(unsigned long long n, int iterations) {
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }

    for (int i = 0; i < iterations; i++) {
        unsigned long long a = 2 + rand() % (n - 3);
        unsigned long long result = fastModExp(a, n - 1, n);

        if (result != 1) {
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

    if (fermatTest(number, iterations)) {
        printf("%llu primszam.\n", number);
    }
    else {
        printf("%llu nem primszam.\n", number);
    }

    return 0;
}
