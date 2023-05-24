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

// Miller-Rabin prímszámteszt
int millerRabin(unsigned long long n, int k) {
    if (n <= 1) {
        return 0;
    }
    if (n <= 3) {
        return 1;
    }
    if (n % 2 == 0) {
        return 0;
    }

    unsigned long long r = 0;
    unsigned long long d = n - 1;
    while (d % 2 == 0) {
        r++;
        d /= 2;
    }

    for (int i = 0; i < k; i++) {
        unsigned long long a = 2 + rand() % (n - 3);
        unsigned long long x = fastModExp(a, d, n);

        if (x == 1 || x == n - 1) {
            continue;
        }

        int prime = 0;
        for (unsigned long long j = 0; j < r - 1; j++) {
            x = fastModExp(x, 2, n);
            if (x == n - 1) {
                prime = 1;
                break;
            }
        }

        if (prime == 0) {
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

    if (millerRabin(number, iterations)) {
        printf("%llu primszam.\n", number);
    }
    else {
        printf("%llu nem primszam.\n", number);
    }

    return 0;
}
