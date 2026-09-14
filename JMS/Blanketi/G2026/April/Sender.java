/*
Zadatak - Jun 2026

Koristeci JMS Kreirati aplikaciju za obavestenje.

Sistem sadrzi funkciju za slanje Obavestenja, koje poseduje Autora, Datum i Tekst.

Po prijemu obavestenja prikazati sve vrednosti korisniku.
*/
package JMS.Blanketi.G2026.April;

import javax.naming.*;

import java.util.Scanner;

import javax.jms.*;

public class Sender {
    private final Queue qObavestenja;
    private final QueueConnection qc;
    private final QueueSession qs;
    private final QueueSender sender;

    public Sender() throws Exception {
        InitialContext ictx = new InitialContext();

        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        qObavestenja = (Queue) ictx.lookup("qObavestenja");

        ictx.close();

        qc = (QueueConnection) qcf.createQueueConnection();
        qs = (QueueSession) qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
        sender = qs.createSender(qObavestenja);
    }

    public void posaljiObavestenje(String autor, String datum, String tekst) throws Exception {
        MapMessage msg = qs.createMapMessage();
        msg.setString("autor", autor);
        msg.setString("datum", datum);
        msg.setString("tekst", tekst);
        sender.send(msg);

    }

    public void zatvori() throws Exception {
        qc.close();
    }

    public static void main(String[] args) throws Exception {
        Sender s = new Sender();

        @SuppressWarnings("resource")
        Scanner in = new Scanner(System.in);

        while (true) {
            System.out.println("Unesi autora (prazan red za kraj):");
            String autor = in.nextLine();
            if (autor.isEmpty())
                break;

            System.out.println("Unesi datum:");
            String datum = in.nextLine();

            System.out.println("Unesi tekst:");
            String tekst = in.nextLine();

            s.posaljiObavestenje(autor, datum, tekst);
        }

        s.zatvori();
    }
}
