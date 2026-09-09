# Distribuirani sistemi — skraćena verzija

# 1. UVOD

## 1.1 Definicija distribuiranog sistema

Distribuirani sistem je skup nezavisnih racunara koji korisniku deluju kao jedan jedinstven sistem. Komponente komuniciraju i uskladjuju akcije razmenom poruka.

## 1.2 Prednosti distribuiranih sistema nad centralizovanim

1. **Ekonomske** – vise obicnih racunara je jeftinije od jednog mainframe-a
2. **Brzina** – posao se deli na vise masina (Google, DNS)
3. **Pouzdanost** – kvar jednog racunara ne zaustavlja sistem (radi i sa 15% masina u kvaru)
4. **Inkrementalni rast** – sistem se lako siri dodavanjem novih racunara
5. **Deljenje resursa** – vise korisnika pristupa istoj bazi/fajlovima (Google Drive)
6. **Komunikacija** – mreza omogucava chat, mail itd.
7. **Efikasnost** – opterecenje se rasporedjuje na slobodne masine

## 1.3 Mane distribuiranih sistema

1. **Kompleksnost softvera** – teze pisanje i trazenje gresaka (poruke kasne, gube se)
2. **Umrezavanje** – pad ili preopterecenje mreze obara/usporava sistem
3. **Bezbednost** – podaci putuju mrezom, rizik od presretanja

## 1.4 Osnovne osobine distribuiranog sistema

Heterogenost, transparentnost, otvorenost, skalabilnost.

## 1.5 Heterogenost

Razlike izmedju racunara u sistemu javljaju se na 4 nivoa:

1. **Hardver** – razliciti skupovi instrukcija, razlicito pamcenje podataka
2. **Operativni sistem** – npr. Windows i Linux imaju razlicite mrezne interfejse
3. **Programski jezici** – razlicito predstavljanje teksta i struktura
4. **Razliciti autori softvera** – bez zajednickih pravila nema komunikacije (resenje: OSI model)

## 1.6 Transparentnost

Cilj: korisnik ne zna gde se sta fizicki nalazi u mrezi — slika jedinstvenog sistema.

### 1.6.1 Tipovi transparentnosti:

1. **Pristupna** – lokalnom i udaljenom resursu pristupa se na isti nacin
2. **Lokacijska** – ne znamo gde se resurs nalazi (google.com ne otkriva drzavu servera)
3. **Migraciona** – resurs se premersta, ime i adresa ostaju isti
4. **Konkurencije** – vise korisnika koristi isti resurs uz zakljucavanje i sinhronizaciju
5. **Replikacije** – vise kopija fajla vidimo kao jedan
6. **Za otkaze** – kvar resursa neprimetno preusmerava na drugi
7. **Paralelizacije** – paralelno izvrsavanje je skriveno od nas

## 1.7 Otvorenost

Otvoreni sistem dozvoljava dodavanje novih servisa kojima se pristupa preko razlicitih klijenata (Chrome, Firefox...). Usluge su definisane interfejsima preko IDL-a (definicije funkcija, tipovi parametara, izuzeci). Dobra definicija interfejsa = svaki proces na mrezi moze koristiti uslugu.

## 1.8 Skalabilnost

Sposobnost sistema da raste kroz tri dimenzije:

1. **Broj korisnika i resursa** – rast bez pada performansi
2. **Geografska udaljenost** – radi jednako dobro bez obzira na fizicku udaljenost
3. **Administrativna** – lako upravljanje kroz vise domena/organizacija

Uska grla od 3 vrste centralizacije:

1. **Centralizovane usluge** – jedan server je usko grlo (replikacija nekada nemoguca iz bezbednosti)
2. **Centralizovani podaci** – svi podaci na jednom mestu ruše sistem (DNS)
3. **Centralizovani algoritmi** – umesto njih decentralizovani koji: nemaju kompletnu sliku stanja, odlucuju lokalno, tolerisu otkaz jedne masine, ne pretpostavljaju globalni casovnik

### 1.8.1 Geografska skalabilnost

Sistem projektovan za LAN tesko se siri na WAN: sinhrona komunikacija u WAN-u kasni i do 1000 puta vise — problem za interaktivne aplikacije.

### 1.8.2 Tehnike skaliranja

1. **Skrivanje komunikacionog kasnjenja** – asinhrona komunikacija (klijent ne ceka odgovor) + deo posla na klijentu (validacija forme u pretrazivacu)
2. **Distribucija** – komponenta se deli na delove po vise masina (DNS po domenima)
3. **Replikacija** – kopije resursa blize korisnicima; mora se tolerisati izvesna nekonzistentnost

## 1.9 Middleware

Komunikacija procesa zasniva se na razmeni poruka (nema zajednickog adresnog prostora). Middleware je softverski sloj izmedju aplikacije i mreznog OS-a: sakriva heterogenost platformi i slozenost mreze, pruza zajednicke usluge (autentifikacija, autorizacija, sinhronizacija), tako da aplikacije ne rade direktno sa soketima i TCP/UDP.

Modeli komunikacije: RPC, RMI i message-oriented (npr. MPI).

## 1.10 RPC i objektno orijentisani pristup

**RPC** – resursi su funkcije/procedure; poziv udaljene funkcije izgleda kao lokalni. Problemi: prenos parametara, pronalazenje procedure, semantika gresaka.

**Objektno orijentisani pristup (RMI)** – resursi su objekti sa interfejsom koji skriva detalje; poziv metoda postaje poruka, izvrsava se udaljeno, rezultat se vraca (mreza skrivena kao i kod RPC).

## 1.11 Tipovi distribuiranog sistema

1. po arhitekturi: klijent-server i peer-to-peer
2. po oblasti primene: racunarski, informacioni i sveprisutni sistemi

## 1.12 Distribuirani racunarski sistemi

Za visokoperformansne zadatke:

1. **Klasteri** – slicni racunari, homogenost (isti OS, ista lokalna mreza), paralelno izracunavanje. Primer: Beowulf (Linux klaster sa master cvorom). Middleware: MPI.
2. **Grid** – heterogeni sistemi iz razlicitih domena povezani internetom i zajednickim middleware-om; korisnici u "virtuelnim organizacijama" — potrebna autentifikacija.

## 1.13 Distribuirani informacioni sistemi

Sistemi za obradu transakcija — skup operacija koje se izvrsavaju kao jedna nedeljiva celina (placanje karticom, rezervacija karte).

Komande: `Begin_transaction`, `End_transaction`, `Abort_transaction`, `Read`, `Write`.

ACID osobine:

1. **Atomicity** – obavi se kompletno ili nimalo
2. **Consistency** – ne narusava pravila sistema
3. **Isolation** – konkurentne transakcije se izvrsavaju kao serijalizovane
4. **Durability** – zavrsena transakcija se ne ponistava

## 1.14 Sveprisutni distribuirani sistemi

Za razliku od klastera i grida (fiksni cvorovi), ovi su dinamicki: mali mobilni uredjaji na baterijama sa bezicnim vezama, bez centralne administracije — uredjaji sami otkrivaju okruzenje. Primeri: kucni sistemi, monitoring pacijenata, senzorske mreze.
