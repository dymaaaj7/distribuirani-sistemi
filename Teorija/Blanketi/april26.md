Pitanje 1 i 2 - Lekcija 1
Pitanje 3 4 5 6 7 8 - lekcija 2
pitanje 9 - lekcija 4

1. Šta predstavlja skalabilnost distribuiranog sistema? Navesti i objasniti tehnike skaliranja distribuiranih sistema.

2. Objasniti pojam distribuirane transakcije i dati primer izvršenja jedna distribuirane transakcije.

3. U call-by-copy/restore semantici, šta se dešava sa parametrima?
   A. Parametri se šalju kao kopija serveru
   B. Promene se automatski reflektuju na klijentu u realnom vremenu
   C. Nakon izvršenja, rezultati se kopiraju nazad klijentu
   D. Parametri se ne vraćaju klijentu

4. Koja je uloga klijent stuba i server stuba u RPC komunikaciji i na koji način oni omogućavaju transparentno pozivanje udaljenih procedura?

5.
a) Izlaz iz IDL kompajlera sastoji se od više fajlova. Koji su to fajlovi i šta sadrže? Napisati na koji način se vrši generisanje ovih fajlova i kako se na osnovu generisanih fajlova formiraju izvršna klijentska i serverska aplikacija (za svaki korak napisati odgovarajuću komandu).
b) Napisati SunRPC definiciju interfejsa koji omogućava pristup jednoj udaljenoj proceduri koja pronalazi minimum i maksimum elemenata niza celih brojeva koji se šalju serveru. Napisati kako izgleda poziv procedure u klijentskoj aplikaciji.

6. Šta se dešava kada server promeni IP adresu u sistemu sa dinamičkim povezivanjem?
   A. Klijent automatski dobija novu adresu bez intervencije posrednika
   B. Server ažurira svoju registraciju kod bindera sa novom adresom
   C. Binder briše prethodnu adresu i prekida sve postojeće veze klijenata
   D. Klijent mora ručno da unese novu adresu servera

7. Navesti i objasniti sve tipove tranzijentnih sinhronih komunikacija. Za svaki tip navesti odgovarajući primer.

8.
a) Koji od sledećih tipova komunikacije message queuing sistemi najčešće podržavaju?
   A. Sinhroni poziv udaljene procedure (RPC)
   B. Stream-based komunikacija
   C. Asinhrona komunikacija
   D. Peer-to-peer socket komunikacija
b) Interfejs koji je na raspolaganju aplikacijama za razmenu poruka kod message-queuing sistema. Objasniti.

9.
a) Događaji A, B, C i D u distribuiranom sistemu imaju sledeće vektorske časovnike
   A[1,0,0],  B[2,0,0],  C[2,1,0],  D[1,2,0]
Koji od gore navedenih vektorskih časovnika nije moguć i zašto?
b) Da li je tačna sledeća tvrdnja koja se odnosi na Lamportove markice. Obrazložiti odgovor.
   Ako je L(A) < L(B), tada je A -> B ili A || B.

10.
a) Da li je sledeće skladište podataka sekvencijalno konzistentno?
   Inicijalno: x=0, y=0
   P1: W(x)1 R(y)0
   P2: W(x)2 R(x)0
Obrazložiti odgovor. Šta treba promeniti da bi se odgovor promenio?
b) Da li je sledeće skladište podataka kauzalno konzistentno? Obrazložiti odgovor.
Inicijalno: x=0, y=0
P1: W(x)1
P2: R(x)1   W(y)1
P3: R(y)1   R(x)0

11. a) Opisati Lamportov algoritam za rešavanje problema vizantijskih generala. Pokazati da se u sistemu sa 3 procesa u kojem jedan proces ispoljava greške vizantijskog tipa ne može postići konsenzus.
