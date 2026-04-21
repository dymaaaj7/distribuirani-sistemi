# Distribuirani Sistemi — MPI Vežbe

Ovaj repozitorijum sadrži materijale, vežbe i laboratorijske zadatke iz predmeta **Distribuirani Sistemi**, sa fokusom na paralelno programiranje koristeći **MPI (Message Passing Interface)**.

---

## 📁 Struktura projekta

```
.
├── MPI/
│   ├── README.md              # (prazan — za buduće proširenje)
│   ├── SPISAKFUNKCIJA.md      # Referenca najčešće korišćenih MPI funkcija
│   ├── Vezbe/                 # Vežbe sa časova
│   │   ├── MPI_1_1.cpp
│   │   ├── MPI_1_2.cpp
│   │   ├── MPI_1_3.cpp
│   │   ├── MPI_1_4.cpp
│   │   ├── MPI_2_1.cpp
│   │   ├── MPI_2_2.cpp
│   │   ├── MPI_2_3.cpp
│   │   ├── MPI_3_1.cpp
│   │   ├── MPI_3_2.cpp
│   │   ├── MPI_3_3.cpp
│   │   └── MPI_3_4.cpp
│   ├── Lab/
│   │   └── PS-NP-Lab1PTP/     # Laboratorijska vežba — Point-to-Point komunikacija
│   │       ├── zadatak_1.cpp
│   │       ├── zadatak_2.cpp
│   │       ├── zadatak_3.cpp
│   │       ├── zadatak_4.cpp
│   │       └── zadatak_5.cpp
│   └── Blanketi/              # (prazan direktorijum — za buduće materijale)
└── Distribuirani Sistemi_PrezentacijeSpojene.pdf
```

---

## 🛠️ Preduslovi

Za kompilaciju i pokretanje programa potrebno je imati instaliranu MPI implementaciju, npr. **OpenMPI** ili **MPICH**:

```bash
# Ubuntu/Debian
sudo apt-get install openmpi-bin libopenmpi-dev

# Arch Linux
sudo pacman -S openmpi
```

---

## 🚀 Kompilacija i pokretanje

### Kompilacija pojedinačnog programa

```bash
mpicxx -o naziv_programa naziv_programa.cpp
```

### Pokretanje sa zadatim brojem procesa

```bash
mpirun --oversubscribe -np 4 ./naziv_programa
```

> Napomena: opcija `--oversubscribe` omogućava pokretanje više procesa nego što ima fizičkih jezgara (korisno na laptopovima).

---

## 📚 Pregled vežbi

### Vežbe — Point-to-Point (P-to-P) komunikacija

| Fajl | Opis |
|------|------|
| [`MPI_1_1.cpp`](MPI/Vezbe/MPI_1_1.cpp) | Lančano slanje podataka od procesa `P0` do `Pn` i nazad. Unos se završava negativnom vrednošću. |
| [`MPI_1_2.cpp`](MPI/Vezbe/MPI_1_2.cpp) | Suma prvih `n` celih brojeva koristeći P-to-P komunikaciju — svaki proces učestvuje u akumulaciji. |
| [`MPI_1_3.cpp`](MPI/Vezbe/MPI_1_3.cpp) | Suma `N` celih brojeva ( `N` je stepen dvojke ) korišćenjem parnog spajanja procesa u logaritamskom broju koraka. |
| [`MPI_1_4.cpp`](MPI/Vezbe/MPI_1_4.cpp) | Implementacija grupne operacije `MPI_Scatter` koristeći isključivo P-to-P komunikaciju u hiperkub topologiji. |

### Vežbe — Grupne operacije

