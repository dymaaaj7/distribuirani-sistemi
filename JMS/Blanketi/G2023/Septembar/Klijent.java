/*
Zadatak - April 2025 (isti kao Septembar 2023; varijanta Oktobra 2023)

Kreirati sistem za održavanje optimalne temperature u hotelu.

Klijent na osnovu temperature koju dobije od senzora (postoji sistem koji šalje
poruku koja je temperatura na određenoj lokaciji, ne implementirati).

Klijent na osnovu zahtevane temperature i dobijene temperature šalje zahtev za
grejanje, hlađenje ili isključivanje sistema na toj lokaciji (obradu zahteva
ne implementirati).

Potrebno je prikazati koja je akcija preuzeta.

Omogućiti da korisnik može da podesi željenu temperaturu i moguće odstupanje
(npr. 0.3 °C).

Smatrati da se podešavanje vrši samo na startovanju klijenta.

Voditi računa da se poruke dostavljaju samo zainteresovanim stranama, a ne svima.

Navesti na početku sve komunikacione kanale koje koristite i njihovu namenu.

---
KOMUNIKACIONI KANALI:
qTemperatura - sluzi da prima trenutne temperature od senzura sa lokacija
qAkcija - sluzi da salje instrukciju sobi sta da uradi sa trenutnom temperaturom (na osnovu akcije)
*/

package JMS.Blanketi.G2023.Septembar;

import java.util.Scanner;

import javax.jms.*;
import javax.naming.*;

public class Klijent {
    private final String lokacija;
    private final double zeljenaTemp;
    private final double odstupanje;

    private final Queue qTemperatura;
    private final Queue qAkcija;
    private final QueueConnection qc;
    private final QueueSession qs;

    private final QueueSender sender;
    private QueueReceiver receiver;

    public Klijent(String lokacija, double zeljenaTemp, double odstupanje) throws Exception {
        this.lokacija = lokacija;
        this.zeljenaTemp = zeljenaTemp;
        this.odstupanje = odstupanje;

        InitialContext ictx = new InitialContext();

        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        qTemperatura = (Queue) ictx.lookup("qTemperatura");
        qAkcija = (Queue) ictx.lookup("qAkcija");

        ictx.close();

        qc = (QueueConnection) qcf.createQueueConnection();
        qs = (QueueSession) qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
        sender = qs.createSender(qAkcija);
    }

    public void kreni() throws Exception {
        receiver = qs.createReceiver(qTemperatura, "Lokacija= '" + lokacija + "'");

        receiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message message) {
                try {
                    MapMessage msg = (MapMessage) message;
                    double t = msg.getDouble("temperatura");

                    String akcija;
                    if (t < zeljenaTemp - odstupanje) {
                        akcija = "GREJANJE";
                    } else if (t > zeljenaTemp + odstupanje) {
                        akcija = "HLADJENJE";
                    } else {
                        akcija = "ISKLJUCIVANJE";
                    }
                    posaljiZahtev(akcija);
                    System.out.println("[" + lokacija + "] " + t + "°C -> akcija: " + akcija);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        qc.start();
    }

    public void posaljiZahtev(String akcija) throws Exception {
        MapMessage msg = qs.createMapMessage();
        msg.setString("akcija", akcija);
        msg.setStringProperty("Lokacija", lokacija);
        sender.send(msg);
    }

    public void zatvori() throws Exception {
        qc.close();
    }

    public static void main(String[] args) throws Exception {
        @SuppressWarnings("resource")
        Scanner in = new Scanner(System.in);
        System.out.println("Lokacija:");
        String lokacija = in.nextLine();
        System.out.println("Zeljena temperatura:");
        double zeljenaTemp = Double.parseDouble(in.nextLine());
        System.out.println("Odstupanje:");
        double odstupanje = Double.parseDouble(in.nextLine());
        Klijent k = new Klijent(lokacija, zeljenaTemp, odstupanje);

        k.kreni();

        System.out.println("Klijent za sobu: " + lokacija + " aktivan.");
        System.in.read();

        k.zatvori();

    }
}
