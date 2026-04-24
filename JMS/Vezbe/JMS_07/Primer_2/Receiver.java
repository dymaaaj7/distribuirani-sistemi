package JMS.Vezbe.JMS_07.Primer_2;

import javax.naming.*;
import javax.jms.*;

public class Receiver {
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

        QueueReceiver receiver = qs.createReceiver(queue);
        TextMessage msg;

        qc.start();

        for (int i = 0; i < 10; i++) {
            msg = (TextMessage) receiver.receive();
            System.out.println("[" + Integer.toString(i)
                    + "] Poruka primljena: " + msg.getText());
        }

        qc.close();
    }

}