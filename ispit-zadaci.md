<!-- markdownlint-disable MD024 -->
# Praćenje odrađenih zadataka sa ispita

Legenda:

- `[ ]` – nije odrađen
- `[x]` – odrađen

---

## MPI

### 2026

- [x] **April** — a - Tip 3c — Cela matrica A (Bcast) + s kolona B (P-to-P), min u B, min po vrstama C; b - Jedinstven — Kružna razmena b1→sledeći, prima od prethodnog
- [x] **Jun** — a - Tip 6 — niz + formula, isti zadatak kao Oktobar 2025 b (R = Σ(ā+aᵢ)/(b+c)); b - teorijski — Gather + Bcast (kao April 2026 b)

### 2025

- [x] **Oktobar** — a - Tip 4 — stablo (kao Oktobar 2022 a); b - Tip 6 — niz + formula, R = Σ(ā+aᵢ)/(b+c)
- [x] **Jun** — a - Tip 3a — Matrica × matrica, po q kolona A + q vrsta B, proizvod kolona B (bez ekstrema); b - Jedinstven — Bcast niza X iz P2, formula yi=(p(p+1)/2)*xi, Reduce SUM
- [x] **April** — Tip 2 — Matrica × vektor, po `s` kolona, max + suma po vrstama (kao April 2022)
- [x] **Januar** — Tip 3b — Matrica × matrica, po `m` vrsta A + cela B, suma kolona B, maksimum u A

### 2024

- [ ] **Oktobar** —
- [x] **Septembar** — a - Tip 1 — Ciklična raspodela dvostruke petlje, min broj prostih sabiraka (kao Jun 2020); b - Jedinstven — Bcast niza X iz P2, formula yi=(p(p+1)/2)*xi, Reduce SUM (kao Jun 2025 b)
- [ ] **Jun** —

### 2023

- [ ] **Oktobar 2** —
- [ ] **Oktobar** —
- [x] **Septembar** — Matrica × matrica, po r vrsta matrice A, proizvod kolona A, prikaz u procesu sa minimumom
- [x] **Jun 2** — Matrica × matrica, po q kolona A i q vrsta B, max u B + proizvod kolona B
- [ ] **Jun** —
- [ ] **April** —
- [ ] **Januar** —

### 2022

- [x] **Oktobar 2** — Tip 3b — Matrica × matrica, po `s` vrsta matrice A, cela matrica B, proizvod kolona A, maksimum u C
- [x] **Oktobar** — a - stablo; b - matrica i matrica
- [x] **Septembar** — Tip 1 - Niz, kao Decembar 2022
- [x] **Jun 2** — Tip 3 - Matrica i matrica (A×B, q kolona/vrsta)
- [ ] **Jun** — Čudno
- [x] **April** — Matrica i vektor, po q kolona/elemenata vektora
- [x] **Januar** — Niz, kao Jun 2020
- [x] **Decembar** — Niz

### 2021

- [ ] **Oktobar 2** —
- [ ] **Oktobar** —
- [x] **Septembar** — Matrica × matrica, po l vrsta matrice A + proizvod kolona matrice A
- [x] **Jun** — Matrica i vektor, po l kolona/elemenata vektora
- [ ] **Maj** —
- [x] **April** — Matrica i vektor — po jedna kolona/element vektora
- [x] **Decembar** — Matrica i vektor, po q kolona/elemenata vektora (kao April 2022)

### 2020

- [ ] **Oktobar** —
- [ ] **Septembar** —
- [ ] **Jul — dodatni** —
- [x] **Jun** — Niz, kao Januar 2022

### 2019

- [ ] **Oktobar** —
- [ ] **Septembar** —
- [ ] **Jun** —
- [ ] **April** —
- [ ] **Januar** —

---

## JMS

### 2026

- [x] **April** — Aplikacija za obavestenja (3×) — isti kao Jun 2026 i Jun 2025
- [x] **Jun** — Aplikacija za obavestenja (3×) — Sender sa funkcijom za slanje + Receiver sa listenerom, MapMessage (Autor/Datum/Tekst), isti kao April 2026 i Jun 2025
- [x] **Jun 2** — Razmena poruka (1×) — Start(ime) prima / Posalji(ime, tekst) šalje; TextMessage, property Ime za selector, load-balans = "isti korisnik"
- [ ] **Septembar** —

### 2025

- [ ] **Septembar** — Ispitivanje (4×) — isti kao Januar 2025, Septembar i Oktobar 2024
- [x] **Oktobar 2** — Temperatura (4×) — urađena (Oktobar_2/Klijent.java), isti kao April 2025 i Septembar 2023
- [ ] **Oktobar** — Minesweeper (1×) — svako polje je klijent, selector po x/y koordinatama, agregacija odgovora suseda
- [x] **Jun** — Aplikacija za obavestenja (3×) — identičan Junu 2026 i Aprilu 2026
- [x] **April** — Temperatura (4×) — hotel, selector po lokaciji, isti kao Septembar 2023 i Oktobar 2 2025; varijanta Oktobra 2023
- [ ] **Januar** — Ispitivanje (4×) — funkcije Pokreni/PosaljiPitanje, isti kao Septembar 2025, Septembar i Oktobar 2024

