<!-- markdownlint-disable MD024 -->

# MPI — Skripta za ispit

> **Cilj:** pročitaš samo ovu skriptu → umeš da rešiš svaki MPI zadatak sa ispita.
> Kompletni programi u skripti su testirani (kompajlirani i pokrenuti); skeletovi su
> fragmenti koje prilagođavaš zadatku. Rešenja svih rokova su u
> [`Blanketi/`](Blanketi/) — ovde je **kako se dolazi do njih**.

**Kako se koristi:** čitaj redom (sekcije 1–2 su temelj, 3–7 su tipovi zadataka,
8–12 su ispitna taktika). Svaka sekcija sa zadacima ima *zadatak za samostalan rad* —
uradi ga bez gledanja rešenja, pa uporedi sa fajlom u `Blanketi/`.

**Kompajliranje i pokretanje** (lokalno, `mpicc`/`mpirun` su instalirani):

```bash
mpicc zadatak.c -o zadatak
mpirun -np 4 ./zadatak     # -np = broj procesa
```

---

## Sadržaj

1. [Minimum teorije — funkcije koje su ti dovoljne](#1-minimum-teorije)
2. [Univerzalni skelet — recept za 90% zadataka](#2-univerzalni-skelet)
3. [Tip 1 — Ciklična raspodela petlje](#3-tip-1--ciklična-raspodela-petlje)
4. [Tip 2 — Matrica × vektor](#4-tip-2--matrica--vektor)
5. [Tip 3 — Matrica × matrica (4 varijante)](#5-tip-3--matrica--matrica)
6. [Tip 4 — Stablo / hiperkub](#6-tip-4--stablo--hiperkub)
7. [Specijalni zadaci](#7-specijalni-zadaci)
8. [Konverzija a) → b): grupne operacije → P-to-P](#8-konverzija-a--b)
9. [Algoritam rešavanja na ispitu](#9-algoritam-rešavanja-na-ispitu)
10. [Najčešće greške](#10-najčešće-greške)
11. [Master tabela svih rokova](#11-master-tabela-svih-rokova)
12. [Plan vežbanja](#12-plan-vežbanja)

---

## 1. Minimum teorije

### Model za 30 sekundi

MPI program = **isti kod** koji se pokreće kao **p nezavisnih procesa** (nema deljene
memorije — procesi komuniciraju samo porukama). To se zove **SPMD**
(*Single Program, Multiple Data*). Procesi se razlikuju po **rangu** (0, 1, ..., size−1).
Svaki kod na ispitu počinje i završava se ovako:

```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size;
    MPI_Init(&argc, &argv);                          // MORA prva
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);            // ko sam ja? (0..size-1)
    MPI_Comm_size(MPI_COMM_WORLD, &size);            // koliko nas ima?

    /* ... kod zadatka ... */

    MPI_Finalize();                                  // MORA poslednja
    return 0;
}
```

`MPI_COMM_WORLD` = komunikator koji obuhvata sve procese (jedina koji koristiš na ispitu).

### Dve porodice komunikacije

| | **P-to-P** (point-to-point) | **Grupne** (kolektivne) |
|---|---|---|
| Učesnici | dva procesa | **SVI** procesi u komunikatoru |
| Funkcije | `MPI_Send`, `MPI_Recv` | `Bcast`, `Scatter`, `Gather`, `Reduce`, `Scan`, `Barrier` |
| Ko mora da zove | ta dva koja komuniciraju | **svaki proces, isti redosled** |
| Tag | ima (razlikuje poruke) | nema |

Zadaci na ispitu dolaze u varijantama **a)** = grupne operacije i **b)** = P-to-P.
Konkretno: *b) je skoro uvek isti zadatak kao a), samo sa zamenjenim grupnim operacijama*
(sekcija 8).

### P-to-P: `MPI_Send` / `MPI_Recv`

```c
MPI_Send(bafer, broj, TIP, dest, tag, MPI_COMM_WORLD);
MPI_Recv(bafer, broj, TIP, source, tag, MPI_COMM_WORLD, &status);
```

- `dest`/`source` — rang procesa sa kojim se komunicira.
- `tag` — oznaka poruke; `Send` i `Recv` se uparuju po **(source, tag)**.
  Jedan `Recv` primi **jednu** poruku.
- `bafer` mora biti **neprekidan** niz u memoriji (zato se kolone matrice
  *pakuju* u pomoćni niz pre slanja — vidi Tip 2).
- `MPI_Recv` se **blokira** dok poruka ne stigne. `MPI_Send` se "vrati" kad je
  poruka bezbedno poslata (ne mora da znači primljena).

> **Deadlock:** ako dva procesa oba pozovu `Send` (ili oba `Recv`) jedan ka drugom pre
> prima — zaglave se. Pravilo: **jedan šalje, drugi prima u istom trenutku**
> (npr. parni šalju → neparni primaju). Zato u rešenjima uvek stoji
> `if (rank % 2 == 0) Send... else Recv...`.

### Grupne operacije — sve što treba

```
Bcast:     root ──isti podatak──> sve
Scatter:   root ──po deo niza──> svakom po jedan (i root dobija svoj)
Gather:    svi ──po deo──────> root (slože se redom po rangu)
Reduce:    svi ──vrednosti──> root: rezultat operacije (SUM/MAX/MIN/PROD...)
Scan:      kao Reduce, ali proces i dobija rezultat od procesa 0..i
Barrier:   koordinacija — niko ne ide dalje dok svi ne stignu
```

```c
MPI_Bcast (b, n, TIP, root, COMM);
MPI_Scatter(send_b, n_po_procesu, TIP, recv_b, n_po_procesu, TIP, root, COMM);
MPI_Gather (send_b, n_po_procesu, TIP, recv_b, n_po_procesu, TIP, root, COMM);
MPI_Reduce (&lokalno, &rezultat, n, TIP, OPERACIJA, root, COMM);
```

**Zamke:**
- `send_count` kod Scattera/Gathera je broj elemenata **po procesu**, ne ukupan.
- Kod `Reduce` rezultat ima smisla samo u root procesu.
- Svaku grupnu operaciju **mora pozvati svaki proces** — ista funkcija, isti redosled,
  inače deadlock/nedefinisano ponašanje. (Nema "ako sam root onda Bcast"!)

**Tabela operacija:** `MPI_SUM`, `MPI_PROD`, `MPI_MAX`, `MPI_MIN`, `MPI_LAND/LOR`,
`MPI_BAND/BOR/BXOR`, `MPI_MAXLOC`, `MPI_MINLOC`.

**Tipovi:** `MPI_INT`, `MPI_DOUBLE`, `MPI_FLOAT`, `MPI_CHAR` i **`MPI_2INT`**
(dva int-a — ključan za sledeće).

### NAJVAŽNIJI MEHANIZAM ISPITA: MINLOC / MAXLOC — "ko štampa"

Skoro svaki zadatak traži: *"prikaži rezultat u procesu koji sadrži maksimum/minimum"*.
MPI to rešava u tri koraka preko strukture **vrednost + rang**:

```c
struct { int value; int rank; } in, out;   // MPI_2INT = (int, int)!

in.value = lokalni_maksimum;               // šta sam JA izračunao
in.rank  = rank;

MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, 0, MPI_COMM_WORLD);
//  ^ out.value = globalni maksimum, out.rank = KO GA IMA
MPI_Bcast(&out, 1, MPI_2INT, 0, MPI_COMM_WORLD);
//  ^ sada SVI znaju ko štampa (zato što se finalni rezultat šalje baš u taj proces)
```

- `MPI_MAXLOC` = maksimum vrednosti + rang procesa koji ga drži; `MPI_MINLOC` = minimum.
- Inicijalizacija pre petlje traganja: `in.value = INT_MIN` za max
  (`INT_MAX` za min) iz `<limits.h>`, ili prvi element koji proces svakako ima.
- Konačno sakupljanje ide **u `out.rank`**, ne u 0:
  `MPI_Reduce(..., out.rank, ...)` / `MPI_Gather(..., out.rank, ...)`
  i na kraju `if (rank == out.rank) { printf... }`.

**Zašto dva koraka (Reduce pa Bcast)?** Zato što root (obično 0) samo *sazna* ko je
pobednik — a svi ostali procesi moraju znati kome da šalju svoje parcijale. Zato se
`out` emituje svima. **Ovo je najčešći izvor grešaka na ispitu** — zapamti redosled:
**`Reduce(LOC)` → `Bcast(out)` → `sakupljanje(u out.rank)`**.

---

## 2. Univerzalni skelet

**Svaki zadatak Tip 1/2/3 (a varijanta) je isti scenario u 5 faza:**

```
┌──────────────────────────────────────────────────────────────┐
│ 1. RASPODELA   root deli podatke (Scatter/Bcast ili P-to-P)  │
│ 2. LOKALNI RAČUN   svaki proces računa svoje parcijale       │
│    + lokalni ekstrem u struct {value, rank}                  │
│ 3. Reduce(MINLOC/MAXLOC) u root  → ko je pobednik?           │
│ 4. Bcast(out) → svi znaju out.rank                           │
│ 5. SAKUPLJANJE rezultata u out.rank + štampa                 │
└──────────────────────────────────────────────────────────────┘
```

Razlike između zadataka su samo **parametri** ovog skeleta:

| Faza | Šta varira |
|---|---|
| Raspodela | kolone/vrste matrice? cela matrica? vektor? po koliko? |
| Lokalni račun | šta se akumulira (suma, proizvod, max...) i čiji se ekstrem traži |
| Sakupljanje | **`Gather` ili `Reduce(SUM)`?** (vidi pravilo ispod) |

### Pravilo: `Gather` ili `Reduce(SUM)` za rezultujuću matricu/vektor?

Pitaj se: **da li se elementi rezultata PREKLAPAJU među procesima?**

- **Disjunktni delovi** (svaki proces izračuna *različite vrste/kolone* C)
  → **`MPI_Gather`** — delovi se samo slože jedan pored drugog.
  *(Tip 3b: vrste C · Tip 3c: kolone C)*
- **Delimični doprinosi istim elementima** (svaki proces ima *parcijalnu sumu* za
  svaki element C) → **`MPI_Reduce(MPI_SUM)`**.
  *(Tip 1: suma · Tip 2: vektor c · Tip 3a i 3d: cela C)*

> Provera zdravog razuma: ako bi dva procesa mogla da "sabraju" isti element C,
  treba `Reduce(SUM)`. Ako svaki element C živi tačno u jednom procesu — `Gather`.

### Pakovanje kolona matrice za slanje

C matrice su **po vrstama** u memoriji — vrsta je neprekidna, kolona NIJE.
`MPI_Send` šalje samo neprekidne nizove, zato se kolona/kolone **pakuju**:

```c
// root šalje procesu p kolone p*q .. p*q+q-1 matrice a[rows][cols]:
int tmp[rows][q];
for (int i = 0; i < rows; i++)
    for (int j = 0; j < q; j++)
        tmp[i][j] = a[i][p * q + j];        // pakuj u pomoćnu
MPI_Send(tmp, rows * q, MPI_INT, p, 0, MPI_COMM_WORLD);
```

Primalac čita iz `local_a` normalno po vrstama. (Vrste se ne pakuju —
`MPI_Scatter(a, s*m, ...)` ih deli direktno iz matrice, jer su po vrstama već
neprekidne. Zadaci to eksplicitno traže: *"vrste direktno iz matrice,
elementi odjednom"*.)

---

## 3. Tip 1 — Ciklična raspodela petlje

**Pojavljuje se:** Jun 2020, Januar 2022, Decembar 2022, Septembar 2022
(svaki kao a) + b) par).

### Tekst (generički)

> Realizovati dvostruku petlju `for(i) for(j) s += i + j` **ravnomernom cikličnom
> raspodelom** među p procesa. **Nije dozvoljeno korišćenje indeksiranih
> promenljivih.** Rezultat prikazati u procesu sa najmanjim/najvećim brojem
> sabiraka koji su prosti brojevi.

### Ključna ideja

Dvostruka petlja ima `N × JN` iteracija. Numeriši ih jednim brojem
`t = 0, 1, 2, ...` i dodeli ih **ciklično**: proces `rank` radi iteracije
`t = rank, rank+size, rank+2·size, ...` (zato "bez indeksiranih promenljivih" —
ne smeš da podeliš po blokovima `for(i = rank*N/size; ...)`, nego baš ciklično).
U svakoj iteraciji **rekonstruišeš** `i` i `j` iz `t`:

```
t  →  i = t / JN  + pomak_i     (celobrojno deljenje — "koja vrsta")
   →  j = B + t % JN            (uzlazni j)  ili  B − t % JN  (silazni j)
```

### Formule (nauči napamet — uvek su iste)

| Situacija | `JN` (koliko vrednosti uzima j) |
|---|---|
| `j++` od B, uslov `j < C` | `C − B` |
| `j++` od B, uslov `j <= C` | `C − B + 1` |
| `j--` od B, uslov `j > C` | `B − C` |
| `j--` od B, uslov `j >= C` | `B − C + 1` |

Ukupno iteracija: `t < N * JN` (N = broj vrednosti i).

**Primer (Decembar 2022):** `for (i = y; i < y + N; i++) for (j = N; j >= 0; j--)`
→ `JN = N − 0 + 1 = N+1`, pa je `i = t/(N+1) + y`, `j = N − t%(N+1)`, ukupno `N*(N+1)`.

### Kompletan primer — Jun 2020 a) (testirano, radi)

```c
#include <stdio.h>
#include <mpi.h>
#define N 4

int is_prime(int n)
{
    if (n < 2)
        return 0;
    for (int i = 2; i * i <= n; i++)   // OBAVEZNO <= (ne <) — 4, 9, 25 su KVADRATI
        if (n % i == 0)
            return 0;
    return 1;
}

int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    int local_sum = 0;
    struct { int value; int rank; } in, out;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int JN = N;                                  // j ide 0..N-1
    in.value = 0;                                // broj MOJIH prostih sabiraka
    in.rank = rank;

    for (int t = rank; t < N * JN; t += size)    // t += size je OBAVEZAN!
    {
        int i = t / JN;                          // rekonstrukcija i
        int j = t % JN;                          // rekonstrukcija j (uzlazno)
        local_sum += i + j;
        if (is_prime(i + j))
            in.value++;
    }

    MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, MPI_COMM_WORLD);
    MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

    int suma = 0;
    MPI_Reduce(&local_sum, &suma, 1, MPI_INT, MPI_SUM, out.rank, MPI_COMM_WORLD);

    if (rank == out.rank)
        printf("Suma = %d | proces %d ima najmanje prostih sabiraka: %d\n",
               suma, out.rank, out.value);

    MPI_Finalize();
    return 0;
}
```

Pokretanje: `mpirun -np 3 ./a.out` → `Suma = 48 | proces 1 ima najmanje prostih sabiraka: 0`.

### Za b) varijantu

Tri grupne operacije se zamenjuju šablonima iz sekcije 8 (MINLOC ručno, Bcast ručno,
SUMA ručno — sve tri meta su `out.rank`, pazi!).

### Samostalan rad

1. Napiši b) varijantu gornjeg (uputi: sekcija 8, šabloni 1–3), uporedi sa
   [`Blanketi/2020/jun_b.c`](Blanketi/2020/jun_b.c) — ali pazi, taj fajl ima bug
   `i*i < n` u `is_prime` (tabela u sekciji 10)!
2. Reši Decembar 2022 a) od nule (pomak `y`, silazni `j`, **max** prostih umesto min),
   pa uporedi sa [`Blanketi/2022/decembar_a.c`](Blanketi/2022/decembar_a.c).

---

## 4. Tip 2 — Matrica × vektor

**Pojavljuje se:** April 2021, Jun 2021, Decembar 2021, April 2022, April 2025.

### Tekst (generički)

> Množi se matrica `A` i vektor `b`, prikazuje rezultujući vektor `c`. Takođe se
> traži maksimum/minimum elemenata `A` i suma/proizvod elemenata svake vrste.
> Root šalje svakom procesu **po q kolona matrice A** (P-to-P, svi elementi
> odjednom) i **po q elemenata vektora b**. Rezultat se prikazuje u procesu koji
> sadrži maksimum/minimum **posle raspodele**.

### Zašto baš kolone?

Zato što se `c[i] = Σ a[i][j]·b[j]` prirodno paralelizuje po kolonama: proces koji
drži kolone `j1..j2` i elemente `b[j1..j2]` računa **parcijalni doprinos** svakom
`c[i]` — pa se na kraju **sabira** (`Reduce(SUM)`). Istovremeno u istoj petlji prolazi
i suma/proizvod vrsta (koji se takođe sabira/množi preko procesa) i ekstrem
(`MAXLOC/MINLOC`).

### Skelet

```c
#define n VRSTE
#define q KOLONE_PO_PROCESU          // size*q = broj kolona A

int local_a[n][q], local_b[q], local_c[n], local_row[n];
struct { int value; int rank; } in, out;

/* --- 1. RASPODELA --- */
if (rank == root)
{
    for (int i = 0; i < n; i++)                       // sebi
        for (int j = 0; j < q; j++)
            local_a[i][j] = a[i][j];
    int tmp[n][q];
    for (int p = 1; p < size; p++)                    // ostalima
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < q; j++)
                tmp[i][j] = a[i][p * q + j];          // PAKOVANJE kolona
        MPI_Send(tmp, n * q, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
}
else
    MPI_Recv(local_a, n * q, MPI_INT, root, 0, MPI_COMM_WORLD, &status);

MPI_Scatter(b, q, MPI_INT, local_b, q, MPI_INT, root, MPI_COMM_WORLD);

/* --- 2. LOKALNI RAČUN (sve u jednoj dvostrukoj petlji!) --- */
in.value = local_a[0][0]; in.rank = rank;
for (int i = 0; i < n; i++)
{
    local_c[i] = 0;
    local_row[i] = 0;                                 // ili 1 za PROIZVOD
    for (int j = 0; j < q; j++)
    {
        local_c[i]   += local_a[i][j] * local_b[j];   // parcijalni c
        local_row[i] += local_a[i][j];                // parcijalna suma vrste
        if (local_a[i][j] > in.value)                 // < za minimum
            in.value = local_a[i][j];
    }
}

/* --- 3/4. KO ŠTAMPA --- */
MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, MPI_COMM_WORLD);
MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

/* --- 5. SAKUPLJANJE u out.rank: PREKLAPANJE → sve ide Reduce --- */
MPI_Reduce(local_c,   c,   n, MPI_INT, MPI_SUM,  out.rank, MPI_COMM_WORLD);
MPI_Reduce(local_row, row, n, MPI_INT, MPI_SUM,  out.rank, MPI_COMM_WORLD);
// (za proizvod vrsta: MPI_PROD, a local_row[i] = 1 na početku)

if (rank == out.rank)
    /* štampa c, out.value/out.rank, row[] */;
```

### Varijante po rokovima

| Rok | Po kolono | Ekstrem u A | Operacija po vrstama | Ko štampa |
|---|---|---|---|---|
| April 2021 | **1** | **min** | **proizvod** | proces sa min |
| Jun 2021 | `l` | max | suma | proces sa max |
| Decembar 2021 = April 2022 | `q` | max | suma | proces sa max |
| April 2025 | `s` | max | suma | proces sa max |

### Samostalan rad

Reši April 2022 a) od nule (q kolone, max, suma), pa uporedi sa
[`Blanketi/2022/april_a.c`](Blanketi/2022/april_a.c). Zatim napiši b) varijantu
(sve tri Reduce zamene + Scatter b se menja petljom Send) i uporedi sa
[`Blanketi/2022/april_b.c`](Blanketi/2022/april_b.c).

---

## 5. Tip 3 — Matrica × matrica

**Pojavljuje se:** Septembar 2021, Jun 2 2022/2023, Oktobar 2 2022, Septembar 2023,
Januar 2025, Jun 2025 a, April 2026 a — **najzastupljeniji tip**.

### Četiri varijante — cela poenta ispita

Pitanje koje određuje sve: **šta svaki proces dobija?**

| Varijanta | Proces dobija | Lokalno računa | C se sakuplja | Primer roka |
|---|---|---|---|---|
| **3a** | `q` kolona A + `q` vrsta B | parcijalne doprinose **celoj** C | `Reduce(SUM)` | Jun 2 2022/23, Jun 2025 a |
| **3b** | vrste A (`Scatter`) + **celu** B (`Bcast`) | **različite vrste** C | **`Gather`** | Sept 2021, Okt 2 2022, Sept 2023, Jan 2025 |
| **3c** | **celu** A (`Bcast`) + kolone B (P-to-P) | **različite kolone** C | **`Gather`** | April 2026 a |
| **3d** | **1** kolonu A + **1** vrstu B | spoljašnji proizvod → doprinos celoj C | `Reduce(SUM)` | Oktobar 2022 b |

**Zašto?** Matrično množenje je `C[i][j] = Σ_l A[i][l]·B[l][j]` — zbir po `l`.
Suma se može seći po `l` (onda svaki proces doprinese svakom elementu → 3a, 3d),
ili se matrice seku po vrstama A / kolonama B (onda delovi C ne zavise jedni od
drugih → 3b, 3c → Gather).

### Tip 3a — `q` kolona A (P-to-P) + `q` vrsta B (Scatter)

Proces p drži kolone `A[·][p*q .. p*q+q-1]` i vrste `B[p*q .. p*q+q-1]` — njihov
proizvod je tačno deo sume po `l` za **svaki** element C:

```c
int local_a[k][q], local_b[q][n], local_c[k][n];   // A je k×m, B je m×n, q*size = m

/* raspodela kolona A: ista petlja pakovanja kao Tip 2 (samo k elemenata po koloni) */
/* raspodela vrsta B: vrste su po memoriji već neprekidne → direktno iz matrice: */
MPI_Scatter(b, q * n, MPI_INT, local_b, q * n, MPI_INT, root, MPI_COMM_WORLD);

for (int i = 0; i < k; i++)
    for (int j = 0; j < n; j++)
    {
        local_c[i][j] = 0;
        for (int l = 0; l < q; l++)
            local_c[i][j] += local_a[i][l] * local_b[l][j];   // doprinos mom bloku l
    }

/* C se sabira: */ MPI_Reduce(local_c, c, k * n, MPI_INT, MPI_SUM, ..., MPI_COMM_WORLD);
```

Uz ekstrem (Jun 2 2022: max u B) ide standardna `struct` + `MAXLOC` + `Bcast` mašinerija
i finalne redukcije idu u `out.rank`. Jun 2025 a je ista stvar **bez ekstrema** —
sve ide prosto u root.

### Tip 3b — vrste A (`Scatter`) + cela B (`Bcast`) → vrste C → `Gather`

```c
int local_a[s][m], local_c[s][n];                   // A je k×m, s*size = k

MPI_Scatter(a, s * m, MPI_INT, local_a, s * m, MPI_INT, root, MPI_COMM_WORLD);
MPI_Bcast(b, m * n, MPI_INT, root, MPI_COMM_WORLD); // svi dobijaju celu B

for (int i = 0; i < s; i++)
    for (int j = 0; j < n; j++)
    {
        local_c[i][j] = 0;
        for (int l = 0; l < m; l++)
            local_c[i][j] += local_a[i][l] * b[l][j];   // potpune vrste — pun zbir po l
        /* ovde ide i traganje za ekstremom u C (ili u local_a za ekstrem u A) */
    }

/* Sekundarna operacija (npr. proizvod kolona A): svaki proces ima SVE kolone
   (u svojim vrstama) → parcijalni proizvodi → MPI_PROD redukcija: */
MPI_Reduce(local_col_prod, col_prod, m, MPI_INT, MPI_PROD, out.rank, MPI_COMM_WORLD);

/* C: DISJUNKTNE vrste → Gather (blok procesa p završi na svom mestu): */
MPI_Gather(local_c, s * n, MPI_INT, c, s * n, MPI_INT, out.rank, MPI_COMM_WORLD);
```

**Kompletan testirani primer:** [`Blanketi/2022/oktobar_2_a.c`](Blanketi/2022/oktobar_2_a.c)
(s vrsta, cela B, proizvod kolona A, max u C) — pročitaj ga sa skeletom pored sebe.

> Napomena: tekstovi 3b zadataka znaju da traže "P-to-P za vrste A" — rešenja u repu
> ipak koriste `MPI_Scatter` (ispravno jer je `k` deljivo sa `s·size`); P-to-P slanje
> vrsta se radi u b) varijantama. Ako tekst eksplicitno traže P-to-P u a) — uradi
> petlju Send/Recv kao za kolone, samo bez pakovanja (vrste su već neprekidne).

