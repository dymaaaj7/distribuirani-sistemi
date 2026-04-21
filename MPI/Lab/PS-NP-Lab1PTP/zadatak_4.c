/*

Zadatak 4.

Napisati MPI program koji u root procesu (P0) deli ulazni niz dužine N, tako što
svakom procesu šalje po N/P elemenata odjednom, gde je P broj procesa (N je
deljivo sa P).

Po dobijanju podnizova, procesi treba da pronađu minimalnu vrednost i vrate je
root procesu.

On, nakom prijema svih minimalnih vrednosti, određuje najmanji element od svih
prihvaćenih i štampa ga.

Koristiti nizove sa realnim brojevima.

Zadatak realizovati korišćenjem MPI Point-to-Point operacija.

*/

#include <stdio.h>
#include <mpi.h>

#define N 12

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int chunk_size = N / size;

    float *podNiz = (float *)malloc(chunk_size * sizeof(float));

    if (rank == 0)
    {
        float niz[N];
        for (int i = 0; i < N; i++)
            niz[i] = (float)i + 1.0;

        for (int i = 0; i < chunk_size; i++)
        {
            podNiz[i] = niz[i];
        }

        for (int i = 1; i < size; i++)
        {
            MPI_Send(&niz[i * chunk_size],
                     chunk_size,
                     MPI_FLOAT,
                     i,
                     0,
                     MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(podNiz, chunk_size, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
    }

    float lokMin = __FLT_MAX__;
    for (int i = 0; i < chunk_size; i++)
    {
        if (podNiz[i] < lokMin)
            lokMin = podNiz[i];
    }
    printf("P%d: lokalni minimum = %.1f\n", rank, lokMin);
    if (rank == 0)
    {
        float globalMin = lokMin;
        float primljenMin;

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&primljenMin, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &status);
            if (globalMin > primljenMin)
                globalMin = primljenMin;
        }
        printf("\nGlobalni minimum: %.1f\n", globalMin);
    }
    else
    {
        MPI_Send(&lokMin, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }

    free(podNiz);
    MPI_Finalize();
    return 0;
}