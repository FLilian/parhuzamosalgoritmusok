#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct {
    int number;
    int iterations;
    bool isPrime;
} ThreadArgs;

void *fermatPrimeTest(void *arg) {
    ThreadArgs *args = (ThreadArgs *)arg;
    int n = args->number;
    int iterations = args->iterations;
    bool isPrime = true;

    for (int i = 0; i < iterations; i++) {
        int a = rand() % (n - 2) + 2;


        int result = 1;
        for (int j = 0; j < n - 1; j++) {
            result = (result * a) % n;
        }

        if (result != 1) {
            isPrime = false;
            break;
        }
    }

    args->isPrime = isPrime;
    pthread_exit(NULL);
}

int main() {
    int number, iterations, numThreads;

    printf_s('Adjon meg egy szamot: ');
    scanf('%d', &number);
    printf_s('Adjon meg egy iteracios szamot: ');
    scanf('%d', &iterations);
    printf_s('Adjon meg a szalak szamat: ');
    scanf('%d', &numThreads);

    pthread_t threads[numThreads];
    ThreadArgs args[numThreads];

    for (int i = 0; i < numThreads; i++) {
        args[i].number = number;
        args[i].iterations = iterations;
        pthread_create(&threads[i], NULL, fermatPrimeTest, (void *)&args[i]);
    }

    bool isPrime = true;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
        if (!args[i].isPrime) {
            isPrime = false;
            break;
        }
    }

    if (isPrime) {
        printf('%d prim.\n', number);
    } else {
        printf('%d nem prim.\n', number);
    }

    return 0;
}
