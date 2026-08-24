<!-- markdownlint-disable MD024 -->

# MPI Blanketi — Šabloni i Tipovi Zadataka

Ispitni zadaci se svode na **7 tipova**: Tip 1, Tip 2, četiri varijante Tip 3 (3a–3d) i Tip 4. Povremeno se pojavi **jedinstven** zadatak (Jun 2025 b, April 2026 b).

**a/b odnos:** kod većine rokova b) je direktna P-to-P zamena grupnih operacija iz a). **Izuzeci** (a i b su različiti zadaci): Oktobar 2022, Jun 2025, April 2026. April 2025 nema b fajl; Jun 2 2022/2023 su jedan fajl.

## Pregled tipova

| Tip    | Naziv                               | Ključna ideja                                                          | Primeri                                 |
| ------ | ----------------------------------- | ---------------------------------------------------------------------- | --------------------------------------- |
| **1**  | Ciklična raspodela dvostruke petlje | `for(i) for(j)` → rekonstrukcija indeksa iz `t = rank, rank+size, ...` | `2020/jun_a.c`, `2022/decembar_a.c`     |
| **2**  | Matrica × Vektor                    | svaki proces dobije `q` kolona A + `q` elemenata b                     | `2022/april_a.c`, `2025/april.c`        |
| **3a** | Matrica × Matrica                   | `q` kolona A + `q` vrsta B → deo C                                     | `2022/jun_2.c`, `2025/jun_a.c`          |
| **3b** | Matrica × Matrica                   | vrste A (`Scatter`) + cela B (`Bcast`) → vrste C → `Gather`            | `2022/oktobar_2_a.c`, `2025/januar_a.c` |
| **3c** | Matrica × Matrica                   | cela A (`Bcast`) + kolone B → kolone C → `Gather`                      | `2026/april_a.c`                        |
| **3d** | Matrica × Matrica                   | 1 kolona A + 1 vrsta B → spoljašnji proizvod, `Reduce(SUM)` u C        | `2022/oktobar_b.c`                      |
| **4**  | Stablo / Hiperkub                   | širenje podatka iz P0 svima u `log₂(p)` koraka                         | `2022/oktobar_a.c`                      |

**Univerzalni redosled (Tip 1/2/3):** Raspodela → Lokalni račun → `Reduce(MINLOC/MAXLOC)` → `Bcast(out)` → sakupljanje rezultata u `out.rank`.

---

## Tip 1: Ciklična raspodela dvostruke petlje

```c
for (int t = rank; t < total; t += size) {   // t += size je OBAVEZAN!
    int i = t / JN + offset_i;               // koja vrsta
    int j = (smer == GORE) ? B + (t % JN)    // uzlažni j
                           : B - (t % JN);   // silazni j
    local_sum += i + j;
    if (is_prime(i + j)) in.value++;
}
// zatim: Reduce(MINLOC/MAXLOC) → Bcast(out) → Reduce(SUM, out.rank)
```

| Parametar               | Formula                                                     |
| ----------------------- | ----------------------------------------------------------- |
| `JN` (broj vrednosti j) | `j < C` → `C - B`; `j <= C` → `C - B + 1` (isto za silazni) |
| `total`                 | `N * JN`                                                    |
| `is_prime` uslov        | `i * i <= n` (ne `<`!)                                      |

Ekstrem: Jun 2020 / Januar 2022 traže **min** prostih sabiraka; Decembar/Septembar 2022 **max** (uz pomak `y` i silazni `j`).

---

## Tip 2: Matrica × Vektor (kolonska raspodela)

```c
// 1. Root P-to-P šalje po q kolona A svakom procesu; vektor b ide MPI_Scatter
// 2. Lokalno:
for (int i = 0; i < rows; i++)
    for (int j = 0; j < q; j++) {
        local_c[i]       += local_a[i][j] * local_b[j];  // vektor c
        local_row_sum[i] += local_a[i][j];               // suma/proizvod vrste
        in.value = max(in.value, local_a[i][j]);         // ekstrem u A
    }
// 3. Reduce(MAXLOC) → Bcast(out) → Reduce(SUM, out.rank) za row_sum i c
```

Razlike po rokovima: April 2021 = po **1** kolona, **min**, proizvod vrsta; ostali (April 2022, Jun 2021, Decembar 2021, April 2025) = po `q`/`l`/`s` kolona, **max**, suma vrsta.

---

## Tip 3: Matrica × Matrica — četiri varijante

