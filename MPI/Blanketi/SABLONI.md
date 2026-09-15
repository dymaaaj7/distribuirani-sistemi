<!-- markdownlint-disable MD024 -->
# MPI Blanketi — Šabloni (kompaktna verzija)

> Detaljna analiza sa svim varijantama i obrazloženjima: [SABLONI-DETALJNO.md](SABLONI-DETALJNO.md)
> Rešenja po rokovima: [2020](2020/) – [2026](2026/)

**Suština:** od 27 zadataka sa rokova 2020–2026, njih **19 je jedan te isti pipeline** (Tip 1, 2, 3). Ostalo su samostalni tipovi: hiperkub — Tip 4 (2 zadatka), Bcast + formula — Tip 5 (2), niz + formula — Tip 6 (2), teorijska pitanja (2). Uči se skelet + tabela prekidača, ne sve tipove odvojeno.

---

## 1. Univerzalni skelet (Tip 1, 2, 3 i varijante)

```
RASPODELA → LOKALNI RAČUN → Reduce(LOC) → Bcast → Reduce(SUM)/Gather → štampa
```

```c
struct { int value; int rank; } in, out;

// --- 1. RASPODELA (v. cheat sheet ispod: Scatter / Bcast / P-to-P) ---

// --- 2. LOKALNI RAČUN + lokalni ekstrem ---
in.value = /* prvi element koji se poredi */;
in.rank  = rank;
for (...)
{
    // račun: množenje, suma, proizvod, formula...
    if (x > in.value)            // ili "<" za minimum
        in.value = x;
}

// --- 3. KO ŠTAMPA: proces sa ekstremom ---
MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC /* ili MPI_MINLOC */, root, MPI_COMM_WORLD);
MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

// --- 4. REZULTAT ide u proces koji štampa ---
MPI_Reduce(local, global, n, MPI_INT, MPI_SUM /* ili MPI_PROD */, out.rank, MPI_COMM_WORLD);

// --- 5. ŠTAMPA ---
if (rank == out.rank) { /* printf... */ }
```

**Zapamti:**

- `in.rank = rank` je obavezno — bez toga MINLOC/MAXLOC nema od koga da bira.
- Redukcija (korak 4) ide u `out.rank`, **ne u root** — zato Bcast (korak 3) mora doći pre nje.
- Root grupe operacije može biti **bilo koji rang** (npr. proces 2 u Tip 5), ne samo 0.
- Zadaci bez ekstrema (Jun 2025 a) preskaču korak 3 — root štampa.
- Po zastavici u tekstu: min/max, u kojoj matrici (A/B/C), suma/proizvod, po čemu se broje prosti.

### Raspodela — cheat sheet

| Zadatak | Proces dobija | Kako se šalje |
|---|---|---|
| matrica × vektor (Tip 2) | `q` kolona A + `q` elemenata b | kolone P-to-P, b `MPI_Scatter` |
| mat × mat (Tip 3a) | `q` kolona A + `q` vrsta B | kolone P-to-P, vrste `MPI_Scatter` |
| mat × mat, `q = 1` | 1 kolona A + 1 vrsta B | kolona P-to-P, vrsta `MPI_Scatter` |
| mat × mat (Tip 3b) | vrste A + **celu** B | vrste P-to-P, B `MPI_Bcast` |
| mat × mat (Tip 3c) | **celu** A + kolone B | A `MPI_Bcast`, kolone P-to-P |

Slanje bloka P-to-P (root ostavlja sebi prvi, ostalima šalje `p*blok`-ti deo):

```c
if (rank == root) {
    // kopija za root direktno iz matrice
    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        /* pakuj blok p u tmp */
        MPI_Send(tmp, blok, MPI_INT, p, 0, MPI_COMM_WORLD);
    }
} else {
    MPI_Recv(local, blok, MPI_INT, root, 0, MPI_COMM_WORLD, &status);
}
```

### Reduce ili Gather?

