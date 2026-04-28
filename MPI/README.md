# MPI — Message Passing Interface

Ovaj folder sadrži sve materijale vezane za **MPI** deo predmeta Distribuirani Sistemi: vežbe sa časova, laboratorijske zadatke, rešenja ispitnih zadataka (blanketa) i reference.

> [← Nazad na root README](../README.md)

---

## 📁 Struktura

```
MPI/
├── README.md              # Ovaj fajl
├── SPISAKFUNKCIJA.md      # Referenca najčešće korišćenih MPI funkcija
├── template.c             # Šablon za MPI programe
├── Vezbe/                 # Vežbe sa časova
│   ├── MPI_1_1.c
│   ├── MPI_1_2.c
│   ├── MPI_1_3.c
│   ├── MPI_1_4.c
│   ├── MPI_2_1.c
│   ├── MPI_2_2.c
│   ├── MPI_2_3.c
│   ├── MPI_3_1.c
│   ├── MPI_3_2.c
│   ├── MPI_3_3.c
│   └── MPI_3_4.c
├── Lab/
│   └── PS-NP-Lab1PTP/     # Laboratorijska vežba — Point-to-Point komunikacija
│       ├── primer.c
│       ├── zadatak_1.c
│       ├── zadatak_2.c
│       ├── zadatak_3.c
│       ├── zadatak_4.c
│       └── zadatak_5.c
└── Blanketi/              # Rešenja ispitnih zadataka po godinama
    ├── SABLONI.md         # Izvučeni šabloni i tipovi zadataka
    ├── 2020/
    │   ├── jun_a.c
    │   └── jun_b.c
    ├── 2021/
    │   ├── april_a.c
    │   ├── april_b.c
    │   ├── jun_a.c
    │   ├── jun_b.c
    │   ├── septembar_a.c
    │   └── septembar_b.c
    └── 2022/
        ├── april_a.c
        ├── april_b.c
        ├── decembar_a.c
        ├── decembar_b.c
        ├── januar_a.c
        ├── januar_b.c
        ├── jun_2.c
        ├── oktobar_2_a.c
        ├── oktobar_2_b.c
        ├── oktobar_a.c
        ├── oktobar_b.c
        ├── septembar_a.c
        └── septembar_b.c
```

---

## 📚 Pregled vežbi

### Vežbe — Point-to-Point (P-to-P) komunikacija

| Fajl | Opis |
|------|------|
| [`MPI_1_1.c`](Vezbe/MPI_1_1.c) | Lančano slanje podataka od procesa `P0` do `Pn` i nazad. Unos se završava negativnom vrednošću. |
| [`MPI_1_2.c`](Vezbe/MPI_1_2.c) | Suma prvih `n` celih brojeva koristeći P-to-P komunikaciju — svaki proces učestvuje u akumulaciji. |
| [`MPI_1_3.c`](Vezbe/MPI_1_3.c) | Suma `N` celih brojeva (`N` je stepen dvojke) korišćenjem parnog spajanja procesa u logaritamskom broju koraka. |
| [`MPI_1_4.c`](Vezbe/MPI_1_4.c) | Implementacija grupne operacije `MPI_Scatter` koristeći isključivo P-to-P komunikaciju u hiperkub topologiji. |

### Vežbe — Grupne operacije

| Fajl | Opis |
|------|------|
| [`MPI_2_1.c`](Vezbe/MPI_2_1.c) | Pronalaženje minimalne i maksimalne vrednosti među procesima, zajedno sa identifikatorima procesa (`MPI_MINLOC` / `MPI_MAXLOC`). |
| [`MPI_2_2.c`](Vezbe/MPI_2_2.c) | Pronalaženje maksimalne vrednosti na svakoj od 30 lokacija i identifikatora procesa koji je sadrži (`MPI_MAXLOC` nad nizovima). |
| [`MPI_2_3.c`](Vezbe/MPI_2_3.c) | Numerička integracija — aproksimacija broja **π** integracijom funkcije `f(x)=4/(1+x²)` na intervalu `[0,1]`. |
| [`MPI_3_1.c`](Vezbe/MPI_3_1.c) | Skalarni proizvod dva vektora dimenzije `N` korišćenjem `MPI_Scatter` i `MPI_Reduce`. |
| [`MPI_3_2.c`](Vezbe/MPI_3_2.c) | Množenje matrice `A(n×n)` i vektora `b(n)` — distribucija vrsta matrice i emitovanje vektora (`MPI_Scatter` + `MPI_Bcast` + `MPI_Gather`). |
| [`MPI_3_3.c`](Vezbe/MPI_3_3.c) | Pronalaženje minimalnog neparnog broja deljivog zadatom vrednošću `x` iz intervala `[a,b]` — raspodela brojeva po procesima u cikličkom redosledu. |
| [`MPI_3_4.c`](Vezbe/MPI_3_4.c) | Množenje matrice `A(m×n)` i vektora `b(n)` — distribucija kolona matrice `A` P-to-P operacijama, a preostalo grupnim operacijama. |

