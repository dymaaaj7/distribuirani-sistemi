# Zadatak - Januar 2022 (varijanta — Jun 2023 i Oktobar 2 2023 su isti zadatak, ali sa non-durable isporukom)

Koristeći JMS napisati jednostavni mail server.

Svaki korisnik pri startovanju klijenta unese svoj nadimak.

Korisnik se može prijaviti sa istim nadimkom sa više različitih klijenata.

Korisnik pod svojim nadimkom šalje poruku drugom klijentu tako što navede njegov nadimak (podržati više primalaca istovremeno), naziv poruke i sadržaj poruke.

Servis isporučuje poruke svim korisnicima koji su trenutno aktivni.

Ukoliko korisnik nije aktivan, poruka mu se isporucuje kad postane aktivan.

Klijent po prijemu poruke prikazuje poruku sledećim parametrima: ko je poslao, kad je poslata, kome je sve poslata, naslov i sadržaj poruke.

Voditi računa da se poruke dostavljaju samo zainteresovanim stranama, a ne svima.

Navesti na početku sve komunikacione kanale koje koristite i njihovu namenu.
