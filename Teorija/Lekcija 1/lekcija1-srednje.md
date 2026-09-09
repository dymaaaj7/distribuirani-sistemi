# Distribuirani sistemi — srednja verzija

# 1. UVOD

## 1.1 Definicija distribuiranog sistema

Distribuirani sistem je skup nezavisnih racunara koji korisniku deluju kao jedan jedinstven sistem. Hardver i softver su rasporedjeni na umrezenim masinama koje medjusobno komuniciraju i uskladjuju akcije razmenom poruka.

## 1.2 Prednosti distribuiranih sistema nad centralizovanim

1. **Ekonomske** – bolji odnos cena i performanse; jeftinije je povezati 10 obicnih racunara u klaster nego kupiti jedan mainframe
2. **Brzina** – posao se deli na vise racunara (Google, DNS — odgovor u milisekundi)
3. **Pouzdanost** – kvar jednog racunara ne zaustavlja sistem; dobro projektovani sistem radi i sa 15% masina u kvaru
4. **Inkrementalni rast (skalabilnost)** – sistem se lako siri dodavanjem novih racunara
5. **Deljenje resursa** – vise korisnika sa razlicitih lokacija pristupa istoj bazi, fajlovima ili web stranici (Google Drive)
6. **Komunikacija** – mreza omogucava komunikaciju izmedju ljudi (chat, mail)
7. **Efikasnost** – opterecenje se pametno rasporedjuje na masine koje nisu zauzete

## 1.3 Mane distribuiranih sistema

1. **Veca kompleksnost softvera** – teze je napisati program za 10 racunara nego za jedan; trazenje gresaka je tesko jer poruke kasne, gube se ili stizu pogresnim redosledom
2. **Umrezavanje** – ako mreza pukne ili se preoptereti, sistem staje ili usporava
3. **Bezbednost** – podaci i lozinke stalno putuju mrezom — rizik od presretanja ako nisu sifrovani

## 1.4 Osnovne osobine distribuiranog sistema

Heterogenost, transparentnost, otvorenost, skalabilnost.

## 1.5 Heterogenost

Racunari u sistemu nisu identicni; razlike se javljaju na 4 nivoa:

1. **Hardver** – razliciti skupovi instrukcija, drugacije pamcenje podataka u memoriji
2. **Operativni sistem** – npr. Windows i Linux imaju potpuno razlicite mrezne interfejse
3. **Programski jezici** – Java, Python i C++ razlicito predstavljaju tekst i strukture u memoriji
4. **Razliciti autori softvera** – bez zajednickih pravila dva programa ne mogu komunicirati (jedno od resenja: OSI model)

## 1.6 Transparentnost

Glavni cilj: pruziti sliku jedinstvenog sistema, da korisnici ne znaju gde se sta fizicki nalazi u mrezi.

### 1.6.1 Tipovi transparentnosti

1. **Pristupna** – lokalnim i udaljenim resursima pristupamo na potpuno isti nacin
2. **Lokacijska** – ne znamo gde se resurs fizicki nalazi (google.com ne otkriva u kojoj drzavi je server)
3. **Migraciona** – fajl ili baza se premeste na drugi server, a mi to ne primetimo jer ime i adresa ostaju isti
4. **Transparentnost konkurencije** – vise ljudi koristi isti resurs istovremeno, uz pomoc zakljucavanja i sinhronizacije (semafori)
5. **Transparentnost replikacije** – vise kopija istog fajla (radi sigurnosti i brzine) vidimo kao jedan
6. **Transparentnost za otkaze** – kvar resursa ne primecujemo, neprimetno smo prebaceni na drugi
7. **Transparentnost paralelizacije** – paralelno izvrsavanje na vise racunara je skriveno od nas

## 1.7 Otvorenost

Otvoreni distribuirani sistem dozvoljava dodavanje novih servisa kojima se pristupa preko razlicitih klijenata (web servisu pristupamo Chrome-om, Brave-om, Firefox-om...). Usluge su definisane po standardnim pravilima (sintaksa i semantika), preko interfejsa opisanih pomocu IDL-a (Interface Definition Language) — definicije funkcija, tipovi parametara i rezultata, moguci izuzeci. Ako je interfejs dobro definisan, bilo koji proces na mrezi moze bez problema da koristi tu uslugu.

## 1.8 Skalabilnost

Sposobnost sistema da se prosiri kroz tri dimenzije:

1. **Broj korisnika i resursa** – dodavanje bez pada performansi
2. **Geografska udaljenost** – sistem radi jednako dobro iako su korisnici i serveri fizicki udaljeni
3. **Administrativna** – lako upravljanje sistemom kroz vise domena ili organizacija

Uska grla dolaze od 3 vrste centralizacije:

1. **Centralizovane usluge** – rad na jednom serveru je usko grlo; replikacija nekada nije opcija zbog bezbednosti (server sa bankovnim racunima)
2. **Centralizovani podaci** – svi podaci na jednom mestu ruše sistem (da DNS nije distribuiran, internet bi pao)
3. **Centralizovani algoritmi** – umesto njih decentralizovani koji: nemaju kompletnu sliku stanja, odlucuju na osnovu lokalnih podataka, podnose otkaz jedne masine i ne pretpostavljaju globalni casovnik (tacna sinhronizacija u DS nije moguca — veci sistem, vece neslaganje)

