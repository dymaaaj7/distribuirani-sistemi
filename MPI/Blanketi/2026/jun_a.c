/*
Napisati MPI program koji racuna vrednost izraza za niz a:

         N
         Σ (ā + aᵢ)        ā = (1/N) Σ aᵢ   (srednja vrednost niza a)
        i=1
R = ─────────────
        b + c

Niz je inicijalizovan u root procesu, a svaki proces od root procesa dobija
jednak deo elemenata niza.

Vrednosti b i c su inicijalizovane u procesu koji sadrzi maksimalni element
niza a, nakon distribuiranja elemenata niza po procesima.

Svaki proces ucestvuje u generisanju rezultata.

Rezultat prikazati u procesu koji sadrzi najmanji broj prostih brojeva nakon
distribuiranja niza a po procesima.

Zadatak resiti koriscenjem grupnih operacija.
*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 8
#define k 2

// size = N/k

// Windows: mpicc jun_a.c -o jun_a.exe && mpiexec -n 4 jun_a.exe
// Linux:   mpicc jun_a.c -o jun_a && mpirun -np 4 ./jun_a

int is_prime(int n)
{
    if (n < 2)
        return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[N], local_a[k];

    int local_sum = 0, sum_elem = 0;
    double avg;                   // srednja vrednost
    double local_part, total = 0; // doprinos sumi iznad razlomka i ukupna suma
    int bc[2];                    // bc[0] = b, bc[1] = c;
    double R;                     // konacni rezultat

    struct
    {
        int value;
        int rank;
    } in_max, out_max; // za proces sa maksimalnim elementom niza
    struct
    {
        int value;
        int rank;
    } in_min, out_min; // za najmanji broj prostih brojeva

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        for (int i = 0; i < N; i++)
            a[i] = i + 1;
    }

    MPI_Scatter(a, k, MPI_INT, local_a, k, MPI_INT, root, MPI_COMM_WORLD);

    in_max.value = local_a[0];
    in_max.rank = rank;
    in_min.value = 0;
    in_min.rank = rank;

    for (int i = 0; i < k; i++)
    {
        local_sum += local_a[i];
        if (local_a[i] > in_max.value)
            in_max.value = local_a[i];
        if (is_prime(local_a[i]))
            in_min.value++;
    }

    MPI_Reduce(&local_sum, &sum_elem, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Bcast(&sum_elem, 1, MPI_INT, root, MPI_COMM_WORLD);
    avg = (double)sum_elem / N;

    MPI_Reduce(&in_max, &out_max, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out_max, 1, MPI_2INT, root, MPI_COMM_WORLD);

    if (rank == out_max.rank)
    {
        bc[0] = 10;
        bc[1] = 5;
    }

    MPI_Bcast(bc, 2, MPI_INT, out_max.rank, MPI_COMM_WORLD);

    MPI_Reduce(&in_min, &out_min, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out_min, 1, MPI_2INT, root, MPI_COMM_WORLD);

    local_part = local_sum + k * avg;
    MPI_Reduce(&local_part, &total, 1, MPI_DOUBLE, MPI_SUM, out_min.rank, MPI_COMM_WORLD);

    if (rank == out_min.rank)
    {
        R = total / (bc[0] + bc[1]);
        printf("Proces %d (najmanje prostih: %d)\n", out_min.rank, out_min.value);
        printf("Σ(ā+aᵢ) = %.2f, ā = %.2f\n", total, avg);
        printf("R = %.2f / %d = %.2f\n", total, bc[0] + bc[1], R);
    }

    MPI_Finalize();
    return 0;
}