### Tip 3c — cela A (`Bcast`) + kolone B (P-to-P) → kolone C → `Gather

```c
MPI_Bcast(a, n * k, MPI_INT, root, MPI_COMM_WORLD);   // svi dobijaju celu A
/* kolone B: pakovanje kao u Tipu 2 → local_b[k][s] */

for (int i = 0; i < n; i++)
    for (int j = 0; j < s; j++)
    {
        local_c[i][j] = 0;
        for (int l = 0; l < k; l++)
            local_c[i][j] += a[i][l] * local_b[l][j];  // RAZLIČITE KOLONE C
    }

MPI_Gather(local_c, n * s, MPI_INT, c, n * s, MPI_INT, out.rank, MPI_COMM_WORLD);
```

> **Zamka (April 2026 a):** `Gather` slaže blokove **po rangu** — dobiješ matricu čije
> su kolone pomešane (blok lokal_c procesa 2 stane "gde bi bile njegove vrste").
> Ako se traži štampa cele C, posle Gather-a je **presloži**: element
> `c[i][mesto]` ide na mesto `c[i][p*s + j]` (isto pakovanje kao pri slanju, unazad).
> Operacije "po vrstama C" (npr. min vrste) računaj **nakon** preslaganja.

### Tip 3d — 1 kolona A + 1 vrsta B = spoljašnji proizvod

