<!-- markdownlint-disable MD024 -->
# MPI Blanketi — Šabloni i Tipovi Zadataka

Na osnovu analize zadataka sa prethodnih rokova, MPI ispitni zadaci se mogu svrstati u **četiri osnovna tipa**. Većina zadataka dolazi u varijanti **a)** (grupne operacije) i **b)** (P-to-P operacije), pri čemu je b) uvek direktna zamena grupnih operacija iz a).

## Pregled tipova

| Tip | Naziv | Ključna ideja |
|-----|-------|---------------|
| **Tip 1** | Ciklična raspodela dvostruke petlje | `for(i) for(j)` rekonstrukcija indeksa iz `t = rank, rank+size, ...` |
| **Tip 2** | Matrica × Vektor (kolonska raspodela) | Svaki proces dobije `q` kolona A + `q` elemenata b |
| **Tip 3a** | Matrica × Matrica (kolone A + vrste B) | Svaki proces dobije `q` kolona A + `q` vrsta B → izračuna deo C |
| **Tip 3a var.** | Matrica × Matrica (po 1 kolona / 1 vrsta) | Spoljašnji proizvod `local_a ⊗ local_b` + `MPI_Reduce(SUM)` u C |
| **Tip 3b** | Matrica × Matrica (vrste A + cela B) | Svaki proces dobije vrste A + celu B (`MPI_Bcast`) → izračuna vrste C |
| **Tip 4** | Stablo / Hiperkub | Širenje podatka iz P0 svim ostalim u `log₂(p)` koraka |

---

## Tip 1: Ciklična raspodela dvostruke petlje (Niz zadatak)

### Primeri
- **Jun 2020** = **Januar 2022**
- **Decembar 2022** (varijanta sa pomakom `y` i silaznim `j`)

### Tekst zadatka (generički)
Realizovati dvostruku petlju `for(i) for(j) s += i + j` ravnomernom cikličnom raspodelom među `p` procesa. Nije dozvoljeno korišćenje indeksiranih promenljivih. Rezultat prikazati u procesu koji ima najmanji/najveći broj sabiraka koji su prosti brojevi.

### Šablon

```c
// --- 1. REKONSTRUKCIJA INDEKSA ---
// t = rank, rank+size, rank+2*size, ...
// i = t / JN + pocetak_i
// j++ → j = B + (t % JN)
// j-- → j = B - (t % JN)

for (int t = rank; t < total_iterations; t += size) {
    int i = t / JN + offset_i;
    int j = (direction == UP) ? B + (t % JN) : B - (t % JN);
    local_sum += i + j;

    if (is_prime(i + j))  // ili neki drugi uslov
        in.value++;
}

// --- 2. PRONALAZAK PROCESA SA EKSTREMOM (MIN/MAX) ---
// a) Grupne:  MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC/MAXLOC, root, ...)
// b) P-to-P:  root prikuplja od svih u petlji, traži min/max ručno

// --- 3. BROADCAST KO STAMPA ---
// a) Grupne:  MPI_Bcast(&out, 1, MPI_2INT, root, ...)
// b) P-to-P:  root šalje svima u petlji

// --- 4. SUMIRANJE U IZABRANI PROCES ---
// a) Grupne:  MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, out.rank, ...)
// b) P-to-P:  svi šalju out.rank, on sabira

if (rank == out.rank) {
    printf("Suma: %d\n", sum);
    printf("Proces %d ima ekstrem: %d\n", out.rank, out.value);
}
```

### Kalkulacija parametara za cikličnu raspodelu

| Korak | Formula |
|-------|---------|
| Broj vrednosti `j` (`JN`) | `j++` i `j < C` → `JN = C - B`; `j <= C` → `JN = C - B + 1` |
| | `j--` i `j > C` → `JN = B - C`; `j >= C` → `JN = B - C + 1` |
| Ukupno iteracija | `t_max = N * JN` |
| Rekonstrukcija `i` | `i = t / JN + pocetak_i` |
| Rekonstrukcija `j` | `j++` → `j = B + (t % JN)`; `j--` → `j = B - (t % JN)` |

