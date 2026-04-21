/*

Napisati MPI program koji pronalazi proizvod matrice Amxn i vektora bn.

Matrica A i vector b se inicijalizuju u procesu 0.

Izračunavanje se obavlja tako što se svakom procesu distribuira po kolona
matrice A i po 1 element vektora b.

Za distribuciju kolona po procesima koristiti P-t-P operacije, za sve ostalo
grupne operacije.

Svi procesi učestvuju u izračunavanju.

Rezultat se prikazuje u procesu koji, nakon distribuiranja kolona matrice A,
sadrži minimum svih elemenata matrice A.

*/