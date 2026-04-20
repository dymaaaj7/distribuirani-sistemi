# MPI Funkcije — Referenca

## Sadržaj

1. [Inicijalizacija](#inicijalizacija)
2. [Topologija](#topologija)
3. [Blokiajuca komunikacija](#blokiajuca-komunikacija)
4. [Neblokiajuca komunikacija](#neblokiajuca-komunikacija)
5. [Cekanje na zavrsetak](#cekanje-na-zavrsetak)
6. [Grupne operacije](#grupne-operacije)

---

## Inicijalizacija

### `MPI_Init`

```c
int MPI_Init(int *argc, char ***argv)
```

**Opis:** Mora biti pozvana pre svih ostalih MPI funkcija. Inicijalizuje MPI izvršno okruženje i prosleđuje argumente komandne linije MPI biblioteci.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `argc` | `int *` | Pokazivač na broj argumenata komandne linije (iz `main` funkcije) |
| `argv` | `char ***` | Pokazivač na niz stringova argumenata komandne linije |

> **Napomena:** Mora biti pozvana tačno jednom. Na kraju programa obavezno pozvati `MPI_Finalize()`.

---

## Topologija

### `MPI_Comm_rank`

```c
int MPI_Comm_rank(MPI_Comm comm, int *rank)
```

**Opis:** Vraća rang tekućeg procesa unutar zadatog komunikatora. Rangovi su numerisani od `0` do `size - 1`.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `comm` | `MPI_Comm` | Komunikator (obično `MPI_COMM_WORLD` — svi procesi) |
| `rank` | `int *` | Izlaz: rang tekućeg procesa (0, 1, 2, ...) |

---

### `MPI_Comm_size`

```c
int MPI_Comm_size(MPI_Comm comm, int *size)
```

**Opis:** Vraća ukupan broj procesa koji su deo zadatog komunikatora.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `comm` | `MPI_Comm` | Komunikator čiju veličinu tražimo |
| `size` | `int *` | Izlaz: ukupan broj procesa u komunikatoru |

---

## Blokiajuca komunikacija

### `MPI_Send`

```c
int MPI_Send(void *buf, int count, MPI_Datatype dtype, int dest, int tag, MPI_Comm comm)
```

**Opis:** Šalje poruku odredišnom procesu. Funkcija se blokira dok se podaci ne kopiraju iz bafera i dok prenos nije bezbedan za nastavak — odredišni proces ne mora nužno da primi pre nego što ova funkcija vrati kontrolu.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `buf` | `void *` | Pokazivač na podatke koji se šalju (bаfer za slanje) |
| `count` | `int` | Broj elemenata koji se šalju |
| `dtype` | `MPI_Datatype` | Tip podataka (npr. `MPI_INT`, `MPI_DOUBLE`, `MPI_CHAR`) |
| `dest` | `int` | Rang odredišnog procesa |
| `tag` | `int` | Oznaka poruke — služi za razlikovanje poruka istog tipa; odgovara `MPI_Recv` sa istim tagom |
| `comm` | `MPI_Comm` | Komunikator (obe strane moraju biti u istom) |

> **Napomena:** Paran poziv: `MPI_Recv` na odredišnom procesu. Deadlock ako oba procesa zovu `Send` pre `Recv`!

---

### `MPI_Recv`

```c
int MPI_Recv(void *buf, int count, MPI_Datatype dtype, int source, int tag, MPI_Comm comm, MPI_Status *status)
```

**Opis:** Prima poruku od navedenog izvora. Blokira se dok poruka ne stigne — proces čeka na mestu poziva sve dok poruka sa odgovarajućim `source` i `tag` ne bude primljena.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `buf` | `void *` | Bаfer u koji se upisuju primljeni podaci |
| `count` | `int` | Maksimalan broj elemenata koji se mogu primiti (veličina bafera) |
| `dtype` | `MPI_Datatype` | Tip podataka koji se prima |
| `source` | `int` | Rang procesa pošiljaoca; `MPI_ANY_SOURCE` za primanje od bilo koga |
| `tag` | `int` | Oznaka poruke; `MPI_ANY_TAG` za primanje sa bilo kojim tagom |
| `comm` | `MPI_Comm` | Komunikator |
| `status` | `MPI_Status *` | Izlaz: sadrži informacije o primljenoj poruci (izvor, tag, greška); `MPI_STATUS_IGNORE` ako nije potrebno |

---

## Neblokiajuca komunikacija

### `MPI_Isend`

```c
int MPI_Isend(void *buf, int count, MPI_Datatype dtype, int dest, int tag, MPI_Comm comm, MPI_Request *request)
```

**Opis:** Pokreće slanje poruke i odmah vraća kontrolu — ne čeka da se prenos završi. Program može nastaviti rad dok se prenos odvija u pozadini. Obavezno pozvati `MPI_Wait` ili `MPI_Test` pre pristupa baferu.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `buf` | `void *` | Bаfer sa podacima za slanje — ne sme se menjati dok prenos nije završen |
| `count` | `int` | Broj elemenata |
| `dtype` | `MPI_Datatype` | Tip podataka |
| `dest` | `int` | Rang odredišnog procesa |
| `tag` | `int` | Oznaka poruke |
| `comm` | `MPI_Comm` | Komunikator |
| `request` | `MPI_Request *` | Izlaz: ručka (handle) za praćenje statusa prenosa — koristi se u `MPI_Wait`/`MPI_Test` |

> **Napomena:** Prefiks `I` = Immediate (odmah). Bаfer ne treba dirati pre `MPI_Wait`!

---

### `MPI_Irecv`

```c
int MPI_Irecv(void *buf, int count, MPI_Datatype dtype, int source, int tag, MPI_Comm comm, MPI_Request *request)
```

**Opis:** Pokreće prijem poruke i odmah vraća kontrolu ne čekajući da poruka stigne. Program može nastaviti rad. Podaci u baferu su validni tek nakon `MPI_Wait`.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `buf` | `void *` | Bаfer u koji će se upisati primljeni podaci — ne čitati dok prenos nije završen |
| `count` | `int` | Maksimalan broj elemenata |
| `dtype` | `MPI_Datatype` | Tip podataka |
| `source` | `int` | Rang pošiljaoca; `MPI_ANY_SOURCE` za bilo koji izvor |
| `tag` | `int` | Oznaka poruke; `MPI_ANY_TAG` za bilo koji tag |
| `comm` | `MPI_Comm` | Komunikator |
| `request` | `MPI_Request *` | Izlaz: ručka za praćenje statusa prijema |

---

## Cekanje na zavrsetak

### `MPI_Wait`

```c
int MPI_Wait(MPI_Request *request, MPI_Status *status)
```

**Opis:** Blokira se sve dok se neblokiajuća operacija (`Isend`/`Irecv`) identifikovana ručkom `request` ne završi. Nakon toga je bаfer siguran za upotrebu.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `request` | `MPI_Request *` | Ručka od prethodnog `MPI_Isend` ili `MPI_Irecv`; nakon završetka postavlja se na `MPI_REQUEST_NULL` |
| `status` | `MPI_Status *` | Izlaz: informacije o završenoj operaciji; `MPI_STATUS_IGNORE` ako nije potrebno |

---

### `MPI_Test`

```c
int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status)
```

**Opis:** Proverava da li je neblokiajuća operacija završena i odmah vraća kontrolu bez čekanja. Koristi se za "polling" — periodičnu proveru u petlji.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `request` | `MPI_Request *` | Ručka od prethodnog `Isend`/`Irecv` |
| `flag` | `int *` | Izlaz: `1` (true) ako je operacija završena, `0` ako nije |
| `status` | `MPI_Status *` | Izlaz: popunjava se samo ako je `flag == 1` |

> **Napomena:** Razlika od `MPI_Wait`: `Test` ne blokira — odmah vraća odgovor da li je gotovo.

---

## Grupne operacije

> Grupne operacije mora pozvati **svaki proces** u komunikatoru.

## Operacije za kontrolu procesa

### `MPI_Barrier`

```c
int MPI_Barrier(MPI_Comm comm)
```

**Opis:** Blokira svaki proces dok svi procesi u komunikatoru ne stignu do iste tačke. Koristi se za sinhronizaciju — niko ne nastavlja dok svi ne stignu.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `comm` | `MPI_Comm` | Komunikator čiji se procesi sinhronizuju |

---
## Operacije za globalna izracunavanja

### `MPI_Reduce`

```c
int MPI_Reduce(void *send_buffer, void *recv_buffer, int count, MPI_Datatype datatype, MPI_Op operation, int rank, MPI_Comm comm)
```

**Opis:** Primenjuje operaciju (npr. sabiranje, maksimum) na odgovarajuće elemente iz svih procesa i rezultat smešta u `recv_buffer` root procesa.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `send_buffer` | `void *` | Ulazni bаfer — podaci koji se šalju ka redukciji |
| `recv_buffer` | `void *` | Izlazni bаfer — samo root proces prima rezultat |
| `count` | `int` | Broj elemenata koji se redukuju |
| `datatype` | `MPI_Datatype` | Tip podataka |
| `operation` | `MPI_Op` | Operacija: `MPI_SUM`, `MPI_MAX`, `MPI_MIN`, `MPI_PROD`, `MPI_LAND`... |
| `rank` | `int` | Rang root procesa koji prima rezultat redukcije |
| `comm` | `MPI_Comm` | Komunikator |

> **Napomena:** `recv_buffer` je smislen samo za root. Ostali procesi mogu prosleđivati `NULL`.

---

### `MPI_Scan`

```c
int MPI_Scan(void *send_buffer, void *recv_buffer, int count, MPI_Datatype datatype, MPI_Op operation, MPI_Comm comm)
```

**Opis:** Svaki proces `i` dobija rezultat primene operacije na podatke procesa `0, 1, ..., i` (inkluzivna prefiksna suma/max/itd). Sličan `Reduce`, ali svaki proces čuva sopstveni delimični rezultat.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `send_buffer` | `void *` | Lokalni ulazni podaci procesa |
| `recv_buffer` | `void *` | Izlaz: delimičan rezultat za tekući proces |
| `count` | `int` | Broj elemenata |
| `datatype` | `MPI_Datatype` | Tip podataka |
| `operation` | `MPI_Op` | Operacija (`MPI_SUM`, `MPI_MAX`, ...) |
| `comm` | `MPI_Comm` | Komunikator |

> **Napomena:** Za razliku od `Reduce`, nema `rank` parametra — svaki proces dobija rezultat.

---

## Operacije za prenos podataka

### `MPI_Bcast`

```c
int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int rank, MPI_Comm comm)
```

**Opis:** Root proces šalje isti bаfer svim ostalim procesima. Svi procesi pozivaju ovu funkciju — root šalje, ostali primaju.

| Parametar | Tip | Opis |
|-----------|-----|------|
| `buffer` | `void *` | Kod root-a: podaci koji se šalju. Kod ostalih: bаfer koji prima podatke |
| `count` | `int` | Broj elemenata koji se šalju/primaju |
| `datatype` | `MPI_Datatype` | Tip podataka |
| `rank` | `int` | Rang root procesa koji emituje podatke |
| `comm` | `MPI_Comm` | Komunikator |

> **Napomena:** Isti poziv za sve procese — MPI automatski razlikuje ko šalje a ko prima na osnovu ranga.

---

### `MPI_Scatter`

```c
int MPI_Scatter(void *send_buffer, int send_count, MPI_Datatype send_type,
                void *recv_buffer, int recv_count, MPI_Datatype recv_type,
                int rank, MPI_Comm comm)
```

**Opis:** Root proces deli `send_buffer` na jednake delove i svaki proces (uključujući root) dobija jedan deo. Suprotno od `MPI_Gather`.

| Parametar | Tip | Opis |
| ----------- | ----- | ------ |
| `send_buffer` | `void *` | Bаfer sa svim podacima koje root raspodeljuje (relevantan samo za root) |
| `send_count` | `int` | Broj elemenata koji se šalju **svakom procesu** (ne ukupno!) |
| `send_type` | `MPI_Datatype` | Tip podataka koji se šalju |
| `recv_buffer` | `void *` | Bаfer u koji svaki proces prima svoju porciju |
| `recv_count` | `int` | Broj elemenata koje prima svaki proces |
| `recv_type` | `MPI_Datatype` | Tip podataka koji se primaju |
| `rank` | `int` | Rang root procesa koji vrši raspodelu |
| `comm` | `MPI_Comm` | Komunikator |

> **Napomena:** `send_count` je po procesu, ne ukupno. Ukupno = `send_count × broj_procesa`.

---

### `MPI_Gather`

```c
int MPI_Gather(void *send_buffer, int send_count, MPI_Datatype send_type,
               void *recv_buffer, int recv_count, MPI_Datatype recv_type,
               int rank, MPI_Comm comm)
```

**Opis:** Svaki proces šalje svoja lokalna data root procesu, koji ih spaja u jedan niz u `recv_buffer`. Suprotno od `MPI_Scatter`.

| Parametar | Tip | Opis |
| ----------- | ----- | ------ |
| `send_buffer` | `void *` | Lokalni podaci svakog procesa koji se šalju root-u |
| `send_count` | `int` | Broj elemenata koje svaki proces šalje |
| `send_type` | `MPI_Datatype` | Tip podataka koji se šalju |
| `recv_buffer` | `void *` | Bаfer u koji root upisuje sve primljene podatke (relevantan samo za root) |
| `recv_count` | `int` | Broj elemenata primljenih od svakog pojedinačnog procesa |
| `recv_type` | `MPI_Datatype` | Tip podataka koji se primaju |
| `rank` | `int` | Rang root procesa koji prima sve podatke |
| `comm` | `MPI_Comm` | Komunikator |

> **Napomena:** `recv_buffer` kod root-a mora biti dovoljno velik: `recv_count × broj_procesa` elemenata.

---

## Osnovni tipovi podataka

| MPI tip | C tip |
| --------- | ------- |
| `MPI_CHAR` | `signed char` |
| `MPI_SHORT` | `signed short int` |
| `MPI_INT` | `signed int` |
| `MPI_LONG` | `signed long int` |
| `MPI_UNSIGNED_CHAR` | `unsigned short char` |
| `MPI_UNSIGNED_SHORT` | `unsigned short int` |
| `MPI_UNSIGNED` | `unsigned int` |
| `MPI_UNSIGNED_LONG` | `unsigned long int` |
| `MPI_FLOAT` | `float` |
| `MPI_DOUBLE` | `double` |
| `MPI_LONG_DOUBLE` | `long double` |
| `MPI_BYTE` | `none` |
| `MPI_PACKED` | `none` |

## Slozeni tipovi podataka

| MCI Tip | Struktura | Opis |
| ----- | ----------- | ------ |
| `MPI_FLOAT_INT` | `float` + `int` | float vrednost + int indeks |
| `MPI_DOUBLE_INT` | `double` + `int` | double vrednost + int indeks |
| `MPI_LONG_INT` | `long` + `int` | long vrednost + int indeks |
| `MPI_2INT` | `int` + `int` | int vrednost + int indeks |
| `MPI_SHORT_INT` | `short` + `int` | short vrednost + int indeks |
| `MPI_LONG_DOUBLE_INT` | `long double` + `int` | long double vrednost + int indeks |

> **Napomena:** MPI_DOUBLE_INT i MPI_2INT su specijalni složeni tipovi koji se koriste uz MPI_MINLOC i MPI_MAXLOC operacije.

## Operacije (MPI_Op)

| Operacija | Opis |
| ----------- | ------ |
| `MPI_MAX` | Maksimum |
| `MPI_MIN` | Minimum |
| `MPI_SUM` | Sabiranje |
| `MPI_PROD` | Množenje |
| `MPI_LAND` | Logičko AND |
| `MPI_BAND` | Bitovsko AND |
| `MPI_LOR` | Logičko OR |
| `MPI_BOR` | Bitovsko OR |
| `MPI_LXOR` | Logičko XOR |
| `MPI_BXOR` | Bitovsko XOR |
| `MPI_MINLOC` | Globalni minimum + indeks procesa koji ga sadrži |
| `MPI_MAXLOC` | Globalni maksimum + indeks procesa koji ga sadrži |
