package JMS.Vezbe.JMS_08;

import javax.jms.*;

public class RedirectStampaML implements MessageListener{

    Centrala centrala;

    public RedirectStampaML(Centrala c){
        centrala=c;
    }

    @Override
    public void onMessage(Message message){
        try {
            centrala.RedirectStampa(message);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }   
}
