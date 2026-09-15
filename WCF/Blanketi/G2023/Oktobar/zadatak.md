# Zadatak - Oktobar 2023

Koristeći WCF kreirajte sistem za **prijavu na turnir studentskih igara**.

Potrebno je podržati mogućnost:

- **Prijava** — Student (indeks, ime, prezime, prosečna ocena) i disciplina.
- **Odjava** — Student i disciplina koju odjavljuje.
- Sve prijavljene discipline za studenta.
- Broj studenata po disciplini.
- Sve grupe i njihove učesnike za traženu disciplinu.

Smatrati da svaka disciplina inicijalno deli u **grupe od po 16 studenata**. Voditi računa da se pri dodavanju/uklanjanju studenta grupe **rebalansiraju**:

- grupe su uređene po proseku u nerastućem redosledu
- grupe su balansirane ako je prosek učesnika na i-toj poziciji bolji od svih učesnika ostalih grupa na i+1 poziciji

Obavezno izdvojiti:

- interfejs
- implementaciju
- web.config (dovoljan je samo deo za setovanje servisa)
- klijentsku stranu koja demonstrira rad servisa i klijentski config ako je potreban

Klijentska strana mora pozvati sve metode servisa i prikazati njihov rezultat ako postoji.
