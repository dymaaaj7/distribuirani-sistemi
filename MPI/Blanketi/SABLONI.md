<!-- markdownlint-disable MD024 -->
# MPI Blanketi — Šabloni

**Suština:** 19 od 27 zadataka (2020–2026) je jedan te isti pipeline (Tip 1, 2, 3). Ostalo: hiperkub (Tip 4), Bcast + formula (Tip 5), niz + formula (Tip 6), teorijska pitanja. Uči se skelet + tablice, ne svaki zadatak ponaosob.

---

## 1. Univerzalni skelet (Tip 1, 2, 3)

```
RASPODELA → LOKALNI RAČUN → Reduce(LOC) → Bcast → Reduce(SUM)/Gather → štampa
```

```c
struct { int value; int rank; } in, out;

// 1. RASPODELA — po tekstu zadatka (tablica ispod)

// 2. LOKALNI RAČUN + lokalni ekstrem
in.value = /* prvi element */;  in.rank = rank;      // min: uslov "<"
for (...)
{
    local_c[i][j] += komad_A * komad_B;              // rezultat — uvek isti oblik
    if (x > in.value) in.value = x;                  // zadatak bez ekstrema: preskoči
}

// 3. KO ŠTAMPA: proces sa ekstremom
MPI_Reduce(&in, &out, 1, MPI_2INT, MPI_MAXLOC /* min: MPI_MINLOC */, root, MPI_COMM_WORLD);
MPI_Bcast(&out, 1, MPI_2INT, root, MPI_COMM_WORLD);

// 4. REZULTAT ide u out.rank (bez ekstrema: u root)
MPI_Reduce(local_sum, sum, n, MPI_INT, MPI_SUM /* proizvod: MPI_PROD */, out.rank, MPI_COMM_WORLD);

// 5. ŠTAMPA
if (rank == out.rank) { /* printf... */ }
```

- `in.rank = rank` je obavezno — MINLOC/MAXLOC bira po ranku.
- Redukcija ide u `out.rank`, ne u root — zato Bcast mora doći pre nje.
- Neutralni element: 0 za sumu, **1 za proizvod**.

### Raspodela — tablica

| Zadatak | Proces dobija | Kako se šalje |
|---|---|---|
| matrica × vektor (Tip 2) | `q` kolona A + `q` elemenata b | kolone P-to-P, b `MPI_Scatter` |
| mat × mat (Tip 3a) | `q` kolona A + `q` vrsta B | kolone P-to-P, vrste `MPI_Scatter` |
| mat × mat, `q = 1` | 1 kolona A + 1 vrsta B | kolona P-to-P, vrsta `MPI_Scatter` |
| mat × mat (Tip 3b) | vrste A + **celu** B | vrste P-to-P, B `MPI_Bcast` |
| mat × mat (Tip 3c) | **celu** A + kolone B | A `MPI_Bcast`, kolone P-to-P |

Slanje bloka P-to-P (root ostavlja sebi prvi deo direktno iz matrice):

```c
if (rank == root) {
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

- Doprinosi procesa se **preklapaju** (parcijalne sume/spoljašnji proizvodi) → `MPI_Reduce(MPI_SUM)`.
- Delovi su **različiti** (svoje vrste/kolone C) → `MPI_Gather`.

---

## 2. Ciklična raspodela petlje (Tip 1)

`for(i) for(j) s += i + j` ciklično, **bez indeksiranih promenljivih** — `i` i `j` se rekonstruišu iz linearne pozicije `t = rank, rank+p, rank+2p, ...`:

```c
int JN = /* broj vrednosti j (tablica) */;
for (int t = rank; t < N * JN; t += size) {
    int i = t / JN + pocetak_i;
    int j = silazno ? B - (t % JN) : B + (t % JN);
    local_sum += i + j;
    if (is_prime(i + j)) in.value++;      // broj prostih sabiraka
}
```

| Unutrašnja petlja | JN | Rekonstrukcija `j` |
|---|---|---|
| `j++`, `j < C` | `C - B` | `j = B + t % JN` |
| `j++`, `j <= C` | `C - B + 1` | `j = B + t % JN` |
| `j--`, `j > C` | `B - C` | `j = B - t % JN` |
| `j--`, `j >= C` | `B - C + 1` | `j = B - t % JN` |

Posle petlje — skelet iz sekcije 1, koraci 3–5 (MINLOC/MAXLOC po **broju prostih**, Bcast, Reduce(SUM) u `out.rank`).

---

## 3. Hiperkub / stablo (Tip 4)

Podatak iz P0 svim ostalima u `log₂(p)` koraka (size je stepen dvojke):

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

- Tri kategorije po koraku: `rank < half` šalje, `half ≤ rank < 2·half` prima, ostali čekaju. `else` umesto `else if` = deadlock.
- U koraku `i` broj procesa koji znaju podatak se udvostručuje: 1 → 2 → 4 → ... → p.
- Grupna zamena: `MPI_Bcast` (Okt 2025 a pita baš to).

---

## 4. Sitni tipovi

### Tip 5: Bcast + formula + Reduce (Sept 2024 b = Jun 2025 b)

Bez raspodele — `yi = (p(p+1)/2)·xi` se dobija kroz redukciju `Σ(rank+1)`:

```c
MPI_Bcast(x, n, MPI_INT, 2, MPI_COMM_WORLD);       // inicijalizacija u procesu 2!
for (int i = 0; i < n; i++)
    local_y[i] = x[i] * (rank + 1);