|               | **3a**                     | **3b**                                      | **3c**                               | **3d**                                 |
| ------------- | -------------------------- | ------------------------------------------- | ------------------------------------ | -------------------------------------- |
| Proces dobija | `q` kol A + `q` vrsta B    | vrste A (`Scatter`) + cela B (`Bcast`)      | cela A (`Bcast`) + kolone B (P-to-P) | **1** kol A + **1** vrsta B            |
| Izračuna      | deo C (`k×n`)              | **vrste** C                                 | **kolone** C                         | spoljašnji proizvod (doprinos celoj C) |
| C se sakuplja | `Reduce(SUM)`              | **`Gather`**                                | **`Gather`**                         | `Reduce(SUM)`                          |
| Ekstrem       | max u B (Jun 2025 a: nema) | max u C / min u A / max u A                 | min u B                              | nema                                   |
| Sekundarno    | proizvod kolona B          | proizvod kolona A (Jan 2025: suma kolona B) | min po vrstama C (posle Gather-a)    | —                                      |

```c
// Zajednički skelet:
// 1. raspodela po tabeli iznad
// 2. lokalno množenje: local_c[i][j] += local_a[i][l] * local_b[l][j]
//    (3d: local_c[i][j] = local_a[i] * local_b[j])
// 3. Reduce(LOC) → Bcast(out)          — osim 3d i Jun 2025 a (nema ekstrema)
// 4. sakupljanje u out.rank po tabeli  — Gather kad su delovi C disjunktni!
```

> **Scatter vs P-to-P:** tekstovi Tip 3b zadataka traže P-to-P za vrste A, ali sva rešenja koriste `MPI_Scatter` (ispravno, jer je `k` deljivo brojem vrsta po procesu). P-to-P slanje vrsta/kolona radi se u **b)** varijantama.

> **Gather vs Reduce(SUM) za C:** kad svaki proces ima **disjunktne** vrste/kolone C (3b, 3c) — `Gather`. Kad svi imaju **delimične doprinose** istim elementima (3a, 3d) — `Reduce(SUM)`.

---

## Tip 4: Stablo / Hiperkub (`log₂(p)` koraka, size = stepen dvojke)

```c
for (int i = 1; i <= log2(size); i++) {
    int half = 1 << (i - 1);        // 2^(i-1) = koliko procesa već zna podatak
    if (rank < half)
        MPI_Send(&data, 1, MPI_INT, rank + half, 0, ...);   // šalje svom paru
    else if (rank < 2 * half)
        MPI_Recv(&data, 1, MPI_INT, rank - half, 0, ...);   // prima od para
    // ostali čekaju sledeći korak
}
```

Svaki korak udvostručuje broj procesa koji znaju podatak: 1 → 2 → 4 → ... → `size`.

---

## Zamena grupnih operacija P-to-P (za b) varijante)

Sve zamene su isti obrazac — **root radi petlju po procesima, ostali jedan Send/Recv**:

```c
// Reduce/Bcast/Gather(X, root)  →
if (rank != root)
    MPI_Send(&x, n, tip, root, 0, ...);          // svi šalju root-u
else
    for (int p = 0; p < size; p++) {
        if (p == root) continue;
        MPI_Recv(&x, n, tip, p, 0, ...);         // root prima od svakog
        // za Reduce: odmah kombinuj u akumulator (sum += x, max, ...)
    }
// Bcast: iste petlje, samo root šalje svima, ostali primaju.
```

**Ključno:** kad je meta `out.rank` a ne `root` — pošaljioci su svi **osim `out.rank`**, primalac je **samo `out.rank`**.

---

## Master tabela po rokovima