Trik: matricu `C = A×B` možemo pisati kao **zbir spoljašnjih proizvoda**
`C = Σ_l (kolona_l A) ⊗ (vrsta_l B)`, gde je `⊗` spoljašnji proizvod:
`local_c[i][j] = local_a[i] * local_b[j]`. Proces l dobije tačno jedan sabirak
te sume:

```c
int local_a[k], local_b[m], local_c[k][m];       // size = unutrašnja dimenzija!

/* kolona A se šalje P-to-P (paket od k elemenata), vrsta B preko Scatter: */
MPI_Scatter(b, m, MPI_INT, local_b, m, MPI_INT, root, MPI_COMM_WORLD);

for (int i = 0; i < k; i++)
    for (int j = 0; j < m; j++)
        local_c[i][j] = local_a[i] * local_b[j];  // nema unutrašnje petlje!

MPI_Reduce(local_c, c, k * m, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
```

### Samostalan rad

1. Jun 2 2022 (3a): [`Blanketi/2022/jun_2.c`](Blanketi/2022/jun_2.c) — probaj prvo
   sam, bez gledanja.
2. Januar 2025 (3b sa novim detaljima — **suma kolona B**, ekstrem **max u A**):
   [`Blanketi/2025/januar_a.c`](Blanketi/2025/januar_a.c).
