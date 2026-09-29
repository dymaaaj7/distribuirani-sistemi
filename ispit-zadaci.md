<!-- markdownlint-disable MD024 -->
# Blanketi — pregled po rokovima

Legenda:

- `[x]` – rešenje postoji u repozitorijumu
- `[ ]` – samo tekst zadatka (ili nema ničega)

---

## MPI

<!-- Napomene, kratki rečnik: A×B / A×b = množenje matrica / matrice vektorom;
     "m vrsta A + cela B" = šta master deli procesima (P2P = vrsta se šalje P2P);
     "štampa u max(A)" = prikaz u procesu koji sadrži ekstrem matrice A;
     ciklična = suma i+j po cikličnoj raspodeli petlje. -->

### 2026

- [x] **April** — a: A×B, cela A svima (Bcast) + po s kolona B (P2P); min u B, min po vrstama C. b: kružna razmena — svako šalje sledbeniku
- [x] **Jun** — a: niz, R = Σ(ā+aᵢ)/(b+c). b: teorija, minimalan broj poziva (Gather pa Bcast)

### 2025

- [x] **Oktobar** — a: stablo broadcast (P2P) + pitanje za Bcast zamenu. b: niz, R = Σ(ā+aᵢ)/(b+c)
- [x] **Septembar** — A×B, m vrsta A + cela B; suma kolona B; štampa u max(A) — identičan Jan 2025
- [x] **Jun** — a: A×B, q kolona A (P2P) + q vrsta B (Scatter); proizvod kolona B. b: Bcast niza iz P2, yi=(p(p+1)/2)·xi, Reduce SUM
- [x] **April** — A×b, po s kolona A (P2P) + s elemenata b; max u A, suma po vrstama A — kao April 2022
- [x] **Januar** — A×B, m vrsta A + cela B; suma kolona B; štampa u max(A) (a: grupne, b: P2P)

### 2024

- [ ] **Oktobar** —
- [x] **Septembar** — a: ciklična, min broj prostih sabiraka. b: Bcast niza iz P2, yi=(p(p+1)/2)·xi — kao Jun 2025 b
- [ ] **Jun** —

### 2023

- [ ] **Oktobar 2** —
- [ ] **Oktobar** — a: stablo — kao Okt 2022 a. b: A×B, kolona A (P2P) + vrsta B — kao Okt 2022 b
- [x] **Septembar** — A×B, r vrsta A + cela B; proizvod kolona A; štampa u min(A)
- [x] **Jun 2** — A×B, q kolona A + q vrsta B; proizvod kolona B; max u B
- [ ] **Jun** —
- [ ] **April** —
- [ ] **Januar** —

### 2022

- [x] **Oktobar 2** — A×B, s vrsta A + cela B; proizvod kolona A; max u C
- [x] **Oktobar** — a: stablo broadcast. b: A×B, kolona A (P2P) + vrsta B (Scatter)
- [x] **Septembar** — ciklična, j na dole + pomeraj y — kao Decembar 2022
- [x] **Jun 2** — A×B, q kolona A + q vrsta B; proizvod kolona B; max u B — kao Jun 2 2023
- [ ] **Jun** — (tekst nejasan)
- [x] **April** — A×b, po q kolona A + q elemenata b; max u A, suma po vrstama
- [x] **Januar** — ciklična, min prostih — kao Jun 2020
- [x] **Decembar** — ciklična, j na dole + pomeraj y — kao Septembar 2022

### 2021

- [ ] **Oktobar 2** —
- [ ] **Oktobar** —
- [x] **Septembar** — A×B, l vrsta A + cela B; proizvod kolona A
- [x] **Jun** — A×b, po l kolona A + l elemenata b
- [ ] **Maj** —
- [x] **April** — A×b, po 1 kolona A + 1 element b
- [x] **Decembar** — A×b, po q kolona A + q elemenata b — kao April 2022

### 2020

- [ ] **Oktobar** —
- [ ] **Septembar** —
- [ ] **Jul — dodatni** —
- [x] **Jun** — ciklična, min prostih — kao Januar 2022

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
- [x] **Jun 2** — Razmena poruka (1×) — Start(ime) prima / Posalji(ime, tekst) šalje, više klijenata istog imena = isti korisnik
- [ ] **Septembar** —

### 2025

- [ ] **Septembar** — Ispitivanje (4×) — isti kao Januar 2025, Septembar i Oktobar 2024
- [x] **Oktobar 2** — Temperatura (4×) — isti kao April 2025 i Septembar 2023
- [ ] **Oktobar** — Minesweeper (1×) — svako polje je klijent, selector po x/y koordinatama, agregacija odgovora suseda
- [x] **Jun** — Aplikacija za obavestenja (3×) — identičan Junu 2026 i Aprilu 2026
- [x] **April** — Temperatura (4×) — hotel, selector po lokaciji, isti kao Septembar 2023 i Oktobar 2 2025; varijanta Oktobra 2023
- [x] **Januar** — Ispitivanje (4×) — urađeno (Sistem.java); funkcije Pokreni/PosaljiPitanje, topic za pitanja + queue za odgovore

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
