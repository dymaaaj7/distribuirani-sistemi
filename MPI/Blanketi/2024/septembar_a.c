/*

Napisati MPI program kojim se vrsi realizacija sumiranja opisanog sa:

for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
        s += i + j;

ravnomernom ciklicnom raspodelom posla izmedju p procesa.

Ovo znaci da ako je k indeks procesa, onda proces Pk(0 <= k <= p - 1 ) treba da
izvrsi k, k+p, k+2p, ..., k+N^2-p sumiranje po redu u sekvencijalnom izvrsenju
programa.

Pretpostaviti da je N vece od broja procesa p i da je N deljivo sa p.

Rezultat programa prikazati u procesu koji sadrzi najmanji broj sabiraka koji su
prosti brojevi.

Nije dozvoljeno koriscenje indeksiranih promenljivih.

Zadatak resiti koriscenjem grupnih operacija.

*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define N 8

// mpiexec -n 4 septembar_a.exe

int is_prime(int n)
{
    if (n < 2)
        return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0)
            return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int local_sum = 0, sum = 0;
    struct
    {
        int value;
        int rank;
    } in, out;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    in.value = 0;
    in.rank = rank;
    for (int i = rank; i < N * N; i += size)
    {
        int a = i / N; // 0, 0, 1, 1
        int b = i % N; // 0, 3, 0, 3
        local_sum += a + b;
        if (is_prime(a + b))
            in.value++;
    }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        printf("%d ", sum);
        printf("%d %d ", out.value, out.rank);
    }

    MPI_Finalize();
    return 0;
}
