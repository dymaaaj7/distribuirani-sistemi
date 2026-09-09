# SADRZAJ

2. [KOMUNIKACIJA](#2-komunikacija)
   2.1. [Poziv udaljene procedure (Remote Procedure Call – RPC)](#21-poziv-udaljene-procedure-remote-procedure-call--rpc)
   2.2. [Kako RPC funkcionise?](#22-kako-rpc-funkcionise)
   2.3. [Prenos parametara kod RPC](#23-prenos-parametara-kod-rpc)
   2.4. [Povezivanje klijnta i servera (Binding)](#24-povezivanje-klijnta-i-servera-binding)
   2.5. [Greške i semantika RPC poziva](#25-greške-i-semantika-rpc-poziva)
   2.6. [Programiranje sa RPC i IDL](#26-programiranje-sa-rpc-i-idl)

---

# 2. KOMUNIKACIJA

## 2.1 Poziv udaljene procedure (Remote Procedure Call – RPC)

Distribuirani sistemi se zasnivaju na slanju poruka, ali to otkriva detalje mreze i narusava transparentnost. RPC (Remote Procedure Call) omogućava programu da pozove proceduru koja se nalazi na drugoj mašini, kao da je lokalna. Podaci se prenose preko mreže, a klijent čeka rezultat. RPC je osnova klijent-server modela i mnogih distribuiranih sistema.

Problemi koje RPC mora da resi jesu:

1. **Prenos parametara** – po vrednosti ili referenci
2. **Format podataka** – uskladjivanje razlicitih formata klijenata i servera
3. **Pronalazenje procedure** – lociranje masine/servera koji izvrsava proceduru
4. **Rukovanje greskama** – sta ako otkazu klijent, server ili mreza

Primeri RPC tehnologija su : Sun RPC, DCE RPC, MS DCOM, CORBA, Java RMI.

## 2.2 Kako RPC funkcionise?

Cilj RPC-a je da udaljeni poziv izgleda kao običan lokalni poziv. To se postize pomocu stub funkcija:

1. **Klijentski stub** – izgleda kao obična funkcija, ali šalje zahtev serveru i prima rezultat preko mreže
2. **Server stub** – prima zahtev, raspakuje parametre, poziva pravu serversku funkciju i vraća rezultat klijentu

### 2.2.1 10 koraka izvrsenja RPC poziva:

1. Klijent poziva klijentski stub sa parametrima.
2. Klijentski stub pakuje argumente u poruku (marshaling).
3. Lokalni OS šalje poruku udaljenom OS-u preko mreže.
4. Serverski OS prosleđuje poruku serverskom stubu.
5. Serverski stub raspakuje argumente i poziva serversku proceduru.
6. Server izvršava proceduru i vraća rezultat.
7. Serverski stub pakuje rezultat i šalje odgovor.
8. Serverski OS šalje odgovor klijentskom OS-u.
9. Klijentski OS prosleđuje odgovor klijentskom stubu.
10. Klijentski stub raspakuje rezultat i vraća ga klijentu.

Sva mrežna komunikacija je skrivena u stub funkcijama - klijent i server "vide" samo obične pozive procedura. Stub funkcije se generišu za svaku udaljenu proceduru posebno.

## 2.3 Prenos parametara kod RPC

Klijentski stub uzima parametre, pakuje ih u poruku zajedno sa identifikacijom procedure i salje serveru.

Server raspakuje poruku, poziva odgovarajucu proceduru, a rezultat vraca istim postupkom u suprotnom smeru.

Razlikujemo vise nacina prenosa parametra:

1. **Prenos po vrednosti** – u poruku se kopira vrednost parametara, a server radi sa kopijom
2. **Prenos po referenci** – adresa se ne moze direktno poslati jer vazi samo na lokalnoj masini, pa se kod RPC-a najcesce salje sam podatak, server ga menja i vraca klijentu.
3. **Call-by-copy/restore** – parametar se pri pozivu kopira, a nakon izvrsenja se izmenjena vrednost vraca preko originala. Kombinuje prenos po vrednosti i referenci

Nacin prenosa parametara zavisi od jezika i tipa podataka, a naicn prenosa odredjuje programski jezik. Npr. u C-u se skalarni tipovi prenose po vrednosti, a polja preko referenci.

Problem je i sto razlicite masine mogu drugacije predstavljati podatke (big/little-endian, ASCII/EBCDIC). Zato stubovi koriste standardni format i vrse konverziju pri slanju i prijemu.

## 2.4 Povezivanje klijnta i servera (Binding)

Pre poziva udaljene procedure, klijent mora pronaći server i njegovu adresu.

1. **Statičko povezivanje:** adresa servera je ugrađena u klijentski stub. Jednostavno i brzo, ali ako server promeni adresu, klijent mora da se rekompajlira.
2. **Dinamičko povezivanje:** klijentski stub kontaktira server imena/direktorijum, koji na osnovu procedure pronalazi adresu servera. Ako se adresa promeni, menja se samo zapis u direktorijumu, bez izmene klijenta.

## 2.5 Greške i semantika RPC poziva

Kod lokalnog poziva procedura se izvrsi tacno jednom.

Kod udaljenog poziva moguce je da se procedura izvrsi:

1. **0 puta** – server otkaze pre izvrsenja
2. **tacno jednom** – sve je u redu
3. **jednom ili vise puta** – veliko kasnjenje ili gubitak odgovora izaziva retransmisiju

Vecina RPC sistema nudi:

1. **“bar jednom” semantiku** – prihvatljivo za idempotentne funkcije, koje se mogu ponavljati bez posledica (npr. citanje)
2. **“najvise jednom” semantiku** – za funkcije koje nisu idempotentne, kao npr. modifikacija fajla.

Zato se transparentnost ovde delimicno gubi. Aplikacija mora biti spremna da obradi moguce greske.

## 2.6 Programiranje sa RPC i IDL

Većina jezika (C, C++, Java) nema ugrađenu podršku za RPC, pa se koristi IDL (Interface Definition Language). Programiranje sa RPC i IDL moze da se opise kroz sledece korake:

1. IDL definise interfejs (udaljene procedure, njhove parametre i povratne vrednosti)
2. IDL/RPC kompajler generise klijentski i serverski stub
3. Stubovi se povezuju sa programima, tj. klijentski i serverski kod se linkuju sa odgovarajucim stubovima.