### Ključne grupne operacije
1. `MPI_Reduce(..., MPI_2INT, MPI_MINLOC/MPI_MAXLOC, root)` — nalazi proces sa ekstremom
2. `MPI_Bcast(..., root)` — širi koji proces štampa
3. `MPI_Reduce(..., MPI_SUM, out.rank)` — sumira u ciljani proces

---

## Tip 2: Matrica × Vektor (kolonska raspodela)

### Primeri
- **April 2021** — po **jedna** kolona / element vektora
- **April 2022** — po **q** kolona / elemenata vektora
- **Jun 2021** — po **l** kolona / elemenata vektora

### Tekst zadatka (generički)
Napisati MPI program koji množi matricu `A` i vektor `b`, prikazuje rezultujući vektor `c`. Takođe pronalazi maksimum/minimum elemenata matrice `A` i sumu/proizvod elemenata svake vrste. Root proces šalje svakom procesu po `q` kolona matrice `A` (ili po jednu) i po `q` elemenata vektora `b`. Rezultati se prikazuju u procesu koji sadrži maksimum/minimum nakon raspodele.

### Šablon

```c
// --- 1. RASPODELA KOLONA MATRICE A (P-to-P) ---
// Root šalje svakom procesu po q kolona
if (rank == root) {
    // ostavlja sebi prvih q kolona
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < q; j++)
            local_a[i][j] = a[i][j];

    // šalje ostalima
    for (int p = 1; p < size; p++) {
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < q; j++)
                tmp_a[i][j] = a[i][p*q + j];
        MPI_Send(tmp_a, rows*q, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
} else {
    MPI_Recv(local_a, rows*q, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
}

// --- 2. RASPODELA VEKTORA b ---
// a) Grupne:  MPI_Scatter(b, q, MPI_INT, local_b, q, MPI_INT, root, ...)
// b) P-to-P:  root šalje po q elemenata svakom procesu

// --- 3. LOKALNO IZRAČUNAVANJE ---
in.value = local_a[0][0];  // ili INT_MIN/INT_MAX
in.rank = rank;
for (int i = 0; i < rows; i++) {
    local_c[i] = 0;
    local_row_sum[i] = 0;
    for (int j = 0; j < q; j++) {
        local_c[i]      += local_a[i][j] * local_b[j];  // vektor c
        local_row_sum[i] += local_a[i][j];               // suma vrste
        if (local_a[i][j] > in.value)                    // max (ili < za min)
            in.value = local_a[i][j];
    }
}

// --- 4. PRONALAZAK GLOBALNOG EKSTREMA ---
// a) Grupne:  MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC/MINLOC, root, ...)
// b) P-to-P:  root prikuplja i poređuje

// --- 5. BROADCAST KO STAMPA ---
// a) Grupne:  MPI_Bcast(&out, 1, MPI_2INT, root, ...)
// b) P-to-P:  root šalje svima

// --- 6. REDUKCIJA REZULTATA U IZABRANI PROCES ---
// a) Grupne:
//    MPI_Reduce(local_row_sum, row_sum, rows, MPI_INT, MPI_SUM, out.rank, ...)
//    MPI_Reduce(local_c, c, rows, MPI_INT, MPI_SUM, out.rank, ...)
// b) P-to-P:  svi šalju nizove out.rank, on sabira po elementima

if (rank == out.rank) {
    // štampa vektor c, ekstrem, sume po vrstama
}
```

### Varijante

| Parametar | April 2021 | April 2022 | Jun 2021 |
|-----------|------------|------------|----------|
| Kolona po procesu | 1 (`k` = broj kolona) | `q` (konstanta) | `l` (konstanta) |
| Operacija na vrstama | proizvod | suma | suma |
| Ekstrem | minimum | maksimum | maksimum |
| Stampanje | proces sa minimumom | proces sa maksimumom | proces sa maksimumom |

