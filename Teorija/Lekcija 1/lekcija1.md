# SADRZAJ

1. [UVOD](#1-uvod)
   1.1. [Definicija distribuiranog sistema](#11-definicija-distribuiranog-sistema)
   1.2. [Prednosti distribuiranih sistema nad centralizovanim](#12-prednosti-distribuiranih-sistema-nad-centralizovanim)
   1.3. [Mane distribuiranih sistema](#13-mane-distribuiranih-sistema)
   1.4. [Osnovne osobine distribuiranih sistema](#14-osnovne-osobine-distribuiranog-sistema)
   1.5. [Heterogenost](#15-heterogenost)
   1.6. [Transparentnost](#16-transparentnost)
   1.7. [Otvorenost](#17-otvorenost)
   1.8. [Skalabilnost](#18-skalabilnost)
   1.9. [Middleware](#19-middleware)
   1.10. [RPC i objektno orijentisani pristup](#110-rpc-i-objektno-orijentisani-pristup)
   1.11. [Tipovi distribuiranog sistema](#111-tipovi-distribuiranog-sistema)
   1.12. [Distribuirani racunarski sistemi](#112-distribuirani-racunarski-sistemi)
   1.13. [Distribuirani informacioni sistemi](#113-distribuirani-informacioni-sistemi)
   1.14. [Sveprisutni distribuirani sistemi](#114-sveprisutni-distribuirani-sistemi)

---

# 1. UVOD

## 1.1 Definicija distribuiranog sistema

Distribuirani sistem je skup nezavisnih racunara koji korisniku deluju kao jedan jedinstven sistem. Hardver i softver su rasporedjeni na umrezenim masinama koje medujsobno komuniciraju i uskladjuju akcije razmenom poruka.

## 1.2 Prednosti distribuiranih sistema nad centralizovanim

1. **Ekonomske** – bolji odnos cena i performanse, jer je jeftinije povezati 10 obicnih racunara u klaster, nego kupiti jedan preskupi "mainframe" racunar (bolje vise sistema nego jedan veliki)
2. **Brzina** – posao se deli na vise racunara, kao na primer kod Google-a i DNS-a gde se pretraga deli na vise hiljada masina i zato odgovor stize u milisekundi.
3. **Pouzdanost** – ako se jedan racunar pokvari, sistem nastavlja da radi. Dobro projektovani distribuirani sistemi mogu neometano raditi cak i ako je npr 15% masina u kvaru.
4. **Inkrementalni rast (skalabilnost)** – sistem se lako siri dodavanjem novih racunara
5. **Deljenje resursa** – vise korisnika sa razlicitih lokacija moze da pristupi istoj bazi, fajlovima ili web stranici (npr. Google Drive, web sajtovi)
6. **Komunikacija** – mreza omogucava komunikaciju i razmenu informacija izmedju ljudi (chat, mail)
7. **Efikasnost** – opterecenje (traffic) se pametno rasporedjuje na masine koje trenutno nisu zauzete.

## 1.3 Mane distribuiranih sistema

1. **Veca kompleksnost softvera** – mnogo je teze napisati program koji funkcionise na 10 racunara istovremeno nego na jednom. Trazenje gresaka je tesko jer poruke kasne, gube se ili stizu pogresnim redosledom.
2. **Umrezavanje** – ako mreza pukne ili se preoptereti, ceo distribuirani sistem staje ili usporava
3. **Bezbednost** – posto osetljivi podaci i lozinke stalno putuju putem mreze, postoji rizik od presretanja ako nisu sifrovani.

## 1.4 Osnovne osobine distribuiranog sistema

1. Heterogenost
2. Transparentnost
3. Otvorenost
4. Skalabilnost

## 1.5 Heterogenost

Distribuirani sistem cine racunari koji nisu identicni, vec se njihove razlike javljaju u 4 nivoa:

1. **Hardver** – procesori mogu koristiti razlicite skupove instrukcija i drugacije pamte podatke u memoriji
2. **Operativni sistem** – na primer, Windows i Linux imaju potpuno razlicite mrezne interfejse
3. **Programski jezici** – Java, Python i C++ na razlicite nacine predstavljaju tekst i strukture u memoriji
4. **Razliciti autori softvera** – ako dva tima prave programe bez zajednickih pravila, oni ne mogu medjusobno komunicirati, pa se za jedno od resenja tog problema koristi OSI model

## 1.6 Transparentnost

Glavni cilj transparentnosti jeste pruziti sliku jedinstvenog sistema, da korisnici ne znaju gde se sta fizicki nalazi u mrezi.

### 1.6.1 Tipovi transparentnosti:

1. **Pristupna** – lokalnim i udaljenim resursima pristupamo na potpuno isti nacin
2. **Lokacijska** – ne znamo gde se resurs fizicki nalazi (npr. google.com ne otkriva u kojoj drzavi je server)
3. **Migraciona** – fajl ili baza se premeste na drugi server, a mi to ne primetimo jer im ime i adresa ostaju isti
4. **Transparentnost konkurencije** – vise ljudi koristi isti resurs istovremeno bez pravljenja gresaka uz pomoc zakljucavanja i sinhronizacije (npr. semafori)
5. **Transparentnost replikacije** – postoji vise kopija istog fajla radi sigurnosti i brzine, ali vidimo ga kao jedan jedinstven fajl
6. **Transparentnost za otkaze** – ako je neki resurs ili deo sistema u kvaru, mi to ne primecujemo, vec neprimetno budemo prebaceni na drugi
7. **Transparentnost paralelizacije** – paralelno izvrsavanje na vise racunara je skriveno od nas

## 1.7 Otvorenost

Otvoreni distribuirani sistem dozvoljava dodavanje novih servisa, kojima se pristupa preko razlicitih klijenata (npr. web servisu pristupamo pomocu Chrome-a, Brave-a, Firefoxa-a itd.).

Otvoreni distribuirani sistem nudi usluge po standardnim pravilima koje definisu sintaksu i semantiku usluga (npr. format, sadrzaj i znacenje poruka koje se razmenjuju).

Usluge su definisane preko interfejsa i opisuju se pomocu IDL-a (Interface Definition Language), koji sadrzi definicije funkcija, tipove parametara i rezultata i moguce izuzetke.

Sustina je da ako je interfejs dobro definisan, bilo koji proces (softver) na mrezi moze bez problema da pristupi i koristi tu uslugu.

## 1.8 Skalabilnost

Skalabilnost je sposobnost sistema da se prosiri i raste kroz tri dimenzije:

1. **Broj korisnika i resursa** – mogucnost dodavanja korisnika i resursa bez pada performansi
2. **Geografska udaljenost** – mogucnost sistema da radi jednako dobro iako su korisnici i serveri fizicki udaljeni
3. **Administrativna** – lako upravljanje sistemom koji se prostire kroz vise razlicitih domena ili organizacija.

Medjutim, ako sistem treba da podrzi vise korisnika ili resursa, do problema i uskih grla dolazi zbog 3 vrste centralizacije:

1. **Centralizovane usluge** – rad na samo jednom serveru stvara usko grlo i replikacija nekada nije opcija zbog bezbednosti (npr. server sa bankovnim racunima gradjana)
2. **Centralizovani podaci** – svi podaci na jednom mestu ruse sistem (npr. da DNS nije distribuirana baza, internet bi pao zbog preopterecenja
3. **Centralizovani algoritmi** – Nisu pozeljni vec se moraju koristiti decentralizovani koji:
   - nemaju kompletnu sliku stanja sistema
   - odlučuju na osnovu lokalnih podataka
   - otkaz jedne mašine ne ruši sistem
   - ne pretpostavljaju postojanje globalnog časovnika (tačna sinhronizacija časovnika u DS nije moguća - veći sistem, veće neslaganje)

### 1.8.1 Geografska skalabilnost

Distribuirani sistem projektovan za LAN se tesko siri na WAN jer sinhrona komunikacija, kod koje klijent ceka odgovor, dobro radi na LAN-u, ali u WAN-u kasnjenje raste i do 1000 puta, sto je problem za interaktivne aplikacije.

### 1.8.2 Tehnike skaliranja

Kada sistem postane prevelik, primenjujemo tri tehnike:

1. **Skrivanje komunikacionog kasnjenja** – Cilj je da klijent ne gubi vreme cekajuci odgovor sa mreze. Postize se asinhronom komunikacijom, gde klijent posalje zahtev i odmah nastavlja sa drugim radom, a sistem mu prekidom javi kad stize odgovor. Takodje, deo posla se prebacuje na klijenta (npr. validacija forme – provera e-mail adrese u pretrazivacu pre slanja serveru).
2. **Distribucija** – Cilj je da se razbije jedna velika komponenta ili baza na manje delove i rasporede se na vise masina. Na primer DNS koji je podeljen po domenima, deluje nam kao jedan sistem, a fizicki je podeljen na hiljade servera.
3. **Replikacija** – Pravljenje kopija resursa blize korisnicima radi brzeg pristupa i rasterecenja servera. Posto trenutna sinhronizacija svih kopija nije moguca, distribuirani sistem mora tolerisati odredjeni nivo nekonzistentnosti.

## 1.9 Middleware

Komunikacija između procesa je osnova svakog distribuiranog sistema i zasniva se na razmeni poruka, jer ne postoji zajednički adresni prostor. Da bi programeri lakše razvijali takve sisteme, potrebna je dodatna softverska podrška koja pojednostavljuje mrežnu komunikaciju, sinhronizaciju i bezbednost.

Middleware je dodatni softverski sloj između aplikacije i mrežnog operativnog sistema, čiji je cilj da sakrije heterogenost platformi (razlike u hardveru i OS-u) i složenost mrežne komunikacije.

Dok lokalni operativni sistem upravlja pojedinačnim računarom, middleware aplikacijama pruža zajedničke usluge poput autentifikacije, autorizacije i sinhronizacije. Na taj način, middleware omogućava aplikacijama da komuniciraju bez direktnog rada sa mrežnim soketima i protokolima nižeg nivoa (TCP i UDP).

Ne postoji jedinstveni standard za middleware, a najčešći modeli komunikacije su :

1. RPC,
2. RMI i
3. message-oriented pristup, kao što je MPI.

## 1.10 RPC i objektno orijentisani pristup

RPC (Remote Procedure Call) je middleware model gde su resursi funkcije/procedure. Cilj je da poziv udaljene funkcije u kodu izgleda potpuno kao lokalni poziv, gde klijent prosledi parametre, funkcija se izvrsi na udaljenom serveru, a rezutlat se transparentno vrati. Medjutim problemi su:

- prenos parametara,
- pronalaženje procedure,
- semantika grešaka.

Objektno orijentisani pristup (npr. RMI) je model gde su resursi objekti sa interfejsom koji skriva unutrašnje detalje. Objekat je lociran na jednoj mašini, interfejs dostupan na drugima. Poziv metoda nad tim objektom se transformise u poruku, izvršava se udaljeno, a rezultat se vraća pozivaocu (kao i kod RPC, mreža je skrivena).

## 1.11 Tipovi distribuiranog sistema

Distribuirane sisteme smo podelili

1. po arhitekturi na
   - klijent-server i
   - peer-to peer,
2. po oblasti primene na
   - distribuirane racunarske sisteme (klasteri, grid),
   - distribuirane informacione sisteme (transakcioni, poslovna integracija) i
   - sveprisutne (ugradjene) distribuirane sisteme kao sto su mobilni i embedded uredjaji.

## 1.12 Distribuirani racunarski sistemi

Distribuirani racunarski sistemi se koriste za izvrsenje visokoperformansnih zadataka. Mogu biti:

1. **Klasteri** – grupa slicnih racunara na malom rastojanju. Kod njih je kljucna homogenost. Svi cvorovi imaju isti OS i povezani su istom brzom lokalnom mrezom. Koriste se za paralelno izracunavanje.

   Primer je Beowulf (Linux klaster sa master cvorom koji alocira zadatke)

   Najcesci tip middleware-a kod klastera je MPI i sadrzi biblioteku funkcija za izvrsenje paralelnih programa.

2. **Grid** – grupa razlicitih racunarskih sistema sa heterogenim hardverom i OS, iz razlicitih geografskih i administrativnih domena, povezanih preko interneta zajednickim middleware-om. Korisnici su organizovani u "virtuelne organizacije" sa pravom pristupa resursima, pa je neophodno obezbediti mehanizme za autentifikaciju korisnika ili aplikacije.

## 1.13 Distribuirani informacioni sistemi

Predstavljaju sisteme za obradu transakcija, gde transakcija predstavlja skup operacija koje se izvrsavaju kao jedna nedeljiva operacija (npr. placanje karticom, rezervacija avio karte).

Komande za rad sa transakcijama jesu:

1. `Begin_transaction`
2. `End_transaction`
3. `Abort_transaction`
4. `Read`
5. `Write`

Svaka transakcija mora da zadovolji ACID osobine (test), a to su

1. **Atomicity** – trasankcija se obavi kompletno ili se uopste ne obavi
2. **Consistency** – ne narusava pravila/ogranicenja sistema
3. **Isolation** – konkurentne transakcije se izvrsavaju kao serijalizovane
4. **Durability** – zavrsena transakcija se ne ponistava

## 1.14 Sveprisutni distribuirani sistemi

Za razliku od klastera i grida, koji su stabilni i imaju fiksne čvorove, sveprisutni distribuirani sistemi su dinamični jer ih čine mali, mobilni uređaji na baterijama sa bežičnim vezama.

Nemaju centralno administrativno upravljanje, pa uređaji moraju sami da otkrivaju okruženje i prilagođavaju se promenama.

Primeri su kućni sistemi (pametno svetlo, alarmi), monitoring pacijenata i senzorske mreže koje šalju podatke do bazne stanice.
