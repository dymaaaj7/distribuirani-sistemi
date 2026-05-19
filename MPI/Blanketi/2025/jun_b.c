/*

Koriscenjem grupne operacije izvrsiti slanje n podataka niza X svim procesima,
gde se elementi niza inciijalizuju u procesu sa rankom 2.

Koriscenjem jos jedne grupne operacije kreirati niz Y, yi=(p(p+1)/2)*xi, i=0,n-1
(p - ukupan broj procesa).

Izmedju dve grupne operacije nije dozvoljena jos jedna naredba.

*/
#include <mpi.h>
#include <stdio.h>

#define n 8

int main(int argc, char *argv[])
{
    int rank, size, root = 2;
    int X[n], Y[n];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
        for (int i = 0; i < n; i++)
            X[i] = i + 1;

    MPI_Bcast(X, n, MPI_INT, root, MPI_COMM_WORLD);

    int local_y[n];
    for (int i = 0; i < n; i++)
        local_y[i] = (rank * (rank + 1) / 2) * X[i];

    MPI_Reduce(local_y, Y, n, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}