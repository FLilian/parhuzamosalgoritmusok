// sajnos ez csak 10 számjegyig pontos utána már nem , de magábóla fermat féle teszt miatt nem lehetséges, ha nagyobb számot akarunk tesztelni akkor másik algoritmus kell hozzá
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Struktúra a szálak paramétereinek tárolásához
typedef struct {
    unsigned long long number;  // Tesztelendő szám
    int iterations;             // Iterációs szám
    int thread_id;              // Szál azonosítója
} ThreadData;

// Fermat-féle prímteszt függvény
int fermat_test(unsigned long long n, int k) {
    if (n <= 1)
        return 0;

    if (n <= 3)
        return 1;

    for (int i = 0; i < k; i++) {
        unsigned long long a = 2 + rand() % (n - 3);
        unsigned long long x = 1;
        unsigned long long power = n - 1;

        while (power > 0) {
            if (power & 1)
                x = (x * a) % n;
            a = (a * a) % n;
            power >>= 1;
        }

        if (x != 1)
            return 0;
    }

    return 1;
}

// Szál függvény a prímteszt végrehajtásához
void *thread_function(void *data) {
    ThreadData *thread_data = (ThreadData *)data;
    int result = fermat_test(thread_data->number, thread_data->iterations);

    printf("Szál %d: %llu %s prim.\n", thread_data->thread_id, thread_data->number, result ? "egy" : "nem");

    free(thread_data);
    pthread_exit(NULL);
}

int main() {
    unsigned long long number;
    int iterations, num_threads;

    printf("Tesztelendo szam: ");
    scanf("%llu", &number);

    printf("Iteracios szam: ");
    scanf("%d", &iterations);

    printf("Szalak szama: ");
    scanf("%d", &num_threads);

    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; i++) {
        ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
        data->number = number;
        data->iterations = iterations;
        data->thread_id = i;

        pthread_create(&threads[i], NULL, thread_function, (void *)data);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
