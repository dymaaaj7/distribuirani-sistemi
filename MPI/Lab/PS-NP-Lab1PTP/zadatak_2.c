/*

Zadatak 2.

Napisati MPI program u kome root proces (P0) šalje identičnu poruku msg svim
procesima, a zatim izmenjenu poruku prihvata iz svakog procesa.

Poruka se menja tako što se kompletno okrene redosled svih karaktera(npr. poruka
"lab" vežba” bi nakon promena bila “abžev bal”).

Komunikaciju realizovati korišćenjem blokirajućih funkcija, a zatim ponoviti
komunikaciju korišćenjem neblokirajućih funkcija za slanje i prijem.

Izračunati vreme trajanja celokupne komunikacije u oba slučaja (koristiti
funkciju double MPI_Wtime(void) za merenje vremena) i analizirati rešenje u
zavisnosti od veličine poruke msg.

Zadatak realizovati korišćenjem MPI Point-to-Point operacija.

*/