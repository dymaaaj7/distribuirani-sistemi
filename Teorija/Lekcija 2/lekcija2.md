# 2. KOMUNIKACIJA

**SADRŽAJ**

- [2. KOMUNIKACIJA](#2-komunikacija)
  - [2.1 Poziv udaljene procedure (Remote Procedure Call – RPC)](#21-poziv-udaljene-procedure-remote-procedure-call--rpc)
  - [2.2 Kako RPC funkcioniše?](#22-kako-rpc-funkcioniše)
    - [2.2.1 10 koraka izvršenja RPC poziva](#221-10-koraka-izvršenja-rpc-poziva)
  - [2.3 Prenos parametara kod RPC](#23-prenos-parametara-kod-rpc)
  - [2.4 Povezivanje klijenta i servera (Binding)](#24-povezivanje-klijenta-i-servera-binding)
  - [2.5 Programiranje sa RPC i IDL](#25-programiranje-sa-rpc-i-idl)
  - [2.6 Sun RPC](#26-sun-rpc)
    - [2.6.1 Osnovni koncept i rpcgen](#261-osnovni-koncept-i-rpcgen)
    - [2.6.2 Definicija interfejsa – .x fajl](#262-definicija-interfejsa--x-fajl)
    - [2.6.3 Imenovanje procedura](#263-imenovanje-procedura)
    - [2.6.4 Klijent i server](#264-klijent-i-server)
    - [2.6.5 Pozivanje i pokretanje](#265-pozivanje-i-pokretanje)
    - [2.6.6 Primer](#266-primer)
    - [2.6.7 Prednosti](#267-prednosti)
  - [2.7 Distribuirano računarsko okruženje (DCE)](#27-distribuirano-računarsko-okruženje-dce)
    - [2.7.1 DCE RPC](#271-dce-rpc)
    - [2.7.2 Direktorijumski servis](#272-direktorijumski-servis)
    - [2.7.3 Bezbednosni servis](#273-bezbednosni-servis)
    - [2.7.4 Distribuirani fajl servis](#274-distribuirani-fajl-servis)
    - [2.7.5 Servis distribuiranog vremena](#275-servis-distribuiranog-vremena)
  - [2.8 DCE RPC i IDL](#28-dce-rpc-i-idl)
    - [2.8.1 IDL i UUID](#281-idl-i-uuid)
    - [2.8.2 Semantika RPC poziva](#282-semantika-rpc-poziva)
    - [2.8.3 IDL kompajler](#283-idl-kompajler)
    - [2.8.4 Programiranje DCE RPC aplikacije](#284-programiranje-dce-rpc-aplikacije)
    - [2.8.5 Registrovanje i pronalaženje servera](#285-registrovanje-i-pronalaženje-servera)
  - [2.9 RMI – Poziv udaljenih metoda (OO pristup)](#29-rmi--poziv-udaljenih-metoda-oo-pristup)
    - [2.9.1 Osnovni koncept](#291-osnovni-koncept)
    - [2.9.2 Pozivanje udaljenog metoda](#292-pozivanje-udaljenog-metoda)
    - [2.9.3 Referenca udaljenog objekta](#293-referenca-udaljenog-objekta)
    - [2.9.4 Povezivanje klijenta i servera](#294-povezivanje-klijenta-i-servera)
    - [2.9.5 Prenos argumenata](#295-prenos-argumenata)
    - [2.9.6 Semantika poziva](#296-semantika-poziva)
    - [2.9.7 Primeri RMI middleware sistema](#297-primeri-rmi-middleware-sistema)
  - [2.10 Tipovi komunikacija](#210-tipovi-komunikacija)
    - [2.10.1 Kombinacije tipova komunikacija](#2101-kombinacije-tipova-komunikacija)
  - [2.11 MPI (Message Passing Interface)](#211-mpi-message-passing-interface)
  - [2.12 Sistemi sa redovima poruka (Message Queuing sistemi)](#212-sistemi-sa-redovima-poruka-message-queuing-sistemi)
  - [2.13 Publish-subscribe sistemi za razmenu poruka](#213-publish-subscribe-sistemi-za-razmenu-poruka)

---

## 2.1 Poziv udaljene procedure (Remote Procedure Call – RPC)

Distribuirani sistemi se zasnivaju na slanju poruka, ali to otkriva detalje mreze i narusava transparentnost. RPC (Remote Procedure Call) omogućava programu da pozove proceduru koja se nalazi na drugoj mašini, kao da je lokalna. Podaci se prenose preko mreže, a klijent čeka rezultat. RPC je osnova klijent-server modela i mnogih distribuiranih sistema.

Problemi koje RPC mora da resi jesu:

1. **Prenos parametara** – po vrednosti ili referenci
2. **Format podataka** – uskladjivanje razlicitih formata klijenata i servera
3. **Pronalazenje procedure** – lociranje masine/servera koji izvrsava proceduru
4. **Rukovanje greskama** – sta ako otkazu klijent, server ili mreza

Primeri RPC tehnologija su: Sun RPC, DCE RPC, MS DCOM, CORBA, Java RMI.

## 2.2 Kako RPC funkcioniše?

Cilj RPC-a je da udaljeni poziv izgleda kao običan lokalni poziv. To se postize pomocu stub funkcija:

1. **Klijentski stub** – izgleda kao obična funkcija, ali šalje zahtev serveru i prima rezultat preko mreže
2. **Server stub** – prima zahtev, raspakuje parametre, poziva pravu serversku funkciju i vraća rezultat klijentu

### 2.2.1 10 koraka izvršenja RPC poziva

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

Sva mrežna komunikacija je skrivena u stub funkcijama – klijent i server "vide" samo obične pozive procedura. Stub funkcije se generišu za svaku udaljenu proceduru posebno.

## 2.3 Prenos parametara kod RPC

Klijentski stub uzima parametre, pakuje ih u poruku zajedno sa identifikacijom procedure i salje serveru.
Server raspakuje poruku, poziva odgovarajucu proceduru, a rezultat vraca istim postupkom u suprotnom smeru.

Razlikujemo vise nacina prenosa parametra:

1. **Prenos po vrednosti** – u poruku se kopira vrednost parametara, a server radi sa kopijom
2. **Prenos po referenci** – adresa se ne moze direktno poslati jer vazi samo na lokalnoj masini, pa se kod RPC-a najcesce salje sam podatak, server ga menja i vraca klijentu.
3. **Call-by-copy/restore** – parametar se pri pozivu kopira, a nakon izvrsenja se izmenjena vrednost vraca preko originala. Kombinuje prenos po vrednosti i referenci.

Nacin prenosa parametara zavisi od jezika i tipa podataka, a nacin prenosa odredjuje programski jezik. Npr. u C-u se skalarni tipovi prenose po vrednosti, a polja preko referenci.

Problem je i sto razlicite masine mogu drugacije predstavljati podatke (big/little-endian, ASCII/EBCDIC). Zato stubovi koriste standardni format i vrse konverziju pri slanju i prijemu.

## 2.4 Povezivanje klijenta i servera (Binding)

Pre poziva udaljene procedure, klijent mora pronaći server i njegovu adresu.

1. **Statičko povezivanje** – adresa servera je ugrađena u klijentski stub. Jednostavno i brzo, ali ako server promeni adresu, klijent mora da se rekompajlira.
2. **Dinamičko povezivanje** – klijentski stub kontaktira server imena/direktorijum, koji na osnovu procedure pronalazi adresu servera. Ako se adresa promeni, menja se samo zapis u direktorijumu, bez izmene klijenta.

## 2.5 Programiranje sa RPC i IDL

Većina jezika (C, C++, Java) nema ugrađenu podršku za RPC, pa se koristi IDL (Interface Definition Language). Programiranje sa RPC i IDL moze da se opise kroz sledece korake:

1. IDL definise interfejs (udaljene procedure, njihove parametre i povratne vrednosti)
2. IDL/RPC kompajler generise klijentski i serverski stub
3. Stubovi se povezuju sa programima, tj. klijentski i serverski kod se linkuju sa odgovarajucim stubovima.

## 2.6 Sun RPC

### 2.6.1 Osnovni koncept i rpcgen

Sun RPC koristi XDR (eXternal Data Representation) za definisanje interfejsa.

`rpcgen` je interfejs kompajler koji iz datoteke interfejsa automatski generise tri kljucna fajla:

1. **Header fajl** (`primer.h`) – sadrzi tipove, konstante, identifikatore, prototipove
2. **Klijentski stub** (`primer_clnt.c`) – pakuje i salje parametre, prima rezultat
3. **Serverski stub** (`primer_svc.c`) – prima zahtev i poziva serversku proceduru

### 2.6.2 Definicija interfejsa – .x fajl

Svaka RPC procedura se identifikuje pomocu broja programa, broja verzije i broja procedure.

Primer strukture .x fajla:

```c
struct Brojevi {
    int a;
    int b;
};

program KALKULATOR{
    version KALK_VERSION{
        int KVADRAT(int) = 1;
        int SABERI(Brojevi) = 2;
    } = 1;
} = 0x20000009;
```

Nazivi programa, verzije i procedura se uvek pisu velikim slovima. RPC procedura prima jedan argument, dok se za vise parametara koristi `struct`.

### 2.6.3 Imenovanje procedura

Nakon `rpcgen -C primer.x` imena procedura se koriste sa dodacima:

1. **klijent** – `saberi_1(…)`
2. **server** – `saberi_1_svc(…)`

### 2.6.4 Klijent i server

Klijent:

- Kreira RPC vezu pomocu `CLIENT *cln` i `clnt_create(…)`
- Poziva proceduru, npr. `saberi_1(&brojevi, cln)`
- Argument se prosledjuje kao pokazivac, a rezultat se dobija kao pokazivac.
- Na kraju zatvara vezu pomocu `clnt_destroy(cln)`

Server:

- Implementira serversku verziju procedure, npr. `saberi_1_svc(…)`
- Prima pokazivac na argument i `struct svc_req *`
- Rezultat se najcesce deklarise kao `static`, jer se klijentu vraca pokazivac na rezultat.

### 2.6.5 Pozivanje i pokretanje

Klijent mora znati ime udaljenog servera, koji registruje svoje usluge preko portmapper procesa na serverskoj masini, na portu 111.

Redosled jeste:

1. Generisanje stub funkcija – `rpcgen -C primer.x`
2. Kompajliranje i linkovanje klijenta i klijentskog stuba – `cc -o klijent klijent.c primer_clnt.c`
3. Kompajliranje i linkovanje servera i serverskog stuba – `cc -o server server.c primer_svc.c`
4. Pokretanje servera – `./server`
5. Pokretanje klijenta – `./klijent -h remus 12 15`

### 2.6.6 Primer

```c
Minimum i maksimum niza
struct Niz {
    int elementi<100>;
};

struct MinMax {
    int min;
    int max;
};

program MINMAX_PROG {
    version MINMAX_VERS {
        MinMax pronadji_min_max(Niz) = 1;
    } = 1;
} = 0x29999999;
```

Poziv u klijentu

```c
Niz niz;
MinMax *rezultat;

rezultat = pronadji_min_max_1(&niz, cln);
```

### 2.6.7 Prednosti

Prednosti Sun RPC: aplikacija ne mora da vodi računa o transportnoj adresi, veličini poruke, fragmentaciji ni reasembliranju — sve je to skriveno u middleware sloju, čime se dobija veća portabilnost.

## 2.7 Distribuirano računarsko okruženje (DCE)

DCE je middleware baziran na RPC. To je skup servisa i alata koji se moze instalirati na vrhu postojeceg OS, a sluzi kao platforma za izgradnju distribuiranih aplikacija.

DCE je prvi middleware sistem koji je projektovan kao poseban nivo apstrakcije izmedju mreznog OS-a i distribuirane aplikacije.

Programski model na kome se bazira DCE je klijent-server.

DCE nudi 5 servisa – RPC, direktorijumski, distribuirani fajl servis, bezbednosni i servis distribuiranog vremena.

### 2.7.1 DCE RPC

DCE RPC je osnovni komunikacioni mehanizam. Omogucava pozivanje udaljenih procedura kao lokalnih, pri cemu skriva mreznu komunikaciju i razlike u formatima podataka. Time olaksava razvoj distribuiranih aplikacija na razlicitim sistemima.

### 2.7.2 Direktorijumski servis

Direktorijumski servis (Cell Directory Service – CDS) preslikava logicka imena u informacije o resursu (npr. ime servera -> adresa) i omogucava pronalazenje servera na kome se nalazi udaljena procedura. DCE Celije mogu medjusobno komunicirati radi lociranja resursa van lokacije celije, slicno DNS-u.

### 2.7.3 Bezbednosni servis

Omogucava autentifikaciju procesa i autorizaciju korisnika za pristup resursima.

### 2.7.4 Distribuirani fajl servis

DFS omogucava bezbedan pristup i deljenje udaljenih fajlova kao da su lokalni, uz kesiranje, bezbednost i skalabilnost.

### 2.7.5 Servis distribuiranog vremena

Sinhronizuje časovnike različitih masina radi lakše konzistentnosti u sistemu.

## 2.8 DCE RPC i IDL

### 2.8.1 IDL i UUID

IDL (Interface Definition Language) predstavlja vezu između klijenta i servera i definiše udaljene procedure, njihove parametre, tipove i konstante.

Svaki IDL interfejs ima globalno jedinstveni identifikator (UUID) koji omogućava serveru da prepozna traženi interfejs. UUID je 128-bitni broj i generiše se pomoću `uuidgen`.

Primer:

```md
[uuid(3d6ead56-06e3-11ca-8dd1-826901beabcd),
 version(1.0)]
interface math {
    long get_sum([in] long first, [in] long second);
}
```

### 2.8.2 Semantika RPC poziva

DCE RPC podržava dve semantike:

1. **„Bar jednom"** – koristi se za idempotentne procedure, koje mogu biti izvršene više puta bez neželjenih posledica.
2. **„Samo jednom"** – koristi se za ne-idempotentne procedure, kod kojih ponavljanje poziva može izazvati neželjene efekte.

### 2.8.3 IDL kompajler

Kada je IDL fajl završen, koristi se IDL kompajler: `idl primer.idl`

Kompajler generiše tri fajla:

1. **Header fajl** (`primer.h`) – sadrži UUID, tipove, konstante i prototipove funkcija.
2. **Klijentski stub** (`primer_cstub.c`) – pakuje i šalje parametre, prima rezultat i prosleđuje ga klijentu.
3. **Serverski stub** (`primer_sstub.c`) – prima zahtev i poziva odgovarajuću serversku proceduru.

Nakon toga se pišu klijentski i serverski program, koji se kompajliraju i povezuju sa odgovarajućim stubovima.

### 2.8.4 Programiranje DCE RPC aplikacije

U opštem slučaju, programer piše tri dela:

1. **Klijentski kod**
2. **Serverski inicijalizacioni kod** – registruje server u direktorijumskom servisu.
3. **Serverski operativni kod** – implementira udaljene procedure.

### 2.8.5 Registrovanje i pronalaženje servera

Da bi klijent mogao da pozove server, server mora biti registrovan i spreman da primi poziv.

DCE pronalazi server u dva koraka:

1. Lociranje serverske mašine
2. Lociranje server procesa na toj mašini.

Na svakoj serverskoj mašini postoji DCE daemon (`rpcd`) koji održava vezu između servera i njegovog broja porta (endpoint-a).

Registracija i poziv se odvijaju ovim redom:

1. Server dobija broj porta od OS-a i registruje ga u `rpcd`.
2. Server se registruje u direktorijumskom servisu sa imenom mašine i interfejsima koje implementira.
3. Klijent od direktorijumskog servisa dobija adresu serverske mašine.
4. Klijent od `rpcd` dobija broj porta željenog servera.
5. Klijent poziva udaljenu proceduru.

## 2.9 RMI – Poziv udaljenih metoda (OO pristup)

### 2.9.1 Osnovni koncept

RMI (Remote Method Invocation) je RPC primenjen na objektno-orijentisan način. Omogućava pozivanje metoda objekta koji se nalazi na drugom hostu.

Objekat skriva svoje stanje i implementaciju iza interfejsa, a udaljeni metodi su definisani u remote interfejsu. Interfejs sadrzi samo definicije metoda, dok se njihova implementacija nalazi na serveru.

Objekat fizički postoji samo na jednoj mašini (serveru), dok je njegov interfejs dostupan i na drugim mašinama. Zato se govori o udaljenim/distribuiranim objektima.

### 2.9.2 Pozivanje udaljenog metoda

Poziv se odvija sledecim tokom:

> Klijent → Proxy → Skeleton → Udaljeni objekat

- **Proxy** – predstavlja udaljeni objekat kod klijenta i implementira isti interfejs, analogno klijentskom stubu. Poziv metoda pakuje u poruku koja sadrži referencu objekta, identifikator metoda i parametre i salje ih serveru.
- **Skeleton** – predstavlja serversku stranu, analogno serverskom stubu. Prima i raspakuje poruku i poziva odgovarajući metod na pravom objektu.

### 2.9.3 Referenca udaljenog objekta

Da bi server znao koji objekat treba pozvati, koristi se referenca udaljenog objekta.

Referenca mora biti jedinstvena u prostoru i vremenu i može sadržati:

- IP adresu i port računara,
- vreme kreiranja objekta,
- lokalni broj objekta,
- informacije o interfejsu objekta.

### 2.9.4 Povezivanje klijenta i servera

Klijent do reference udaljenog objekta dolazi preko bindera, koji povezuje: tekstualno ime objekta → referencu udaljenog objekta.

- Server registruje objekat u binderu.
- Klijent preko bindera pronalazi njegovu referencu.

### 2.9.5 Prenos argumenata

Kod RMI-ja način prenosa zavisi od vrste argumenta:

- **Udaljeni (remote) objekti** → prenose se po referenci – šalje se samo njihova referenca, jer objekat ostaje na svojoj mašini.
- **Lokalni/serijalizovani objekti i skalarne vrednosti** → prenose se po vrednosti – kopiraju se u poruku.

### 2.9.6 Semantika poziva

Semantika poziva kod RMI-ja je slična kao kod RPC-a. Najčešće se koristi **„najviše jednom"** (at-most-once) semantika, kako bi se izbeglo neželjeno ponovno izvršavanje metoda koji menjaju stanje objekta.

### 2.9.7 Primeri RMI middleware sistema

1. CORBA
2. Java RMI
3. DCOM

## 2.10 Tipovi komunikacija

Razlikujemo 4 tipa komunikacija:

1. **Perzistentne (istrajne)** – poruka se cuva u komunikacionom serveru sve dok se ne isporuci odredistu. Izvor ne mora biti aktivan nakon sto dostavi poruku u komunikacioni server, a primalac ne mora biti aktivan u trenutku kada izvor posalje poruku.
2. **Tranzijentne** – poruka se cuva samo dok se izvorisna i odredisna aplikacija izvrsavaju. Ako komunikacioni server ne moze da je prosledi sleecem serveru ili odredistu, poruka se odbacuje (npr. zbog isteka timeout-a).
3. **Asinhrone** – pošiljalac nastavlja rad odmah nakon slanja poruke, koja se pamti ili u lokalnom baferu ili u komunikacionom serveru.
4. **Sinhrone** – pošiljalac se blokira dok se poruka ne zapamti u baferu odredišnog hosta.

### 2.10.1 Kombinacije tipova komunikacija

1. **Perzistentne asinhrone** – poruka se čuva u baferu lokalnog hosta ili komunikacionom serveru sve dok se ne isporuči, dok pošiljalac nije blokiran i može odmah da nastavi sa radom.
2. **Perzistentne sinhrone** – pošiljalac je blokiran dok se poruka ne zapamti na odredišnom hostu (nije neophodno da odredišna aplikacija tada bude aktivna).
3. **Tranzijentne asinhrone** – poruka se privremeno čuva u baferu izvornog hosta, nakon čega pošiljalac nastavlja rad, dok se poruka prosleđuje ka odredištu; ako prijemnik tada nije aktivan, poruka se gubi.
4. **Tranzijentne sinhrone** – pošiljalac je blokiran dok se ne ispuni jedan od sledećih uslova:
   - poruka se zapamti u baferu odredišnog hosta;
   - poruka se prosledi odredišnom procesu;
   - odredišni proces pošalje odgovor (RPC i RMI).

## 2.11 MPI (Message Passing Interface)

MPI je middleware standard za razmenu poruka između konkurentnih procesa u paralelnim sistemima (klasteri, mreže radnih stanica, multiračunari, multiprocesori).

Nastao je kao hardverski nezavisan standard radi rešavanja problema prenosivosti programa.

Podržava tranzijentne, point-to-point i grupne komunikacije. Implementiran je kao biblioteka funkcija za Fortran, C i C++.

Komunikacija se odvija unutar grupa procesa, gde par (ID grupe, ID procesa) jedinstveno određuje izvor/odredište poruke. Može postojati više preklapajućih grupa koje se izvršavaju istovremeno.

Podržava skoro sve oblike tranzijentne komunikacije.

## 2.12 Sistemi sa redovima poruka (Message Queuing sistemi)

Ovi sistemi podržavaju perzistentnu asinhronu komunikaciju, pogodnu kad prenos poruke može trajati i nekoliko minuta. Aplikacije komuniciraju preko redova čekanja (queues): pošiljaocu se garantuje samo da će poruka stići u red primaoca, ne i kada (ili da li) će biti pročitana. Pošiljalac i primalac rade potpuno nezavisno jedan od drugog, a poruka ostaje u redu dok je neko ne pokupi.

Osnovne operacije nad redom: `put` (neblokirajuće slanje), `get` (blokirajuće preuzimanje najstarije poruke), `poll` (neblokirajuća varijanta `get`-a) i `notify` (registracija handlera koji se poziva kad stigne nova poruka).

Ovi sistemi podržavaju point-to-point model u više konfiguracija (1-1, 1-više, više-1, više-više pošiljalaca/primalaca), ali svaku poruku uvek preuzima tačno jedan primalac.

## 2.13 Publish-subscribe sistemi za razmenu poruka

Za razliku od message queuing sistema (gde poruku prima samo jedan primalac), kod publish-subscribe modela poruku može primiti više Subscriber-a.

Publisher objavljuje poruku u okviru Topic-a (teme) umesto u red; više Subscriber-a može da se pretplati na Topic i prima objavljene poruke, ali samo one poslate nakon njihove prijave. Implementacija može zahtevati da Subscriber bude aktivan da bi primio poruku.
