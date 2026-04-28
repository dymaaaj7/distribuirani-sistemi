# MPI Blanketi — Šabloni i Tipovi Zadataka

Na osnovu analize zadataka sa prethodnih rokova, MPI ispitni zadaci se mogu svrstati u **dva osnovna tipa**. Svaki zadatak dolazi u varijanti **a)** (grupne operacije) i **b)** (P-to-P operacije), pri čemu je b) uvek direktna zamena grupnih operacija iz a).

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

### Tip 3b: Raspodela po vrstama matrice A (i cela matrica B)

### Primeri
- **Oktobar 2 2022** — po `s` vrsta matrice A, cela matrica B, proizvod kolona matrice A

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

| Parametar | Oktobar 2 2022 |
|-----------|----------------|
| Vrsta A po procesu | `s` (konstanta) |
| Matrica B | cela (`MPI_Bcast`) |
| Operacija na A | proizvod kolona |
| Ekstrem | maksimum u C |
| Štampanje | proces sa maksimumom |

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
| Oktobar 2 2022 | Tip 3b | Po `s` vrsta A, cela B, **max** u C + **proizvod** kolona A |

---

## Saveti za učenje

1. **Naučiti šablon Tip 1** — petlja sa rekonstrukcijom indeksa je najčešća na ispitu. Razumeti kako se iz `t` rekonstruišu `i` i `j` za uzlažni i silazni smer.
2. **Naučiti šablon Tip 2** — raspodela kolona matrice P-to-P, zatim grupne operacije za redukciju. Zamena a→b je uvek ista.
3. **Pamtiti strukturu** `struct { int value; int rank; }` — neophodna za `MPI_MINLOC`/`MPI_MAXLOC`.
4. **Redosled operacija** je uvek isti: **Raspodela → Lokalni račun → Reduce(LOC) → Bcast → Reduce(SUM)**.