MPI_Reduce(local_y, y, n, MPI_INT, MPI_SUM, 2, MPI_COMM_WORLD);
```

### Tip 6: Niz + formula (Oktobar 2025 b = Jun 2026 a — identičan tekst)

`R = Σ(ā+aᵢ)/(b+c)` — Scatter niza + skelet iz sekcije 1, uz tri specifičnosti:

```c
MPI_Scatter(a, k, MPI_INT, local_a, k, MPI_INT, root, MPI_COMM_WORLD);  // k = N/size

// ā zahteva GLOBALNU sumu pre lokalnog računa:
MPI_Reduce(&local_sum, &sum_elem, 1, MPI_INT, MPI_SUM, root, MPI_COMM_WORLD);
MPI_Bcast(&sum_elem, 1, MPI_INT, root, MPI_COMM_WORLD);
avg = (double)sum_elem / N;

// doprinos procesa za Σ(ā+aᵢ) po sopstvenim elementima:
local_part = local_sum + k * avg;
```

- `b` i `c` se inicijalizuju u **MAXLOC procesu** i odatle emituju (`MPI_Bcast` sa root = `out_max.rank`).
- Štampa u procesu sa **najmanjim brojem prostih** (MINLOC po brojaču).
- Finalno: `MPI_Reduce(&local_part, &total, 1, MPI_DOUBLE, MPI_SUM, out_min.rank, ...)` pa `R = total/(b+c)`.

### Teorijska pitanja (2026)

- **April 2026 b** — kružna razmena: svako šalje `b1` sledećem `(rank+1)%size`, prima od prethodnog.
- **Jun 2026 b** — svi imaju `b1[2]`, svima treba `b2[8]`: `MPI_Gather` u root + `MPI_Bcast` iz root-a (jednim pozivom: `MPI_Allgather`).

---

## 5. a → b konverzija (grupne → P-to-P)

Varijanta b) uvek menja iste operacije istim receptom (root = bilo koji ciljni proces):

| Grupna | P-to-P zamena |
|---|---|
| `MPI_Reduce(..., MPI_MINLOC/MAXLOC, root)` | svi šalju `in` root-u; root u petlji prima i upoređuje (čuva ekstrem sa rankom) |
| `MPI_Bcast(..., root)` | root u petlji šalje svima; ostali primaju |
| `MPI_Reduce(..., MPI_SUM/PROD, target)` — skalar | svi šalju target-u; on sabira/množi |
| `MPI_Reduce(..., MPI_SUM, target)` — niz | svi šalju niz target-u; on sabira **po elementima** |
| `MPI_Scatter` | root u petlji šalje svakom njegov deo |
| `MPI_Gather` | svi šalju root-u; on slaže delove |

---

## 6. Rok-indeks (koji rok je koji šablon)

- **Tip 1**: Jun 2020 = Jan 2022 = Sept 2024 a (bazni, min prostih) · Sept 2022 = Dec 2022 (pomak `y`, silazni `j`, max prostih)
- **Tip 2** (matrica × vektor): Apr 2021 (po 1 kolona) · Jun 2021 = Dec 2021 = Apr 2022 = Apr 2025 (po q kolona)
- **Tip 3b** (vrste A + cela B): Sept 2021 · Okt 2 2022 · Sept 2023 (min u A) · Jan 2025 = Sept 2025 (max u A, suma kolona B)
- **Tip 3a** (kolone A + vrste B): Jun 2 2022 = Jun 2 2023 · Jun 2025 (bez ekstrema) · Okt 2022 b (`q=1`)
- **Tip 3c** (cela A + kolone B): Apr 2026 a
- **Tip 4**: Okt 2022 a = Okt 2025 a
- **Tip 5**: Sept 2024 b = Jun 2025 b
- **Tip 6**: Okt 2025 b = Jun 2026 a
- **Teorija**: Apr 2026 b (kružna) · Jun 2026 b (Gather + Bcast)