### 2024

- [ ] **Oktobar** — Ispitivanje (4×) — identičan Januaru 2025 i Septembru 2024
- [ ] **Septembar** — Ispitivanje (4×) — identičan Januaru 2025, Septembru 2025 i Oktobru 2024
- [ ] **Jun** —

### 2023

- [ ] **Oktobar 2** — Mail (3×) — isti kao Jun 2023; Jan 2022 je durable varijanta
- [ ] **Oktobar** — Temperatura (4×) — delta varijanta sa "dopuniti", bez slanja zahteva (samo ispis)
- [x] **Septembar** — Temperatura (4×) — isti kao April 2025 i Oktobar 2 2025
- [ ] **Jun 2** — Merge sort (2×) — rekurzija porukama, deljenje niza na polovine, agregacija dva dela, isti kao Oktobar 2022
- [ ] **Jun** — Mail (3×) — topic obavezan, više primalaca, JMSTimestamp, isti kao Oktobar 2 2023; Jan 2022 je durable varijanta
- [ ] **April** — Pomoć-u-mreži (1×) — necentralizovana, funkcija "pomoć", selector po tipu posla
- [ ] **Januar** — Lanac stanica (3×) — ObjectMessage sa listom poslova, prosleđivanje, isti kao Oktobar 2 i Decembar 2022

### 2022

- [ ] **Oktobar 2** — Lanac stanica (3×) — isti kao Decembar 2022 i Januar 2023
- [ ] **Oktobar** — Merge sort (2×) — rekurzija porukama, isti kao Jun 2 2023
- [ ] **Septembar** —
- [ ] **Jun 2** — Prodavnica (3×) — ObjectMessage + propertiji, selector sa AND/BETWEEN, isti kao April i Jun 2022
- [ ] **Jun** — Prodavnica (3×) — isti kao April 2022 i Jun 2 2022
- [ ] **April** — Prodavnica (3×) — isti kao Jun 2022 i Jun 2 2022
- [ ] **Januar** — Mail (3×) — durable varijanta, poruke čekaju neaktivne korisnike, isti zadatak kao Jun 2023 i Oktobar 2 2023
- [ ] **Decembar** — Lanac stanica (3×) — isti kao Oktobar 2 2022 i Januar 2023

### 2021 — 2019

- (prazni folderi — tekstovi još nisu nađeni)

---

## WCF

### 2026

- [ ] **April** — Kalkulator varijanta — bez duplex-a + prosleđivanje izuzetka klijentu (FaultException)
- [ ] **Jun** — Kalkulator (4×) — full-duplex, callback vraća izraz, isti kao Januar 2025 i Oktobar 2 2025
- [ ] **Jun 2** — Tačno vreme (2×) — trenutno vreme + brojač poziva po sesiji, isti kao Septembar 2026
- [ ] **Septembar** — Tačno vreme (2×) — isti kao Jun 2 2026

### 2025

- [ ] **Septembar** — Čet (4×) — isti kao Jun 2025 (sesija i dalje važi)
- [ ] **Oktobar 2** — Kalkulator (4×) — isti kao Jun 2026 i Januar 2025
- [ ] **Oktobar** — Matrice (3×) — Result struktura (flag greške, opis, rezultat)
- [ ] **Jun** — Čet (4×) — duplex, sesija i dalje važi (isti Septembar 2025)
- [ ] **April** — Čet (4×) — delta: re-registracija nadimka poništava staru sesiju
- [ ] **Januar** — Kalkulator (4×) — replika Jun 2026

### 2024

- [ ] **Oktobar** — Vozila (2×) — isti kao Septembar 2024
- [ ] **Septembar** — Vozila (2×) — vlasnik + vozilo + istorija, server dodeljuje broj (isti Oktobar 2024)
- [ ] **Jun** — Skladišta (1×) — vlasnik + skladište + istorija zakupa

### 2023

- [ ] **Oktobar 2** — Matrice (3×) — isti kao Oktobar 2025 i Januar 2023
- [ ] **Oktobar** — Turnir (1×) — grupe od 16, rebalans po proseku (final boss)
- [ ] **Septembar** — Prijava ispita (1×) — prijava/odjava, liste i brojači
- [ ] **Jun 2** — Parcele (1×) — poligon → površina
- [ ] **Jun** — Čet + broadcast `SVI` + istorija poruka (delta na čet porodicu)
- [ ] **April** — Sokovi (3×) — stateful mikser, gustina kao ponder
- [ ] **Januar** — Matrice (3×) — loše skeniran tekst, isti kao Oktobar 2025

### 2022

- [ ] **Jun** — Sokovi (3×) — isti kao April 2022 i April 2023
- [ ] **April** — Sokovi (3×) — isti kao Jun 2022

### 2021 — 2019

- (prazni folderi — tekstovi još nisu nađeni)

---

## Nedostajući rokovi

- 2026 — Oktobar, Decembar
- 2025 — Decembar
- 2024 — April, Januar, Decembar
- 2023 — Decembar
- 2021 — Januar
- 2020 — April, Januar, Decembar
- 2019 — Decembar