- Svaki proces doprinosi **celom** rezultatu (3a: parcijalni `local_c[k][n]`; `q=1`: spoljašnji proizvod `local_a[i]·local_b[j]`; Tip 2: parcijalne sume vektora c) → **`MPI_Reduce(MPI_SUM)`**.
- Svaki proces ima **svoj deo** bez preklapanja (3b: svoje vrste C; 3c: svoje kolone C) → **`MPI_Gather`** (radi i Reduce jer se delovi ne preklapaju, ali Gather je prirodniji).

### Šablon 3 petlje (univerzalan za sve matrice zadatke)

Jedan skelet — tekst zadatka samo popuni imenice. Zameniš `A`→`B`, `<`→`>`, `+=`→`*=` i gotovo:

```c
// 1. RASPODELA — po tekstu (cheat sheet iznad)

// 2. EKSTREM — ista petlja, samo min/max po tekstu (ili je nema)
in.value = INT_MIN;  in.rank = rank;               // min: INT_MAX i uslov >
for (i po lokalnom delu)
    if (in.value < local[i][j]) in.value = ...;

// 3. REZULTAT — uvek isti oblik, nikad se ne menja
local_c[i][j] += komad_A * komad_B;

// 4. AGREGACIJA — ista petlja: + ili *, kolone ili vrste (ili je nema)
for (j = 0; j < n; j++) local_sum[j] = 0;          // proizvod: kreni od 1!
for (i = rank; i < k; i += size)                   // ciklično po celoj matrici
    local_sum[j] += b[i][j];

// 5. SPAJANJE — uvek iste 4 linije, samo destinacija (DEST)
MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC, root, ...);
MPI_Bcast(&out, ...);                              // svi moraju znati out.rank
MPI_Gather(local_c, ..., DEST, ...);
MPI_Reduce(local_sum, sum, n, MPI_SUM, DEST, ...);

// 6. ISPIS — u istom procesu gde se skupilo
```

Sve o destinaciji (DEST):

- tekst traži ekstrem → sve ide u **out.rank** (i ispis tamo)
- ne traži → sve ide u **root**

Poeni koje pojedu: neutralni element (0 za sumu, **1 za proizvod**), i ne mešati dve stvari u istu petlju.

---

## 2. Ciklična raspodela petlje (Tip 1)

`for(i) for(j) s += i + j` cikličnom raspodelom, **bez indeksiranih promenljivih** — rekonstrukcija `i` i `j` iz linearne pozicije `t = rank, rank+p, rank+2p, ...`:

```c
int JN = /* broj vrednosti j (v. tabelu) */;
for (int t = rank; t < N * JN; t += size) {
    int i = t / JN + pocetak_i;
    int j = silazno ? B - (t % JN) : B + (t % JN);
    local_sum += i + j;
    if (is_prime(i + j))
        in.value++;              // broj prostih sabiraka
}
```

| Unutrašnja petlja | JN | Rekonstrukcija `j` |
|---|---|---|
| `j++`, `j < C` | `C - B` | `j = B + t % JN` |
| `j++`, `j <= C` | `C - B + 1` | `j = B + t % JN` |
| `j--`, `j > C` | `B - C` | `j = B - t % JN` |
| `j--`, `j >= C` | `B - C + 1` | `j = B - t % JN` |

Ukupan broj iteracija je `N * JN`. Posle petlje — opet univerzalni skelet, koraci 3–5 (MINLOC/MAXLOC po **broju prostih**, Bcast, Reduce(SUM) u `out.rank`).

---

## 3. Hiperkub / stablo (Tip 4)

Slanje podatka iz P0 svim ostalima u `log₂(p)` koraka (size je stepen dvojke):

```c
int steps = (int)log2(size);

for (int i = 1; i <= steps; i++) {
    int half = 1 << (i - 1);              // 2^(i-1)
    if (rank < half)                      // šalje svom paru
        MPI_Send(&data, 1, MPI_INT, rank + half, 0, MPI_COMM_WORLD);
    else if (rank < 2 * half)             // prima od para
        MPI_Recv(&data, 1, MPI_INT, rank - half, 0, MPI_COMM_WORLD, &status);
    // rank >= 2*half ne radi ništa u ovom koraku!
}
```

