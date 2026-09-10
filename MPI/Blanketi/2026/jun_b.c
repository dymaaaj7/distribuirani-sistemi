/*

Procesi u MPI programu imaju podatke koji izgledaju ovako:

P0 [    b1[ 0 1 ]    b2[ ? ? ? ? ? ? ? ? ]   ]
P1 [    b1[ 2 4 ]    b2[ ? ? ? ? ? ? ? ? ]   ]
P2 [    b1[ 9 3 ]    b2[ ? ? ? ? ? ? ? ? ]   ]
P3 [    b1[ 5 7 ]    b2[ ? ? ? ? ? ? ? ? ]   ]

Navesti minimalni broj poziva MPI funkcija za komunikaciju (potrebno je za svaku
navesti ceo poziv funkcije i argumente) koje dovode do toga da se podaci razmene
kao sto je prikazano na sledecoj slici:

P0 [    b1[ 0 1 ]    b2[ 0 1 2 4 9 3 5 7 ]   ]
P1 [    b1[ 2 4 ]    b2[ 0 1 2 4 9 3 5 7 ]   ]
P2 [    b1[ 9 3 ]    b2[ 0 1 2 4 9 3 5 7 ]   ]
P3 [    b1[ 5 7 ]    b2[ 0 1 2 4 9 3 5 7 ]   ]

Resenje:

MPI_Gather(b1, 2, MPI_INT, b2, 2, MPI_INT, root, MPI_COMM_WORLD);
MPI_Bcast(b2, 8, MPI_INT, root, MPI_COMM_WORLD);

*/
