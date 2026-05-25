package JMS.Vezbe.JMS_08;

import java.util.Scanner;

import javax.jms.*;
import javax.naming.*;

public class Sestra {

    private final QueueConnection qc;
    private final QueueSession qs;
    private final QueueSender sender;

    public Sestra() throws Exception {
        System.out.println("Sestra :: pribavljanje contexta");

        InitialContext ictx = new InitialContext();

        Queue obavestiLekara = (Queue) ictx.lookup("qObavestiLekara");
        Queue stampa = (Queue) ictx.lookup("qStampa");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qCentrala");
        ictx.close();

        qc = (QueueConnection) qcf.createConnection();
        qs = (QueueSession) qc.createSession(true, Session.AUTO_ACKNOWLEDGE);

        sender = (QueueSender) qs.createSender(obavestiLekara);
        QueueReceiver receiver = (QueueReceiver) qs.createReceiver(stampa);

        receiver.setMessageListener(new StampaMessageListener());

        qc.start();
        System.out.println("Pokrenuto slusanje");
    }

    public void ObavestiLekaraDaJePacijentStigao(String lekar, String pacijent)
            throws Exception {
        TextMessage msg = qs.createTextMessage(pacijent);
        // TextMessage msg = qs.createTextMessage();
        // msg.setText(pacijent);
        msg.setStringProperty("Lekar", lekar);

        sender.send(msg);
        qs.commit();
    }

    public void Zatvori() throws Exception {
        qc.close();
    }

    public static void main(String[] args) throws Exception {
        Sestra s = new Sestra();

        try (Scanner in = new Scanner(System.in)) {
            System.out.println("Unesite pacijenta:");
            String p = in.nextLine();

            System.out.println("Unesite lekara:");
            String l = in.nextLine();

            s.ObavestiLekaraDaJePacijentStigao(l, p);
            System.in.read();
        }

        s.Zatvori();
    }
}
