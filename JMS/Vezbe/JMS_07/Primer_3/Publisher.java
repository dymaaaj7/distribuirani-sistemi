package JMS.Vezbe.JMS_07.Primer_3;

import javax.jms.*;
import javax.naming.*;

public class Publisher {
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

        TopicPublisher tp = ts.createPublisher(topic);
        TextMessage tmsg = ts.createTextMessage();

        for (int i = 0; i < 10; i++) {
            tmsg.setText("Test number " + i);
            tp.send(topic, tmsg);
            tp.send(topic, tmsg);
        }
        ts.commit();

        tc.close();
    }
}
