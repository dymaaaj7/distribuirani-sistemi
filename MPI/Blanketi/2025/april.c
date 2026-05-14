/*

Napisati MPI program koji realizuje mozenje matrice Anxk i vektora bk, cime se
dobija i prikazuje rezultujuci vektor c.

Takodje, program pronalazi i prikazuje maksimalnu vrednost elemenata u matrici
A, kao i sumu elemenata svake vrste matrice A.

Izracunavanje se obavlja tako sto master proces salje svakokm procesu po s
kolona matrice A (s - zadataka konstanta) i po s elemenata vektora b.

Svi elementi kolona matrice A se salju odjednom.

Svi procesi ucestvuju u izracunavanjima potrebnim za generisanje rezultata
programa.

Svi rezultati programa se prikazuju u procesu koji sadrzi maksimlanu vrednost
elemenata u matrici A, nakon raspodele kolona po procesima.

Zadatak resiti koriscenjem grupnih operacija, osim za slanje kolona matrice A

*/

#include <mpi.h>
#include <stdio.h>
#include <limits.h>

#define n 3
#define k 4
#define s 2

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[n][k], b[k], c[n];
    int local_a[n][s], local_b[s], local_c[n];
    struct
    {
        int value;
        int rank;
    } in, out;
    int local_row_sum[n], row_sum[n];
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
            b[i] = i + 1;
    }

    if (rank == root)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < s; j++)
                local_a[i][j] = a[i][j + root * s];

        int tmp_a[n][s];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            for (int i = 0; i < n; i++)
                for (int j = 0; j < s; j++)
                    tmp_a[i][j] = a[i][j + p * s];
            MPI_Send(tmp_a, n * s, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, n * s, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    MPI_Scatter(b, s, MPI_INT, local_b, s, MPI_INT, root, MPI_COMM_WORLD);

    in.value = INT_MIN;
    in.rank = rank;
    for (int i = 0; i < n; i++)
        local_row_sum[i] = 0;

    for (int i = 0; i < n; i++)
    {
        local_c[i] = 0;
        for (int j = 0; j < s; j++)
        {
            local_row_sum[i] += local_a[i][j];
            local_c[i] += local_a[i][j] * local_b[j];
            if (local_a[i][j] > in.value)
                in.value = local_a[i][j];
        }
    }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    MPI_Reduce(local_row_sum,
               row_sum,
               n,
               MPI_INT,
               MPI_SUM,
               out.rank,
               MPI_COMM_WORLD);
    MPI_Reduce(local_a, c, n, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuci vektor c:\n");
        printf("[ ");
        for (int i = 0; i < n; i++)
            printf("%d ", c[i]);
        printf("]\n\n");

        printf("---------------------------------\n\n");

        printf("Maksimalna vrednost u A: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");

        printf("Suma elemenata po vrstama matrice A:\n");
        for (int i = 0; i < n; i++)
            printf("  Vrsta %d: %d\n", i + 1, row_sum[i]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}