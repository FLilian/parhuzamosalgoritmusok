#include <iostream>
#include <gmp.h>
#include <omp.h>

// Miller-Rabin prímteszt
bool millerRabinTest(const mpz_t n, const unsigned int k, const unsigned int numThreads, bool& isPrime) {
    if (mpz_cmp_ui(n, 2) < 0)
        return false;

    // n-1 = 2^r * d, d páratlan
    mpz_t d, n_minus_one;
    mpz_init(d);
    mpz_init(n_minus_one);
    mpz_sub_ui(n_minus_one, n, 1);

    unsigned int r = 0;
    mpz_set(d, n_minus_one);
    while (mpz_even_p(d)) {
        mpz_divexact_ui(d, d, 2);
        r++;
    }

    // Prímteszt eredménye a szálakon belül
    bool localIsPrime = true;

    // k iteráció
    #pragma omp parallel for reduction(&&:localIsPrime) num_threads(numThreads)
    for (unsigned int i = 0; i < k; i++) {
        // Random alap
        gmp_randstate_t state;
        gmp_randinit_default(state);
        gmp_randseed_ui(state, omp_get_thread_num() + 1);

        // Véletlenszerű alap a [2, n-2] intervallumból
        mpz_t a;
        mpz_init(a);
        mpz_urandomm(a, state, n_minus_one);
        mpz_add_ui(a, a, 2);

        // a^d mod n kiszámítása
        mpz_t x;
        mpz_init(x);
        mpz_powm(x, a, d, n);

        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_minus_one) == 0) {
            // Prím valószínűséggel
            mpz_clears(a, x, NULL);
            gmp_randclear(state);
            continue;
        }

        bool isComposite = true;
        for (unsigned int j = 1; j < r; j++) {
            // x^2 mod n kiszámítása
            mpz_powm_ui(x, x, 2, n);

            if (mpz_cmp_ui(x, 1) == 0) {
                // Samara prím
                isComposite = true;
                break;
            }

            if (mpz_cmp(x, n_minus_one) == 0) {
                // Prím valószínűséggel
                isComposite = false;
                break;
            }
        }

        if (isComposite) {
            // Kompozit
            localIsPrime = false;
        }

        mpz_clears(a, x, NULL);
        gmp_randclear(state);
    }

    isPrime = localIsPrime;

    mpz_clear(d);
    mpz_clear(n_minus_one);

    return true;
}

int main() {
    // Szám bekérése a felhasználótól
    mpz_t n;
    mpz_init(n);

    std::string numberStr;
    std::cout << "Kérem, adja meg a számot: ";
    std::cin >> numberStr;

    if (mpz_set_str(n, numberStr.c_str(), 10) == -1) {
        std::cout << "Hibás számformátum!" << std::endl;
        mpz_clear(n);
        return 1;
    }

    // Iterációk száma
    unsigned int k;
    std::cout << "Kérem, adja meg az iterációk számát: ";
    std::cin >> k;

    // Szálak száma
    unsigned int numThreads;
    std::cout << "Kérem, adja meg a szálak számát: ";
    std::cin >> numThreads;

    // Prímteszt
    bool isPrime;
    millerRabinTest(n, k, numThreads, isPrime);

    // Eredmény kiírása
    if (isPrime)
        std::cout << "A megadott szám prím." << std::endl;
    else
        std::cout << "A megadott szám összetett." << std::endl;

    mpz_clear(n);
    return 0;
}