---

## Tip 3: Matrica × Matrica

Postoje **dve varijante** Tip 3, koje se razlikuju po tome kako se raspodeljuje matrica A:

### Tip 3a: Raspodela po kolonama matrice A (i vrstama B)

### Primeri
- **Jun 2 2022** — po `q` kolona matrice A i `q` vrsta matrice B po procesu

### Tekst zadatka (generički — Jun 2 2022)
Napisati MPI program koji množi matricu `A(k×m)` i matricu `B(m×n)`, prikazuje rezultujuću matricu `C`. Takođe pronalazi proizvod elemenata svake kolone matrice `B`. Root proces šalje svakom procesu po `q` kolona matrice A (P-to-P) i po `q` vrsta matrice B (grupna operacija). Rezultati se prikazuju u procesu koji sadrži maksimum matrice B nakon raspodele.

### Šablon

```c
// --- 1. RASPODELA q KOLONA MATRICE A (P-to-P) ---
// Root šalje svakom procesu q kolona, svaka kolona odjednom
if (rank == root) {
    for (int i = 0; i < k; i++)
        for (int j = 0; j < q; j++)
            local_a[i][j] = a[i][j + root * q];

    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        for (int j = 0; j < q; j++) {
            for (int i = 0; i < k; i++)
                tmp_a[i] = a[i][j + p * q];
            MPI_Send(tmp_a, k, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
} else {
    for (int j = 0; j < q; j++)
        MPI_Recv(&local_a[0][j], k, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
}

// --- 2. RASPODELA q VRSTA MATRICE B (grupna) ---
// MPI_Scatter(b, q*n, MPI_INT, local_b, q*n, MPI_INT, root, ...)

// --- 3. LOKALNO IZRAČUNAVANJE ---
in.value = local_b[0][0];
in.rank = rank;
for (int j = 0; j < n; j++) local_col_prod[j] = 1;

for (int i = 0; i < q; i++)
    for (int j = 0; j < n; j++) {
        local_col_prod[j] *= local_b[i][j];     // proizvod kolona B
        if (local_b[i][j] > in.value)            // lokalni max B
            in.value = local_b[i][j];
    }

for (int i = 0; i < k; i++)
    for (int j = 0; j < n; j++) {
        local_c[i][j] = 0;
        for (int l = 0; l < q; l++)
            local_c[i][j] += local_a[i][l] * local_b[l][j];
    }

// --- 4. GLOBALNI MAKSIMUM MATRICE B ---
// MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, ...)
// MPI_Bcast(&out, 1, MPI_2INT, root, ...)

// --- 5. REDUKCIJA REZULTATA U IZABRANI PROCES ---
// MPI_Reduce(local_col_prod, col_prod, n, MPI_INT, MPI_PROD, out.rank, ...)
// MPI_Reduce(local_c, c, k*n, MPI_INT, MPI_SUM, out.rank, ...)

if (rank == out.rank) {
    // štampa matricu C, maksimum, proizvode kolona B
}
```

### Varijante — Tip 3a

| Parametar | Jun 2 2022 |
|-----------|------------|
| Kolona A / Vrsta B po procesu | `q` (konstanta) |
| Operacija na B | proizvod kolona |
| Ekstrem | maksimum u B |
| Štampanje | proces sa maksimumom |

---

### Tip 3a varijanta: Po 1 kolona A i 1 vrsta B (size = broj kolona A = broj vrsta B)

### Primeri
- **Oktobar 2022 b** — po **jedna** kolona A (P-to-P) i **jedna** vrsta B (`MPI_Scatter`), `MPI_Reduce(MPI_SUM)` u C

### Tekst zadatka (generički — Oktobar 2022 b)
Napisati MPI program koji realizuje množenje matrice `A(k×l)` i matrice `B(l×m)` i prikazuje rezultujuću matricu `C(k×m)`. Master proces šalje svakom procesu po **jednu** kolonu matrice A (svi elementi kolone odjednom) i po **jednu** vrstu matrice B (svi elementi vrste odjednom). Slanje kolona matrice A koristi P-to-P, a sve ostalo grupne operacije.

