/*

Napisati MPI program koji realizuje mnozenje matrice Akxl i vektora bl, cime se
izracunava i prikazuje rezultujuci vektor c.

Takodje, program pronalazi i prikazuje maksimalnu vrednost elemenata u matrici
A, kao i sumu elemenata svake vrste matrice A.

Izracunavanje se obavlja tako sto root proces salje svakom procesu po q kolona
matrice A (q - zadata konstanta, l deljivo sa q) i po q elemenata vektora b.

Svi elementi kolona matrice A se salju odjednom.

Svi procesi ucestvuju u izracunavanjima potrebnim za generisanje rezultata.

Svi rezultati programa se prikazuju u procesu koji sadrzi maksimalnu vrednost
elemenata u matrici A, nakon raspodele kolona po procesima..

Zadatak resiti:
a)  koriscenjem grupnih operacija, osim za slanje kolona matrice A
b)  koriscenjem P-t-P operacija za upotrebljene grupne operacije pod a)

*/

// 1 rezultujuca matrica C
// maksimalna vrednost elementa
// suma svake vrste

#include <mpi.h>
#include <stdio.h>

#define k 3
#define l 6
#define q 2

// size = l / q

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[k][l], b[l], c[k];
    int local_a[k][q], local_b[q], local_c[k];

    int row_sum[k], local_row_sum[k];
    struct
    {
        int value;
        int rank;
    } in, out;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // inicijalizacija matrice a i vektora b
    if (rank == root)
    {
        for (int i = 0; i < k; i++)
            for (int j = 0; j < l; j++)
                a[i][j] = i + j;
        for (int i = 0; i < l; i++)
            b[i] = i + 1;
    }

    // slanje q kolona matrice A
    if (rank == root)
    {
        // root ostavlja sebi prvih q kolona
        for (int i = 0; i < k; i++)
            for (int j = 0; j < q; j++)
                local_a[i][j] = a[i][j];

        int tmp_a[k][q];
        for (int p = 1; p < size; p++)
        {
            for (int i = 0; i < k; i++)
                for (int j = 0; j < q; j++)
                    // p*q je offset
                    tmp_a[i][j] = a[i][p * q + j];
            MPI_Send(tmp_a, k * q, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, k * q, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    // slanje q elemenata vektora b
    MPI_Scatter(b, q, MPI_INT, local_b, q, MPI_INT, root, MPI_COMM_WORLD);

    // lokalno izracunavanje: parcijalni doprinosi za c, sumu vrsti i maksimum
    in.value = local_a[0][0]; // pocetna vrednost za trazenje maksimuma
    in.rank = rank;
    for (int i = 0; i < k; i++)
    {
        local_c[i] = 0;
        local_row_sum[i] = 0;
        for (int j = 0; j < q; j++)
        {
            // parcijalni proizvod vrste sa vektorom b
            local_c[i] += local_a[i][j] * local_b[j];
            // parcijalna suma vrste i
            local_row_sum[i] += local_a[i][j];
            // azuriranje lokalnog maksimuma
            if (local_a[i][j] > in.value)
                in.value = local_a[i][j];
        }
    }

    // redukcija - globalni maksimum i njegov proces (MPI_MAXLOC)
    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);

    // broadcast ko stampa rezultate
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    // redukcija - suma vrsti i vektor c u procesu sa maksimumom
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

        printf("Maksimalna vrednost: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");

        printf("Suma elemenata po vrstama:\n");
        for (int i = 0; i < k; i++)
            printf("  Vrsta %d: %d\n", i + 1, row_sum[i]);

        printf("\n=================================\n");
    }
    MPI_Finalize();
    return 0;
}