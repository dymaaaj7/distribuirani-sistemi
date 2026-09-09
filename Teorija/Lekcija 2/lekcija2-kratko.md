# Distribuirani sistemi — skraćena verzija

# 2. KOMUNIKACIJA

## 2.1 Poziv udaljene procedure (Remote Procedure Call – RPC)

Slanje poruka otkriva detalje mreze i narusava transparentnost. RPC omogucava poziv procedure na drugoj masini kao da je lokalna — podaci idu preko mreze, klijent ceka rezultat. Osnova klijent-server modela.

Problemi: prenos parametara, format podataka, pronalazenje procedure, rukovanje greskama.

Tehnologije: Sun RPC, DCE RPC, MS DCOM, CORBA, Java RMI.

## 2.2 Kako RPC funkcionise?

Cilj: udaljeni poziv izgleda kao lokalni — pomocu stub funkcija:

1. **Klijentski stub** – salje zahtev serveru i prima rezultat preko mreze
2. **Server stub** – prima zahtev, raspakuje parametre, poziva pravu funkciju, vraca rezultat

Sva mreza komunikacija skrivena je u stubovima (generisu se za svaku udaljenu proceduru posebno).

### 2.2.1 10 koraka izvrsenja RPC poziva:

Klijent → klijentski stub (marshaling) → lokalni OS → mreza → serverski OS → serverski stub (raspakuje) → server. Rezultat se vraca istim putem u suprotnom smeru.

## 2.3 Prenos parametara kod RPC

Stub pakuje parametre + identifikaciju procedure u poruku; rezultat se vraca istim putem.

1. **Po vrednosti** – salje se kopija vrednosti, server radi sa kopijom
2. **Po referenci** – adresa vazi samo lokalno, pa se salje sam podatak, server ga menja i vraca
3. **Call-by-copy/restore** – kopira se pri pozivu, izmenjeno se vraca preko originala (kombinacija prva dva)

Nacin odredjuje jezik (C: skalari po vrednosti, polja preko referenci). Razliciti formati masina (big/little-endian, ASCII/EBCDIC) → stubovi vrse konverziju u standardni format.

## 2.4 Povezivanje klijenta i servera (Binding)

1. **Staticko povezivanje** – adresa servera upisana u klijentski stub; promena adrese = rekompajliranje klijenta
2. **Dinamicko povezivanje** – stub pita server imena/direktorijum; promena adrese menja samo zapis u direktorijumu

## 2.5 Greške i semantika RPC poziva

Lokalni poziv: izvrsi se tacno jednom. Udaljeni: 0 puta (server otkaze), 1 put (sve u redu) ili vise puta (kasnjenje/gubitak odgovora → retransmisija).

1. **“bar jednom” semantika** – idempotentne funkcije (citanje)
2. **“najvise jednom” semantika** – ne-idempotentne (modifikacija fajla)

Transparentnost se delimicno gubi — aplikacija mora obraditi greske.

## 2.6 Programiranje sa RPC i IDL

C/C++/Java nemaju ugradjen RPC → koristi se IDL (Interface Definition Language):

1. IDL definise interfejs (procedure, parametri, povratne vrednosti)
2. Kompajler generise klijentski i serverski stub
3. Stubovi se linkuju sa klijentskim i serverskim kodom
