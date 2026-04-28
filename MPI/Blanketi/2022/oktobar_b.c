/*

Napisati MPI program koji realizuje mnozenje matrice Akxl i matrice Blxm, cime
se dobija i prikazuje rezultujuca matrica Ckxm.

Izracunavanje se obavlja tako sto master proces salje svakom procesu po jednu
kolonu matrice A i po jednu vrstu matrice B.

Svi elementi kolone matrice A, kao i elementi vrste matrice B se salju odjednom.

Svi procesi učestvuju u izracunavanjima potrebnim za generisanje rezultata.

Zadatak resiti primenom grupnih operacija, osim za slanje kolona matrice A za
koje se koristi Point-to-Point operacija.

*/

#include <mpi.h>
#include <stdio.h>
#define k 3
#define l 4
#define m 2

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[k][l], b[l][m], c[k][m];
    int local_a[k], local_b[m], local_c[k][m];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        for (int i = 0; i < k; i++)
            for (int j = 0; j < l; j++)
                a[i][j] = i + j;
        for (int i = 0; i < l; i++)
            for (int j = 0; j < m; j++)
                b[i][j] = i * j;
    }

    if (rank == root)
    {
        int tmp_a[k];
        for (int i = 0; i < k; i++)
            local_a[i] = a[i][root];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            for (int i = 0; i < k; i++)
                tmp_a[i] = a[i][p];
            MPI_Send(tmp_a, k, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, k, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    MPI_Scatter(b, m, MPI_INT, local_b, m, MPI_INT, root, MPI_COMM_WORLD);

    for (int i = 0; i < k; i++)
        for (int j = 0; j < m; j++)
            local_c[i][j] = local_a[i] * local_b[j];

    MPI_Reduce(local_c, c, k * m, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == root)
    {
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuca matrica C [%d x %d]:\n", k, m);
        for (int i = 0; i < k; i++)
        {
            printf("[ ");
            for (int j = 0; j < m; j++)
                printf("%d ", c[i][j]);
            printf("]\n");
        }
        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}