3. April 2026 a (3c + preslaganje posle Gathera):
   [`Blanketi/2026/april_a.c`](Blanketi/2026/april_a.c) — ima poznate bugove iz
   sekcije 10, nađi ih pre nego što pogledaš tabelu.

---

## 6. Tip 4 — Stablo / hiperkub

**Pojavljuje se:** Oktobar 2022 a (i kao tema vežbi `MPI_1_3/1_4`).

### Tekst

> Korišćenjem **P-to-P** komunikacije poslati podatak iz procesa 0 svim ostalim.
> Broj procesa je **stepen dvojke**. Komunikacija u **log₂(p) koraka**.

### Ideja

Lanac (P0→P1→P2→...) traži `p−1` uzastopnih slanja. Ako onaj ko dobije podatak
**odmah prosledi dalje**, broj procesa koji znaju podatak se **udvostručuje**
svakog koraka: 1 → 2 → 4 → ... → p, ukupno **log₂(p)** koraka.

```
Korak 1:  P0 → P1
Korak 2:  P0 → P2   P1 → P3
Korak 3:  P0 → P4   P1 → P5   P2 → P6   P3 → P7
```

U koraku sa udaljenošću `step` (1, 2, 4, ...): procesi `rank < step` **već znaju**
i šalju partneru `rank + step`; procesi `step ≤ rank < 2·step` **primaju** od
`rank − step`; ostali **čekaju**.

