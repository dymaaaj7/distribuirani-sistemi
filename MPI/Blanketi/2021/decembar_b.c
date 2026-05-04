/*

Napisati MPI program koji realizuje mnozenje matrice Akxl i vektora bl, cime se
izracunava i prikazuje rezultujuci vektor c.

Takodje, program pronalazi i prikazuje maksimalnu vrednost elemenata u matrici
A, kao i sumu elemenata svake vrste matrice A.

Izracunavanje se obavlja tako sto root proces salje svakom procesu po q kolona
matrice A (q - zadata konstanta, l deljivo sa q) i po q elemenata vektora b.

Svi elementi kolona matrice A se salju odjednom.

Svi procesi ucestvuju u izracunavanjima potrebnim za generisanje rezultata.

Svi rezultati programa se prikazuju u procesu koji sadrzi maksimalnu vrednost
elemenata u matrici A, nakon raspodele kolona po procesima.

Zadatak resiti:
a)  koriscenjem grupnih operacija, osim za slanje kolona matrice A
b)  koriscenjem P-t-P operacija za upotrebljene grupne operacije pod a)

*/