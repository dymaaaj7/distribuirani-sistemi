/*

Napisati MPI program koji pronalazi proizvod matrice Anxn i vektora bn.

Matrica A i vector b se inicijalizuju u procesu 0.

Izračunavanje se obavlja tako što se svakom procesu distribuira po vrsta matrice
A i ceo vektor b.

Svi procesi učestvuju u izračunavanju.

Rezultat se prokazuje u procesu 0.

*/

#include <mpi.h>
#include <stdio.h>

#define n 6

int main(int argc, char *argv[])
{
    int rank, size;
    int a[n][n], b[n], local_a[n], local_c, c[n];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Inicijalizacija matrice "a" i niza "b"
    if (rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                a[i][i] = i + j;
            }
            b[i] = i + 1;
        }
    }
    // Slanje svakom procesu po 1 vrsta (n po n elemenata).
    MPI_Scatter(&a[0][0], n, MPI_INT, local_a, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Slanje svakom procesu ceo niz b
    MPI_Bcast(b, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Racunanje i-tog elementa finalnog vektora
    local_c = 0;
    for (int i = 0; i < n; i++)
        local_c += local_a[i] * b[i];

    // skupljanje svakog elementa niza u finalni niz
    MPI_Gather(&local_c, 1, MPI_INT, &c[0], n, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int i = 0; i < n; i++)
        {
            printf("%d ", c[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}