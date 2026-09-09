/*

Napisati MPI program koji izracunava i prikazuje proizvod matrice Anxk i vektora
Bk tako sto master proces salje svakom procesu po jednu kolonu matrice A i po
jedan element vektora B.

Svi elementi jedne kolone matrice A se salju odjednom.

Svi procesi ucestvuju u izracunavanjima potrebnim za generisanje rezultata
programa.

Nakon slanja elemenata matrice A, u okviru programa se izracunava i prikazuje
minimum vrednosti elemenata matrice A, kao i proizvod elemenata svake vrste
matrice A.

Program treba da obezbedi da se rezultati programa nalaze i prikazuju u procesu
koji nakon slanja elemenata matrice A sadrzi minimum svih vrednosti u matrici A.

Zadatak resiti:
a)  koriscenjem grupnih operacija, osim za slanje kolona matrice A
b)  koriscenjem P-t-P operacija

*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define n 3
#define k 5

// Windows: mpicc april_a_v2.c -o april_a_v2.exe && mpiexec -n 5 april_a_v2.exe
// Linux:   mpicc april_a_v2.c -o april_a_v2 && mpirun -np 5 ./april_a_v2

int main(int argc, char *argv[])
{
    int rank, size, root = 0;

    int a[n][k], b[k], c[n];
    int local_a[n], local_b, local_c[n];

    // Za MPI_MINLOC: struktura MORA biti { value, rank } tim redom
    // (prvi int je vrednost, drugi je rang) inace MINLOC radi nad rangovima.
    struct
    {
        int value;
        int rank;
    } in, out;

    int row_prod[n];

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Master inicijalizuje matricu A i vektor b
    if (rank == root)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < k; j++)
                a[i][j] = 1 + i + j;
        for (int i = 0; i < k; i++)
            b[i] = i + 2;
    }

    // Distribucija kolona matrice A (P2P, kolona se salje odjednom):
    // master zadrzava svoju kolonu, ostalima salje po jednu kolonu
    if (rank == root)
    {
        for (int i = 0; i < n; i++)
            local_a[i] = a[i][rank];

        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;

            int tmp[n];
            for (int i = 0; i < n; i++)
                tmp[i] = a[i][p];
            MPI_Send(tmp, n, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(local_a, n, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    // Svaki proces dobija po jedan element vektora b (grupna operacija)
    MPI_Scatter(b, 1, MPI_INT, &local_b, 1, MPI_INT, root, MPI_COMM_WORLD);

    // Lokalni proracun: doprinos c = A * b (kolona * svoj element b)
    // i trazenje lokalnog minimuma kolone
    in.value = local_a[0];
    in.rank = rank;

    for (int i = 0; i < n; i++)
    {
        local_c[i] = local_a[i] * local_b;
        if (local_a[i] < in.value)
            in.value = local_a[i];
    }

    // Globalni minimum + rang procesa koji ga drzi (MINLOC),
    // pa Bcast da svi saznaju u kom procesu treba da budu rezultati
    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    // Redukcije sa root-om = out.rank (proces sa globalnim minimumom):
    // c = A * b po zbiru doprinosa, row_prod = proizvod elemenata svake vrste
    MPI_Reduce(local_c, c, n, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);
    MPI_Reduce(local_a,
               row_prod,
               n,
               MPI_INT,
               MPI_PROD,
               out.rank,
               MPI_COMM_WORLD);

    // Prikaz rezultata u procesu koji sadrzi minimum matrice A
    if (rank == out.rank)
    {
        printf("Proces %d drzi minimum matrice A: %d\n", out.rank, out.value);
        printf("A * b = [ ");
        for (int i = 0; i < n; i++)
            printf("%d ", c[i]);
        printf("]\n");
        printf("Proizvodi vrsta = [ ");
        for (int i = 0; i < n; i++)
            printf("%d ", row_prod[i]);
        printf("]\n");
    }

    MPI_Finalize();
    return 0;
}
