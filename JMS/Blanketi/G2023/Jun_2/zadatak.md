# Zadatak - Jun 2 2023 (isti kao Oktobar 2022)

Pomoću JMS-a kreirati sistem koji omogućava rad merge sorta.

Svaki klijent poseduje svoj identifikator ID (smatrati da će sistem sam da dodeli ID i da je on jedinstven u celom sistemu).

Klijent unosi niz koji je potrebno sortirati i šalje ostalim klijentima niz na sortiranje.

Bilo koji klijent može da odradi deo sortiranja.

Sortiranje treba izvesti tako da klijent koji započinje sortiranje deli niz u dva približno jednaka dela (u slučaju da je neparan broj elemenata jedan deo treba da bude za 1 veći od drugog dela) i svaki deo šalje dalje na sortiranje.

Ukoliko niz sadrži jedan element smatrati da je sortiran.

Sortirani podniz je potrebno proslediti klijentu koji je poslao zahtev za sortiranje.

Kada oba dela niza stignu potrebno ih je sklopiti u jedan niz i proslediti nazad klijentu koji je tražio njihovo sortiranje, sve dok ceo niz ne bude sortiran.
