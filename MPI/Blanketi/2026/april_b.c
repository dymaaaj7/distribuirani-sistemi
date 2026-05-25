/*

Procesi u MPI programu imaju podatke koji izgledaju kao na slici a)

Navesti dve MPI_Funkcije za komunikaciju (potrebno je navesti ceo poziv funkcije
i argumente) koje dovode do toga da se podaci razmene onako kako je prikazano na
slici b).

*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    int b1[2], b2[2];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // inicijalizacija b1 po procesima
    if (rank == 0)
    {
        b1[0] = 0;
        b1[1] = 6;
    }
    if (rank == 1)
    {
        b1[0] = 2;
        b1[1] = 4;
    }
    if (rank == 2)
    {
        b1[0] = 9;
        b1[1] = 3;
    }
    if (rank == 3)
    {
        b1[0] = 5;
        b1[1] = 7;
    }

    // razmena - svaki proces salje b1 sledecem, prima b1 prethodnog u b2
    MPI_Send(b1, 2, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    MPI_Recv(b2, 2,
             MPI_INT,
             (rank - 1 + size) % size,
             0,
             MPI_COMM_WORLD,
             &status);

    printf("Proces %d: b1=[%d,%d], b2=[%d,%d]\n",
           rank, b1[0], b1[1], b2[0], b2[1]);

    MPI_Finalize();
    return 0;
}