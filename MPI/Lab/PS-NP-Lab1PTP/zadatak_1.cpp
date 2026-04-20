/*

Zadatak 1.

Napisati MPI program koji pronalazi i ispisuje identifikatore procesa čija je
suma elemenata u nizu bafer parna.

Procesi proveravaju parnost niza bafer počevši od poslednjeg procesa (Pn) i
upisuju 1 u celobrojni podatak (proizvoljnog imena) na mesto bita koji odgovara
procesu koji vrši obradu ukoliko je njegova suma parna, a ukoliko je neparna.

Zatim se ovaj podatak šalje prethodnom procesu (Pn-1).

Svaki naredni proces izvršava istu operaciju provere i upisa bita, do procesa P0
koji na osnovu dobijenog celobrojnog podatka pronalazi sve identifikatore
procesa koji imaju parnu sumu i ispisuje ih na ekranu.

Broj procesa treba da je manji ili jednak broju bitova za reprezentaciju
celobrojnog podatka.

Zadatak realizovati korišćenjem MPI Point-to-Point operacija.

*/

#include <mpi.h>
#include <stdio.h>
#include <time.h>

#define ARRAY_SIZE 10

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank + 137);

    int bafer[ARRAY_SIZE];
    int suma = 0;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        bafer[i] = rand() % 20 + 1;
        suma += bafer[i];
    }

    int rezultat[size];
    for (int i = 0; i < size; i++)
        rezultat[i] = 0;

    if (rank == size - 1)
    {
        rezultat[rank] = (suma % 2 == 0) ? 1 : 0;
        if (size > 1)
            MPI_Send(rezultat, size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    }
    else if (rank > 0)
    {
        MPI_Recv(rezultat, size, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &status);
        rezultat[rank] = (suma % 2 == 0) ? 1 : 0;
        MPI_Send(rezultat, size, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
    }
    else
    {
        if (size > 1)
            MPI_Recv(rezultat,
                     size,
                     MPI_INT,
                     rank + 1,
                     0,
                     MPI_COMM_WORLD,
                     &status);
        rezultat[rank] = (suma % 2 == 0) ? 1 : 0;
        printf("Procesi sa parnom sumom: \n");
        for (int i = 0; i < size; i++)
        {
            if (rezultat[i] == 1)
            {
                printf("P%d ", i);
            }
        }
        printf("\n");
    }
}