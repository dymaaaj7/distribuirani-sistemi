/*

a)

Napisati MPI koji koriscenjem PtP komunikacije vrsi slanje jednog podatka iz
procesa 0 svim ostalim procesima u komunikatoru (broj procesa je stepen 2).

Procesi nakon primanja pokazuju dobijene vrednosti.

Procesi su uredjeni u stablo, komunikacija se odvija na sledeci nacin, za broj
procesa 8.

0 -(korak 1)-> 1
0 -(korak 2)-> 2
1 -(korak 2)-> 3
0 -(korak 3)-> 4
1 -(korak 3)-> 5
2 -(korak 3)-> 6
3 -(korak 3)-> 7

b)

Napisati MPI program koji realizuje mnozenje matrice Akxl i matrice Blxm, cime
se dobija i prikazuje rezultujuca matrica Ckxm.

Program pronalazi i proizvod svake vrste matrice a.

Izracunavanje se obavlja tako sto master proces salje svakom procesu po jednu
kolonu matrice A i po jednu vrstu matrice B.

Svi elementi kolone matrice A, kao i elementi vrste matrice B se salju odjednom.

Svi procesi učestvuju u izracunavanjima potrebnim za generisanje rezultata.

Zadatak resiti primenom grupnih operacija, osim za slanje kolona matrice A za
koje se koristi Point-to-Point operacija.

*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// mpiexec -n 8 oktobar_a.exe

int main(int argc, char *argv[])
{
    int rank, size, data, root = 0;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
        data = 67;

    int steps = (int)log2(size);

    for (int i = 1; i <= steps; i++)
    {
        int half = 1 << i - 1;
        if (rank < half)
            MPI_Send(&data, 1, MPI_INT, rank + half, 0, MPI_COMM_WORLD);
        else if (rank < 2 * half)
            MPI_Recv(&data, 1, MPI_INT, rank - half, 0, MPI_COMM_WORLD, &status);
    }

    if (rank != root)
        printf("Proces %d primio broj %d.\n", rank, data);

    MPI_Finalize();
    return 0;
}
