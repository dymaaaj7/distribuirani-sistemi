# Zadatak - Decembar 2022 (isti kao Oktobar 2 2022 i Januar 2023)

Pomoću JMS-a kreirati sistem za pomoć u proizvodnji.

Svaki klijent predstavlja jednu radnu stanicu.

Može imati više radnih stanica koje obavljaju isti posao.

Radna stanica je opisana svojim identifikatorom i poslom koji obavlja.

Proizvod je opisan listom poslova koji trebaju da se obave i količinom tog posla.

Poslovi se ne mogu obaviti van redosleda.

Svaki posao se može u nekom trenutku obaviti najviše jednom.

Svaka stanica može u jednom trenutku da odradi najviše jedan proizvod u jednom momentu.

Kompletna lista poslova se šalje svakoj stanici.

Svaka stanica po obavljenom poslu obeležava u listi da je završila posao i prosleđuje listu tako da je sledeća stanica dobije.

Svaka stanica treba da prikaže listu poslova i naznači koji posao obavlja.

Poslednja stanica štampa dodatnu poruku da je proizvod gotov.
