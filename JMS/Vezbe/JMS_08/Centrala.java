package JMS.Vezbe.JMS_08;

import javax.jms.*;
import javax.naming.*;

public class Centrala {

    private final Topic tObavestiLekara;
    private final QueueConnection qc;
    private final QueueSession qs;
    private final TopicConnection tc;
    private final TopicSession ts;
    private final QueueSender senderStampa;
    private final TopicPublisher obavestiLekar;

    public Centrala() throws Exception {
        System.out.println("Centrala: pribavljanje contexta.");

        InitialContext ictx=new InitialContext();
        
        tObavestiLekara = (Topic) ictx.lookup("tObavestiLekara");
        Queue stampaCentala = (Queue) ictx.lookup("qStampaCentrala");
        Queue stampa = (Queue) ictx.lookup("qStampa");
        Queue obavestiLekara = (Queue) ictx.lookup("qObavestiCentralu");
        QueueConnectionFactory qcf = (QueueConnectionFactory)ictx.lookup("qCentrala");
        TopicConnectionFactory tcf=(TopicConnectionFactory)ictx.lookup("tCentrala");
        
        ictx.close();

        qc=(QueueConnection) qcf.createConnection();
        qs=(QueueSession) qc.createQueueSession(true,Session.AUTO_ACKNOWLEDGE);

        tc=(TopicConnection) tcf.createConnection();
        ts=(TopicSession) tc.createTopicSession(true,Session.AUTO_ACKNOWLEDGE);

        System.out.println("Centrala: Recieve and Send.");

        senderStampa=(QueueSender) qs.createSender(stampa);

        QueueReceiver receiverLekar=(QueueReceiver) qs.createReceiver(obavestiLekara);
        receiverLekar.setMessageListener(new RedirectLekarML(this));

        QueueReceiver receiverStampa=(QueueReceiver) qs.createReceiver(stampaCentala);
        receiverStampa.setMessageListener(new RedirectStampaML(this));

        obavestiLekar=(TopicPublisher) ts.createPublisher(tObavestiLekara);

        System.out.println("Centrala: Recieve and Send.");
    }

    void RedirectLekar(Message message) throws Exception {
        obavestiLekar.send(message);
    }

    void RedirectStampa(Message message) throws Exception{
        senderStampa.send(message);
    }

}