### Kompletan primer — Oktobar 2022 a) (testirano, radi)

```c
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[])
{
    int rank, size, data, root = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == root)
        data = 42;

    for (int step = 1; step < size; step *= 2)
    {
        if (rank < step)                          // već zna podatak
            MPI_Send(&data, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD);
        else if (rank < 2 * step)                 // prima baš u ovom koraku
            MPI_Recv(&data, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        /* rank >= 2*step: ne učestvuje, čeka sledeći korak */
    }

    printf("Proces %d ima vrednost: %d\n", rank, data);
    MPI_Finalize();
    return 0;
}
```

> Jedna tipografska greška (`MI_INT`, zaboravljen `&`, pogrešan tag) = pad programa
> ili nula poena. **Uvek kompajliraj i pokreni pre nego što smatraš zadatak gotovim.**

> **Inverzija (redukcija u stablu):** isti obrazac sabira sve → P0: procesi sa
> `rank % (2*step) == step` šalju svoju parcijalnu sumu ka `rank − step`, primalac
> (`rank % (2*step) == 0`) sabira. To je vežba `Vezbe/MPI_1_3.c` i srž onoga
> što `MPI_Reduce` radi interno — često teorijsko pitanje ("zašto log₂(p) koraka?").

### Samostalan rad

Uporedi sa [`Blanketi/2022/oktobar_a.c`](Blanketi/2022/oktobar_a.c) i
[`Blanketi/2022/oktobar_b.c`](Blanketi/2022/oktobar_b.c) (b = Tip 3d istog roka —
dva **različita** zadatka, ne par!).

