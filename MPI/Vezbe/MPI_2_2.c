/*

Svaki od N procesa sadrži 30 realnih brojeva.

Napisati MPI program koji nalazi maksimalnu vrednost na svakoj od 30 lokacija,
kao i identifikator procesa koji sadrži tu vrednost.

*/

#include <mpi.h>
#include <stdio.h>

#define N 30

int main(int argc, char *argv[])
{
    // Inicijalizacija i postavka zadatka
    struct
    {
        double value;
        int rank;
    } in[N], out[N];
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    for (int i = 0; i < N; i++)
    {
        in[i].value = i + 1;
        in[i].rank = rank;
    }

    // Traznje maksimalne vrednosti na svim lokacijama
    MPI_Reduce(in, out, N, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);

    // Stampa rezultata
    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
            printf("Max value at position %d is %lf at process %d",
                   i,
                   out[i].value,
                   out[i].rank);
    }

    MPI_Finalize();
    return 0;
}