# Zadatak - Jun 2023 (čet + broadcast "SVI" + istorija poruka)

Koristeći WCF napisati **čet aplikaciju**.

Svaki korisnik pri startovanju klijenta registruje svoj nadimak. Ukoliko se registruje isti nadimak, smatrati da je prethodna sesija prestala da važi.

Klijent pod svojim nadimkom šalje poruku drugom klijentu tako što navede njegov nadimak, **ili** šalje poruku svima tako što za nadimak korisnika setuje rezervisanu reč `SVI`. Poruke namenjene svima stižu samo onim korisnicima koji su trenutno aktivni.

Korisnik može od servisa da zatraži **istoriju primljenih poruka** za traženi period — Message History (DateTime from, DateTime to). Po pozivu ove metode korisnik prikazuje sve poruke koje je dobio od servisa sa sledećim parametrima:

- ko je poslao
- kad je poslata
- sadržaj poruke
- da li je poruka namenjena samo njemu ili svima

Obavezno napisati i odgovarajuće konfiguracije.