> **Ključna ideja:** Svaki proces dobija jedan **vektor `local_a[k]`** (kolonu A) i jedan **vektor `local_b[m]`** (vrstu B). Lokalni rezultat je **spoljašnji proizvod** ova dva vektora — matrica `local_c[k][m]` gde je `local_c[i][j] = local_a[i] * local_b[j]`. Konačna matrica `C` je **suma** svih `local_c` matrica preko procesa, što direktno odgovara definiciji matričnog množenja `C = A × B = Σ (kolona_A[k] ⊗ vrsta_B[k])`.

### Šablon

```c
// pretpostavka: size = l (broj kolona matrice A = broj vrsta matrice B)
int local_a[k], local_b[m], local_c[k][m], c[k][m];

// --- 1. RASPODELA PO 1 KOLONA MATRICE A (P-to-P) ---
if (rank == root) {
    int tmp_a[k];
    // root sebi uzima prvu kolonu (root)
    for (int i = 0; i < k; i++)
        local_a[i] = a[i][root];

    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        for (int i = 0; i < k; i++)
            tmp_a[i] = a[i][p];
        MPI_Send(tmp_a, k, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
} else {
    MPI_Recv(local_a, k, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
}

// --- 2. RASPODELA PO 1 VRSTA MATRICE B (grupna) ---
// MPI_Scatter raspodeljuje uzastopne vrste B ka procesima
MPI_Scatter(b, m, MPI_INT, local_b, m, MPI_INT, root, MPI_COMM_WORLD);

// --- 3. LOKALNO IZRAČUNAVANJE — SPOLJAŠNJI PROIZVOD ---
for (int i = 0; i < k; i++)
    for (int j = 0; j < m; j++)
        local_c[i][j] = local_a[i] * local_b[j];

// --- 4. SABIRANJE LOKALNIH MATRICA U GLOBALNU C ---
// MPI_Reduce sumira odgovarajuće elemente svih local_c matrica → C
MPI_Reduce(local_c, c, k * m, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);

if (rank == root) {
    // štampa matricu C
}
```

### Karakteristike — Tip 3a varijanta

| Parametar | Oktobar 2022 b |
|-----------|----------------|
| Kolona A po procesu | **1** |
| Vrsta B po procesu | **1** (preko `MPI_Scatter`) |
| Lokalna operacija | spoljašnji proizvod `local_a ⊗ local_b` |
| Redukcija u C | `MPI_Reduce(MPI_SUM, root)` po elementima `k*m` |
| Ekstrem | nema (samo prikaz C u root) |
| Štampanje | root |

> **Razlika od Tip 3a (Jun 2 2022):**
> - Jun 2 2022: `q` kolona A + `q` vrsta B → svaki proces sam izračuna svoj **deo** finalne matrice C, redukcija je samo za poređenje ekstrema.
> - Oktobar 2022 b: 1 kolona A + 1 vrsta B → svaki proces izračuna **delimičan doprinos** ka **celoj** matrici C, redukcija sumira sve doprinose.

---

### Tip 3b: Raspodela po vrstama matrice A (i cela matrica B)

### Primeri
- **Septembar 2021** — po `l` vrsta matrice A, cela matrica B, proizvod kolona matrice A, maksimum u C
- **Oktobar 2 2022** — po `s` vrsta matrice A, cela matrica B, proizvod kolona matrice A
- **Septembar 2023** — po `r` vrsta matrice A, cela matrica B, proizvod kolona matrice A, minimum u A

### Tekst zadatka (generički — Oktobar 2 2022)
Napisati MPI program koji množi matricu `A(k×m)` i matricu `B(m×n)`, prikazuje rezultujuću matricu `C`. Takođe pronalazi proizvod elemenata svake kolone matrice `A`. Root proces šalje svakom procesu po `s` vrsta matrice A (P-to-P) i **celu matricu B** (grupna operacija). Rezultati se prikazuju u procesu koji sadrži maksimum matrice `C`.

