/*

Napisati MPI program koji korišćenjem Point-to-Point komunikacije vrši slanje
jednog podatka iz procesa 0 svim ostalim procesima u komunikatoru
(broj procesa je stepen dvojke).

Procesi nakon primanja prikazuju dobijene vrednosti.

Procesi su uređeni u stablo, komunikacija se odvija kao što je prikazano na
slici, za broj procesa=8.

Napisati grupnu operaciju koja bi zamenila prethodno opisanu komunikaciju.

*/

#include <mpi.h>
#include <stdio.h>
#include <math.h>

// size = p (stepen dvojke)
// Windows: mpicc oktobar_a.c -o oktobar_a.exe && mpiexec -n 8 oktobar_a.exe
// Linux:   mpicc oktobar_a.c -o oktobar_a && mpirun -np 8 ./oktobar_a

// Grupna operacija koja zamenjuje ovu komunikaciju:
// MPI_Bcast(&data, 1, MPI_INT, root, MPI_COMM_WORLD);

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
        printf("Proces %d primio vrednost %d. \n", rank, data);

    MPI_Finalize();
    return 0;
}
