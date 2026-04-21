/*

Napisati MPI program koji izračunava sumu N celih brojeva (N je stepen 2)
korišćenjem Point-to-point komunikacije.

U prvom koraku, procesi se grupišu u parove (P0, P1), (P2, P3),…, (Pp-2, Pp-1).

Zatim se izačunavaju parcijalne sume u svim parovima korišćenjem P-to-P
komunikacije i akumuliraju u procesima (P0, P2,…, Pp-2).

Npr. process Pi (i-parno) izračunava parcijalne sume za par procesa (Pi , Pi+1).

U sledećem koraku razmatraju se parovi (P0, P2), (P4, P6),…, (Pp-4, Pp-2)
pronalaze parcijalne sume i akumuliraju u (P0, P4,…, Pp-4).

Postupak se ponavlja dok ne ostanu 2 procesa i rezultat se akumulira u P0.

*/