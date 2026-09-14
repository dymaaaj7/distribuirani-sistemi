# Zadatak - Jun 2 2022 (isti kao April 2022 i Jun 2022)

Pomoću JMS-a dopuniti sistem koji omogućava obaveštavanje menadžera o promeni stanja robe u prodavnici.

Svi klijenti su povezani u necentralizovanoj mreži.

Sistem šalje poruku koja sadrži Proizvod i javne propertije poruke (string SifraArtikla, decimal novoStanje).

Omogućiti da klijent podesi koje poruke želi da primi.

Na primer: `SifraArtikla = 'Šivaća mašina' AND novoStanje > 500`; `SifraArtikla = '000302501' AND novoStanje BETWEEN 1000 AND 2000`.

Prikazati sadržaj primljene poruke.

```java
public class Proizvod implements Serializable {
    public String sifra;
    public String proizvodjacNaziv;
    public String naziv;
    public decimal kolicina;
    public decimal cena;

    public Proizvod() {

    }
}
```
