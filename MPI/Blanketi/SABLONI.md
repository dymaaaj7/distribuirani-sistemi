<!-- markdownlint-disable MD024 -->

# MPI Blanketi — Šabloni i Tipovi Zadataka

Na osnovu analize zadataka sa prethodnih rokova, MPI ispitni zadaci se mogu svrstati u **sedam osnovnih tipova**: Tip 1, Tip 2, četiri varijante Tip 3 (3a, 3b, 3c, 3d) i Tip 4. Pored toga, povremeno se pojave **jedinstveni** zadaci koji ne pripadaju nijednom tipu (npr. Jun 2025 b, April 2026 b).

Većina rokova dolazi u varijanti **a)** (grupne operacije) i **b)** (P-to-P operacije), gde je b) direktna zamena grupnih operacija iz a) — ali **ne uvek**. Rokovi kod kojih a i b **nisu** isti zadatak: Oktobar 2022 (a = Tip 4, b = Tip 3d), Jun 2025 (a = Tip 3a, b = jedinstven), April 2026 (a = Tip 3c, b = kružna razmena). Tačan pregled a/b odnosa po rokovima je u [master tabeli](#master-tabela-po-rokovima).

## Pregled tipova

| Tip        | Naziv                                     | Ključna ideja                                                                                        |
| ---------- | ----------------------------------------- | ---------------------------------------------------------------------------------------------------- |
| **Tip 1**  | Ciklična raspodela dvostruke petlje       | `for(i) for(j)` rekonstrukcija indeksa iz `t = rank, rank+size, ...`                                 |
| **Tip 2**  | Matrica × Vektor (kolonska raspodela)     | Svaki proces dobije `q` kolona A + `q` elemenata b                                                   |
| **Tip 3a** | Matrica × Matrica (kolone A + vrste B)    | Svaki proces dobije `q` kolona A + `q` vrsta B → izračuna deo C                                      |
| **Tip 3b** | Matrica × Matrica (vrste A + cela B)      | Svaki proces dobije vrste A (`MPI_Scatter`) + celu B (`MPI_Bcast`) → izračuna vrste C → `MPI_Gather` |
| **Tip 3c** | Matrica × Matrica (cela A + kolone B)     | Svaki proces dobije celu A (`MPI_Bcast`) + kolone B → izračuna kolone C → `MPI_Gather`               |
| **Tip 3d** | Matrica × Matrica (po 1 kolona / 1 vrsta) | Spoljašnji proizvod `local_a ⊗ local_b` + `MPI_Reduce(SUM)` u C                                      |
| **Tip 4**  | Stablo / Hiperkub                         | Širenje podatka iz P0 svim ostalim u `log₂(p)` koraka                                                |

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

| Korak                     | Formula                                                     |
| ------------------------- | ----------------------------------------------------------- |
| Broj vrednosti `j` (`JN`) | `j++` i `j < C` → `JN = C - B`; `j <= C` → `JN = C - B + 1` |
|                           | `j--` i `j > C` → `JN = B - C`; `j >= C` → `JN = B - C + 1` |
| Ukupno iteracija          | `t_max = N * JN`                                            |
| Rekonstrukcija `i`        | `i = t / JN + pocetak_i`                                    |
| Rekonstrukcija `j`        | `j++` → `j = B + (t % JN)`; `j--` → `j = B - (t % JN)`      |

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
- **Decembar 2021** — po **q** kolona / elemenata vektora (identičan Aprilu 2022)
- **April 2025** — po **s** kolona / elemenata vektora (identičan Aprilu 2022)

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

| Parametar            | April 2021            | April 2022           | Jun 2021             | Decembar 2021        | April 2025           |
| -------------------- | --------------------- | -------------------- | -------------------- | -------------------- | -------------------- |
| Kolona po procesu    | 1 (`k` = broj kolona) | `q` (konstanta)      | `l` (konstanta)      | `q` (konstanta)      | `s` (konstanta)      |
| Operacija na vrstama | proizvod              | suma                 | suma                 | suma                 | suma                 |
| Ekstrem              | minimum               | maksimum             | maksimum             | maksimum             | maksimum             |
| Stampanje            | proces sa minimumom   | proces sa maksimumom | proces sa maksimumom | proces sa maksimumom | proces sa maksimumom |

---

## Tip 3: Matrica × Matrica

Postoje **četiri varijante** Tip 3, koje se razlikuju po tome šta se raspodeljuje i kako se sakuplja rezultat:

| Varijanta | Raspodela                              | Lokalni rezultat                       | Sakupljanje C     |
| --------- | -------------------------------------- | -------------------------------------- | ----------------- |
| **3a**    | `q` kolona A + `q` vrsta B             | ceo deo C (`k×n`)                      | `MPI_Reduce(SUM)` |
| **3b**    | vrste A (`Scatter`) + cela B (`Bcast`) | vrste C                                | `MPI_Gather`      |
| **3c**    | cela A (`Bcast`) + kolone B            | kolone C                               | `MPI_Gather`      |
| **3d**    | 1 kolona A + 1 vrsta B                 | spoljašnji proizvod (doprinos celoj C) | `MPI_Reduce(SUM)` |

### Tip 3a: Raspodela po kolonama matrice A (i vrstama B)

### Primeri

- **Jun 2 2022** — po `q` kolona matrice A i `q` vrsta matrice B po procesu
- **Jun 2 2023** — po `q` kolona matrice A i `q` vrsta matrice B po procesu
- **Jun 2025 a** — po `q` kolona matrice A i `q` vrsta matrice B po procesu, **bez traženja ekstrema** — samo root štampa

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

| Parametar                     | Jun 2 2022           | Jun 2 2023           | Jun 2025 a      |
| ----------------------------- | -------------------- | -------------------- | --------------- |
| Kolona A / Vrsta B po procesu | `q` (konstanta)      | `q` (konstanta)      | `q` (konstanta) |
| Operacija na B                | proizvod kolona      | proizvod kolona      | proizvod kolona |
| Ekstrem                       | maksimum u B         | maksimum u B         | **nema**        |
| Štampanje                     | proces sa maksimumom | proces sa maksimumom | **root**        |

> **Napomena:** Jun 2025 a je najjednostavnija varijanta Tip 3a — nema traženje ekstrema (`MPI_MAXLOC`/`MPI_MINLOC`), nema `MPI_Bcast` za određivanje ko štampa. Rezultati se prosto redukuju u root proces preko `MPI_Reduce(MPI_SUM)` za matricu C i `MPI_Reduce(MPI_PROD)` za proizvod kolona B.

---

### Tip 3d: Po 1 kolona A i 1 vrsta B — spoljašnji proizvod (size = broj kolona A = broj vrsta B)

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

### Karakteristike — Tip 3d

| Parametar           | Oktobar 2022 b                                  |
| ------------------- | ----------------------------------------------- |
| Kolona A po procesu | **1**                                           |
| Vrsta B po procesu  | **1** (preko `MPI_Scatter`)                     |
| Lokalna operacija   | spoljašnji proizvod `local_a ⊗ local_b`         |
| Redukcija u C       | `MPI_Reduce(MPI_SUM, root)` po elementima `k*m` |
| Ekstrem             | nema (samo prikaz C u root)                     |
| Štampanje           | root                                            |

> **Razlika od Tip 3a (Jun 2 2022):**
>
> - Jun 2 2022: `q` kolona A + `q` vrsta B → svaki proces sam izračuna svoj **deo** finalne matrice C, redukcija je samo za poređenje ekstrema.
> - Oktobar 2022 b: 1 kolona A + 1 vrsta B → svaki proces izračuna **delimičan doprinos** ka **celoj** matrici C, redukcija sumira sve doprinose.

---

### Tip 3b: Raspodela po vrstama matrice A (i cela matrica B)

### Primeri

- **Septembar 2021** — po `l` vrsta matrice A, cela matrica B, proizvod kolona matrice A, maksimum u C
- **Oktobar 2 2022** — po `s` vrsta matrice A, cela matrica B, proizvod kolona matrice A
- **Septembar 2023** — po `r` vrsta matrice A, cela matrica B, proizvod kolona matrice A, minimum u A
- **Januar 2025** — po `m` vrsta matrice A, cela matrica B, **suma** kolona matrice **B**, maksimum u A

### Tekst zadatka (generički — Oktobar 2 2022)

Napisati MPI program koji množi matricu `A(k×m)` i matricu `B(m×n)`, prikazuje rezultujuću matricu `C`. Takođe pronalazi proizvod elemenata svake kolone matrice `A`. Root proces šalje svakom procesu po `s` vrsta matrice A (P-to-P) i **celu matricu B** (grupna operacija). Rezultati se prikazuju u procesu koji sadrži maksimum matrice `C`.

### Šablon

> **Važno:** Sva četiri rešenja Tip 3b (Septembar 2021, Oktobar 2 2022, Septembar 2023, Januar 2025) vrste matrice A raspodeljuju preko **`MPI_Scatter`**, a rezultujuću matricu C sakupljaju preko **`MPI_Gather`** — iako tekstovi zadataka traže P-to-P za slanje vrsta A. Scatter je ispravan jer je `k` deljivo sa brojem vrsta po procesu. P-to-P varijanta slanja vrsta pojavljuje se u **b) rešenjima** (zamena grupnih operacija P-to-P) — vidi sekciju [Zamena grupnih operacija P-to-P](#zamena-grupnih-operacija-p-to-p).

```c
// --- 1. RASPODELA s VRSTA MATRICE A (grupna) ---
// Scatter raspodeljuje uzastopne vrste A: proces p dobije vrste [p*s, p*s+s)
MPI_Scatter(a, s * m, MPI_INT, local_a, s * m, MPI_INT, root, MPI_COMM_WORLD);

// --- 2. RASPODELA CELE MATRICE B (grupna) ---
MPI_Bcast(b, m * n, MPI_INT, root, MPI_COMM_WORLD);

// --- 3. LOKALNO IZRAČUNAVANJE ---
// Proizvod elemenata svake kolone matrice A
for (int j = 0; j < m; j++) local_col_prod[j] = 1;
for (int i = 0; i < s; i++)
    for (int j = 0; j < m; j++)
        local_col_prod[j] *= local_a[i][j];

// Množenje A × B → C (lokalni deo — različite VRSTE matrice C)
in.value = INT_MIN;
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

// --- 5. SAKUPLJANJE REZULTATA U IZABRANI PROCES ---
// col_prod: redukcija po elementima (svi procesi imaju sve kolone)
MPI_Reduce(local_col_prod, col_prod, m, MPI_INT, MPI_PROD, out.rank, ...);
// C: Gather — svaki proces ima RAZLIČITE vrste, nema preklapanja,
// Gather ih složi u ispravan redosled (blok procesa p ide na mesto p)
MPI_Gather(local_c, s * n, MPI_INT, c, s * n, MPI_INT, out.rank, ...);

if (rank == out.rank) {
    // štampa matricu C, maksimum, proizvode kolona A
}
```

> **Zašto `MPI_Gather` a ne `MPI_Reduce(SUM)` za C?** Svaki proces izračuna **disjunktne vrste** matrice C — elementi se ne preklapaju, pa ih treba samo složiti jedan pored drugog (`Gather`), ne sabirati (`Reduce(SUM)` bi ovde takođe dao tačan rezultat jer su ostali elementi nula, ali `Gather` je prirodniji i jeftiniji). Isti obrazac važi i za Tip 3c.

### Varijante — Tip 3b

| Parametar          | Septembar 2021        | Oktobar 2 2022        | Septembar 2023          | Januar 2025              |
| ------------------ | --------------------- | --------------------- | ----------------------- | ------------------------ |
| Vrsta A po procesu | `l` (konstanta)       | `s` (konstanta)       | `r` (konstanta)         | `m` (konstanta)          |
| Raspodela vrsta A  | `MPI_Scatter`         | `MPI_Scatter`         | `MPI_Scatter`           | `MPI_Scatter`            |
| Matrica B          | cela (`MPI_Bcast`)    | cela (`MPI_Bcast`)    | cela (`MPI_Bcast`)      | cela (`MPI_Bcast`)       |
| Sakupljanje C      | `MPI_Gather`          | `MPI_Gather`          | `MPI_Gather`            | `MPI_Gather`             |
| Operacija          | proizvod kolona **A** | proizvod kolona **A** | proizvod kolona **A**   | **suma** kolona **B**    |
| Ekstrem            | maksimum u C          | maksimum u C          | **minimum u A**         | **maksimum u A**         |
| Štampanje          | proces sa maksimumom  | proces sa maksimumom  | **proces sa minimumom** | **proces sa maksimumom** |

> **Napomena:** Iako tekstovi zadataka (npr. Oktobar 2 2022) eksplicitno traže P-to-P za slanje vrsta matrice A, sva rešenja koriste `MPI_Scatter` — moguće zato što je `k` deljivo sa `l`/`s`/`r`/`m`. Septembar 2023 je prvi zadatak gde se ekstrem traži u matrici **A** (minimum), a ne u rezultujućoj matrici **C** (maksimum). Januar 2025 donosi dve novine: operacija je na matrici **B** (suma kolona) umesto na matrici A, a ekstrem je **maksimum u A**.

---

### Tip 3c: Cela matrica A + kolone matrice B

### Primeri

- **April 2026 a** — cela matrica A (`MPI_Bcast`) + po `s` kolona matrice B (P-to-P), minimum u B, minimum po vrstama C

### Tekst zadatka (generički — April 2026 a)

Napisati MPI program koji realizuje množenje matrice `A(n×k)` i matrice `B(k×m)`, prikazuje rezultujuću matricu `C`. Program pronalazi minimum svake vrste matrice `C`. Master proces šalje svakom procesu **celu matricu A** i po `s` kolona matrice B. Rezultati se prikazuju u procesu koji sadrži minimum u matrici B nakon distribuiranja.

> **Ključna ideja:** Ovo je **inverz** od Tip 3b. Umesto što se dele vrste A + cela B (Tip 3b), ovde se šalje cela A + dele kolone B. Svaki proces izračuna **različite kolone** finalne matrice C. Rezultat se sakuplja preko `MPI_Gather` (ne `MPI_Reduce`), jer svaki proces ima različite kolone — nema preklapanja.

### Šablon

```c
// --- 1. RASPODELA CELE MATRICE A (grupna) ---
// MPI_Bcast(a, n*k, MPI_INT, root, ...)

// --- 2. RASPODELA s KOLONA MATRICE B (P-to-P) ---
// Root šalje svakom procesu po s kolona matrice B
if (rank == root) {
    for (int i = 0; i < k; i++)
        for (int j = 0; j < s; j++)
            local_b[i][j] = b[i][root * s + j];

    int tmp_b[k][s];
    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        for (int i = 0; i < k; i++)
            for (int j = 0; j < s; j++)
                tmp_b[i][j] = b[i][j + p * s];
        MPI_Send(tmp_b, k * s, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
} else {
    MPI_Recv(local_b, k * s, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
}

// --- 3. LOKALNO IZRAČUNAVANJE ---
in.value = INT_MAX;
in.rank = rank;
for (int i = 0; i < k; i++)
    for (int j = 0; j < s; j++)
        if (local_b[i][j] < in.value)
            in.value = local_b[i][j];          // lokalni min B

for (int i = 0; i < n; i++)
    for (int j = 0; j < s; j++) {
        local_c[i][j] = 0;
        for (int l = 0; l < k; l++)
            local_c[i][j] += a[i][l] * local_b[l][j];  // kolone C
    }

// --- 4. GLOBALNI MINIMUM MATRICE B ---
// MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MINLOC, root, ...)
// MPI_Bcast(&out, 1, MPI_2INT, root, ...)

// --- 5. GATHER REZULTATA U IZABRANI PROCES ---
// MPI_Gather(local_c, n*s, MPI_INT, c, n*s, MPI_INT, out.rank, ...)

if (rank == out.rank) {
    // minimum po vrstama C (računa se nakon Gather-a)
    for (int i = 0; i < n; i++) {
        row_min[i] = c[i][0];
        for (int j = 0; j < m; j++)
            if (c[i][j] < row_min[i])
                row_min[i] = c[i][j];
    }
    // štampa matricu C, minimum B, minimum po vrstama C
}
```

### Karakteristike — Tip 3c

| Parametar           | April 2026 a                           |
| ------------------- | -------------------------------------- |
| Matrica A           | cela (`MPI_Bcast`)                     |
| Kolona B po procesu | `s` (konstanta, P-to-P)                |
| Lokalni rezultat    | `local_c[n][s]` — **kolone** matrice C |
| Sakupljanje C       | `MPI_Gather` (ne `MPI_Reduce`!)        |
| Ekstrem             | **minimum** u B                        |
| Dodatna operacija   | minimum po vrstama C (nakon Gather-a)  |
| Štampanje           | proces sa minimumom u B                |

> **Razlika od Tip 3b:**
>
> - Tip 3b: vrste A + cela B → svaki proces izračuna **vrste** C → `MPI_Reduce(MPI_SUM)` sakuplja rezultate (jer se vrste C ne preklapaju, može i `MPI_Gather`).
> - Tip 3c: cela A + kolone B → svaki proces izračuna **kolone** C → `MPI_Gather` sakuplja rezultate u ispravan redosled.
>
> **Važno:** U Tip 3c se `MPI_Gather` koristi umesto `MPI_Reduce` zato što svaki proces ima **različite** kolone C — nema preklapanja elemenata, pa se rezultati samo slažu jedan pored drugog.

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

## Master tabela po rokovima

| Rok            | Tip        | Raspodela                                                                              | Ekstrem (gde)            | Sekundarna operacija       | Ko štampa             | a/b odnos                             |
| -------------- | ---------- | -------------------------------------------------------------------------------------- | ------------------------ | -------------------------- | --------------------- | ------------------------------------- |
| Jun 2020       | 1          | ciklična, `i=0..N-1`, `j=0..N-1`                                                       | min prostih sabiraka     | —                          | proces sa min         | pravi par (b = P-to-P zamena)         |
| Januar 2022    | 1          | **identičan Jun 2020**                                                                 | min prostih sabiraka     | —                          | proces sa min         | pravi par                             |
| Decembar 2022  | 1          | ciklična, pomak `y`, silazni `j`                                                       | **max** prostih sabiraka | —                          | proces sa max         | pravi par                             |
| Septembar 2022 | 1          | **identičan Decembar 2022**                                                            | max prostih sabiraka     | —                          | proces sa max         | pravi par                             |
| April 2021     | 2          | po **1** kolona A + 1 elem b                                                           | **min** u A              | **proizvod** po vrstama A  | proces sa min         | pravi par                             |
| April 2022     | 2          | `q` kolona A + `q` elem b                                                              | max u A                  | suma po vrstama A          | proces sa max         | pravi par                             |
| Jun 2021       | 2          | `l` kolona A + `l` elem b                                                              | max u A                  | suma po vrstama A          | proces sa max         | pravi par                             |
| Decembar 2021  | 2          | **identičan April 2022** (`q`)                                                         | max u A                  | suma po vrstama A          | proces sa max         | pravi par                             |
| April 2025     | 2          | **identičan April 2022** (`s`)                                                         | max u A                  | suma po vrstama A          | proces sa max         | **samo a** (nema b fajla)             |
| Jun 2 2022     | 3a         | `q` kolona A + `q` vrsta B                                                             | max u B                  | proizvod kolona B          | proces sa max         | jedan fajl (bez a/b podele)           |
| Jun 2 2023     | 3a         | `q` kolona A + `q` vrsta B                                                             | max u B                  | proizvod kolona B          | proces sa max         | jedan fajl (bez a/b podele)           |
| Jun 2025 a     | 3a         | `q` kolona A + `q` vrsta B                                                             | **nema**                 | proizvod kolona B          | **root**              | **nije par sa b** (različita zadatka) |
| Septembar 2021 | 3b         | `l` vrsta A (`Scatter`) + cela B (`Bcast`)                                             | max u C                  | proizvod kolona A          | proces sa max         | pravi par                             |
| Oktobar 2 2022 | 3b         | `s` vrsta A (`Scatter`) + cela B (`Bcast`)                                             | max u C                  | proizvod kolona A          | proces sa max         | pravi par                             |
| Septembar 2023 | 3b         | `r` vrsta A (`Scatter`) + cela B (`Bcast`)                                             | **min u A**              | proizvod kolona A          | proces sa min         | pravi par                             |
| Januar 2025    | 3b         | `m` vrsta A (`Scatter`) + cela B (`Bcast`)                                             | **max u A**              | **suma kolona B**          | proces sa max         | pravi par                             |
| Oktobar 2022 a | 4          | hiperkub — širenje iz P0                                                               | —                        | —                          | svi procesi           | **nije par sa b** (različita zadatka) |
| Oktobar 2022 b | 3d         | **1** kolona A (P-to-P) + **1** vrsta B (`Scatter`)                                    | nema                     | —                          | root                  | **nije par sa a**                     |
| Jun 2025 b     | jedinstven | `Bcast` niza X iz **P2**, `Reduce(SUM)` u root                                         | nema                     | formula `yi=(p(p+1)/2)·xi` | (ništa — vidi bugove) | **nije par sa a**                     |
| April 2026 a   | 3c         | cela A (`Bcast`) + `s` kolona B (P-to-P)                                               | min u B                  | min po vrstama C           | proces sa min         | **nije par sa b** (različita zadatka) |
| April 2026 b   | jedinstven | kružna razmena — svaki proces šalje `b1` sledećem `(rank+1)%size`, prima od prethodnog | —                        | —                          | —                     | **nije par sa a**                     |

**Pravi a/b parovi** (b je direktna P-to-P zamena grupnih operacija iz a): svi Tip 1 rokovi, svi Tip 2 rokovi (osim Aprila 2025 koji nema b fajl), svi Tip 3b rokovi.
**Nisu parovi:** Oktobar 2022, Jun 2025, April 2026 — a i b su dva različita zadatka.

---

## Poznati bugovi u kodu

Rešenja u ovom folderu imaju greške — **nemoj ih prepisivati na ispitu**. Tabela navodi bug i ispravan obrazac:

| Fajl                              | Bug                                                                                                                                                                             | Ispravan obrazac                                                          |
| --------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------- |
| `2022/januar_a.c`                 | Petlja `for (t = rank; t < N*N; t++)` — **nedostaje `t += size`**, pa svaki proces izvršava SVE iteracije (nema ciklične raspodele)                                             | `for (t = rank; t < N*N; t += size)`                                      |
| `2020/jun_b.c`, `2022/januar_b.c` | `is_prime` koristi `i * i < n` — kvadrati (4, 9, 25, ...) prolaze kao "prosti"                                                                                                  | `i * i <= n`                                                              |
| `2022/septembar_a.c`              | Krajnji `MPI_Reduce(MPI_SUM)` cilja `root` umesto `out.rank` — suma je u root-u, a štampa je `out.rank` (neinicijalizovana vrednost ako se razlikuju)                           | `MPI_Reduce(..., MPI_SUM, out.rank, ...)`                                 |
| `2021/jun_b.c`                    | Prijem `local_c` preskače `p == root` umesto `p == out.rank` — ako pobednik nije root, `out.rank` poziva `MPI_Recv` od samog sebe → **deadlock**                                | `if (p == out.rank) continue;`                                            |
| `2021/septembar_b.c`              | Gather-zamena šalje/prima `l*m` umesto `l*n` (`local_c` je `l×n`); PROD redukcija prima u `col_prod`, ali množi `local_col_prod` (sopstveni bafer)                              | slati `l*n`; množiti **primljeni** bafer                                  |
| `2023/septembar_b.c`              | PROD redukcija: pošaljioci su svi osim root (`rank != root`), poruke idu ka `out.rank`, ali prima samo root → **deadlock** ako `out.rank != root`                               | pošaljioci `rank != out.rank`, primalac `rank == out.rank`                |
| `2025/april.c`                    | `MPI_Reduce(local_a, c, n, ...)` — šalje `local_a` (n×s elemenata!) umesto `local_c` (n elemenata) → prekoračenje bafera + pogrešni podaci                                      | `MPI_Reduce(local_c, c, n, MPI_INT, MPI_SUM, out.rank, ...)`              |
| `2026/april_a.c`                  | Množenje koristi globalnu `b` (nedefinisanu van root-a) umesto `local_b`; `MPI_Gather` slaže blokove kolona po ranku, što **ne rekonstruiše** raspored vrsta C                  | `a[i][l] * local_b[l][j]`; nakon Gather-a presložiti elemente po kolonama |
| `2025/jun_b.c`                    | Koeficijent je `rank*(rank+1)/2` umesto `p*(p+1)/2` (p = ukupan broj procesa) → nakon `Reduce(SUM)` rezultat je `(p-1)p(p+1)/6 · xi` umesto `p(p+1)/2 · xi`; ništa se ne štampa | `size*(size+1)/2`; dodati `printf` u root-u                               |

**Opšta pravila koja ovi bugovi krše (dobre provere pred kraj ispita):**

1. Ciklična raspodela mora imati `t += size` u petlji.
2. Pri P-to-P zameni `MPI_Reduce(..., target)`: pošaljioci su svi **osim target**, primalac je **samo target** — i jedno i drugo vezano za `out.rank`, ne za `root`.
3. U redukciji nizova uvek sabirati/množiti **primljeni** bafer, ne sopstveni.
4. Proveriti dimenzije bafera u `Send`/`Recv`/`Reduce`/`Gather` pozivima (`l×n` nije `l×m`).
5. `is_prime`: uslov je `i * i <= n`.

---

## Saveti za učenje

1. **Naučiti šablon Tip 1** — petlja sa rekonstrukcijom indeksa je najčešća na ispitu. Razumeti kako se iz `t` rekonstruišu `i` i `j` za uzlažni i silazni smer.
2. **Naučiti šablon Tip 2** — raspodela kolona matrice P-to-P, zatim grupne operacije za redukciju. Zamena a→b je uvek ista.
3. **Razumeti razliku Tip 3a vs Tip 3b vs Tip 3c** — Tip 3a deli A po **kolonama** + B po **vrstama** (svaki proces izračuna deo C samostalno); Tip 3b deli A po **vrstama** + cela B (`MPI_Bcast`) (svaki proces izračuna **različite vrste** finalne C); Tip 3c šalje celu A (`MPI_Bcast`) + deli B po **kolonama** (svaki proces izračuna **različite kolone** finalne C, sakuplja se `MPI_Gather`).
4. **Tip 3a varijanta (q=1)** — kada svaki proces dobije po **1** kolonu A i **1** vrstu B, lokalni rezultat je **spoljašnji proizvod** koji se sumira u finalnu C preko `MPI_Reduce(MPI_SUM)`.
5. **Tip 4 (Stablo)** — petlja `for (i = 1; i <= log₂(size); i++)` sa `half = 1 << (i-1)`. Tri kategorije: `rank < half` šalje, `half ≤ rank < 2*half` prima, ostali čekaju. Uvek se podrazumeva da je `size` stepen dvojke.
6. **Pamtiti strukturu** `struct { int value; int rank; }` — neophodna za `MPI_MINLOC`/`MPI_MAXLOC`.
7. **Redosled operacija** je uvek isti za Tip 1/2/3: **Raspodela → Lokalni račun → Reduce(LOC) → Bcast → Reduce(SUM)**.
8. **Jedinstveni zadaci** — povremeno se pojave zadaci koji ne pripadaju nijednom tipu (npr. Jun 2025 b). Važno je razumeti osnovne grupne operacije (`MPI_Bcast`, `MPI_Reduce`) i znati ih kombinovati u novim situacijama.
