# JMS Blanketi — Šabloni

> Napomena: za sad pokriveno onim šta je urađeno (vezbe + Jun 2026). Dopunjuje se kako se rešavaju novi rokovii.
> Teorija: [../Vezbe/MOM_JMS.md](../Vezbe/MOM_JMS.md) · Beleške: [../beleske.md](../beleske.md)

## Skelet (objektni stil — kao Sestra/Lekar u vezbama)

```java
public class Ucesnik {
    private final Queue qX;  private final QueueConnection qc;
    private final QueueSession qs;  private final QueueSender sender;
    private QueueReceiver receiver;              // dodeljuje se tek u kreni() — zato bez final

    public Ucesnik() throws Exception {
        InitialContext ictx = new InitialContext();
        qX = (Queue) ictx.lookup("qIme");        // ISTO ime na SVIM učesnicima!
        QueueConnectionFactory qcf = (QueueConnectionFactory) ictx.lookup("qcf");
        ictx.close();
        qc = qcf.createQueueConnection();
        qs = qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);
        sender = qs.createSender(qX);            // primač nema sender (a receiver pravi u kreni)
    }

    public void posaljiX(...) throws Exception { /* createXxxMessage + send (+ commit ako transakciona) */ }

    public void kreni() throws Exception {       // samo primač
        receiver = qs.createReceiver(qX);
        receiver.setMessageListener(new MessageListener() {
            public void onMessage(Message message) {
                try { /* cast + čitanje + ispis */ }
                catch (Exception e) { e.printStackTrace(); }   // JEDINO mesto sa try-catch
            }
        });
        qc.start();                              // obavezno!
    }

    public void zatvori() throws Exception { qc.close(); }

    public static void main(String[] args) throws Exception {
        // pošiljalac: pozivi funkcije (Scanner unos ili fiksno) + zatvori
        // primalac:  u.kreni(); System.in.read(); u.zatvori();  ← read drži program živim
    }
}
```

## Šta je gde urađeno u vezbama (reference)

| Koncept | Gde |
| --- | --- |
| osnovni queue sender/receiver | `JMS_07/Primer_2` (static stil) |
| Topic publisher/subscriber + transakcije | `JMS_07/Primer_3` |
| selector (`"Lekar = '...'"+l`), property, prazan Message | `JMS_09` (Lekar), `JMS_08` (Lekar.ObavestiCentralu) |
| centralizovana arhitektura (centrala kao router) | `JMS_08` (Centrala + Redirect*ML) |
| necentralizovana (direktna komunikacija) | `JMS_09` |
| MapMessage + funkcija za slanje + listener ispis | `Blanketi/G2026/Jun` |

## Pravila koja se zaborave (iz vezbi i beleški)

- `qc.start()` — **samo primalac**, kraj metode za slušanje.
- `System.in.read()` — kraj main-a **primaoca**, inače se program ugasi pre prijema.
- try-catch **samo u `onMessage`** (interfejs ne baca izuzetke); svuda ostale `throws`.
- JNDI imena identična kod svih učesnika — najčešći bug.
- Transakciona sesija (`true`) zahteva `qs.commit()` posle send — bez toga poruka ostaje u baferu.
- Na ispitu: bez importova i try-catch (osim listenera).

## Provera pre predaje

```bash
javac -cp ../lib/javax.jms-api-2.0.1.jar -d . Sender.java Receiver.java
```
