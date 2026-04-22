/*

Napisati MPI program koji pronalazi proizvod matrice Amxn i vektora bn.

Matrica A i vector b se inicijalizuju u procesu 0.

Izračunavanje se obavlja tako što se svakom procesu distribuira po kolona
matrice A i po 1 element vektora b.

Za distribuciju kolona po procesima koristiti P-t-P operacije, za sve ostalo
grupne operacije.

Svi procesi učestvuju u izračunavanju.

Rezultat se prikazuje u procesu koji, nakon distribuiranja kolona matrice A,
sadrži minimum svih elemenata matrice A.

*/

#include <stdio.h>
#include <mpi.h>
#include <limits.h>

#define m 3
#define n 4
int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[m][n], b[n], c[m];
    int local_a[m], local_b, local_c[m];

    struct
    {
        int value;
        int rank;
    } in, out;

    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                a[i][j] = i + j;
        for (int i = 0; i < n; i++)
            b[i] = i + 1;
    }
    if (rank == root)
    {
        for (int i = 0; i < m; i++)
            local_a[i] = a[i][0];

        int tmp_a[m];
        for (int p = 1; p < size; p++)
        {
            for (int i = 0; i < m; i++)
            {
                tmp_a[i] = a[i][p];
            }
            MPI_Send(tmp_a, m, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(local_a, m, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Scatter(b, 1, MPI_INT, &local_b, 1, MPI_INT, root, MPI_COMM_WORLD);

    for (int i = 0; i < m; i++)
        local_c[i] = local_a[i] * local_b;

    in.value = INT_MAX;
    in.rank = rank;
    for (int i = 0; i < m; i++)
        if (in.value > local_a[i])
            in.value = local_a[i];

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    MPI_Reduce(local_c, c, m, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        printf("\n C = [ ");
        for (int i = 0; i < m; i++)
            printf("%d ", c[i]);
        printf("]\n\n");
        printf("Minimum element: %d is at process %d.\n\n",
               out.value,
               out.rank);
    }
    MPI_Finalize();
    return 0;
}