| Fajl | Opis |
|------|------|
| [`MPI_2_1.cpp`](MPI/Vezbe/MPI_2_1.cpp) | Pronalaženje minimalne i maksimalne vrednosti među procesima, zajedno sa identifikatorima procesa (`MPI_MINLOC` / `MPI_MAXLOC`). |
| [`MPI_2_2.cpp`](MPI/Vezbe/MPI_2_2.cpp) | Pronalaženje maksimalne vrednosti na svakoj od 30 lokacija i identifikatora procesa koji je sadrži (`MPI_MAXLOC` nad nizovima). |
| [`MPI_2_3.cpp`](MPI/Vezbe/MPI_2_3.cpp) | Numerička integracija — aproksimacija broja **π** integracijom funkcije `f(x)=4/(1+x²)` na intervalu `[0,1]`. |
| [`MPI_3_1.cpp`](MPI/Vezbe/MPI_3_1.cpp) | Skalarni proizvod dva vektora dimenzije `N` korišćenjem `MPI_Scatter` i `MPI_Reduce`. |
| [`MPI_3_2.cpp`](MPI/Vezbe/MPI_3_2.cpp) | Množenje matrice `A(n×n)` i vektora `b(n)` — distribucija vrsta matrice i emitovanje vektora (`MPI_Scatter` + `MPI_Bcast` + `MPI_Gather`). |
| [`MPI_3_3.cpp`](MPI/Vezbe/MPI_3_3.cpp) | Pronalaženje minimalnog neparnog broja deljivog zadatom vrednošću `x` iz intervala `[a,b]` — raspodela brojeva po procesima u cikličkom redosledu. |
| [`MPI_3_4.cpp`](MPI/Vezbe/MPI_3_4.cpp) | Množenje matrice `A(m×n)` i vektora `b(n)` — distribucija kolona matrice `A` P-to-P operacijama, a preostalo grupnim operacijama. |

---

## 🧪 Laboratorijska vežba: Point-to-Point komunikacija

| Fajl | Opis |
|------|------|
| [`zadatak_1.cpp`](MPI/Lab/PS-NP-Lab1PTP/zadatak_1.cpp) | Svaki proces proverava parnost sume elemenata niza `bafer` i upisuje rezultat u odgovarajući bit celobrojnog podatka. Rezultat se šalje unazad do `P0`. |
| [`zadatak_2.cpp`](MPI/Lab/PS-NP-Lab1PTP/zadatak_2.cpp) | Root proces šalje identičnu poruku svim procesima, a zatim prima izmenjenu (obrnutu) poruku. Merenje vremena blokirajućih i neblokirajućih operacija. |
| [`zadatak_3.cpp`](MPI/Lab/PS-NP-Lab1PTP/zadatak_3.cpp) | Root proces šalje dva stringa svim ostalim procesima. Procesi prihvataju poruke u obrnutom redosledu i vraćaju svoj identifikator. |
| [`zadatak_4.cpp`](MPI/Lab/PS-NP-Lab1PTP/zadatak_4.cpp) | Root proces deli niz dužine `N` na `N/P` elemenata po procesu. Svaki proces nalazi lokalni minimum i šalje ga nazad. Root određuje globalni minimum. |
| [`zadatak_5.cpp`](MPI/Lab/PS-NP-Lab1PTP/zadatak_5.cpp) | Root proces prima sume elemenata niza od svih procesa i ispisuje identifikatore onih čija je suma deljiva identifikatorom procesa. |

---

## 📖 Dodatak: Referenca MPI funkcija

Fajl [`MPI/SPISAKFUNKCIJA.md`](MPI/SPISAKFUNKCIJA.md) sadrži priručnu referencu najčešće korišćenih MPI funkcija sa opisima parametara i napomenama:

- **Inicijalizacija i finalizacija**: `MPI_Init`, `MPI_Finalize`
- **Topologija**: `MPI_Comm_rank`, `MPI_Comm_size`
- **Blokirajuća komunikacija**: `MPI_Send`, `MPI_Recv`
- **Neblokirajuća komunikacija**: `MPI_Isend`, `MPI_Irecv`
- **Sinhronizacija**: `MPI_Wait`, `MPI_Test`, `MPI_Barrier`
- **Grupne operacije**: `MPI_Bcast`, `MPI_Scatter`, `MPI_Gather`, `MPI_Reduce`, `MPI_Scan`
- **Tipovi podataka** i **operacije** (`MPI_Op`)

---

## 📝 Napomene

- Kod nekih vežbi i zadataka (`MPI_1_3.cpp`, `MPI_1_4.cpp`, `MPI_2_3.cpp`, `MPI_3_3.cpp`, `MPI_3_4.cpp`, `zadatak_2.cpp`, `zadatak_3.cpp`) **nedostaje implementacija** — ostavljeni su samo komentari sa tekstom zadatka.
- Neke implementacije sadrže uočljive greške (npr. beskonačna petlja u [`zadatak_5.cpp`](MPI/Lab/PS-NP-Lab1PTP/zadatak_5.cpp) na liniji 36 usled nedostajućeg `i++`, ili uslov `rank < 0` koji se nikad ne ispunjava). Ove zadatke treba pažljivo pregledati pre pokretanja.

---

*Autor: Student kursa Distribuirani Sistemi*
