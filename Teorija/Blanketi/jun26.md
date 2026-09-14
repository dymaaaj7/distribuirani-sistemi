Lekcija 1 - pITANJE 1
Lekcija 2 - Pitanja 2 3 4 5
Lekcija 3 - Pitanja 6 7 8
Lekcija 4 - pitanja 9 10 11

1.
a) Šta je funkcija middleware u distribuiranom sistemu?
b) Šta predstavlja skalabilnost distribuiranog sistema?
c) Šta se podrazumeva pod pristupnom transparentnošću:
   A. udaljenim resursima se pristupa korišćenjem lokaciono nezavisnih imena
   B. lokalnim i udaljenim resursima se pristupa korišćenjem istih operacija
   C. repliciranim resursima se pristupa kao da postoji samo jedna kopija
   D. resurs će upravljati svim zahtevima na isti način bez obzira na lokaciju klijenta

2.
a) Objasniti aktivnosti koje obavljaju klijent stub i server stub prilikom poziva udaljene procedure.
b) Napisati Sun RPC interfejs za sistem elektronskog glasanja koji omogućava registrovanim biračima da glasaju za kandidate i pregledaju rang-listu kandidata. Server treba da implementira mehodu vote, kojom klijent prosleđuje jedinstveni broj birača, identifikator kandidata i vreme glasanja, pri čemu server proverava da li je birač već glasao i, ukoliko je glasanje validno, evidentira glas, vraćajući informaciju o uspešnosti operacije. Takođe, potrebno je implementirati mehodu getTopCandidates, koja vraća listu identifikatora kandidata sortiranu opadajuće prema broju osvojenih glasova.

3.
a) Koji tip povezivanja između klijenta i servera je podržan u DCE RPC-u? Opišite postupak povezivanja.
b) Koje semantike udaljenog poziva procedure podržava DCE RPC i na koji način su implementirane?

4.
a) Šta je tačno u slučaju reference udaljenog objekta?
   A. Referenca udaljenog objekta omogućava pristup metodama objekta kao da je lokalni
   B. Referenca udaljenog objekta sadrži direktnu memorijsku adresu udaljenog objekta
   C. Stub koristi referencu da usmeri poziv preko mreže ka pravom objektu
   D. Udaljene reference ne mogu biti serijalizovane ni prosleđene drugim procesima
b) Interfejs koji je na raspolaganju aplikacijama za razmenu poruka kod sistema sa redovima poruka. Objasniti.

5. Koji sve tipovi komunikacija u distribuiranom sistemu su podržani od strane MPI i kojim funkcijama? Obrazložiti.

6. Zašto je u DS teško postići savršeni sinhronizaciju časovnika?

7. Kristijanov algoritam usvaja da je:
a) Kašnjenje kroz mrežu tačno poznato
b) Kašnjenja zahteva i odgovora su približno jednaka
c) Serverski časovnik je uvek sporiji
d) Svi klijenti međusobno komuniciraju da bi odredili tačno vreme.
Odabrati korektan odgovor i obrazložiti ga.

8. Zašto Berkeley algoritam šalje korekciju vremena umesto tačnog vremena?

9. Sistem ima 10 procesa. Koliko je poruka potrebno razmeniti da bi se ušlo u kritičnu sekciju ako se koristi
a) Centralizovani algoritam
b) Ricart–Agrawala algoritam

10.
a) Šta će se dogoditi ako se koristi Ricat–Agrawal algoritam i dva procesa koji žele pristup istoj kritičnoj sekciji, generišu zahtev sa istom vrednošću Lamportove markice?
b) Pet procesa jednovremeno traži zahtev da pristupe istoj kritičnoj sekciji. Lamportove markice zahteva su P1: 9, P2: 6, P3: 6, P4: 12, P5: 10. Prikazati kako izgledaju baferi svakog procesa ako se koristi Ricat-Agrawal algoritam. Koji zahtevi će odma biti potvrđeni?

11. U distribuiranom sistemu sa 5 procesa, P1 P2 P3 P4 P5, za izbor koordinatora koristi se Bully algoritam. P5 je koordinator. Proces P2 detektuje otkaz koordinatora. Prikazati sve poruke koje će se razmeniti do izbora novog koordinatora.