- **Tri kategorije po koraku:** `rank < half` šalje, `half ≤ rank < 2·half` prima, ostali čekaju. `else` umesto `else if` = deadlock.
- U koraku `i` broj procesa koji znaju podatak se **udvostručuje**: 1 → 2 → 4 → ... → p.
- Grupna zamena: `MPI_Bcast(&data, 1, MPI_INT, root, MPI_COMM_WORLD)` (Okt 2025 a pita baš to).

---

## 4. Sitni tipovi

### Tip 5: Bcast + formula + Reduce (Septembar 2024 b = Jun 2025 b)

Dve grupne operacije, bez raspodele — `yi = (p(p+1)/2)·xi`:

```c
MPI_Bcast(x, n, MPI_INT, 2, MPI_COMM_WORLD);       // inicijalizacija u procesu 2!
for (int i = 0; i < n; i++)
    local_y[i] = x[i] * (rank + 1);
MPI_Reduce(local_y, y, n, MPI_INT, MPI_SUM, 2, MPI_COMM_WORLD);
```

Formula se **dobija kroz redukciju**: `Σ(rank+1) = p(p+1)/2`.

### Tip 6: Niz + formula (Oktobar 2025 b = Jun 2026 a — identičan tekst!)

`R = Σ(ā+aᵢ)/(b+c)`, gde je `ā` srednja vrednost niza. Scatter niza + skelet iz sekcije 1, uz tri specifičnosti:

```c
MPI_Scatter(a, k, MPI_INT, local_a, k, MPI_INT, root, MPI_COMM_WORLD);  // k = N/size

// lokalni prolaz: suma, lokalni max, broj prostih (kao u skeletu)

// ā zahteva GLOBALNU sumu pre lokalnog računa:
MPI_Reduce(&local_sum, &sum_elem, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
MPI_Bcast(&sum_elem, 1, MPI_INT, root, MPI_COMM_WORLD);
avg = (double)sum_elem / N;

// doprinos procesa za Σ(ā+aᵢ) po sopstvenim elementima:
local_part = local_sum + k * avg;
```

- `b` i `c` se inicijalizuju u **MAXLOC procesu** i odatle emituju (`MPI_Bcast` sa root = `out_max.rank`)
- štampa u procesu sa **najmanjim brojem prostih** (MINLOC po brojaču)
- finalno: `MPI_Reduce(&local_part, &total, 1, MPI_DOUBLE, MPI_SUM, out_min.rank, ...)` pa `R = total/(b+c)`

### Teorijska pitanja (2026)

- **April 2026 b** — kružna razmena: svaki proces šalje `b1` sledećem `(rank+1)%size`, prima od prethodnog (`MPI_Send` + `MPI_Recv`).
- **Jun 2026 b** — svi imaju `b1[2]`, svi treba `b2[8]` sa svim elementima: `MPI_Gather(b1, 2, MPI_INT, b2, 2, MPI_INT, root, ...)` + `MPI_Bcast(b2, 8, MPI_INT, root, ...)`. (Jednim pozivom: `MPI_Allgather` — ako je rađen na predmetu.)

---

## 5. a → b konverzija (grupne → P-to-P)

Varijanta b) uvek menja iste tri operacije istim receptom (root = bilo koji ciljni proces):

| Grupna | P-to-P zamena |
|---|---|
| `MPI_Reduce(..., MPI_MINLOC/MAXLOC, root)` | svi šalju `in` root-u; root u petlji prima i upoređuje (čuva manji/veći sa rankom) |
| `MPI_Bcast(..., root)` | root u petlji šalje svima; ostali primaju |
| `MPI_Reduce(..., MPI_SUM/PROD, target)` — skalar | svi šalju target-u; on sabira/množi |
| `MPI_Reduce(..., MPI_SUM, target)` — niz | svi šalju niz target-u; on sabira **po elementima** |
| `MPI_Scatter` | root u petlji šalje svakom njegov deo |
| `MPI_Gather` | svi šalju root-u; on slaže delove |

