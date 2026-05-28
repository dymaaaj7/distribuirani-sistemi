package JMS.Vezbe.JMS_09.Necentralizovana;

import java.util.Scanner;

import javax.jms.*;
import javax.naming.*;

public class Lekar {
    private final Queue qZaLekara;
    private final Queue qStampa;
    private final QueueConnection qc;
    private final QueueSession qs;
    private final QueueSender sender;
    private QueueReceiver receiver;

    public Lekar() throws Exception {

        InitialContext ictx = new InitialContext();

        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        qZaLekara = (Queue) ictx.lookup(("qObavestiLekara"));
        qStampa = (Queue) ictx.lookup("qStampa");

        ictx.close();

        qc = (QueueConnection) qcf.createQueueConnection();
        qs = (QueueSession) qc.createQueueSession(true, Session.AUTO_ACKNOWLEDGE);

        sender = (QueueSender) qs.createSender(qStampa);
    }

    public void ObavestiSestru(String pacijentPoruka) throws Exception {
        // msg.getText() vraca celu poruku sestre, izvlacimo samo ime pacijenta String
        String imePacijenta = pacijentPoruka.replace("Pacijent stigao. Ime: ", "");
        TextMessage txt = qs.createTextMessage(imePacijenta);
        sender.send(txt);
        qs.commit();
    }

    public void KreniSaRadom(String lekar) throws Exception {
        receiver = (QueueReceiver) qs.createReceiver(qZaLekara, "Lekar '" + lekar + "'");

        receiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message message) {
                TextMessage msg = (TextMessage) message;
                try {
                    System.out.println(lekar + " primio pacijenta: " + msg.getText());
                    ObavestiSestru(msg.getText());
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        qc.start();
    }

    public void Zatvori() throws Exception {
        qc.close();
    }

    public static void main(String[] args) throws Exception {
        Scanner in = new Scanner(System.in);
        System.out.println("Unesi lekara: ");
        String ime = in.nextLine();

        Lekar l = new Lekar();

        l.KreniSaRadom(ime);

        in.next();
    }
}
