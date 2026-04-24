package JMS.Vezbe.JMS_07.Primer_3;

import javax.jms.*;
import javax.naming.*;

public class Subscriber {
    static Context ictx = null;

    public static void main(String args[]) throws Exception {
        ictx = new InitialContext();

        Topic topic = (Topic) ictx.lookup("topic");
        TopicConnectionFactory tcf = (TopicConnectionFactory) ictx.lookup(
                "jms/__defaultConnectionFactory");

        ictx.close();

        TopicConnection tc = tcf.createTopicConnection();
        TopicSession ts = tc.createTopicSession(
                true, Session.AUTO_ACKNOWLEDGE);
        TopicSubscriber tsub = ts.createSubscriber(topic);
        tsub.setMessageListener(new MsgListener("listenerjms"));

        tc.start();

        System.out.println("Subscriber: ");
        System.in.read();

        tc.close();
    }
}
