#include <iostream>
#include <gmp.h>
#include <omp.h>

bool millerRabin(const mpz_t n, int k) {
     if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0)
        return true;
    if (mpz_cmp_ui(n, 1) <= 0 || mpz_even_p(n))
        return false;

    mpz_t s, d;
    mpz_init(s);
    mpz_init(d);
    mpz_sub_ui(d, n, 1);

    int r = 0;
    while (mpz_even_p(d)) {
        mpz_divexact_ui(d, d, 2);
        r++;
    }
    mpz_set_ui(s, r);

    mpz_t a, x, n_minus_one;
    mpz_init(a);
    mpz_init(x);
    mpz_init(n_minus_one);
    mpz_sub_ui(n_minus_one, n, 1);

    bool result = true;
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    for (int i = 0; i < k; i++) {
        mpz_urandomm(a, state, n_minus_one);
        mpz_add_ui(a, a, 1);

        mpz_powm(x, a, d, n);
        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_minus_one) == 0)
            continue;

        bool isComposite = true;
        for (int j = 0; j < r - 1; j++) {
            mpz_powm_ui(x, x, 2, n);
            if (mpz_cmp_ui(x, 1) == 0)
                return false;
            if (mpz_cmp(x, n_minus_one) == 0) {
                isComposite = false;
                break;
            }
        }
        if (isComposite) {
            result = false;
            break;
        }
    }

    mpz_clears(s, d, a, x, n_minus_one, NULL);
    gmp_randclear(state);

    return result;
}

int main() {
    mpz_t n;
    mpz_init(n);

    int k = 5; // Ismétlések száma (kisebb értékek is lehetnek, de a nagyobb értékek megbízhatóbb eredményt adnak)

    int numThreads = 4; // A kívánt párhuzamos szálak száma

    omp_set_num_threads(numThreads);

    std::cout << "Adjon meg egy szamot: ";
    std::string input;
    std::cin >> input;

    mpz_set_str(n, input.c_str(), 10);

    #pragma omp parallel
    {
        #pragma omp single nowait
        {
            if (millerRabin(n, k))
                std::cout << "A szam primszam." << std::endl;
            else
                std::cout << "A szam nem primszam." << std::endl;
        }
    }

    mpz_clear(n);

    return 0;
}

