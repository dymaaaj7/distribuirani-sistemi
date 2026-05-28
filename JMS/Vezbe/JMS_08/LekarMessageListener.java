package JMS.Vezbe.JMS_08;

import javax.jms.*;

public class LekarMessageListener implements MessageListener {

    Lekar lekar;

    public LekarMessageListener(Lekar l) {
        lekar = l;
    }

    @Override
    public void onMessage(Message msg) {
        TextMessage txt = (TextMessage) msg;
        
        try {
            System.out.println(lekar.nazivLekara + " primio pacijenta " + txt.getText());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
