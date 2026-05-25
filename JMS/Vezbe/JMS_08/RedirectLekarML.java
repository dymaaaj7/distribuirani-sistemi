package JMS.Vezbe.JMS_08;

import javax.jms.*;

public class RedirectLekarML implements MessageListener{

    Centrala centrala;

    public RedirectLekarML(Centrala l){
        centrala=l;
    }

    @Override
    public void onMessage(Message message){
        try {
            centrala.RedirectLekar(message);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }   
}
