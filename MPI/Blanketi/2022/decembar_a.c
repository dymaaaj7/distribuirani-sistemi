/*

Napisati MPI program kojim se vrsi realizacija sumiranja opisanog sa:


for (int i = y; i < y + N; i++) // y=2,N=6 -> 2 3 4 5 6 7 -> N+1
    for (int j = N; j >= 0; j--) // 6 5 4 3 2 1 0 -> N
        s += i + j;             // N*(N+1) prolazaka za t

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

/*
┌─────────────────────────────────────────────────────┐
│      SABLON ZA CIKLICNU RASPODELU - PROVERITI       │
├─────────────────────────────────────────────────────┤
│                                                     │
│  KORAK 1: broj vrednosti j (JN)                     │
│                                                     │
│    UZLAZNA (j++)                                    │
│      j < C   → JN = C - B                           │
│      j <= C  → JN = C - B + 1                       │
│                                                     │
│    SILAZNA (j--)                                    │
│      j > C   → JN = B - C                           │
│      j >= C  → JN = B - C + 1                       │
│                                                     │
│  KORAK 2: ukupno iteracija                          │
│    t_max = N * JN                                   │
│                                                     │
│  KORAK 3: rekonstrukcija                            │
│    i = t / JN + pocetak_i                           │
│    j++ → j = B + (t % JN)                           │
│    j-- → j = B - (t % JN)                           │
│                                                     │
└─────────────────────────────────────────────────────┘
*/

#include <mpi.h>
#include <stdio.h>

#define N 6
#define y 2

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
        int rank
    } in, out;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    in.value = 0;
    in.rank = rank;
    for (int t = rank; t < N * (N + 1); t++)
    {
        int i = t / (N + 1) + y;
        int j = N - (t % (N + 1));
        local_sum += i + j;

        if (is_prime(i + j))
            in.value++;
    }

    // pronalazak procesa sa najvecim brojem prostih sabiraka
    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    // ukupna suma u out.rank
    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

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