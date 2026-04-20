/*
Zadatak 3. Napisati MPI program u kome root proces (P0) šalje po 2 poruke (stringove, ne
celobrojne vrednosti) svim ostalim procesima. Procesi prihvataju poruke u obrnutom redosledu
(prvo prihvataju drugu, a zatim prvu poruku), štampaju ih i nakon toga šalju svoj identifikator
root procesu. Root proces po prijemu štampa dobijene identifikatore. Izračunati vreme trajanja
celokupne komunikacije (koristiti funkciju double MPI_Wtime(void) za merenje vremena) i
analizirati rešenje u zavisnosti od veličine poruka i broja procesa. Zadatak realizovati
korišćenjem MPI Point-to-Point operacija.*/