---

## 🧪 Laboratorijska vežba: Point-to-Point komunikacija

| Fajl | Opis |
|------|------|
| [`primer.c`](Lab/PS-NP-Lab1PTP/primer.c) | Primer osnovne P-to-P komunikacije između dva procesa. |
| [`zadatak_1.c`](Lab/PS-NP-Lab1PTP/zadatak_1.c) | Svaki proces proverava parnost sume elemenata niza `bafer` i upisuje rezultat u odgovarajući bit celobrojnog podatka. Rezultat se šalje unazad do `P0`. |
| [`zadatak_2.c`](Lab/PS-NP-Lab1PTP/zadatak_2.c) | Root proces šalje identičnu poruku svim procesima, a zatim prima izmenjenu (obrnutu) poruku. Merenje vremena blokirajućih i neblokirajućih operacija. |
| [`zadatak_3.c`](Lab/PS-NP-Lab1PTP/zadatak_3.c) | Root proces šalje dva stringa svim ostalim procesima. Procesi prihvataju poruke u obrnutom redosledu i vraćaju svoj identifikator. |
| [`zadatak_4.c`](Lab/PS-NP-Lab1PTP/zadatak_4.c) | Root proces deli niz dužine `N` na `N/P` elemenata po procesu. Svaki proces nalazi lokalni minimum i šalje ga nazad. Root određuje globalni minimum. |
| [`zadatak_5.c`](Lab/PS-NP-Lab1PTP/zadatak_5.c) | Root proces prima sume elemenata niza od svih procesa i ispisuje identifikatore onih čija je suma deljiva identifikatorom procesa. |

---

## 📝 Blanketi — Ispitni zadaci

Rešenja zadataka sa prethodnih rokova podeljena su po godinama. Svaki rok obično sadrži zadatke `a)` (grupne operacije) i `b)` (P-to-P operacije).

### 2020

| Fajl | Tema |
|------|------|
| [`jun_a.c`](Blanketi/2020/jun_a.c) | Ciklična raspodela dvostruke petlje, suma `i+j`, pronalaženje procesa sa najmanjim brojem prostih sabiraka — **grupne operacije**. |
| [`jun_b.c`](Blanketi/2020/jun_b.c) | Isti zadatak — **P-to-P operacije**. |

### 2021

| Fajl | Tema |
|------|------|
| [`april_a.c`](Blanketi/2021/april_a.c) | Množenje matrice `A(n×k)` i vektora `B(k)` — po jedna kolona/element vektora, minimum i proizvod po vrstama — **grupne operacije**. |
| [`april_b.c`](Blanketi/2021/april_b.c) | Isti zadatak — **P-to-P operacije**. |
| [`jun_a.c`](Blanketi/2021/jun_a.c) | Množenje matrice `A(k×m)` i vektora `b(m)` — po `l` kolona/elemenata vektora, maksimum i suma po vrstama — **grupne operacije**. |
| [`jun_b.c`](Blanketi/2021/jun_b.c) | Isti zadatak — **P-to-P operacije**. |
| [`septembar_a.c`](Blanketi/2021/septembar_a.c) | Množenje matrica `A(k×m)` i `B(m×n)` — po `l` vrsta matrice `A`, cela matrica `B`, proizvod elemenata po kolonama matrice `A`, maksimum u `C` — **grupne operacije**. |
| [`septembar_b.c`](Blanketi/2021/septembar_b.c) | Isti zadatak — **P-to-P operacije**. |

### 2022

