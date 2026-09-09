# Distribuirani sistemi — srednja verzija

# 2. KOMUNIKACIJA

## 2.1 Poziv udaljene procedure (Remote Procedure Call – RPC)

Distribuirani sistemi se zasnivaju na slanju poruka, ali to otkriva detalje mreze i narusava transparentnost. RPC (Remote Procedure Call) omogućava programu da pozove proceduru na drugoj mašini kao da je lokalna — podaci se prenose preko mreze, a klijent čeka rezultat. RPC je osnova klijent-server modela i mnogih distribuiranih sistema.

Problemi koje RPC mora da resi:

1. **Prenos parametara** – po vrednosti ili referenci
2. **Format podataka** – uskladjivanje razlicitih formata klijenata i servera
3. **Pronalazenje procedure** – lociranje masine/servera koji izvrsava proceduru
4. **Rukovanje greskama** – sta ako otkazu klijent, server ili mreza

Primeri RPC tehnologija: Sun RPC, DCE RPC, MS DCOM, CORBA, Java RMI.

## 2.2 Kako RPC funkcionise?

Cilj: udaljeni poziv izgleda kao običan lokalni poziv. Postize se pomocu stub funkcija:

1. **Klijentski stub** – izgleda kao obična funkcija, ali šalje zahtev serveru i prima rezultat preko mreze
2. **Server stub** – prima zahtev, raspakuje parametre, poziva pravu serversku funkciju i vraća rezultat klijentu

Sva mrežna komunikacija je skrivena u stub funkcijama — klijent i server "vide" samo obične pozive procedura. Stubovi se generišu za svaku udaljenu proceduru posebno.

### 2.2.1 10 koraka izvrsenja RPC poziva:

1. Klijent poziva klijentski stub sa parametrima
2. Klijentski stub pakuje argumente u poruku (marshaling)
3. Lokalni OS šalje poruku udaljenom OS-u preko mreze
4. Serverski OS prosleđuje poruku serverskom stubu
5. Serverski stub raspakuje argumente i poziva serversku proceduru
6. Server izvršava proceduru i vraća rezultat
7. Serverski stub pakuje rezultat i šalje odgovor
8. Serverski OS šalje odgovor klijentskom OS-u
9. Klijentski OS prosleđuje odgovor klijentskom stubu
10. Klijentski stub raspakuje rezultat i vraća ga klijentu

## 2.3 Prenos parametara kod RPC

Klijentski stub pakuje parametre u poruku zajedno sa identifikacijom procedure i salje serveru; server raspakuje poruku, poziva odgovarajucu proceduru, a rezultat vraca istim postupkom u suprotnom smeru.

Nacini prenosa parametra:

1. **Prenos po vrednosti** – u poruku se kopira vrednost parametara, server radi sa kopijom
2. **Prenos po referenci** – adresa vazi samo na lokalnoj masini, pa se salje sam podatak, server ga menja i vraca klijentu
3. **Call-by-copy/restore** – parametar se kopira pri pozivu, a izmenjena vrednost se nakon izvrsenja vraca preko originala (kombinuje prenos po vrednosti i referenci)

Nacin prenosa odredjuje programski jezik — u C-u se skalarni tipovi prenose po vrednosti, a polja preko referenci. Razlicite masine drugacije predstavljaju podatke (big/little-endian, ASCII/EBCDIC), pa stubovi koriste standardni format i vrse konverziju pri slanju i prijemu.

## 2.4 Povezivanje klijenta i servera (Binding)

Pre poziva udaljene procedure, klijent mora pronaci server i njegovu adresu:

1. **Statičko povezivanje** – adresa servera je ugradjena u klijentski stub; jednostavno i brzo, ali promena adrese zahteva rekompajliranje klijenta
2. **Dinamičko povezivanje** – klijentski stub kontaktira server imena/direktorijum, koji na osnovu procedure pronalazi adresu servera; promena adrese menja samo zapis u direktorijumu, bez izmene klijenta

## 2.5 Greške i semantika RPC poziva

Kod lokalnog poziva procedura se izvrsi tacno jednom; kod udaljenog je moguce da se izvrsi 0 puta (server otkaze pre izvrsenja), tacno jednom (sve u redu) ili jednom ili vise puta (veliko kasnjenje ili gubitak odgovora izaziva retransmisiju).

Vecina RPC sistema nudi:

1. **“bar jednom” semantiku** – za idempotentne funkcije, koje se mogu ponavljati bez posledica (npr. citanje)
2. **“najvise jednom” semantiku** – za funkcije koje nisu idempotentne (npr. modifikacija fajla)

Zato se transparentnost ovde delimicno gubi — aplikacija mora biti spremna da obradi moguce greske.

## 2.6 Programiranje sa RPC i IDL

Vecina jezika (C, C++, Java) nema ugradjenu podrsku za RPC, pa se koristi IDL (Interface Definition Language). Koraci:

1. IDL definise interfejs (udaljene procedure, njihove parametre i povratne vrednosti)
2. IDL/RPC kompajler generise klijentski i serverski stub
3. Stubovi se povezuju sa programima — klijentski i serverski kod se linkuju sa odgovarajucim stubovima
