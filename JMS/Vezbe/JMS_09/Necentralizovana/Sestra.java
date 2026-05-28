package JMS.Vezbe.JMS_09.Necentralizovana;

import java.util.Scanner;

import javax.jms.*;
import javax.naming.*;

public class Sestra {
    private final Queue qObavestiLekara;
    private final Queue qStampa;
    private final QueueConnection qc;
    private final QueueSession qs;
    private final QueueSender sender;
    private QueueReceiver receiver;

    public Sestra() throws Exception {
        InitialContext ictx = new InitialContext();

        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        qObavestiLekara = (Queue) ictx.lookup("qObavestiLekara");
        qStampa = (Queue) ictx.lookup("qStampa");

        ictx.close();

        qc = (QueueConnection) qcf.createQueueConnection();
        qs = (QueueSession) qc.createQueueSession(true, Session.AUTO_ACKNOWLEDGE);

        sender = (QueueSender) qs.createSender(qObavestiLekara);

        receiver = (QueueReceiver) qs.createReceiver(qStampa);

        receiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message message) {
                try {
                    TextMessage txt = (TextMessage) message;
                    System.out.println("Pregled pacijenta gotov. Ime: " + txt.getText());
                } catch (Exception e) {
                    e.printStackTrace();
                }

            }
        });

        qc.start();
    }

    public void ObavestiLekara(String l, String p) throws Exception {
        TextMessage msg = qs.createTextMessage("Pacijent stigao. Ime: " + p);
        msg.setStringProperty("Lekar", l);

        sender.send(msg);

        qs.commit();
    }

    public void Zatvori() throws Exception {
        qc.close();
    }

    public static void main(String[] args) throws Exception {
        Sestra s = new Sestra();

        @SuppressWarnings("resource")
        Scanner in = new Scanner(System.in);

        System.out.println("Ucitaj pacijenta");
        String p = in.nextLine();

        System.out.println("Ucitaj lekara");
        String l = in.nextLine();

        s.ObavestiLekara(l, p);
    }
}
