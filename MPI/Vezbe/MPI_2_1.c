/*

Napisati MPI program koji nalazi minimalnu i maximalnu vrednost zadate
promenljive za N procesa kao i identifikatore procesa koji sadrže te vrednosti.

*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    struct
    {
        double value;
        int rank;
    } in, out;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    in.value = rank + 1;
    in.rank = rank;

    // Trazenje procesa minimalne vrednosti
    MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("Minmum value %lf is at process %d.\n", out.value, out.rank);

    // Trazenje procesa maksimalne vrednosti
    MPI_Reduce(&in, &out, 1, MPI_DOUBLE_INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("Maximum value %lf is at process %d.\n", out.value, out.rank);

    MPI_Finalize();
    return 0;
}