/*

Isto kao Jun 2020

Napisati MPI program kojim se vrsi realizacija sumiranja opisanog sa:

for (int i = 0; i < N; i++) // 0 1 2 3 4 5
    for (int j = 0; j < N; j++) // 0 1 2 3 4 5
        s += i + j;

ravnomernom ciklicnom raspodelom posla izmedju p procesa.

Ovo znaci da ako je k indeks procesa, onda proces Pk(0 <= k <= p - 1 ) treba da
izvrsi k, k+p, k+2p, ..., k+N^2-p sumiranje po redu u sekvencijalnom izvrsenju
programa.

Pretpostaviti da je N vece od broja procesa p i da je N deljivo sa p.

Rezultat programa prikazati u procesu koji sadrzi najmanji broj sabiraka koji su
prosti brojevi.

Nije dozvoljeno koriscenje indeksiranih promenljivih.

Zadatak resiti:
a)  koriscenjem grupnih operacija
b)  koriscenjem P-t-P operacija

*/

#include <mpi.h>
#include <stdio.h>

#define N 6

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
    for (int t = rank; t < N * N; t++)
    {
        int i = t / N;
        int j = t % N;
        local_sum += i + j;
        if (is_prime(i + j))
            in.value++;
    }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        printf("\nSuma: %d\n", sum);
        printf("Proces %d ima najmanji broj prostih sabiraka: %d\n",
               out.rank,
               out.value);
    }

    MPI_Finalize();
    return 0;
}