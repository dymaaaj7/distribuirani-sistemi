# Zadatak - April 2022 (isti kao Jun 2022 i Jun 2 2022)

Pomoću JMS-a dopuniti sistem koji omogućava obaveštavanje korisnika o promeni stanja robe u prodavnici.

Svi klijenti su povezani u necentralizovanoj mreži.

Sistem šalje poruku koja sadrži Proizvod i javne propertije poruke (string Proizvođač, string ArtikalNaziv, double Cena).

Omogućiti da klijent podesi koje poruke želi da primi.

Na primer: `ArtikalNaziv = 'Sony PS5' AND Cena BETWEEN 50000 AND 80000`.

Prikazati sadržaj primljene poruke.

```java
public class Proizvod implements Serializable {
    public String proizvodID;
    public String proizvodjacNaziv;
    public String naziv;
    public decimal kolicina;
    public decimal cena;

    public Proizvod() {

    }
}
```
