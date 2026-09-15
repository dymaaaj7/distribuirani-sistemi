# Zadatak - Oktobar 2 2025 (isti kao Jun 2026 i Januar 2025)

Koristeći WCF kreirajte **full-duplex** sistem kalkulatora.

Korisnik u svojoj sesiji može da obriše trenutno računanje, doda broj, oduzme broj, pomnoži brojem i podeli rezultat prosleđenim brojem. Svaka operacija se odmah izvršava nad rezultatom (prethodni rezultat) i smešta u rezultat. Operacija vraća rezultat izvršenja. Servis po izvršenju operacije poziva klijenta i prosleđuje mu do tog momenta kreirani izraz (na primer: izraz `2+3-5*7`).

Obavezno izdvojiti:

- interfejs
- implementaciju
- web.config (dovoljan je samo deo za setovanje servisa i na klijentskoj strani deo za callback)
- klijentsku stranu koja demonstrira rad servisa

Klijentska strana mora pozvati sve metode servisa i prikazati njihov rezultat ako postoji.