### Šablon

```c
// --- 1. RASPODELA s VRSTA MATRICE A (P-to-P) ---
// Root šalje svakom procesu s vrsta odjednom, direktno iz matrice A
if (rank == root) {
    for (int i = 0; i < s; i++)
        for (int j = 0; j < m; j++)
            local_a[i][j] = a[i][j];

    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        for (int i = 0; i < s; i++)
            for (int j = 0; j < m; j++)
                local_a[i][j] = a[p * s + i][j];
        MPI_Send(local_a, s * m, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
} else {
    MPI_Recv(local_a, s * m, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
}

// --- 2. RASPODELA CELE MATRICE B (grupna) ---
// MPI_Bcast(b, m*n, MPI_INT, root, ...)

// --- 3. LOKALNO IZRAČUNAVANJE ---
// Proizvod elemenata svake kolone matrice A
for (int j = 0; j < m; j++) local_col_prod[j] = 1;
for (int i = 0; i < s; i++)
    for (int j = 0; j < m; j++)
        local_col_prod[j] *= local_a[i][j];

// Množenje A × B → C (lokalni deo)
in.value = local_c[0][0];
in.rank = rank;
for (int i = 0; i < s; i++)
    for (int j = 0; j < n; j++) {
        local_c[i][j] = 0;
        for (int l = 0; l < m; l++)
            local_c[i][j] += local_a[i][l] * b[l][j];
        if (local_c[i][j] > in.value)
            in.value = local_c[i][j];
    }

// --- 4. GLOBALNI MAKSIMUM MATRICE C ---
// MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, ...)
// MPI_Bcast(&out, 1, MPI_2INT, root, ...)

// --- 5. REDUKCIJA REZULTATA U IZABRANI PROCES ---
// MPI_Reduce(local_col_prod, col_prod, m, MPI_INT, MPI_PROD, out.rank, ...)
// MPI_Reduce(local_c, c, s*n, MPI_INT, MPI_SUM, out.rank, ...)

if (rank == out.rank) {
    // štampa matricu C, maksimum, proizvode kolona A
}
```

### Varijante — Tip 3b

| Parametar | Septembar 2021 | Oktobar 2 2022 | Septembar 2023 |
|-----------|----------------|----------------|----------------|
| Vrsta A po procesu | `l` (konstanta) | `s` (konstanta) | `r` (konstanta) |
| Matrica B | cela (`MPI_Bcast`) | cela (`MPI_Bcast`) | cela (`MPI_Bcast`) |
| Operacija na A | proizvod kolona | proizvod kolona | proizvod kolona |
| Ekstrem | maksimum u C | maksimum u C | **minimum u A** |
| Štampanje | proces sa maksimumom | proces sa maksimumom | **proces sa minimumom** |

> **Napomena:** Septembar 2021 i Septembar 2023 koriste `MPI_Scatter` umesto P-to-P za slanje vrsta matrice A — moguće zato što je k deljivo sa l/r. Kod Oktobra 2 2022, `s` vrsta se šalju P-to-P kao u zadatku. Septembar 2023 je prvi zadatak gde se ekstrem traži u matrici **A** (minimum), a ne u rezultujućoj matrici **C** (maksimum).

---

## Tip 4: Stablo / Hiperkub (širenje podatka u `log₂(p)` koraka)

### Primeri
- **Oktobar 2022 a** — slanje jednog podatka iz P0 svim ostalim procesima (broj procesa je stepen dvojke)

### Tekst zadatka (generički)
Napisati MPI program koji korišćenjem **Point-to-Point** komunikacije vrši slanje jednog podatka iz procesa 0 svim ostalim procesima u komunikatoru. Broj procesa je **stepen dvojke**. Procesi su uređeni u **stablo (hiperkub)**, a komunikacija se odvija u `log₂(p)` koraka — u svakom koraku se broj procesa koji "znaju" podatak udvostručuje.

