/*

Napisati MPI program koji realizuje mozenje matrice Akxm i vektora bm, cime se
izracunava i prikazuje rezultujeci vektor c.

Takodje, program pronalazi i prikazuje maksimalnu vrednost elemenata u matrici
A, kao i sumu elemenata svake vrste matrice A.

Izracunavanje se obavlja tako sto master proces salje svakokm procesu po l
kolona matrice A (l - zadataka konstanta, m deljivo sa l) i po l elemenata
vektora b.

Svi elementi kolona matrice A se salju odjednom.

Svi procesi ucestvuju u izracunavanjima potrebnim za generisanje rezultata
programa.

Svi rezultati programa se prikazuju u procesu koji sadrzi maksimlanu vrednost
elemenata u matrici A, nakon raspodele kolona po procesima.

Zadatak resiti:
a)  koriscenjem grupnih operacija, osim za slanje kolona matrice A
b) koriscenje P-t-P operacija za upotrebljene grupne operacije pod a)

*/

#include <mpi.h>
#include <stdio.h>

#define k 3
#define m 6
#define l 2

// size=m/l

/*
P0      P1      P2
---------------------
0 1     2 3     4 5
5 4 k   3 2 k   1 0 k
0 1     2 3     4 5
l       l       l
*/

int main(int argc, char *argv[])
{
    int rank, size, root = 0;

    int a[k][m], b[m], c[k];
    int local_a[k][l], local_b[l], local_c[k];
    struct
    {
        int value;
        int rank;
    } in, out;
    int row_sum[k], local_row_sum[k];

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
            b[i] = i + 1;
    }

    // slanje l kolona matrice a
    if (rank == root)
    {
        for (int i = 0; i < k; i++)
            for (int j = 0; j < l; j++)
                local_a[i][j] = a[i][j + l * root];

        int tmp_a[k][l];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            for (int i = 0; i < k; i++)
                for (int j = 0; j < l; j++)
                    tmp_a[i][j] = a[i][j + p * l];

            MPI_Send(tmp_a, k * l, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, k * l, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    // slanje l elemanata vektora b
    MPI_Scatter(b, l, MPI_INT, local_b, l, MPI_INT, root, MPI_COMM_WORLD);

    /*
    P0      P1      P2          P0      P1      P2
    -------------------------------------------------
    0 1     2 3     4 5
    5 4 k   3 2 k   1 0 k       1 2     3 4     5 6
    0 1     2 3     4 5
    l       l       l

    LOCAL
    0 1             a00*b0+a01*b1   a10*b0+a11*b1   a20*b0+
    5 4     1 2     0*1+1*2         5*1+4*2         0*1+1*2
    0 1
    */

    // mnozenje u svakom procesu lokalno, plus racunanje lokalne sume vrste
    in.value = local_a[0][0];
    in.rank = rank;
    for (int i = 0; i < k; i++)
    {
        local_c[i] = 0;
        local_row_sum[i] = 0;
    }

    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < l; j++)
        {
            local_c[i] += local_a[i][j] * local_b[j];
            local_row_sum[i] += local_a[i][j];
            if (local_a[i][j] > in.value)
                in.value = local_a[i][j];
        }
    }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, 0, MPI_COMM_WORLD);

    MPI_Reduce(local_row_sum,
               row_sum,
               k,
               MPI_INT,
               MPI_SUM,
               out.rank,
               MPI_COMM_WORLD);
    MPI_Reduce(local_c, c, k, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        printf("=================================\n");
        printf("       REZULTATI PROGRAMA\n");
        printf("=================================\n\n");

        printf("Rezultujuci vektor c:\n");
        printf("[ ");
        for (int i = 0; i < k; i++)
            printf("%d ", c[i]);
        printf("]\n\n");

        printf("---------------------------------\n\n");

        printf("Maksimalna vrednost u A: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");

        printf("Suma elemenata po vrstama matrice A:\n");
        for (int i = 0; i < k; i++)
            printf("  Vrsta %d: %d\n", i + 1, row_sum[i]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}