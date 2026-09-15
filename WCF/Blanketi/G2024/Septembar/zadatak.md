# Zadatak - Septembar 2024

Koristeći WCF kreirati sistem za **registraciju vozila**.

Servis podržava sledeće funkcionalnosti:

- **Registracija vozila** — proslediti Vlasnika (ime, prezime i jmbg), Vozilo (marka, model, boja, broj šasije) i datum kraja registracije. Datum početka registracije je trenutak poziva, a broj registracije dodeljuje serverska strana.
- Vraća listu svih vozila zadatog vlasnika.
- Vraća listu istorije svih registracija zadatog vozila.
- Vraća listu svih vozila i njihovih registracija.

Obavezno izdvojiti:

- interfejs
- implementaciju
- web.config (dovoljan je samo deo za setovanje servisa)
- klijentsku stranu koja demonstrira rad servisa