| Rok            | Tip        | Raspodela                                | Ekstrem (gde)        | Sekundarno        | Ko štampa        | a/b          |
| -------------- | ---------- | ---------------------------------------- | -------------------- | ----------------- | ---------------- | ------------ |
| Jun 2020       | 1          | ciklična `i,j = 0..N-1`                  | min prostih sabiraka | —                 | proces sa min    | par          |
| Januar 2022    | 1          | **= Jun 2020**                           | min                  | —                 | proces sa min    | par          |
| Decembar 2022  | 1          | ciklična, pomak `y`, silazni `j`         | **max** prostih      | —                 | proces sa max    | par          |
| Septembar 2022 | 1          | **= Decembar 2022**                      | max                  | —                 | proces sa max    | par          |
| April 2021     | 2          | 1 kolona A + 1 elem b                    | **min** u A          | proizvod vrsta A  | proces sa min    | par          |
| April 2022     | 2          | `q` kolona A + `q` elem b                | max u A              | suma vrsta A      | proces sa max    | par          |
| Jun 2021       | 2          | `l` kolona A                             | max u A              | suma vrsta A      | proces sa max    | par          |
| Decembar 2021  | 2          | **= April 2022**                         | max u A              | suma vrsta A      | proces sa max    | par          |
| April 2025     | 2          | **= April 2022** (`s`)                   | max u A              | suma vrsta A      | proces sa max    | **samo a**   |
| Jun 2 2022     | 3a         | `q` kol A + `q` vrsta B                  | max u B              | proizvod kolona B | proces sa max    | jedan fajl   |
| Jun 2 2023     | 3a         | isto                                     | max u B              | proizvod kolona B | proces sa max    | jedan fajl   |
| Jun 2025 a     | 3a         | isto                                     | **nema**             | proizvod kolona B | **root**         | **nije par** |
| Septembar 2021 | 3b         | `l` vrsta A + cela B                     | max u C              | proizvod kolona A | proces sa max    | par          |
| Oktobar 2 2022 | 3b         | `s` vrsta A + cela B                     | max u C              | proizvod kolona A | proces sa max    | par          |
| Septembar 2023 | 3b         | `r` vrsta A + cela B                     | **min u A**          | proizvod kolona A | proces sa min    | par          |
| Januar 2025    | 3b         | `m` vrsta A + cela B                     | **max u A**          | suma kolona B     | proces sa max    | par          |
| Oktobar 2022 a | 4          | hiperkub iz P0                           | —                    | —                 | svi              | **nije par** |
| Oktobar 2022 b | 3d         | 1 kol A (P-to-P) + 1 vrsta B (`Scatter`) | nema                 | —                 | root             | **nije par** |
| Jun 2025 b     | jedinstven | `Bcast` X iz **P2**, `Reduce(SUM)`       | nema                 | formula `yi`      | (bug: ne štampa) | **nije par** |
| April 2026 a   | 3c         | cela A (`Bcast`) + `s` kol B (P-to-P)    | min u B              | min po vrstama C  | proces sa min    | **nije par** |
| April 2026 b   | jedinstven | kružna razmena `(rank+1)%size`           | —                    | —                 | —                | **nije par** |

---

## Poznati bugovi u kodu (nemoj prepisivati!)

| Fajl                              | Bug                                                                                                                 | Ispravka                                         |
| --------------------------------- | ------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------ |
| `2022/januar_a.c`                 | petlja bez `t += size` — svaki proces radi SVE iteracije                                                            | `t += size`                                      |
| `2020/jun_b.c`, `2022/januar_b.c` | `is_prime`: `i*i < n` → 4, 9, 25 prolaze kao prosti                                                                 | `i*i <= n`                                       |
| `2022/septembar_a.c`              | krajnji `Reduce(SUM)` cilja `root` umesto `out.rank`                                                                | cilj `out.rank`                                  |
| `2021/jun_b.c`                    | prijem `local_c` preskače `p == root` umesto `out.rank` → deadlock ako pobednik nije root                           | `p == out.rank`                                  |
| `2021/septembar_b.c`              | Gather-zamena šalje `l*m` umesto `l*n`; PROD redukcija množi sopstveni bafer umesto primljenog                      | slati `l*n`; množiti primljeni bafer             |
| `2023/septembar_b.c`              | PROD redukcija: šalju svi osim root, prima samo root, a poruke idu ka `out.rank` → deadlock                         | meta `out.rank` i za slanje i za prijem          |
| `2025/april.c`                    | `Reduce(local_a, c, n, ...)` šalje `local_a` (n×s!) umesto `local_c` (n)                                            | `Reduce(local_c, c, n, ...)`                     |
| `2026/april_a.c`                  | množenje koristi globalnu `b` (van root-a nedefinisanu) umesto `local_b`; `Gather` ne rekonstruiše raspored vrsta C | `local_b`; posle Gather-a presložiti po kolonama |
| `2025/jun_b.c`                    | koeficijent `rank*(rank+1)/2` umesto `p*(p+1)/2` → rezultat `(p-1)p(p+1)/6 · xi`; ništa se ne štampa                | `size*(size+1)/2`; `printf` u root-u             |

**Provere pred kraj ispita:** `t += size` u petlji · P-to-P redukcija vezana za `out.rank`, ne `root` · u redukciji kombinuj **primljeni** bafer · dimenzije bafera (`l×n` ≠ `l×m`) · `is_prime` sa `<=`.

---

## Saveti za učenje

1. **Tip 1** je najčešći — nauči rekonstrukciju indeksa (`t / JN`, `t % JN`) za oba smera.
2. **Tip 2 i Tip 3** dele isti skelet; razlika je samo šta se raspodeljuje i kako se C sakuplja (tabela Tip 3).
3. **`struct { int value; int rank; }`** — obavezna za `MINLOC`/`MAXLOC`.
4. **Tip 4**: `half = 1 << (i-1)`; `rank < half` šalje, `half ≤ rank < 2·half` prima, ostali čekaju.
5. Za **b)** nauči jedan generički P-to-P obrazac (gore) — sve zamene su ista petlja.
