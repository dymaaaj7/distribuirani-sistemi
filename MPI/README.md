# MPI

- `Vezbe/` — vežbe sa časova (P2P i grupne operacije)
- `Lab/` — laboratorijska vežba (P2P)
- `Blanketi/` — rešenja rokova 2020–2026, po godinama; `a` = grupne operacije, `b` = P2P
- [`Blanketi/SABLONI.md`](Blanketi/SABLONI.md) — šabloni za sve tipove zadataka, početi odavde
- [`SPISAKFUNKCIJA.md`](SPISAKFUNKCIJA.md) — referenca MPI funkcija
- [`template.c`](template.c) — osnovni skelet programa

```bash
mpicc zadatak.c -o zadatak
mpirun -np 4 ./zadatak
```

Kod dela vežbi je ostavljen samo tekst zadatka u komentarima, bez implementacije.
