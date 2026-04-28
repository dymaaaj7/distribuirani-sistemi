/*

Napisati MPI program koji realizuje mnozenje matrice Akxm i matrice Bmxn, cime
se dobija i prikazuje rezultujuca matrica C.

Takodje, program pronalazi i prikazuje proizvod elemenata svake kolone matrice
A.

Matrice A i B se inicijalizuju u root procesu.

Izracunavanje se obavlja tako sto master proces salje svakom procesu po s vrsta
matrice A (s - zadata konstanta, k deljivo sa s) i celu matricu B.

Svi elementi iz matrice A se salju odjednom i to direktno iz matrice A.

Svi procesi ucestvuju u izracunavanju potrebnim za generisanje rezultata.

Program treba da obezbedi da se rezultati programa nalaze i prikazuju u procesu
koji sadrzi maksimum svih vrednosti u matrici C.

Zadatak resiti koriscenjem grupnih operacija, osim za slanje kolona matrice A
za koje se koristi Point-to-Point operacija.

*/

#include <stdio.h>
#include <mpi.h>
#include <limits.h>
#define k 4
#define m 5
#define n 3
#define s 2

// size = k/s

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[k][m], b[m][n], c[k][n];
    int local_a[s][m], local_c[s][n];
    struct
    {
        int value;
        int rank;
    } in, out;
    int local_col_prod[m], col_prod[m];
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
    {
        for (int i = 0; i < k; i++)
            for (int j = 0; j < m; j++)
                a[i][j] = i + j;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                b[i][j] = i * j;
    }

    //  MPI_Scatter(a, s * m, MPI_INT, local_a, s * m, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        for (int i = 0; i < s; i++)
            for (int j = 0; j < m; j++)
                local_a[i][j] = a[i + s * root][j];

        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Send(&a[s * p][0], s * m, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, s * m, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    // MPI_Bcast(b, m * n, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Send(b, m * n, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(b, m * n, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    in.value = INT_MIN;
    in.rank = rank;
    for (int i = 0; i < m; i++)
        local_col_prod[i] = 1;

    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < m; j++)
            local_col_prod[j] *= local_a[i][j];

        for (int j = 0; j < n; j++)
        {
            local_c[i][j] = 0;
            for (int l = 0; l < m; l++)
                local_c[i][j] += local_a[i][l] * b[l][j];
            if (local_c[i][j] > in.value)
                in.value = local_c[i][j];
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

    // MPI_Reduce(local_col_prod, col_prod, m, MPI_INT, MPI_PROD, out.rank, MPI_COMM_WORLD);

    if (rank != out.rank)
        MPI_Send(local_col_prod, m, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
    else
    {
        for (int i = 0; i < m; i++)
            col_prod[i] = local_col_prod[i];
        for (int p = 0; p < size; p++)
        {
            if (p == out.rank)
                continue;
            MPI_Recv(local_col_prod, m, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < m; i++)
                col_prod[i] *= local_col_prod[i];
        }
    }

    // MPI_Gather(local_c, s * n, MPI_INT, c, s * n, MPI_INT, out.rank, MPI_COMM_WORLD);
    if (rank != out.rank)
        MPI_Send(local_c, s * n, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
    else
    {
        for (int i = 0; i < s; i++)
            for (int j = 0; j < n; j++)
                c[i + out.rank * s][j] = local_c[i][j];

        for (int p = 0; p < size; p++)
        {
            if (p == out.rank)
                continue;
            MPI_Recv(local_c, s * n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < s; i++)
                for (int j = 0; j < n; j++)
                    c[i + p * s][j] = local_c[i][j];
        }
    }

    if (rank == out.rank)
    {
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuca matrica C [%d x %d]:\n", k, n);
        for (int i = 0; i < k; i++)
        {
            printf("[ ");
            for (int j = 0; j < n; j++)
                printf("%d ", c[i][j]);
            printf("]\n");
        }

        printf("\n---------------------------------\n\n");
        printf("Maksimalna vrednost u C: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");
        printf("Proizvod elemenata po kolonama matrice A:\n");
        for (int j = 0; j < m; j++)
            printf("  Kolona %d: %d\n", j + 1, col_prod[j]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}