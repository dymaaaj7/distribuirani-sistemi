/*

Napisati MPI program koji realizuje mnozenje matrice Akxm i matrice Bmxn, cime
se dobija i prikazuje rezultujuca matrica C.

Takodje, program pronalazi i prikazuje proizvod elemenata svake kolone matrice
B.

Izracunavanje se obavlja tako sto master proces salje svakom procesu po q kolona
matrice A (q - zadata konstanta) i po q vrsta matrice B.

Svi elementi u pojedinacnoj koloni matrice A se salju odjednom.

Svi procesi ucestvuju u izracunavanju potrebnim za generisanje rezultata.

Svi rezultati programa se prikazuju u procesu koji sadrzi maksimalnu vrednost
elemenata u matrici B, nakon raspodele vrsta po procesima.

Zadatak resiti koriscenjem grupnih operacija, osim za slanje kolona matrice A
za koje se koristi Point-to-Point operacija.

*/

#include <mpi.h>
#include <stdio.h>

#define k 3
#define m 4
#define n 2
#define q 2

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[k][m], b[m][n], c[k][n];
    int local_a[k][q], local_b[q][n], local_c[k][n];
    int col_prod[n], local_col_prod[n];
    struct
    {
        int value;
        int rank;
    } in, out;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // inicijalizacija matrica
    if (rank == root)
    {
        for (int i = 0; i < k; i++)
            for (int j = 0; j < m; j++)
                a[i][j] = i + j;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                b[i][j] = i + j;
    }

    // slanje q kolona matrice a
    if (rank == root)
    {
        for (int i = 0; i < k; i++)
            for (int j = 0; j < q; j++)
                local_a[i][j] = a[i][j + root * q];

        int tmp_a[k];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            for (int j = 0; j < q; j++)
            {
                for (int i = 0; i < k; i++)
                {
                    tmp_a[i] = a[i][j + p * q];
                }
                MPI_Send(tmp_a, k, MPI_INT, p, 0, MPI_COMM_WORLD);
            }
        }
    }
    else
    {
        for (int i = 0; i < q; i++)
            MPI_Recv(&local_a[0][i],
                     k,
                     MPI_INT,
                     root,
                     0,
                     MPI_COMM_WORLD,
                     &status);
    }

    // slanje q vrsta matrice b
    MPI_Scatter(b,
                q * n,
                MPI_INT,
                local_b,
                q * n,
                MPI_INT,
                root,
                MPI_COMM_WORLD);

    /*
        1 2 3 4     0 1
        4 3 2 1     3 4
        1 2 3 4     5 6
                    7 8

        1 2     0 1                 3 4     5 6
        4 3     3 4                 2 1     7 8
        1 2                         3 4

        1*2+0*3
    */

    in.value = local_b[0][0];
    in.rank = rank;
    for (int i = 0; i < n; i++)
        local_col_prod[i] = 1;

    // proizvodi kolona i lokalni maksimum
    for (int i = 0; i < q; i++)
        for (int j = 0; j < n; j++)
        {
            // parcijalni proizvod kolona B
            local_col_prod[j] *= local_b[i][j];

            // trazenje lokalnog maksimuma u local_B
            if (local_b[i][j] > in.value)
                in.value = local_b[i][j];
        }

    // lokalni proizvod
    for (int i = 0; i < k; i++)
        for (int j = 0; j < n; j++)
        {
            local_c[i][j] = 0;
            for (int l = 0; l < q; l++)
                local_c[i][j] += local_a[i][l] * local_b[l][j];
        }

    // globalni maksimum matrice b
    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    // proizvod kolona matrice b
    MPI_Reduce(local_col_prod,
               col_prod,
               n,
               MPI_INT,
               MPI_PROD,
               out.rank,
               MPI_COMM_WORLD);

    // skupljanje delova matrice c
    MPI_Reduce(local_c, c, k * n, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

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