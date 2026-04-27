package JMS.Vezbe.JMS_09.Necentralizovana;

import javax.jms.*;

public class SestraML implements MessageListener {

    public SestraML() {
    }

    @Override
    public void onMessage(Message msg) {
        TextMessage txt = (TextMessage) msg;
        try {
            System.out.println(txt.getText());
        } catch (JMSException e) {
            e.printStackTrace();
        }

    }

}
