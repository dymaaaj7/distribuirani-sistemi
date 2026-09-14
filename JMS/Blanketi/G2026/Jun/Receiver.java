/*
Zadatak - Jun 2026

Koristeci JMS Kreirati aplikaciju za obavestenje.

Sistem sadrzi funkciju za slanje Obavestenja, koje poseduje Autora, Datum i Tekst.

Po prijemu obavestenja prikazati sve vrednosti korisniku.
*/
package JMS.Blanketi.G2026.Jun;

import javax.jms.*;
import javax.naming.*;

public class Receiver {
    private final Queue qObavestenja;
    private final QueueConnection qc;
    private final QueueSession qs;
    private QueueReceiver receiver;

    public Receiver() throws Exception {
        InitialContext ictx = new InitialContext();

        qObavestenja = (Queue) ictx.lookup("qObavestenja");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");

        ictx.close();

        qc = qcf.createQueueConnection();
        qs = qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);

    }

    public void kreni() throws Exception {
        receiver = qs.createReceiver(qObavestenja);

        receiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message message) {
                try {
                    MapMessage msg = (MapMessage) message;
                    System.out.println("Autor: " + msg.getString("autor"));
                    System.out.println("Datum: " + msg.getString("datum"));
                    System.out.println("Tekst: " + msg.getString("tekst"));
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        qc.start();
    }

    public void zatvori() throws Exception {
        qc.close();
    }

    public static void main(String[] args) throws Exception {
        Receiver r = new Receiver();
        r.kreni();

        System.out.println("Cekam obavestenja...");
        System.in.read();

        r.zatvori();
    }
}