### Vizuelizacija (za `size = 8`)

```
Korak 1: P0 → P1                         (1 → 2 procesa)
Korak 2: P0 → P2,  P1 → P3              (2 → 4 procesa)
Korak 3: P0 → P4,  P1 → P5,  P2 → P6,  P3 → P7  (4 → 8 procesa)
```

U svakom koraku `i`, procesi `[0, 2^(i-1))` šalju, procesi `[2^(i-1), 2^i)` primaju (par je `rank` ↔ `rank + 2^(i-1)`).

### Šablon

```c
#include <math.h>

int data;
int steps = (int)log2(size);   // broj koraka = log₂(size)

if (rank == root)
    data = /* početna vrednost */;

for (int i = 1; i <= steps; i++) {
    int half = 1 << (i - 1);   // 2^(i-1) = trenutni broj procesa koji znaju podatak

    if (rank < half)
        // pošiljaoci: salju svom paru rank + half
        MPI_Send(&data, 1, MPI_INT, rank + half, 0, MPI_COMM_WORLD);
    else if (rank < 2 * half)
        // primaoci: primaju od para rank - half
        MPI_Recv(&data, 1, MPI_INT, rank - half, 0, MPI_COMM_WORLD, &status);
    // ostali procesi (rank >= 2*half) ne učestvuju u ovom koraku
}

if (rank != root)
    printf("Proces %d primio vrednost: %d\n", rank, data);
```

### Ključne ideje — Tip 4

1. **`half = 1 << (i-1)`** — bitwise levi pomak je elegantan način za računanje `2^(i-1)`.
2. **Tri kategorije procesa u svakom koraku**:
   - `rank < half` → **šalje** procesu `rank + half`
   - `half ≤ rank < 2*half` → **prima** od procesa `rank - half`
   - `rank ≥ 2*half` → **čeka** sledeći korak
3. **Broj procesa raste eksponencijalno**: 1 → 2 → 4 → 8 → ... → `size`.
4. **Logaritamska kompleksnost**: za `p = 2^k` procesa potrebno je tačno `k = log₂(p)` koraka.

> **Napomena:** Ovaj obrazac se koristi i u `MPI_1_4.c` (vežbe) za realizaciju `MPI_Scatter` preko hiperkuba i u `MPI_1_3.c` za sumiranje brojeva u logaritamskom broju koraka. Inverzna varijanta (sve → P0 u `log₂(p)` koraka) se koristi za **redukciju** stilovi `MPI_Reduce`.

---

## Zamena grupnih operacija P-to-P

Sve tri često korišćene grupne operacije u zadacima se zamenjuju na isti način:

### 1. `MPI_Reduce(..., MPI_MINLOC/MAXLOC, root)`
```c
if (rank != root)
    MPI_Send(&in, 1, MPI_2INT, root, 0, MPI_COMM_WORLD);
else {
    out = in;
    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        MPI_Recv(&in, 1, MPI_2INT, p, 0, MPI_COMM_WORLD, &status);
        if (out.value > in.value) out = in;  // MINLOC
        // if (out.value < in.value) out = in;  // MAXLOC
    }
}
```

### 2. `MPI_Bcast(&out, ..., root)`
```c
if (rank == root) {
    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        MPI_Send(&out, 1, MPI_2INT, p, 0, MPI_COMM_WORLD);
    }
} else {
    MPI_Recv(&out, 1, MPI_2INT, root, 0, MPI_COMM_WORLD, &status);
}
```

### 3. `MPI_Reduce(..., MPI_SUM, target_rank)` — skalar
```c
if (rank != target_rank)
    MPI_Send(&local_sum, 1, MPI_INT, target_rank, 0, MPI_COMM_WORLD);
else {
    sum = local_sum;
    for (int p = 0; p < size; p++) {
        if (p == target_rank) continue;
        MPI_Recv(&local_sum, 1, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
        sum += local_sum;
    }
}
```

