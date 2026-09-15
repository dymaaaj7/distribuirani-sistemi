/*
# Zadatak - Januar 2025 (isti kao Septembar 2025, Septembar 2024 i Oktobar 2024)

Koristeći JMS kreirati sistem za ispitivanje i ocenjivanje.

Sistem treba da sadrži funkciju Pokreni(bool nastavnik, string sifraPredmeta) koja za nastavnika (true)
omogućava da dobije odgovore od strane studenata, odnosno za studenta (false) da dobije pitanja koja je
poslao nastavnik.

Može biti više studenata i nastavnika po sifriPredmeta. Pri ocenjivanju voditi računa da samo jedan
nastavnik vrši ocenjivanje.

Zatim sistem sadrži funkciju PosaljiPitanje(string sifraPredmeta, string sifraPitanja, string Pitanje)
kojom nastavnik šalje pitanje studentima gde je Pitanje tekstualnog oblika. Sistem studentu prikazuje
pitanje na ekran po prijemu.

Takođe, sistem sadrži funkciju za slanje odgovora. Odgovor se prikazuje nastavniku koji ocenjuje.

Nije potrebno implementirati pozive ovih funkcija već samo funkcije. Ostatak sistema smatrati realizovanim.

Voditi računa da samo zainteresovane strane dobiju poruke.

Obavezno na početku skicirati tok razmene poruka.

---
SKICA TOKA RAZMENE PORUKA:

   PosaljiPitanje(sifra, idPitanja, tekst)              PosaljiOdgovor(sifra, idPitanja, tekst)
        │                                                    │
        ▼                                                    ▼
┌───────────────┐  topic: SVIM studentima predmeta    ┌───────────────┐
│   tPitanja    │───────────────────────────────────▶ │   qOdgovori   │──▶ TAČNO JEDAN nastavnik
│ (topic)       │  selector: SifraPredmeta            │  (queue)      │    (onaj koji primi — ocenjuje)
└───────────────┘                                     └───────────────┘
   Pokreni(false, sifra): student                         Pokreni(true, sifra): nastavnik
   = subscriber na tPitanja                               = receiver na qOdgovori

- topic za pitanja: svi studenti predmeta moraju da vide pitanje (kopija svima)
- queue za odgovore: "samo jedan nastavnik ocenjuje" = queue daje svaki odgovor tačno jednom primaocu
- selector po SifraPredmeta na oba kanala — predmeti se ne mešaju
*/
package JMS.Blanketi.G2025.Januar;

import javax.jms.*;
import javax.naming.*;

public class Sistem {

    private final Topic tPitanja;
    private final Queue qOdgovori;

    private final QueueConnection qc;
    private final TopicConnection tc;
    private final QueueSession qs;
    private final TopicSession ts;
    private final QueueSender queueSender;
    private final TopicPublisher topicPublisher;

    private QueueReceiver receiver;
    private TopicSubscriber subscriber;

    public Sistem() throws Exception {
        InitialContext ictx = new InitialContext();
        tPitanja = (Topic) ictx.lookup("tPitanja");
        qOdgovori = (Queue) ictx.lookup("qOdgovori");
        TopicConnectionFactory tcf = (TopicConnectionFactory) ictx.lookup("tcf");
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        ictx.close();

        qc = (QueueConnection) qcf.createQueueConnection();
        tc = (TopicConnection) tcf.createTopicConnection();
        qs = (QueueSession) qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
        ts = (TopicSession) tc.createTopicSession(false, Session.AUTO_ACKNOWLEDGE);

        queueSender = (QueueSender) qs.createSender(qOdgovori);
        topicPublisher = (TopicPublisher) ts.createPublisher(tPitanja);
    }

    public void Pokreni(boolean nastavnik, String sifraPredmeta) throws Exception {
        if (nastavnik) {
            receiver = (QueueReceiver) qs.createReceiver(qOdgovori, "SifraPredmeta = '" + sifraPredmeta + "'");
            receiver.setMessageListener(new MessageListener() {
                @Override
                public void onMessage(Message message) {
                    try {
                        TextMessage msg = (TextMessage) message;
                        String sifraPitanja = msg.getStringProperty("sifraPitanja");
                        String odgovor = msg.getText();
                        System.out.println("Odgovor na pitanje " + sifraPitanja + ": " + odgovor);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        } else {
            // noLocal=true — student i ne objavljuje pitanja, pa je svejedno; false bi
            // radilo isto
            subscriber = (TopicSubscriber) ts.createSubscriber(
                    tPitanja,
                    "SifraPredmeta = '" + sifraPredmeta + "'",
                    true);
            subscriber.setMessageListener(new MessageListener() {
                @Override
                public void onMessage(Message message) {
                    try {
                        TextMessage msg = (TextMessage) message;
                        String sifraPitanja = msg.getStringProperty("sifraPitanja");
                        String pitanje = msg.getText();
                        System.out.println("Pitanje " + sifraPitanja + ": " + pitanje);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            });
        }

        qc.start();
        tc.start();
    }

    public void PosaljiPitanje(String sifraPredmeta, String sifraPitanja, String pitanje) throws Exception {
        TextMessage message = ts.createTextMessage();
        message.setText(pitanje);
        message.setStringProperty("sifraPredmeta", sifraPredmeta);
        message.setStringProperty("sifraPitanja", sifraPitanja);
        topicPublisher.publish(message);
    }

    public void PosaljiOdgovor(String sifraPredmeta, String sifraPitanja, String odgovor) throws Exception {
        TextMessage message = qs.createTextMessage();
        message.setText(odgovor);
        message.setStringProperty("sifraPredmeta", sifraPredmeta);
        message.setStringProperty("sifraPitanja", sifraPitanja);
        queueSender.send(message);
    }

    public void zatvori() throws Exception {
        qc.close();
        tc.close();
    }
}
