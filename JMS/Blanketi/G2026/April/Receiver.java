/*
 * # Zadatak - April 2026 (isti kao Jun 2026 i Jun 2025)
 *
 * Koristeci JMS Kreirati aplikaciju za obavestenje.
 *
 * Sistem sadrzi funkciju za slanje Obavestenja, koje poseduje Autora, Datum i
 * Tekst.
 *
 * Po prijemu obavestenja prikazati sve vrednosti korisniku.
 */
package JMS.Blanketi.G2026.April;

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

        qc = (QueueConnection) qcf.createQueueConnection();
        qs = (QueueSession) qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
    }

    public void kreni() throws Exception {
        receiver = (QueueReceiver) qs.createReceiver(qObavestenja);
        receiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message message) {
                try {
                    MapMessage msg = (MapMessage) message;
                    String autor = msg.getString("autor");
                    String datum = msg.getString("datum");
                    String tekst = msg.getString("tekst");
                    System.out.println(autor);
                    System.out.println(datum);
                    System.out.println(tekst);
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

        System.out.println("Cekam obavestenja");
        System.in.read();

        r.zatvori();
    }
}