```c
// primer: Reduce(SUM) niza -> P-to-P
if (rank != target) {
    MPI_Send(local_arr, n, MPI_INT, target, 0, MPI_COMM_WORLD);
} else {
    for (int i = 0; i < n; i++) global_arr[i] = local_arr[i];
    for (int p = 0; p < size; p++) {
        if (p == target) continue;
        MPI_Recv(local_arr, n, MPI_INT, p, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < n; i++) global_arr[i] += local_arr[i];
    }
}
```

---

## 6. Rok-indeks (koji rok je koji šablon)

| Rok | Šablon | Bitno |
|---|---|---|
| Jun 2020 | Tip 1 | bazni: `i,j` od 0 do N-1, **min** prostih |
| Januar 2022 | Tip 1 | identičan Junu 2020 |
| Septembar 2024 a | Tip 1 | identičan Junu 2020 |
| Septembar 2022 | Tip 1 | pomak `y`, silazni `j`, **max** prostih |
| Decembar 2022 | Tip 1 | isto kao Septembar 2022 |
| April 2021 | Tip 2 | po **1** kolona, **min** + proizvod po vrstama |
| Jun 2021 | Tip 2 | po `l` kolona, max + suma |
| Decembar 2021 | Tip 2 | po `q` kolona, max + suma |
| April 2022 | Tip 2 | identičan Decembru 2021 |
| April 2025 | Tip 2 | identičan (po `s`) |
| Septembar 2021 | Tip 3b | `l` vrsta A + cela B, max u **C**, proizvod kolona A |
| Oktobar 2 2022 | Tip 3b | `s` vrsta A + cela B, max u **C** |
| Septembar 2023 | Tip 3b | `r` vrsta A + cela B, **min u A** |
| Januar 2025 | Tip 3b | `m` vrsta A + cela B, max u A, **suma kolona B** |
| Septembar 2025 | Tip 3b | **identičan Januaru 2025** (tekst reč po reč) |
| April 2026 a | Tip 3c | cela A + `s` kolona B, **min u B**, `MPI_Gather` |
| Jun 2 2022 | Tip 3a | `q` kolona A + `q` vrsta B, max u B, proizvod kolona B |
| Jun 2 2023 | Tip 3a | identičan Junu 2 2022 |
| Jun 2025 a | Tip 3a | isto, ali **bez ekstrema** — root štampa |
| Oktobar 2022 b | Tip 3a (`q=1`) | 1 kolona + 1 vrsta → spoljašnji proizvod + Reduce(SUM) |
| Oktobar 2025 b = Jun 2026 a | Tip 6 | `R = Σ(ā+aᵢ)/(b+c)`; b i c u MAXLOC procesu, štampa u MINLOC po broju prostih |
| Oktobar 2022 a = Oktobar 2025 a | Tip 4 | hiperkub; 2025 dodaje pitanje o `MPI_Bcast` |
| Septembar 2024 b = Jun 2025 b | Tip 5 | Bcast iz P2 + formula `yi=(p(p+1)/2)·xi` + Reduce |
| April 2026 b | teorija | kružna razmena u prstenu |
| Jun 2026 b | teorija | Gather + Bcast (ili Allgather) |

---

## 7. Kako se uči

1. **Skelet naizust** (sekcija 1) — koraci 1–5 + zašto Bcast ide pre Reduce u `out.rank`.
2. **Cheat sheet raspodele** — po tekstu zadatka prepoznaj šta ko dobija; ostalo je uvek isto.
3. **Reduce vs Gather pravilo** — preklapa li se doprinos procesa (Reduce) ili su delovi različiti (Gather).
4. **Tip 1 rekonstrukcija indeksa** — formula tabela iz sekcije 2.
5. **Hiperkub** — `half = 1 << (i-1)`, tri kategorije, `else if`!
6. **a→b konverzija** — uvek isti recept, sekcija 5.
7. Kad ideš kroz rokove, radi po Rok-indeksu: prvi svake vrsti, ostali su replike.
