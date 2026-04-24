package JMS.Vezbe.JMS_07.Primer_3;

import javax.jms.*;

public class MsgListener implements MessageListener {
    private String id;

    public MsgListener() {
        id = "";
    }

    public MsgListener(String id) {
        this.id = id;
    }

    public void onMessage(Message msg) {
        TextMessage tsmg = (TextMessage) msg;
        try {
            System.out.println(id + ": " + tsmg.getText());
        } catch (JMSException jE) {
            jE.printStackTrace();
        }
    }
}