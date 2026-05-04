/*

Napisati MPI program koji realizuje mnozenje matrice Amxk i matrice Bkxn, cime
se izracunava i prikazuje rezultujuci vektor c.

Takodje, program pronalazi i prikazuje proizvod elemenata svake kolone matrice B

Izracunavanje se obavlja tako sto root proces salje svakom procesu po q kolona
matrice A (q - zadata konstanta) i po q vrsta matrice B.

Svi elementi kolona matrice A se salju odjednom.

Svi procesi ucestvuju u izracunavanjima potrebnim za generisanje rezultata.

Svi rezultati programa se prikazuju u procesu koji sadrzi maksimalnu vrednost
elemenata u matrici B, nakon raspodele kolona po procesima.

Zadatak resiti koriscenjem grupnih operacija, osim za slanje kolona matrice A.

*/

#include <mpi.h>
#include <stdio.h>
#include <limits.h>

#define m 3
#define k 4
#define n 3
#define q 2

/*
    mpiexec -n 2 jun_2.exe
*/

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[m][k], b[k][n], c[m][n];
    int local_a[m][q], local_b[q][n], local_c[m][n];
    int local_col_prod[n], col_prod[n];
    struct
    {
        int value;
        int rank;
    } in, out;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < k; j++)
                a[i][j] = i + j + 2;
        for (int i = 0; i < k; i++)
            for (int j = 0; j < n; j++)
                b[i][j] = i + j - 1;
    }

    if (rank == root)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < q; j++)
                local_a[i][j] = a[i][j + q * root];
        int tmp_a[m][q];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            for (int i = 0; i < m; i++)
                for (int j = 0; j < q; j++)
                    tmp_a[i][j] = a[i][j + p * q];
            MPI_Send(tmp_a, m * q, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, m * q, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    MPI_Scatter(b,
                q * n,
                MPI_INT,
                local_b,
                q * n,
                MPI_INT,
                root,
                MPI_COMM_WORLD);

    in.value = INT_MIN;
    in.rank = rank;
    for (int i = 0; i < n; i++)
        local_col_prod[i] = 1;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            local_c[i][j] = 0;
            for (int l = 0; l < q; l++)
                local_c[i][j] += local_a[i][l] * local_b[l][j];
        }
    }

    for (int i = 0; i < q; i++)
        for (int j = 0; j < n; j++)
        {
            local_col_prod[j] *= local_b[i][j];
            if (in.value < local_b[i][j])
                in.value = local_b[i][j];
        }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    MPI_Reduce(local_col_prod,
               col_prod,
               n,
               MPI_INT,
               MPI_PROD,
               out.rank,
               MPI_COMM_WORLD);
    MPI_Reduce(local_c, c, m * n, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuca matrica C [%d x %d]:\n", m, n);
        for (int i = 0; i < m; i++)
        {
            printf("[ ");
            for (int j = 0; j < n; j++)
                printf("%4d ", c[i][j]);
            printf("]\n");
        }

        printf("\n---------------------------------\n\n");
        printf("Maksimalna vrednost u B: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");
        printf("Proizvod elemenata po kolonama matrice B:\n");
        for (int j = 0; j < n; j++)
            printf("  Kolona %d: %d\n", j + 1, col_prod[j]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}