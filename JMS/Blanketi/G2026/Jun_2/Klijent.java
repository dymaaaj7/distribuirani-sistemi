/*
# Zadatak - Jun 2 2026

Koristeći JMS kreirajte sistem za **razmenu poruka**.

Sistem se sastoji od dve metode:

- **Start(string ime)** — startuje klijenta koji prima poruke adresirane na zadato ime
- **Posalji(string ime, string tekst)** — šalje poruku (prosleđeni tekst) klijentu sa prosleđenim imenom

Može postojati više klijenata sa istim imenom — smatrati da su svi klijenti sa istim imenom **isti korisnik**.

---
KOMUNIKACIONI KANALI:

qPoruke (queue) — jedini kanal; klijent šalje poruku sa property-jem Ime (primalac) i
                  Posiljalac, a prima samo poruke adresirane na svoje ime (selector: Ime).
                  Queue load-balansuje među klijentima istog imena — kako su "isti korisnik",
                  nebitno je koji tačno klijent dobije poruku.
*/
package JMS.Blanketi.G2026.Jun_2;

import java.util.Scanner;

import javax.jms.*;
import javax.naming.*;

public class Klijent {
    private final String ime;

    private final Queue qPoruke;
    private final QueueConnection qc;
    private final QueueSession qs;
    private final QueueSender sender;
    private QueueReceiver receiver;

    public Klijent(String ime) throws Exception {
        this.ime = ime;

        InitialContext ictx = new InitialContext();
        qPoruke = (Queue) ictx.lookup("qPoruke");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        ictx.close();

        qc = (QueueConnection) qcf.createQueueConnection();
        qs = (QueueSession) qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
        sender = (QueueSender) qs.createSender(qPoruke);
    }

    public void Start(String ime) throws Exception {
        receiver = (QueueReceiver) qs.createReceiver(qPoruke, "Ime = '" + ime + "'");

        receiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message message) {
                try {
                    TextMessage msg = (TextMessage) message;
                    String posiljalac = msg.getStringProperty("Posiljalac");
                    String tekst = msg.getText();
                    System.out.println("[" + posiljalac + "] " + tekst);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        qc.start();
    }

    public void Posalji(String ime, String tekst) throws Exception {
        TextMessage msg = qs.createTextMessage();
        msg.setText(tekst);
        msg.setStringProperty("Ime", ime); // primalac — po ovom selector filtrira
        msg.setStringProperty("Posiljalac", this.ime); // pošiljalac — da primalac zna od koga je
        sender.send(msg);
    }

    public void zatvori() throws Exception {
        qc.close();
    }

    public static void main(String[] args) throws Exception {
        @SuppressWarnings("resource")
        Scanner in = new Scanner(System.in);
        System.out.println("Unesi svoje ime:");
        String ime = in.nextLine();

        Klijent k = new Klijent(ime);
        k.Start(ime);

        System.out.println("Klijent " + ime + " aktivan...");
        System.in.read();

        k.zatvori();
    }
}
