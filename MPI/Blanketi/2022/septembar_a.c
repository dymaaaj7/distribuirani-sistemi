/* Kao Decembar 222

Napisati MPI program kojim se vrsi realizacija sumiranja opisanog sa:


for (int i = y; i < y + N; i++) 6 prolazaka
    for (int j = N; j >= 0; j--) 7 prolazaka
        s += i + j;

ravnomernom ciklicnom raspodelom posla izmedju p procesa.

Ovo znaci da ako je k indeks procesa, onda proces Pk(0 <= k <= p - 1 ) treba da
izvrsi k, k+p, k+2p, ..., k+N^2-p sumiranje po redu u sekvencijalnom izvrsenju
programa.

Pretpostaviti da je N vece od broja procesa p i da je N deljivo sa p, a da je y
zadata konstanta.

Rezultat programa prikazati u procesu koji sadrzi najveci broj sabiraka koji su
prosti brojevi.

Nije dozvoljeno koriscenje indeksiranih promenljivih.

Zadatak resiti:
a)  koriscenjem grupnih operacija
b)  koriscenjem P-t-P operacija

*/

#include <mpi.h>
#include <stdio.h>

#define N 6
#define y 2

// size = 3

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
    for (int t = rank; t < N * (N + 1); t += size)
    {
        // ============================================================
        // REKONSTRUKCIJA i i j iz rednog broja iteracije t
        // ============================================================
        // originalna petlja:
        //   i ide od y do y+N-1  →  N vrednosti
        //   j ide od N do 0      →  N+1 vrednosti (JN = N+1)
        //
        // tabela za N=6, y=2:
        //   t= 0 → i=2, j=6
        //   t= 1 → i=2, j=5
        //   t= 2 → i=2, j=4
        //   ...
        //   t= 6 → i=2, j=0  (zavrsena prva vrsta, 7 koraka)
        //   t= 7 → i=3, j=6  (nova vrsta, j se resetuje)
        //   t=13 → i=3, j=0
        //   t=14 → i=4, j=6
        //
        // i = t / (N+1) + y
        //   - t/(N+1) govori koja si vrsta (svaka traje N+1 koraka)
        //   - +y jer i ne krece od 0 vec od y
        //
        // j = N - (t % (N+1))
        //   - t%(N+1) govori koliko si odmakao unutar vrste
        //   - N- jer j ide unazad od N do 0
        // ============================================================
        int i = y + t / (N + 1);
        int j = N - (t % (N + 1));
        local_sum += i + j;
        if (is_prime(i + j))
            in.value++;
    }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

    if (rank == out.rank)
    {
        printf("\nSuma: %d\n", sum);
        printf("Proces %d ima najveci broj prostih sabiraka: %d\n",
               out.rank,
               out.value);
    }

    MPI_Finalize();
    return 0;
}