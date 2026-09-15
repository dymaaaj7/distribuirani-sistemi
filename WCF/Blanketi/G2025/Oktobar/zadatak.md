# Zadatak - Oktobar 2025

Koristeći WCF napisati sistem za rad **aritmetičkih operacija nad matricama**.

Operacije koje sistem treba da podrži:

- setovanje matrice na serverskoj strani
- sabiranje
- množenje matrice skalarom
- množenje matrice matricom
- transponovanje matrice
- preuzimanje matrice sa servisa

Operacije kao jedan od operanada uzimaju matricu koja je setovana na serverskoj strani; drugi operand se prosleđuje ako je potreban.

Sve operacije treba da imaju **zajedničku strukturu** kojom vraćaju rezultat, koja obavezno mora sadržati:

- flag da li je došlo do greške
- opis greške (ako je do nje došlo)
- sam rezultat operacije ako je uspešno izvršena

Klijent mora prikazati poziv svake operacije i prikaz rezultata. Obavezno napisati odgovarajuće konfiguracije.
