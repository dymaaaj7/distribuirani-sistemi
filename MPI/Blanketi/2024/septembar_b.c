/*

Koriscenjem grupne operacije izvrsiti slanje n podataka niza X svim procesima,
gde se podaci inicijalizuju u procesu sa rankom 2.

Koriscenjem jos jedne grupne operacije kreirati niz Y, yi=(p*(p+1)/2)*xi, gde je
i=0,n-1, a p ukupan broj procesa.

*/

#include <mpi.h>
#include <stdio.h>`
#include <stdlib.h>

#define n 8

// size = p

int main(int argc, char *argv[])
{
    int rank, size, root = 2;
    int x[n], local_y[n], y[n];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
        for (int i = 0; i < n; i++)
            x[i] = i + 1;

    MPI_Bcast(x, n, MPI_INT, root, MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
        local_y[i] = x[i] * (rank + 1);

    MPI_Reduce(local_y, y, n, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == root)
        for (int i = 0; i < n; i++)
            printf("%d ", y[i]);

    MPI_Finalize();
    return 0;
}
