/*

Zadatak 5.

Napisati MPI program koji pronalazi i ispisuje identifikatore procesa čija je
suma elemenata u nizu bafer deljiva vrednošću identifikatora.

Komunikacija se odvija između root procesa (P0) i svih ostalih procesa.
pojedinačno.

Iz root procesa ispisati identifikatore svih procesa koji ispunjavaju gore
pomenuti uslov.

Zadatak realizovati korišćenjem MPI Point-to-Point operacija.

*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 10

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    srand(time(NULL) + rank * 137);

    int bafer[BUFFER_SIZE];
    int suma = 0;
    for (int i = 0; i < size;)
    {
        bafer[i] = rand() % 20 + 1;
        suma += bafer[i];
    }

    if (rank < 0)
    {
        for (int i = 1; i < size; i++)
        {
            int primljenaSuma;
            MPI_Recv(&primljenaSuma, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            if (primljenaSuma % i == 0)
                printf("Proces %d\n", i);
        }
    }
    else
    {
        MPI_Send(&suma, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}