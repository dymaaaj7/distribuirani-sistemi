/*

Napisati MPI program koji realizuje mnozenje matrice Anxk i matrice Bkxn, cime
se dobija i prikazuje rezultujuca matrica C.

Takodje, program pronalazi i prikazuje proizvod elemenata svake kolone matrice
B.

Izracunavanje se obavlja tako sto master proces salje svakom procesu po q kolona
matrice A (q - zadata konstanta) i po q vrsta matrice B.

Svi elementi kolona matrice A koji se salju svakom procesu se salju odjednom.

Svi procesi ucestvuju u izracunavanju potrebnim za generisanje rezultata.

Zadatak resiti koriscenjem grupnih operacija, osim za slanje kolona matrice A
za koje se koristi Point-to-Point operacija.

*/

#include <mpi.h>
#include <stdio.h>

#define n 3
#define k 4
#define q 2

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[n][k], b[k][n], c[n][n];
    int local_a[n][q], local_b[q][n], local_c[n][n];
    int local_col_prod[n], col_prod[n];

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < k; j++)
                a[i][j] = i + j;
        for (int i = 0; i < k; i++)
            for (int j = 0; j < n; j++)
                b[i][j] = i * j;
    }

    if (rank == root)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < q; j++)
                local_a[i][j] = a[i][j + root * q];

        int tmp_a[n][q];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < q; j++)
                    tmp_a[i][j] = a[i][j + p * q];
            MPI_Send(tmp_a, n * q, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, n * q, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    MPI_Scatter(b,
                q * n,
                MPI_INT,
                local_b,
                q * n,
                MPI_INT,
                root,
                MPI_COMM_WORLD);

    for (int i = 0; i < n; i++)
        local_col_prod[i] = 1;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            local_c[i][j] = 0;
            for (int l = 0; l < q; l++)
                local_c[i][j] += local_a[i][l] * local_b[l][j];
        }
        for (int j = 0; j < q; j++)
            for (int l = 0; l < n; l++)
                local_col_prod[l] *= local_b[j][l];
    }

    MPI_Reduce(local_c, c, n * n, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
    MPI_Reduce(local_col_prod,
               col_prod,
               n,
               MPI_INT,
               MPI_PROD,
               root,
               MPI_COMM_WORLD);

    if (rank == root)
    {
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuca matrica C [%d x %d]:\n", n, n);
        for (int i = 0; i < n; i++)
        {
            printf("[ ");
            for (int j = 0; j < n; j++)
                printf("%d ", c[i][j]);
            printf("]\n");
        }

        printf("---------------------------------\n\n");
        printf("Proizvod elemenata po kolonama matrice B:\n");
        for (int j = 0; j < n; j++)
            printf("    Kolona %d: %d\n", j + 1, col_prod[j]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}