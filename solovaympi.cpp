#include <iostream>
#include <gmp.h>
#include <gmpxx.h>
#include <mpi.h>

// Segédfüggvény a modulo hatványozás gyors számításához
mpz_class modulo_power(mpz_class base, mpz_class exponent, mpz_class modulus) {
    mpz_class result;
    mpz_powm(result.get_mpz_t(), base.get_mpz_t(), exponent.get_mpz_t(), modulus.get_mpz_t());
    return result;
}

// Segédfüggvény a Jacobi szimbólum kiszámításához
int jacobi(mpz_class a, mpz_class n) {
    int j = 1;
    if (a < 0) {
        a = -a;
        if (n % 4 == 3)
            j = -j;
    }
    while (a != 0) {
        while (a % 2 == 0) {
            a /= 2;
            if (n % 8 == 3 || n % 8 == 5)
                j = -j;
        }
        mpz_class temp = a;
        a = n;
        n = temp;

        if (a % 4 == 3 && n % 4 == 3)
            j = -j;
        a %= n;
    }
    if (n == 1)
        return j;
    return 0;
}

// Solovay-Strassen prímteszt
bool solovay_strassen(mpz_class n, int iterations, int rank, int size) {
    if (n == 2 || n == 3)
        return true;
    if (n <= 1 || n % 2 == 0)
        return false;

    mpz_class n_minus_1 = n - 1;
    bool is_prime = true;

    for (int i = 0; i < iterations; i++) {
        mpz_class a;
        gmp_randstate_t state;
        gmp_randinit_default(state);
        gmp_randseed_ui(state, rank + 1);
        mpz_urandomm(a.get_mpz_t(), state, n_minus_1.get_mpz_t());
        mpz_class x = modulo_power(a, (n_minus_1) / 2, n);

        if (x != 1 && x != n_minus_1) {
            mpz_class j = jacobi(a, n);
            mpz_class y = modulo_power(j, (n - 1) / 2, n);
            if (y != n_minus_1) {
                is_prime = false;
                break;
            }
        }
    }

    int global_is_prime;
    MPI_Reduce(&is_prime, &global_is_prime, 1, MPI_INT, MPI_LAND, 0, MPI_COMM_WORLD);

    return global_is_prime;
}

int main(int argc, char** argv) {
    mpz_class number;
    int iterations;
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        std::cout << "Enter the number to test: ";
        std::cin >> number;
        std::cout << "Enter the number of iterations: ";
        std::cin >> iterations;
    }

    MPI_Bcast(number.get_mpz_t(), sizeof(mpz_t), MPI_BYTE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&iterations, 1, MPI_INT, 0, MPI_COMM_WORLD);

    bool is_prime = solovay_strassen(number, iterations, rank, size);

    if (rank == 0) {
        if (is_prime)
            std::cout << number << " is likely prime." << std::endl;
        else
            std::cout << number << " is not prime." << std::endl;
    }

    MPI_Finalize();

    return 0;
}

