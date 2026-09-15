# Zadatak - Jun 2 2026

Koristeći JMS kreirajte sistem za **razmenu poruka**.

Sistem se sastoji od dve metode:

- **Start(string ime)** — startuje klijenta koji prima poruke adresirane na zadato ime
- **Posalji(string ime, string tekst)** — šalje poruku (prosleđeni tekst) klijentu sa prosleđenim imenom

Može postojati više klijenata sa istim imenom — smatrati da su svi klijenti sa istim imenom **isti korisnik**.
