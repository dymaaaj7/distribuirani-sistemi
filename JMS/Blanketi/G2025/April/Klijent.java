/*
Zadatak - April 2025 (isti kao Septembar 2023 i Oktobar 2 2025; varijanta Oktobra 2023)

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

qTemperatura (queue) — sistem senzora objavljuje izmerenu temperaturu po lokaciji;
                      klijent prima samo poruke svoje lokacije (selector: Lokacija).
qAkcija (queue) — klijent šalje zahtev za grejanje/hlađenje/isključivanje za svoju
                  lokaciju; prima sistem za grejanje (ne implementira se).
*/
package JMS.Blanketi.G2025.April;

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
        qTemperatura = (Queue) ictx.lookup("qTemperatura");
        qAkcija = (Queue) ictx.lookup("qAkcija");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        ictx.close();

        qc = qcf.createQueueConnection();
        qs = qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
        sender = qs.createSender(qAkcija);
    }

    public void kreni() throws Exception {
        // filter — klijent prima SAMO temperature svoje lokacije
        receiver = qs.createReceiver(qTemperatura, "Lokacija = '" + lokacija + "'");

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
                        akcija = "ISKJUCIVANJE"; // unutar opsega — sistem miruje
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

    private void posaljiZahtev(String akcija) throws Exception {
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
        String lok = in.nextLine();
        System.out.println("Zeljena temperatura:");
        double zelj = Double.parseDouble(in.nextLine());
        System.out.println("Odstupanje:");
        double odst = Double.parseDouble(in.nextLine());

        Klijent k = new Klijent(lok, zelj, odst);
        k.kreni();

        System.out.println("Klijent za " + lok + " aktivan...");
        System.in.read();

        k.zatvori();
    }
}
