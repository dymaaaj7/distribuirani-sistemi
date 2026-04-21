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

#include <mpi.h>
#include <stdio.h>

#define n 3
#define k 4

// size = k

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int a[n][k], b[k], c[n];
    int local_a[n], local_b, local_c[n];
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

    // MPI_Scatter(b, 1, MPI_INT, &local_b, 1, MPI_INT, root, MPI_COMM_WORLD);
    if (rank == root)
    {
        local_b = b[rank];
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Send(&b[p], 1, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    else
        MPI_Recv(&local_b, 1, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

    in.value = local_a[0];
    in.rank = rank;
    for (int i = 0; i < n; i++)
    {
        local_c[i] = local_a[i] * local_b;
        if (local_a[i] < in.value)
            in.value = local_a[i];
    }

    // MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    if (rank != root)
    {
        MPI_Send(&in, 1, MPI_2INT, root, 0, MPI_COMM_WORLD);
    }
    else
    {
        out = in;
        for (int p = 0; p < size; p++)
        {
            if (p == root)
                continue;
            MPI_Recv(&in, 1, MPI_2INT, p, 0, MPI_COMM_WORLD, &status);
            if (out.value > in.value)
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

    // MPI_Reduce(local_c, c, n, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);
    if (rank != out.rank)
        MPI_Send(local_c, n, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
    else
    {
        for (int i = 0; i < n; i++)
            c[i] = local_c[i];
        for (int p = 0; p < size; p++)
        {
            if (p == out.rank)
                continue;
            MPI_Recv(local_c, n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < n; i++)
                c[i] += local_c[i];
        }
    }

    // MPI_Reduce(local_a, row_prod, n, MPI_INT, MPI_PROD, out.rank, MPI_COMM_WORLD);
    if (rank != out.rank)
        MPI_Send(local_a, n, MPI_INT, out.rank, 0, MPI_COMM_WORLD);
    else
    {
        for (int i = 0; i < n; i++)
            row_prod[i] = local_a[i];
        for (int p = 0; p < size; p++)
        {
            if (p == out.rank)
                continue;
            MPI_Recv(local_a, n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
            for (int i = 0; i < n; i++)
                row_prod[i] *= local_a[i];
        }
    }

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

        printf("Minimalna vrednost u A: %d\n", out.value);
        printf("Nalazi se u procesu: %d\n\n", out.rank);

        printf("---------------------------------\n\n");

        printf("Proizvod elemenata po vrstama matrice A:\n");
        for (int i = 0; i < n; i++)
            printf("  Vrsta %d: %d\n", i + 1, row_prod[i]);

        printf("\n=================================\n");
    }

    MPI_Finalize();
    return 0;
}