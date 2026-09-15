# Zadatak - April 2026 (varijanta kalkulatora bez duplex-a)

Koristeći WCF kreirajte sistem kalkulatora.

Korisnik u svojoj sesiji može da obriše trenutno računanje, doda broj, oduzme broj, pomnoži brojem i podeli rezultat prosleđenim brojem. Svaka operacija se odmah izvršava nad rezultatom (prethodni rezultat) i smešta u rezultat. Operacija vraća rezultat izvršenja i do tog momenta kreirani izraz (na primer: izraz `2+3-5*7`).

Predvideti **prosleđivanje izuzetka klijentu**.

Obavezno izdvojiti:

- interfejs
- implementaciju
- web.config (dovoljan je samo deo za setovanje)
- klijentsku stranu koja demonstrira rad servisa

Klijentska strana mora pozvati sve metode servisa i prikazati njihov rezultat ako postoji.
