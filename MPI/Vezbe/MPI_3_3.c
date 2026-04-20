/*

Napisati MPI program koji pronalazi i prikazuje minimalni neparan broj sa
zadatom osobinom i identifikator procesa koji ga sadrži.

Neparni brojevi se nalaze u intervalu [a,b](a i b su zadate konstante).

Osobina koju broj treba da poseduje je da je deljiv zadatom vrednošću x.

Prilikom ispitivanja (da li broj poseduje zadatu osobinu ili ne) svaki proces
generiše i ispituje odgovarajuće neparne brojeve na način prikazan na slici
(za primer broj_procesa=4 i a=3, b=31, x=5).

Konačne rezultate treba da prikaže proces koji sadrži najmanji broj takvih
brojeva.

Zadatak rešiti korišćenjem grupnih operacija.

Primer:
_________________________
| P 0 | P 1 | P 2 | P 3 |
|-----------------------|
|  3  |  5  |  7  |  9  |
|-----------------------|
| 11  | 13  | 15  | 17  |
|-----------------------|
| 19  | 21  | 23  | 25  |
|-----------------------|
| 27  | 29  | 31  |     |
|_______________________|

broj_procesa=4 i a=3, b=31, x=5 =>
min=5,
id=1
broj_brojeva_sa_zadatom_osobinom=3
id_procesa_koji_prikazuje_rezultate=0

*/

#define a 3
#define b 31
#define x 5

#include <stdio.h>
#include <mpi.h>
#include <limits.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    struct
    {
        int value;
        int rank;
    } in_count, out_count, in_min, out_min;
    int local_count = 0;
    int local_min = INT_MAX;

    for (int i = 2 * rank + a; i <= b; i += size * 2)
    {
        if (i % x == 0)
        {
            local_count++;
            if (i < local_min)
                local_min = i;
        }
    }

    // 1. Proces sa najmanjim brojem pogodjenih brojeva
    in_count.value = local_count;
    in_count.rank = rank;
    MPI_Reduce(&in_count, &out_count, 1, MPI_2INT, MPI_MINLOC, 0, MPI_COMM_WORLD);
    MPI_Bcast(&out_count, 1, MPI_2INT, 0, MPI_COMM_WORLD);

    // 2. Globalni minimum medju brojevima sa osobinom
    in_min.value = local_min;
    in_min.rank = rank;
    MPI_Reduce(&in_min, &out_min, 1, MPI_2INT, MPI_MINLOC, out_count.rank, MPI_COMM_WORLD);

    // 3. Ukupan broj brojeva sa osobinom
    int total_count;
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, out_count.rank, MPI_COMM_WORLD);

    if (rank == out_count.rank)
    {
        printf("min=%d\n", out_min.value);
        printf("id=%d\n", out_min.rank);
        printf("broj_brojeva_sa_zadatom_osobinom=%d\n", total_count);
        printf("id_procesa_koji_prikazuje_rezultate=%d\n", rank);
    }

    MPI_Finalize();
    return 0;
}