### 1.8.1 Geografska skalabilnost

Sistem projektovan za LAN tesko se siri na WAN: sinhrona komunikacija, gde klijent ceka odgovor, dobro radi na LAN-u, ali u WAN-u kasnjenje raste i do 1000 puta — problem za interaktivne aplikacije.

### 1.8.2 Tehnike skaliranja

1. **Skrivanje komunikacionog kasnjenja** – klijent ne gubi vreme cekajuci odgovor: asinhrona komunikacija (posalje zahtev, nastavi rad, sistem ga prekida kad stigne odgovor) + deo posla na klijentu (validacija forme — provera e-mail adrese u pretrazivacu pre slanja serveru)
2. **Distribucija** – jedna velika komponenta ili baza se razbija na manje delove po vise masina (DNS deluje kao jedan sistem, a fizicki je na hiljadama servera)
3. **Replikacija** – kopije resursa blize korisnicima radi brzine i rasterecenja; pošto trenutna sinhronizacija kopija nije moguca, sistem mora tolerisati odredjeni nivo nekonzistentnosti

## 1.9 Middleware

Komunikacija izmedju procesa je osnova distribuiranog sistema i zasniva se na razmeni poruka (nema zajednickog adresnog prostora). Middleware je dodatni softverski sloj izmedju aplikacije i mreznog operativnog sistema, ciji je cilj da sakrije heterogenost platformi i slozenost mrežne komunikacije. Dok lokalni OS upravlja jednim racunarom, middleware aplikacijama pruza zajednicke usluge — autentifikaciju, autorizaciju, sinhronizaciju — tako da aplikacije komuniciraju bez direktnog rada sa soketima i protokolima nizeg nivoa (TCP, UDP).

Nema jedinstvenog standarda; najcesci modeli komunikacije su RPC, RMI i message-oriented pristup (npr. MPI).

## 1.10 RPC i objektno orijentisani pristup

**RPC (Remote Procedure Call)** – resursi su funkcije/procedure; poziv udaljene funkcije u kodu izgleda potpuno kao lokalni poziv: klijent prosledi parametre, funkcija se izvrsi na udaljenom serveru, rezultat se transparentno vrati. Problemi: prenos parametara, pronalazenje procedure, semantika gresaka.

**Objektno orijentisani pristup (RMI)** – resursi su objekti sa interfejsom koji skriva unutrašnje detalje; objekat je na jednoj mašini, interfejs dostupan na drugima. Poziv metoda se transformise u poruku, izvrsava udaljeno, rezultat se vraca pozivaocu — mreza je skrivena kao i kod RPC.

## 1.11 Tipovi distribuiranog sistema

1. po arhitekturi: klijent-server i peer-to-peer
2. po oblasti primene: distribuirane racunarske sisteme (klasteri, grid), distribuirane informacione sisteme (transakcioni, poslovna integracija) i sveprisutne (ugradjene) sisteme (mobilni i embedded uredjaji)

## 1.12 Distribuirani racunarski sistemi

Koriste se za izvrsenje visokoperformansnih zadataka:

1. **Klasteri** – grupa slicnih racunara na malom rastojanju; kljucna je homogenost (isti OS, ista brza lokalna mreza); paralelno izracunavanje. Primer: Beowulf (Linux klaster sa master cvorom koji alocira zadatke). Najcesci middleware: MPI (biblioteka funkcija za paralelne programe).
2. **Grid** – grupa razlicitih sistema sa heterogenim hardverom i OS, iz razlicitih geografskih i administrativnih domena, povezanih internetom zajednickim middleware-om. Korisnici su u "virtuelnim organizacijama" sa pravom pristupa resursima, pa je potrebna autentifikacija korisnika ili aplikacije.

## 1.13 Distribuirani informacioni sistemi

Sistemi za obradu transakcija, gde je transakcija skup operacija koje se izvrsavaju kao jedna nedeljiva operacija (placanje karticom, rezervacija avio karte).

Komande: `Begin_transaction`, `End_transaction`, `Abort_transaction`, `Read`, `Write`.

Svaka transakcija mora zadovoljiti ACID osobine:

1. **Atomicity** – obavi se kompletno ili se uopste ne obavi
2. **Consistency** – ne narusava pravila/ogranicenja sistema
3. **Isolation** – konkurentne transakcije se izvrsavaju kao serijalizovane
4. **Durability** – zavrsena transakcija se ne ponistava

## 1.14 Sveprisutni distribuirani sistemi

Za razliku od klastera i grida, koji su stabilni i imaju fiksne cvorove, ovi su dinamicki: cine ih mali, mobilni uredjaji na baterijama sa bezicnim vezama. Nemaju centralno administrativno upravljanje, pa uredjaji sami otkrivaju okruzenje i prilagodjavaju se promenama. Primeri: kucni sistemi (pametno svetlo, alarmi), monitoring pacijenata, senzorske mreže koje salju podatke do bazne stanice.
