/*

Napisati MPI program koji realizuje mnozenje matrice Anxk i matrice Bkxm, cime
se dobija i prikazuje rezultujuca matrica C.

Program pronalazi i minimum svake vrste matrice C.

Mnozenje se obavlja tako sto master proces salje svakom procesu celu matricu A i
po s kolona (s je zadata konstanta, m je deljivo sa s) matrice B.

Konacni rezultat programa se generise u procesu koji sadrzi minimum u matrici B
nakon distribuiranja, koji ga i prikazuje.

Predvideti da se slanje elemenata kolone matrice B svakom procesu obavlja
odjednom.

Zadatak resiti koriscenjem grupnih operacija, osim za slanje kolona matrice A
za koje se koristi Point-to-Point operacija.

*/

#include <mpi.h>
#include <stdio.h>
#include <limits.h>

#define n 3
#define k 4
#define m 6
#define s 2

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[n][k], b[k][m], c[n][m];
    int local_b[k][s], local_c[n][s];
    struct
    {
        int value;
        int rank;
    } in, out;
    int row_min[n];
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
            for (int j = 0; j < m; j++)
                b[i][j] = i + j + 1;
    }

    MPI_Bcast(a, n * k, MPI_INT, root, MPI_COMM_WORLD);

    if (rank == root)
    {
        for (int i = 0; i < k; i++)
            for (int j = 0; j < s; j++)
                local_b[i][j] = b[i][root * s + j];

        int tmp_b[k][s];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            for (int i = 0; i < k; i++)
                for (int j = 0; j < s; j++)
                    tmp_b[i][j] = b[i][j + p * s];
            MPI_Send(tmp_b, k * s, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_b, k * s, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    in.value = INT_MAX;
    in.rank = rank;
    for (int i = 0; i < k; i++)
        for (int j = 0; j < s; j++)
            if (local_b[i][j] < in.value)
                in.value = local_b[i][j];

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < s; j++)
        {
            local_c[i][j] = 0;
            for (int l = 0; l < k; l++)
                local_c[i][j] += a[i][l] * b[l][j];
        }
    }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    MPI_Gather(local_c,
               n * s,
               MPI_INT,
               c,
               n * s,
               MPI_INT,
               out.rank,
               MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        for (int i = 0; i < n; i++)
        {
            row_min[i] = c[i][0];
            for (int j = 0; j < m; j++)
                if (c[i][j] < row_min[i])
                    row_min[i] = c[i][j];
        }
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuca matrica C [%d x %d]:\n", n, m);
        for (int i = 0; i < n; i++)
        {
            printf("[ ");
            for (int j = 0; j < m; j++)
                printf("%d ", c[i][j]);
            printf("]\n");
        }

        printf("\n---------------------------------\n\n");
        printf("Minimum u B: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");
        printf("Minimum po vrstama matrice C:\n");
        for (int i = 0; i < n; i++)
            printf("  Vrsta %d: %d\n", i + 1, row_min[i]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}