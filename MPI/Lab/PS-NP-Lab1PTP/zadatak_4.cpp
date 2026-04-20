/*
Zadatak 4. Napisati MPI program koji u root procesu (P0) deli ulazni niz dužine N, tako što
svakom procesu šalje po N/P elemenata odjednom, gde je P broj procesa (N je deljivo sa P).
Po dobijanju podnizova, procesi treba da pronađu minimalnu vrednost i vrate je root procesu.
On, nakom prijema svih minimalnih vrednosti, određuje najmanji element od svih prihvaćenih
i štampa ga. Koristiti nizove sa realnim brojevima. Zadatak realizovati korišćenjem MPI
Point-to-Point operacija.*/