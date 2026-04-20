/*

Napisati program koji nalazi sumu n celih brojeva korišćenjem ptp komunikacije
tako da svaki proces učestvuje u sumiranju.

*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size, sum;
    MPI_Status stat;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        sum = 1;
        MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(&sum, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &stat);
        sum += rank + 1;
        if (rank < size - 1)
            MPI_Send(&sum, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        else
        {
            printf("Total sum is %d", sum);
        }
    }

    MPI_Finalize();
    return 0;
}