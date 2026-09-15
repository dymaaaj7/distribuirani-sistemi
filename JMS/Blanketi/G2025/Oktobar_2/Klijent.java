/*
Zadatak - Oktobar 2 2025 (isti kao April 2025 i Septembar 2023)

Kreirati sistem za održavanje **optimalne temperature u hotelu**.

Klijent na osnovu temperature koju dobije od senzora (postoji sistem koji šalje poruku koja je temperatura na određenoj lokaciji — ne implementirati).

Klijent na osnovu zahtevane temperature i dobijene temperature šalje zahtev za grejanje, hlađenje ili isključivanje sistema na toj lokaciji (obradu zahteva ne implementirati).

Potrebno je prikazati koja je akcija preuzeta.

Omogućiti da korisnik može da podesi željenu temperaturu i moguće odstupanje (npr. 0.3 °C). Smatrati da se podešavanje vrši samo na startovanju klijenta.

Voditi računa da se poruke dostavljaju samo zainteresovanim stranama, a ne svima.

Navesti na početku sve komunikacione kanale koje koristite i njihovu namenu.

---
KOMUNIKACIONI KANALI:

qTemperatura (queue) — sistem senzora objavljuje izmerenu temperaturu po lokaciji;
                      klijent prima samo poruke svoje lokacije (selector: Lokacija).
qAkcija (queue) — klijent šalje zahtev za grejanje/hlađenje/isključivanje za svoju
                  lokaciju (property: Lokacija); prima sistem za grejanje (ne implementira se).
*/
package JMS.Blanketi.G2025.Oktobar_2;

import javax.naming.*;

import java.util.Scanner;

import javax.jms.*;

public class Klijent {
    private final String lokacija;
    private final double zeljenaTemperatura;
    private final double odstupanje;

    private final Queue qTemperatura;
    private final Queue qAkcija;

    private final QueueConnection qc;
    private final QueueSession qs;

    private final QueueSender sender;
    private QueueReceiver receiver;

    public Klijent(String lokacija, double zeljenaTemperatura, double odstupanje) throws Exception {
        this.lokacija = lokacija;
        this.zeljenaTemperatura = zeljenaTemperatura;
        this.odstupanje = odstupanje;

        InitialContext ictx = new InitialContext();
        qTemperatura = (Queue) ictx.lookup("qTemperatura");
        qAkcija = (Queue) ictx.lookup("qAkcija");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        ictx.close();

        qc = (QueueConnection) qcf.createQueueConnection();
        qs = (QueueSession) qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
        sender = qs.createSender(qAkcija);
    }

    public void kreni() throws Exception {
        receiver = (QueueReceiver) qs.createReceiver(qTemperatura, "Lokacija = '" + lokacija + "'");

        receiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message message) {
                try {
                    MapMessage msg = (MapMessage) message;
                    double t = msg.getDouble("temperatura");

                    String akcija;
                    if (t < zeljenaTemperatura - odstupanje) {
                        akcija = "Grejanje";
                    } else if (t > zeljenaTemperatura + odstupanje) {
                        akcija = "Hladjenje";
                    } else {
                        akcija = "Iskljucivanje";
                    }

                    posaljiZahtev(akcija);
                    System.out.println("[" + lokacija + "]" + t + "C -> akcija: " + akcija);
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
        // ---- 1. UNOS PODEŠAVANJA (zadatak: "podešavanje se vrši samo na startovanju") ----
        @SuppressWarnings("resource") // ne zatvaramo Scanner — zatvaranjem bismo zatvorili i System.in
        Scanner in = new Scanner(System.in);
        System.out.println("Lokacija:");
        String lokacija = in.nextLine();
        System.out.println("Zeljena temperatura:");
        // parseDouble(nextLine()) umesto nextDouble() — nextDouble ostavlja prazan red u baferu
        double zeljenaTemp = Double.parseDouble(in.nextLine());
        System.out.println("Odstupanje:");
        double odstupanje = Double.parseDouble(in.nextLine());

        // ---- 2. KONSTRUKTOR: JNDI lookup (qTemperatura, qAkcija, qcf) + konekcija + session + sender ----
        Klijent k = new Klijent(lokacija, zeljenaTemp, odstupanje);

        // ---- 3. KRENI: pravi receiver SA SELECTOROM, registruje listener, poziva qc.start() ----
        // VAŽNO: kreni() se izvrši odmah i vrati se — NE čeka poruke ovde!
        // Od ovog trenutka JMS provajder, na SVOM thread-u, zove naš onMessage za svaku poruku.
        k.kreni();

        System.out.println("Klijent za sobu: " + lokacija + " aktivan.");

        // ---- 4. ZADRŽAVAMO PROGRAM ŽIVIM ----
        // main je gotov sa poslom, ali ako izađe iz metode → JVM se gasi → listener umire.
        // read() BLOKIRA main thread dok ne pritisneš Enter — za to vreme listener radi u pozadini.
        System.in.read();

        // ---- 5. ČIST KRAJ: nakon Entera zatvaramo konekciju (i sve što je na njoj) ----
        k.zatvori();
    }
}