---

## 7. Specijalni zadaci

Ponekad se pojavi zadatak van tipova — uvek se svodi na iste mehanizme:

### Jun 2025 b) — Bcast + formula + Reduce

> Proces 2 ima niz `X[n]`; emituje ga svima (`MPI_Bcast` sa **root = 2**!).
> Svaki proces računa `y_i = (p(p+1)/2) · x_i` gde je **p = ukupan broj procesa**
> (`size`, NE `rank`!), pa se `Y` sabere u root.

```c
MPI_Bcast(x, n, MPI_INT, 2, MPI_COMM_WORLD);          // root ne mora biti 0!
for (int i = 0; i < n; i++)
    y[i] = size * (size + 1) / 2 * x[i];
MPI_Reduce(y, rez, n, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
```

Lekcija: root može biti bilo koji rang (ovde 2); `Reduce(SUM)` preko procesa sabira
`size` identičnih vrednosti — otuda `p(p+1)/2` mora da koristi **size**.

### April 2026 b) — Kružna razmena (prsten)

> Svaki proces šalje niz sledećem `(rank+1)%size` i prima od prethodnog
> `(rank-1+size)%size`.

```c
int sledeci  = (rank + 1) % size;
int prethodni = (rank - 1 + size) % size;   // +size da ne ode u minus!

if (rank % 2 == 0)                    // izbegavanje deadlocka u prstenu:
{
    MPI_Send(bafer, n, MPI_INT, sledeci, 0, MPI_COMM_WORLD);
    MPI_Recv(bafer, n, MPI_INT, prethodni, 0, MPI_COMM_WORLD, &status);
}
else
{
    MPI_Recv(bafer, n, MPI_INT, prethodni, 0, MPI_COMM_WORLD, &status);
    MPI_Send(bafer, n, MPI_INT, sledeci, 0, MPI_COMM_WORLD);
}
```

Lekcija: **`(rank ± 1) % size`** je formula za prsten; u prstenu svi šalju svima
→ **parni prvo šalju, neparni prvo primaju** (inače deadlock).

---

## 8. Konverzija a) → b)

b) varijanta = isti zadatak, grupne operacije zamenjene P-to-P.
**Sve zamene su ista ideja:** onaj koji je "root" radi **petlju po procesima**,
ostali rade **jedan** Send ili Recv. Zapamti jedan obrazac — imaš sva četiri.

### Zamena 1 — `MPI_Reduce(LOC)` (traženje pobednika)

```c
/* umesto: MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, ...) */
if (rank != root)
    MPI_Send(&in, 1, MPI_2INT, root, 0, MPI_COMM_WORLD);
else
{
    out = in;                                        // kreni od sebe!
    for (int p = 0; p < size; p++)
    {
        if (p == root) continue;
        MPI_Recv(&in, 1, MPI_2INT, p, 0, MPI_COMM_WORLD, &status);
        if (in.value > out.value) out = in;          // MAXLOC;  < za MINLOC
    }
}
```

### Zamena 2 — `MPI_Bcast(out)`

```c
if (rank == root)
    for (int p = 0; p < size; p++)
    {
        if (p == root) continue;
        MPI_Send(&out, 1, MPI_2INT, p, 0, MPI_COMM_WORLD);
    }
else
    MPI_Recv(&out, 1, MPI_2INT, root, 0, MPI_COMM_WORLD, &status);
```

### Zamena 3 — `MPI_Reduce(SUM/PROD, target)` skalar

```c
if (rank != target)                                   // target = out.rank!
    MPI_Send(&local_x, 1, MPI_INT, target, 0, MPI_COMM_WORLD);
else
{
    x = local_x;
    for (int p = 0; p < size; p++)
    {
        if (p == target) continue;
        MPI_Recv(&local_x, 1, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
        x += local_x;                                 // *= za PROD
    }
}
```

### Zamena 4 — `MPI_Reduce(SUM/PROD, target)` niz

