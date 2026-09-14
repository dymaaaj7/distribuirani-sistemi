# Zadatak - Oktobar 2025

Kreirati sistem koji omogućava igranje Minesweeper igre.

Svako polje u igri predstavlja jednu instancu klijenta u JMS sa svojim x, y koordinatama i flegom da li je bomba ili ne.

Igrač šalje koordinate polja, a polje vraća da li je bomba ili broj okolnih bombi.

Ukoliko nije bomba, polje ispituje okolna polja i vraća broj bombi igraču.

Voditi računa da se poruke dostavljaju samo zainteresovanim stranama, a ne svima.

Navesti na početku sve komunikacione kanale koje koristite i njihovu namenu.
