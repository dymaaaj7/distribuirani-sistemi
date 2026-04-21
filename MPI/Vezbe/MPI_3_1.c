/*

Napisati MPI program koji izračunava vrednost skalarnog proizvoda dva vektora
dimenzije N.

Pretpostaviti da je N deljivo sa brojem procesa.

Vrednosti vektora a i b se učitavaju u procesu P0.

*/

/*

skalarni proizvod
sum=a0*b0+a1*b1+a2*b2+...+a(n-1)*b(n-1)

ako imamo 4 procesa, niz je 8 elemenata, pa po 2 elementa idu u 1 proces
P0=a0*b0+a1*b1
P1=a2*b2+a3*b3
...
P3=a6*b6+a7*b7

*/
#include <mpi.h>
#include <stdio.h>

#define N 8

int main(int argc, char *argv[])
{
    // Inicijalizacija i postavka zadatka
    int rank, size, n_bar, local_sum, sum, a[N], b[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    n_bar = N / size;

    int *local_a = (int *)malloc(n_bar * sizeof(int));
    int *local_b = (int *)malloc(n_bar * sizeof(int));

    // Ucitavanje prvog niza
    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
            scanf("%d", &a[i]);
    }

    // Slanje svakom procesu N/size elemanta niza a
    MPI_Scatter(a, n_bar, MPI_INT, local_a, n_bar, MPI_INT, 0, MPI_COMM_WORLD);

    // Ucitavanje drugog niza
    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
            scanf("%d", &b[i]);
    }

    // Slanje svakom procesu N/size elemanta niza b
    MPI_Scatter(b, n_bar, MPI_INT, local_b, n_bar, MPI_INT, 0, MPI_COMM_WORLD);

    // Racunanje lokalne sume, npr P0=a0*b0+a1*b1, P1=a2*b2+a3*b3
    local_sum = 0;
    for (int i = 0; i < n_bar; i++)
        local_sum += local_sum + (a[i] * b[i]);

    // Racunanje ukupne sume
    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    printf("Total sum is %d", sum);

    MPI_Finalize();
    return 0;
}