```c
if (rank != target)
    MPI_Send(local_niz, n, MPI_INT, target, 0, MPI_COMM_WORLD);
else
{
    for (int i = 0; i < n; i++) niz[i] = local_niz[i];
    for (int p = 0; p < size; p++)
    {
        if (p == target) continue;
        MPI_Recv(local_niz, n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < n; i++) niz[i] += local_niz[i];   // množi PRIMLJENI!
    }
}
```

### Zamena 5 — `MPI_Scatter` / `MPI_Gather` / `MPI_Bcast` podataka

- **Scatter** (npr. q elemenata vektora b): root u petlji `MPI_Send(&b[p*q], q, ...)`,
  ostali jedan `MPI_Recv`.
- **Scatter vrsta matrice** (`s*m` elemenata): `MPI_Send(&a[p*s][0], s*m, ...)` —
  vrste su neprekidne, šalje se **direktno iz matrice**, bez pakovanja.
- **Bcast matrice**: root u petlji šalje svima `m*n` elemenata (ili po vrstama).
- **Gather**: svi šalju root-u; on prima **po redu** `for (p...) MPI_Recv(&c[p*s][0], ...)`
  — tako se blokovi slože tačno kao Gather (kod kolona: primi u `tmp` i raspakuj).

### ⚠️ Zlatno pravilo konverzije

> Kad je meta `out.rank` (a najčešće jeste — finalni rezultat ide pobedniku):
> **pošiljaoci su svi osim `out.rank`, primalac je samo `out.rank`.**
> Ako napišeš `rank != root` za slanje, a poruke uputiš `out.rank` — **deadlock**
> čim pobednik nije root (tačno bug iz `2021/jun_b.c` i `2023/septembar_b.c`).

---

## 9. Algoritam rešavanja na ispitu

Kad dobiješ zadatak, prođi ovih **10 koraka** po redu (na papiru, pre koda):

1. **Identifikuj tip.** Petlja bez nizova → Tip 1. Matrica×vektor → Tip 2.
   Matrica×matrica → koja od 4 varijante Tipa 3 (šta ko dobija?). Stablo/log₂ → Tip 4.
2. **Ispiši dimenzije i ograničenja.** `size = k/s`? `size = m`? Šta mora biti
   deljivo sa čim? (Odredi koliko procesa pokrećeš.)
3. **Odredi raspodelu** — šta root šalje, kome, koliko elemenata, P-to-P ili grupno?
   (Tekst ti ovo kaže doslovno — samo ga prevedi u pozive.)
4. **Odredi lokalni račun** — koja petlja, šta akumulira. Napiši matematiku
   (`c[i] = Σ ...`) pa prepiši u petlje.
5. **Odredi ekstrem**: šta se traži (max/min), u kojoj matrici, **posle raspodele**?
   → `struct {value, rank}` + `MPI_MAXLOC/MINLOC`.
6. **Odredi ko štampa** → `Reduce(LOC)` u root → `Bcast(out)` → sve finalno ide
   **u `out.rank`**.
7. **Gather ili Reduce(SUM)?** — preklapaju li se elementi rezultata? (sekcija 2)
8. **Napiši kod** — koristi skelet iz sekcije 2 kao kostur.
9. **Prođi checklist grešaka** (sekcija 10).
10. **Mentalno testiraj sa size=2**: rank 0 i rank 1 — ko šta šalje, da li se
    Send/Recv uparuju, da li `out.rank` dobija sve što mu treba za štampu.

**Vremenska taktika:** a) i b) dele isti skelet — prvo skroz završi a), pa mehanički
zameni operacije šablonima iz sekcije 8. Ako zapneš na formulu, napiši ostatak koda
— struktura donosi više poena od jedne petlje.

---

## 10. Najčešće greške

Greške koje su napravljene čak i u rešenjima u repu — **proveri svoj kod protiv
svake** pre predaje:

| # | Provera | Zašto |
|---|---|---|
| 1 | `for (t = rank; t < ukupno; **t += size**)` | bez `t += size` svaki proces radi SVE iteracije (bug u `2022/januar_a.c`) |
| 2 | `is_prime`: uslov je `i * i **<=** n` | sa `<` kvadrati (4, 9, 25...) prolaze kao prosti (`2020/jun_b.c`) |
| 3 | Finalna redukcija ide u **`out.rank`**, ne u `root` | inače štampač nema podatke (`2022/septembar_a.c`) |
| 4 | P-to-P zamena redukcije: pošiljaoci `rank != **out.rank**`, primalac `rank == out.rank` | vezano za `root` → deadlock kad pobednik nije root (`2021/jun_b.c`, `2023/septembar_b.c`) |
| 5 | U redukciji nizova kombinuj **primljeni** bafer | množiti sopstveni = pogrešan rezultat (`2021/septembar_b.c`) |
| 6 | Dimenzije u pozivima: šalješ `l×n` a lokalna matrica je `l×m`? | prekoračenje bafera (`2021/septembar_b.c`) |
| 7 | `Reduce` šalje `local_c`, NE `local_a` | pogrešan bafer + prekoračenje (`2025/april.c`) |
| 8 | Posle `Gather` kolona: **presloži** matricu | blokovi su pomešani (`2026/april_a.c`) |
| 9 | Koeficijent `size*(size+1)/2` koristi **size**, ne rank | (`2025/jun_b.c`) |
| 10 | U lokalnom računu koristi **`local_`** promenljive, ne globalne | globalne su nedefinisane van root-a (`2026/april_a.c`) |
| 11 | `struct { int value; int rank; }` — redosled polja! `MPI_2INT` = (vrednost, rang) | obrnuto = pogrešan pobednik |
| 12 | Inicijalizacija ekstrema: `INT_MIN` za max, `INT_MAX` za min (iz `<limits.h>`) | nula ne valja za negativne/pozitivne matrice |
| 13 | Proizvodi počinju od **1**, ne od 0 | `0 * x = 0` poništava ceo proizvod |
| 14 | Svaki proces zove **sve** grupne operacije (iste, istim redom) | "if (rank==root) Bcast" = deadlock |

