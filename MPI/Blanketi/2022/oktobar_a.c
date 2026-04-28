/*

Napisati MPI program koji korišćenjem Point-to-Point komunikacije vrši slanje
jednog podatka iz procesa 0 svim ostalim procesima u komunikatoru (broj procesa
je stepen dvojke).

Procesi nakon primanja prikazuju dobijene vrednosti.

Procesi su uređeni u stablo, komunikacija se odvija kao što je prikazano na
slici, za broj procesa=8.

0 -(korak 1)-> 1
0 -(korak 2)-> 2
1 -(korak 2)-> 3
0 -(korak 3)-> 4
1 -(korak 3)-> 5
2 -(korak 3)-> 6
3 -(korak 3)-> 7

*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int data = 42;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
        data = 42;

    int steps = (int)log2(size);

    for (int i = 1; i <= steps; i++)
    {
        int half = 1 << (i - 1);
        if (rank < half)
            MPI_Send(&data, 1, MPI_INT, rank + half, 0, MPI_COMM_WORLD);
        else if (rank < 2 * half)
            MPI_Recv(&data, 1, MPI_INT, rank - half, 0, MPI_COMM_WORLD, &status);
    }

    if (rank != 0)
        printf("Proces %d primio vrednost: %d\n", rank, data);

    MPI_Finalize();
    return 0;
}