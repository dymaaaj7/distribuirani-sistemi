/*

Napisati MPI program u kome root proces (P0) treba da podeli ulazni niz dužine
p*(p+1) elemenata (p je broj procesa radnika) na delove tako što se svakom
procesu radniku šalje deo niza dužine 2*i, gde je i identifikator procesa
(npr. ako imamo 3 procesa radnika i niz je dužine 12, onda proces 1 dobija prva
dva elementa niza, proces 2 sledeća 4 elementa, a proces 3 poslednjih 6
elemenata niza).

Procesi radnici treba da pronađu sumu vrednosti iz svog dela niza i odštampaju
je na ekranu zajedno sa svojim identifikatorom.

Koristiti celobrojne nizove.

Zadatak realizovati korišćenjem MPI Point-to-Point operacija.

*/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int p = size - 1;

    if (rank == 0)
    {
        int total = p * (p + 1);
        int *array = (int *)malloc(total * sizeof(int));

        for (int i = 0; i < total; i++)
            array[i] = i + 1;

        int offset = 0;
        for (int i = 1; i <= p; i++)
        {
            int chunk_size = 2 * i;
            MPI_Send(&array[offset], chunk_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            offset += chunk_size;
        }

        free(array);
    }
    else
    {
        int chunk_size = 2 * rank;
        int *chunk = (int *)malloc(chunk_size * sizeof(int));
        MPI_Recv(chunk, chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int sum = 0;
        for (int i = 0; i < chunk_size; i++)
            sum += chunk[i];

        printf("Total sum in P  %d is %d.\n", rank, sum);

        free(chunk);
    }

    MPI_Finalize();
    return 0;
}