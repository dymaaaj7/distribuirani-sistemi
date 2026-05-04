/*

Napisati MPI program koji izracunava i prikazuje proizvod matrice Akxm i matrice
Bmxn i generise matricu C.

Takodje, program pronalazi i prikazuje proizvod elemenata svake kolone matrice
A.

Matrice A i B se inicijalizuju u root procesu.

Izracunavanje se obavlja tako sto master proces salje svakom procesu po r vrsta
matrice A (r - zadata konstanta, k deljivo sa r) i celu matricu B.

Svi elementi iz r vrsta matrice A se salju odjednom i to direktno iz matrice A.

Svi procesi ucestvuju u izracunavanju potrebnim za generisanje rezultata.

Program treba da obezbedi da se rezultati programa nalaze i prikazuju u procesu
koji sadrzi minimum svih vrednosti u matrici A.

Zadatak resiti:
a)  koriscenjem grupnih operacija
b)  koriscenjem Point-to-Point operacija

Napisati na koji nacin se iz komandne linije vrsi startovanje napsiane MPI
aplikacije.

*/

#include <mpi.h>
#include <stdio.h>
#include <limits.h>
#define k 4
#define m 3
#define n 2
#define r 2

/*
    Program se pokrece sa mpiexec -n 2 septembar_b.exe
*/

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[k][m], b[m][n], c[k][n];
    int local_a[r][m], local_c[r][n];
    int local_col_prod[m], col_prod[m];
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
        for (int i = 0; i < k; i++)
            for (int j = 0; j < m; j++)
                a[i][j] = i * j;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                b[i][j] = i + j;
    }

    // MPI_Scatter(a,r*m,MPI_INT,local_a,r*m,MPI_INT,root,MPI_COMM_WORLD);
    if (rank == root)
    {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < m; j++)
                local_a[i][j] = a[i + r * root][j];

        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Send(&a[0 + p * r][0], r * m, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, r * m, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

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

    in.value = INT_MAX;
    in.rank = rank;
    for (int i = 0; i < m; i++)
        local_col_prod[i] = 1;

    for (int i = 0; i < r; i++)
    {
        for (int j = 0; j < m; j++)
        {
            local_col_prod[j] *= local_a[i][j];
            if (local_a[i][j] < in.value)
                in.value = local_a[i][j];
        }

        for (int j = 0; j < n; j++)
        {
            local_c[i][j] = 0;
            for (int l = 0; l < m; l++)
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
            if (in.value < out.value)
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
    if (rank != root)
        MPI_Send(local_col_prod, m, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
    else
    {
        for (int i = 0; i < m; i++)
            col_prod[i] = local_col_prod[i];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Recv(local_col_prod, m, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < m; i++)
                col_prod[i] *= local_col_prod[i];
        }
    }

    // MPI_Gather(local_c, r * n, MPI_INT, c, r * n, MPI_INT, out.rank, MPI_COMM_WORLD);
    if (rank != out.rank)
        MPI_Send(local_c, r * n, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
    else
    {
        for (int i = 0; i < r; i++)
            for (int j = 0; j < n; j++)
                c[i + out.rank * r][j] = local_c[i][j];
        for (int p = 0; p < size; p++)
        {
            if (p == out.rank)
                continue;
            MPI_Recv(local_c, r * n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < r; i++)
                for (int j = 0; j < n; j++)
                    c[i + p * r][j] = local_c[i][j];
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
        printf("Minimalna vrednost u A: %d\n", out.value);
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