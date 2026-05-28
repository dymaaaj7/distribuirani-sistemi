package JMS.Vezbe.JMS_08;

import javax.jms.*;

public class StampaMessageListener implements MessageListener {

    public StampaMessageListener() {

    }

    @Override
    public void onMessage(Message msg) {
        TextMessage txt=(TextMessage) msg;
        
        try {
            System.out.println("Pregledan pacijent: " + txt.getText());
        } catch (JMSException e) {
            e.printStackTrace();
        }
    }
}
