package JMS.Vezbe.JMS_07.Primer_2;

import javax.jms.*;
import javax.naming.*;

public class Sender {
    static Context ictx = null;

    public static void main(String[] args)
            throws NamingException, JMSException {
        ictx = new InitialContext();

        Queue queue = (Queue) ictx.lookup("queue");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup(
                "jms/__defaultConnectionFactory");

        ictx.close();

        QueueConnection qc = qcf.createQueueConnection();
        QueueSession qs = qc.createQueueSession(
                false, Session.AUTO_ACKNOWLEDGE);

        QueueSender sender = qs.createSender(queue);
        TextMessage msg = qs.createTextMessage();

        for (int i = 0; i < 10; i++) {
            msg.setText("Hello bitch [" + i + "]!");
            sender.send(msg);
        }

        qc.close();
    }

}