### 4. `MPI_Reduce(..., MPI_SUM, target_rank)` — niz
```c
if (rank != target_rank)
    MPI_Send(local_arr, n, MPI_INT, target_rank, 0, MPI_COMM_WORLD);
else {
    for (int i = 0; i < n; i++) global_arr[i] = local_arr[i];
    for (int p = 0; p < size; p++) {
        if (p == target_rank) continue;
        MPI_Recv(local_arr, n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < n; i++) global_arr[i] += local_arr[i];
    }
}
```

---

## Uočeni patterni i ponavljanja

| Rok | Tip | Napomena |
|-----|-----|----------|
| Jun 2020 | Tip 1 | Bazni oblik, `i=0..N-1`, `j=0..N-1` |
| Januar 2022 | Tip 1 | **Identičan** Jun 2020 |
| Decembar 2022 | Tip 1 | Varijanta: pomak `y`, silazni `j`, traži se **najveći** broj prostih |
| Septembar 2022 | Tip 1 | **Identičan** Decembar 2022 |
| April 2021 | Tip 2 | Po jedna kolona, **min** + **proizvod** po vrstama |
| April 2022 | Tip 2 | Po `q` kolona, **max** + **suma** po vrstama |
| Jun 2021 | Tip 2 | Po `l` kolona, **max** + **suma** po vrstama |
| Jun 2 2022 | Tip 3a | Po `q` kolona A i `q` vrsta B, **max** + **proizvod** kolona B |
| Septembar 2021 | Tip 3b | Po `l` vrsta A (`MPI_Scatter`), cela B, **max** u C + **proizvod** kolona A |
| Oktobar 2 2022 | Tip 3b | Po `s` vrsta A, cela B, **max** u C + **proizvod** kolona A |
| Oktobar 2022 a | Tip 4 | Hiperkub — P-to-P slanje podatka od P0 svim ostalim u `log₂(p)` koraka |
| Oktobar 2022 b | Tip 3a var. | Po **1** kolona A (P-to-P) i **1** vrsta B (`MPI_Scatter`), `MPI_Reduce` u C |
| Septembar 2023 | Tip 3b | Po `r` vrsta A (`MPI_Scatter`/P-to-P), cela B, **min** u A + **proizvod** kolona A |

---

## Saveti za učenje

1. **Naučiti šablon Tip 1** — petlja sa rekonstrukcijom indeksa je najčešća na ispitu. Razumeti kako se iz `t` rekonstruišu `i` i `j` za uzlažni i silazni smer.
2. **Naučiti šablon Tip 2** — raspodela kolona matrice P-to-P, zatim grupne operacije za redukciju. Zamena a→b je uvek ista.
3. **Razumeti razliku Tip 3a vs Tip 3b** — Tip 3a deli A po **kolonama** + B po **vrstama** (svaki proces izračuna deo C samostalno); Tip 3b deli A po **vrstama** + cela B (`MPI_Bcast`) (svaki proces izračuna **različite vrste** finalne C).
4. **Tip 3a varijanta (q=1)** — kada svaki proces dobije po **1** kolonu A i **1** vrstu B, lokalni rezultat je **spoljašnji proizvod** koji se sumira u finalnu C preko `MPI_Reduce(MPI_SUM)`.
5. **Tip 4 (Stablo)** — petlja `for (i = 1; i <= log₂(size); i++)` sa `half = 1 << (i-1)`. Tri kategorije: `rank < half` šalje, `half ≤ rank < 2*half` prima, ostali čekaju. Uvek se podrazumeva da je `size` stepen dvojke.
6. **Pamtiti strukturu** `struct { int value; int rank; }` — neophodna za `MPI_MINLOC`/`MPI_MAXLOC`.
7. **Redosled operacija** je uvek isti za Tip 1/2/3: **Raspodela → Lokalni račun → Reduce(LOC) → Bcast → Reduce(SUM)**.
