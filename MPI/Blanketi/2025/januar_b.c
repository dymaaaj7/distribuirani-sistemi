/*

Napisati MPI program koji realizuje mnozenje matrice Arxk i matrice Bkxn, cime
se dobija i prikazuje rezultujuca matrica C.

Takodje, program pronalazi i prikazuje sumu elemenata svake kolone matrice
B.

Izracunavanje se obavlja tako sto master proces salje svakom procesu po m vrsta
matrice A (m- zadata konstanta, r deljivo sa m) i po celu matricu B.

Svi elementi iz m vrsta matrice A se salju odjednom i to direktno iz matrice A.

Svi procesi ucestvuju u izracunavanju potrebnim za generisanje rezultata.

Program treba da obezbedi da se rezultati programa nalaze i prikazuju u procesu
koji sadrzi maksimum svih vrednosti u matrici A.

Zadatak resiti:
a)  koriscenjem grupnih operacija
b)  koriscenjem P-t-P operacija

*/

#include <mpi.h>
#include <stdio.h>
#include <limits.h>

#define r 4
#define k 5
#define n 3
#define m 2

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[r][k], b[k][n], c[r][n];
    int local_a[m][k], local_c[m][n];
    struct
    {
        int value;
        int rank;
    } in, out;
    int col_sum[n];
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < k; j++)
                a[i][j] = i + j;
        for (int i = 0; i < k; i++)
            for (int j = 0; j < n; j++)
                b[i][j] = i * j;
    }

    // MPI_Scatter(a, m * k, MPI_INT, local_a, m * k, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < k; j++)
                local_a[i][j] = a[i + root * m][j];

        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Send(&a[0 + p * m][0], k * m, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, m * k, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    // MPI_Bcast(b, k * n, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Send(b, k * n, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(b, k * n, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    in.value = INT_MIN;
    in.rank = rank;

    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < k; j++)
            if (in.value < local_a[i][j])
                in.value = local_a[i][j];

        for (int j = 0; j < n; j++)
        {
            local_c[i][j] = 0;
            for (int l = 0; l < k; l++)
                local_c[i][j] += local_a[i][l] * b[l][j];
        }
    }

    // MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    if (rank != root)
        MPI_Send(&in, 1, MPI_2INT, root, 0, MPI_COMM_WORLD);
    else
    {
        out = in;
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Recv(&in, 1, MPI_2INT, p, 0, MPI_COMM_WORLD, &status);
            if (out.value < in.value)
                out = in;
        }
    }
    // MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Send(&out, 1, MPI_2INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(&out, 1, MPI_2INT, root, 0, MPI_COMM_WORLD, &status);

    // MPI_Gather(local_c, m * n, MPI_INT, c, m * n, MPI_INT, out.rank, MPI_COMM_WORLD);
    if (rank != out.rank)
        MPI_Send(local_c, m * n, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
    else
    {
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                c[i + m * out.rank][j] = local_c[i][j];
        for (int p = 0; p < size; p++)
        {
            if (p == out.rank)
                continue;
            MPI_Recv(local_c, m * n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < m; i++)
                for (int j = 0; j < n; j++)
                    c[i + m * p][j] = local_c[i][j];
        }
    }

    if (rank == out.rank)
    {
        for (int i = 0; i < n; i++)
            col_sum[i] = 0;

        for (int i = 0; i < k; i++)
            for (int j = 0; j < n; j++)
                col_sum[j] += b[i][j];
    }

    if (rank == out.rank)
    {
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuca matrica C [%d x %d]:\n", r, n);
        for (int i = 0; i < r; i++)
        {
            printf("[ ");
            for (int j = 0; j < n; j++)
                printf("%d ", c[i][j]);
            printf("]\n");
        }

        printf("\n---------------------------------\n\n");
        printf("Maksimalna vrednost u A: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");
        printf("Suma elemenata po kolonama matrice B:\n");
        for (int j = 0; j < n; j++)
            printf("    Kolona %d: %d\n", j + 1, col_sum[j]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}