**Poslednja provera pre predaje:** prevedi zadatak kroz sekciju 9, korak 10 —
mentalna simulacija sa 2 procesa hvata više bugova nego ponovno čitanje koda.

---

## 11. Master tabela svih rokova

| Rok | Tip | Raspodela | Ekstrem (gde) | Sekundarno | Ko štampa | a/b |
|---|---|---|---|---|---|---|
| Jun 2020 | 1 | ciklična `i,j = 0..N-1` | min prostih sabiraka | — | proces sa min | par |
| Januar 2022 | 1 | = Jun 2020 | min prostih | — | proces sa min | par |
| Decembar 2022 | 1 | pomak `y`, silazni `j` | **max** prostih | — | proces sa max | par |
| Septembar 2022 | 1 | = Decembar 2022 | max prostih | — | proces sa max | par |
| April 2021 | 2 | **1** kolona A + 1 elem b | **min** u A | **proizvod** vrsta | proces sa min | par |
| Jun 2021 | 2 | `l` kolona | max u A | suma vrsta | proces sa max | par |
| Decembar 2021 | 2 | `q` kolona | max u A | suma vrsta | proces sa max | par |
| April 2022 | 2 | `q` kolona | max u A | suma vrsta | proces sa max | par |
| April 2025 | 2 | `s` kolona | max u A | suma vrsta | proces sa max | samo a |
| Jun 2 2022 | 3a | `q` kol A + `q` vrsta B | max u B | proizvod kolona B | proces sa max | jedan fajl |
| Jun 2 2023 | 3a | isto | max u B | proizvod kolona B | proces sa max | jedan fajl |
| Jun 2025 a | 3a | isto | **nema** | proizvod kolona B | **root** | nije par |
| Septembar 2021 | 3b | `l` vrsta A + cela B | max u C | proizvod kolona A | proces sa max | par* |
| Oktobar 2 2022 | 3b | `s` vrsta A + cela B | max u C | proizvod kolona A | proces sa max | par |
| Septembar 2023 | 3b | `r` vrsta A + cela B | **min u A** | proizvod kolona A | proces sa min | par |
| Januar 2025 | 3b | `m` vrsta A + cela B | **max u A** | **suma kolona B** | proces sa max | par |
| Oktobar 2022 a | 4 | hiperkub iz P0 | — | — | svi | nije par |
| Oktobar 2022 b | 3d | 1 kol A + 1 vrsta B | nema | — | root | nije par |
| Jun 2025 b | spec. | `Bcast` X iz **P2**, formula, `Reduce(SUM)` | nema | `y=(p(p+1)/2)·x` | root | nije par |
| April 2026 a | 3c | cela A + `s` kol B (P-to-P) | min u B | min vrsta C (posle Gathera) | proces sa min | nije par |
| April 2026 b | spec. | prsten `(rank±1)%size` | — | — | — | nije par |

\* U `2021/septembar` fajlovima a/b uloge su obrnute (a = P-to-P, b = grupne) —
**proveri tekst zadatka**, ne veruj nazivu fajla.

**Zaključak iz tabele:** ~85% rokova je Tip 1/2/3. Tip 3b ima 4 pojavljivanja,
Tip 2 pet, ciklična petlja četiri. Nauči redom: skelet → Tip 3b → Tip 2 → Tip 1 →
konverzija → ostalo.

---

## 12. Plan vežbanja

### Sezona 1 — skelet + Tip 3b (temelj)

1. Pročitaj sekcije 1–2 ove skripte.
2. Otkucaj (ne prepiši iz repa — napiši iz glave) `2022/oktobar_2_a.c` zadatak:
   Scatter vrsta + Bcast B + proizvod kolona A + MAXLOC u C + Gather.
3. Kompajliraj i pokreni sa 2 i 4 procesa (`mpirun -np 2`).

### Sezona 2 — Tip 2 + Tip 1

1. Pročitaj sekcije 3–4, napiši April 2022 a) pa b) iz glave.
2. Napiši Jun 2020 a) pa b) (Tip 1). Proveri formule za JN na Decembru 2022.

### Sezona 3 — ostale varijante + konverzija

1. Sekcija 5: Jun 2 2022 (3a), Oktobar 2022 b (3d), April 2026 a (3c).
2. Sekcija 6–7: Tip 4 + specijalni.
3. Memoriraj 4 šablona konverzije (sekcija 8) — uzmi bilo koji a) zadatak i
   konvertuj ga bez gledanja.

### Sezona 4 — simulacija ispita

1. Uzmi `2025/januar` tekstove (samo header komentare iz fajlova), zatvori rešenja.
2. Reši oba na vreme (a) ~30 min, b) ~30 min).
3. Prođi checklist iz sekcije 10, tek onda uporedi sa rešenjima.
4. Ponovi sa `2026/april_a` i `2026/april_b`.

### Pokriva li ova skripta sve?

- **Funkcije koje se pojavljuju na ispitima:** `Init/Finalize/rank/size`, `Send/Recv`,
  `Bcast`, `Scatter`, `Gather`, `Reduce`, `Scan` (retko), `MINLOC/MAXLOC` + `MPI_2INT`
  — sve su u sekciji 1. ✓
- Detaljnija referenca potpisa: [`SPISAKFUNKCIJA.md`](SPISAKFUNKCIJA.md).
- Šabloni po rokovima sa varijantama: [`Blanketi/SABLONI.md`](Blanketi/SABLONI.md).
- Za brzo ponavljanje dan uoči: [`Blanketi/SABLONI_MIN.md`](Blanketi/SABLONI_MIN.md).
- Vežbe sa časova (`Vezbe/`, `Lab/`) pokrivaju isto gradivo drugačijim zadacima —
  koristi ih za dodatnu praksu, nisu obavezne za ispit.
