package JMS.Vezbe.JMS_08;

import javax.jms.*;
import javax.naming.*;

import java.util.Scanner;


public class Lekar {
    private final QueueConnection qc;
    private final QueueSession qs;
    private final QueueSender sender;
    private final TopicConnection tc;
    private final TopicSession ts;
    private final Topic obavestiLekara;
    private TopicSubscriber receiver;
    public String nazivLekara;

    public Lekar() throws Exception {
        System.out.println("Lekar: pribavljanje contexta");

        InitialContext ictx = new InitialContext();

        obavestiLekara = (Topic) ictx.lookup("tObavestiLekara");
        Queue stampa = (Queue) ictx.lookup("qStampaCentrala");
        TopicConnectionFactory tcf = (TopicConnectionFactory) ictx.lookup("tCentrala");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qCentrala");

        ictx.close();

        qc = (QueueConnection) qcf.createConnection();
        qs = (QueueSession) qc.createQueueSession(true, Session.AUTO_ACKNOWLEDGE);

        sender = (QueueSender) qs.createSender(stampa);

        tc = (TopicConnection) tcf.createConnection();
        ts = (TopicSession) tc.createTopicSession(true, Session.AUTO_ACKNOWLEDGE);

    }

    private void ObavestiCentralu(String l) throws JMSException{
        Message msg=qs.createMessage();
        msg.setStringProperty("Lekar", l);

        sender.send(msg);

        qs.commit();
    }
    
    private void KreniSaRadom(String l) throws Exception{
        System.out.println("Lekar krece sa radom");

        nazivLekara=l;

        ObavestiCentralu(l);

        receiver=(TopicSubscriber) ts.createSubscriber(obavestiLekara,"Lekar = '"+l+"'", true);
        receiver.setMessageListener(new LekarMessageListener(this));

        qc.start();
        tc.start();
    }

    private void Zatvori() throws JMSException{
        qc.close();
        tc.close();
    }

    public static void main(String[] args) throws Exception {
            Lekar lekar=new Lekar();

            Scanner in=new Scanner(System.in);

            System.out.println("Unesite lekara:");
            String l=in.nextLine();
        
            lekar.KreniSaRadom(l);

            System.out.println("Lekar :: inicijalizacija gotova.");

            System.in.read();

            lekar.Zatvori();     
    }
}
