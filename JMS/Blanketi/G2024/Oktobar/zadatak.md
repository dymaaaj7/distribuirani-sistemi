# Zadatak - Oktobar 2024 (isti kao Januar 2025, Septembar 2025 i Septembar 2024)

Koristeći JMS kreirati sistem za **ispitivanje i ocenjivanje**.

Sistem treba da sadrži funkciju **Pokreni(bool nastavnik, string sifraPredmeta)** koja za nastavnika (true) omogućava da dobije odgovore od strane studenata, odnosno za studenta (false) da dobije pitanja koja je poslao nastavnik.

Može biti više studenata i nastavnika po šifri predmeta. Pri ocenjivanju voditi računa da **samo jedan nastavnik vrši ocenjivanje**.

Zatim sistem sadrži funkciju **PosaljiPitanje(string sifraPredmeta, string sifraPitanja, string Pitanje)** kojom nastavnik šalje pitanje studentima, gde je Pitanje tekstualnog oblika. Sistem studentu prikazuje pitanje na ekran po prijemu.

Takođe, sistem sadrži funkciju za slanje odgovora. Odgovor se prikazuje nastavniku koji ocenjuje.

Nije potrebno implementirati pozive ovih funkcija već samo funkcije. Ostatak sistema smatrati realizovanim.

Voditi računa da samo zainteresovane strane dobiju poruke.

Obavezno na početku skicirati tok razmene poruka.