| Fajl | Tema |
|------|------|
| [`april_a.c`](Blanketi/2022/april_a.c) | Množenje matrice `A(k×l)` i vektora `b(l)` — raspodela po `q` kolona po procesu, maksimum i suma po vrstama — **grupne operacije**. |
| [`april_b.c`](Blanketi/2022/april_b.c) | Isti zadatak — **P-to-P operacije**. |
| [`decembar_a.c`](Blanketi/2022/decembar_a.c) | Ciklična raspodela dvostruke petlje sa pomakom `y` i silaznim `j` — pronalaženje procesa sa najvećim brojem prostih sabiraka — **grupne operacije**. |
| [`decembar_b.c`](Blanketi/2022/decembar_b.c) | Isti zadatak — **P-to-P operacije**. |
| [`januar_a.c`](Blanketi/2022/januar_a.c) | Ciklična raspodela dvostruke petlje `i+j`, pronalaženje procesa sa najmanjim brojem prostih sabiraka — **grupne operacije** (isto kao jun 2020). |
| [`januar_b.c`](Blanketi/2022/januar_b.c) | Isti zadatak — **P-to-P operacije**. |
| [`jun_2.c`](Blanketi/2022/jun_2.c) | Množenje matrice `A(k×m)` i matrice `B(m×n)` — po `q` kolona matrice A i `q` vrsta matrice B po procesu, proizvod kolona matrice B, maksimum — **grupne + P-to-P operacije**. |
| [`oktobar_2_a.c`](Blanketi/2022/oktobar_2_a.c) | Množenje matrica `A(k×m)` i `B(m×n)` — po `s` vrsta matrice A, cela matrica B, proizvod elemenata svake kolone matrice A, maksimum u matrici C — **grupne + P-to-P operacije**. |
| [`oktobar_2_b.c`](Blanketi/2022/oktobar_2_b.c) | Isti zadatak — **P-to-P operacije** (zamena svih grupnih operacija). |
| [`septembar_a.c`](Blanketi/2022/septembar_a.c) | Ciklična raspodela dvostruke petlje sa pomakom `y` i silaznim `j` — pronalaženje procesa sa najvećim brojem prostih sabiraka — **grupne operacije** (isto kao decembar 2022). |
| [`septembar_b.c`](Blanketi/2022/septembar_b.c) | Isti zadatak — **P-to-P operacije**. |
| [`oktobar_a.c`](Blanketi/2022/oktobar_a.c) | Stablo — P-to-P slanje podatka od procesa 0 svim ostalim u logaritamskom broju koraka — **P-to-P operacije**. |
| [`oktobar_b.c`](Blanketi/2022/oktobar_b.c) | Stablo — P-to-P slanje podatka od procesa 0 svim ostalim u logaritamskom broju koraka — **P-to-P operacije**. |

---

## 🔧 Šabloni i tipovi zadataka

Fajl [`Blanketi/SABLONI.md`](Blanketi/SABLONI.md) sadrži detaljnu analizu svih ispitnih zadataka sa izvučenim **šablonima** i **tipovima**:

- **Tip 1**: Ciklična raspodela dvostruke petlje — rekonstrukcija indeksa `i` i `j` iz `t`, pronalaženje procesa sa ekstremom (MINLOC/MAXLOC), sumiranje u izabrani proces.
- **Tip 2**: Matrica × vektor — kolonska raspodela matrice `A`, lokalno izračunavanje, redukcija rezultata u procesu sa ekstremom.
- **Tip 3**: Matrica × matrica — raspodela `q` kolona matrice `A` i `q` vrsta matrice `B`, lokalno množenje, redukcija rezultata u procesu sa ekstremom.
- **Konverzija a) → b)**: Univerzalni šabloni za zamenu `MPI_Reduce`, `MPI_Bcast` i `MPI_Reduce` nad nizovima P-to-P operacijama.
- **Tabela ponavljanja**: Koji su rokovi identični ili varijacije istog zadatka.

---

## 📖 Dodatak: Referenca MPI funkcija

Fajl [`SPISAKFUNKCIJA.md`](SPISAKFUNKCIJA.md) sadrži priručnu referencu najčešće korišćenih MPI funkcija sa opisima parametara i napomenama:

- **Inicijalizacija i finalizacija**: `MPI_Init`, `MPI_Finalize`
- **Topologija**: `MPI_Comm_rank`, `MPI_Comm_size`
- **Blokirajuća komunikacija**: `MPI_Send`, `MPI_Recv`
- **Neblokirajuća komunikacija**: `MPI_Isend`, `MPI_Irecv`
- **Sinhronizacija**: `MPI_Wait`, `MPI_Test`, `MPI_Barrier`
- **Grupne operacije**: `MPI_Bcast`, `MPI_Scatter`, `MPI_Gather`, `MPI_Reduce`, `MPI_Scan`
- **Tipovi podataka** i **operacije** (`MPI_Op`)

---

## 📝 Napomene

- Kod nekih vežbi i zadataka (`MPI_1_3.c`, `MPI_1_4.c`, `MPI_2_3.c`, `MPI_3_3.c`, `MPI_3_4.c`, `zadatak_2.c`, `zadatak_3.c`) **nedostaje implementacija** — ostavljeni su samo komentari sa tekstom zadatka.
- Fajl [`template.c`](template.c) sadrži osnovni šablon za MPI programe sa inicijalizacijom, dobijanjem ranga i veličine komunikatora, kao i finalizacijom.
