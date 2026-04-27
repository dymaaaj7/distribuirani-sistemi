# Distribuirani sistemi — MOM i JMS: Rezime za ispit

---

## 1. MOM (Message-Oriented Middleware)

MOM je softverski sloj koji se nalazi između distribuiranih aplikacija i omogućava im da komuniciraju razmenom poruka, bez direktnog pozivanja jednih od strane drugih.

### Zašto MOM?

Direktna komunikacija između komponenti u distribuiranom sistemu ima ozbiljne probleme:

- **Vremenska zavisnost (temporal coupling)** — pošiljalac i primalac moraju biti aktivni u isto vreme. Ako primalac padne, poruka se gubi.
- **Prostorna zavisnost (spatial coupling)** — pošiljalac mora znati mrežnu adresu primaoca. Svaka promena na strani primaoca zahteva promenu na strani pošiljaoca.
- **Skalabilnost** — jedan endpoint ne može da opsluži hiljade pozivalaca odjednom.

MOM rešava sve ove probleme uvođenjem **brokera** kao posrednika.

### Broker

Broker je srce svakog MOM sistema — konkretna softverska implementacija koja:

- prima poruke od producera i upisuje ih u queue/topic
- čuva poruke na disku dok consumer ne postane dostupan (persistence)
- rutira poruke do pravih destinacija
- upravlja potvrđivanjem prijema (acknowledgement)
- vodi računa o pretplatnicima na topic-ima

> **Analogija:** Broker je kao pošta. Ti (producer) predaš pismo, pošta ga čuva i dostavlja primaocu (consumer) — bez obzira da li je primalac kod kuće u trenutku predaje.

Primeri popularnih brokera: **Apache ActiveMQ**, **IBM MQ**, **RabbitMQ**, **Apache Kafka**.

---

## 2. Modeli komunikacije

### Sinhrona komunikacija

- Oba učesnika moraju biti aktivna istovremeno
- Pošiljalac blokira i čeka potvrdu od primaoca
- Blokirajući pozivi
- Pogodan kada je autorizacija obavezna pre nastavka

**Primer:** Plaćanje kreditnom karticom — sistem mora dobiti potvrdu odobrenja *pre* nego što transakcija prođe. Ne može biti asinhrono.

### Asinhrona komunikacija

- Oba učesnika ne moraju biti aktivna istovremeno
- Pošiljalac šalje poruku i nastavlja dalje bez čekanja
- Nema blokirajućih poziva
- Efikasnije korišćenje hardverskih resursa
- Pogodno za masovnu obradu poruka

**Primer:** Slanje emaila — pošiljalac ne čeka dok primalac ne pročita.

---

## 3. Point-to-Point (P2P)

Komunikacija se odvija kroz **queue** (red čekanja).

- Poruku šalje jedan sender, prima tačno **jedan** consumer — čak i ako više consumera čita iz istog reda
- Poruka ostaje u redu dok je neko ne preuzme (i ako je persistent, preživljava pad brokera)
- Sender obično zna ko je receiver i šta on očekuje
- Servisi su **jače spregnuti** u odnosu na Pub/Sub
- Podržava i sinhron i asinhron način rada

**Kada koristiti P2P:** Kada svaki zahtev treba da obradi tačno jedan servis — npr. obrada narudžbine, slanje emaila, obrada plaćanja.

---

## 4. Publish-and-Subscribe (Pub/Sub)

Komunikacija se odvija kroz **topic**.

- Publisher objavi poruku na topic
- **Svi subscriberi** koji su pretplaćeni na taj topic dobijaju svoju kopiju poruke — broadcast, komunikacija 1-prema-više
- Publisher ne zna ko su subscriberi, niti koliko ih ima
- Servisi su **slabije spregnuti** u odnosu na P2P
- Novi subscriber može se dodati bez ikakve izmene publishera

**Kada koristiti Pub/Sub:** Kada isti događaj treba da obrade različite komponente — npr. pri uspešnoj kupovini: servis za emailove, servis za analitiku i servis za inventar svi dobijaju istu poruku.

---

## 5. JMS (Java Message Service)

JMS je **apstraktni API** — definiše interfejse za rad sa MOM sistemima iz Jave, ali ne propisuje kako broker interno funkcioniše.

> **Ključna rečenica:** JMS je apstraktni API, ne kompletan sistem. Možeš zameniti broker ispod bez menjanja aplikacionog koda.

Napravio ga Sun Microsystems. Analogija: kao što JDBC definiše kako pristupaš bazi podataka, ali konkretan driver (MySQL, PostgreSQL) daje implementaciju — JMS je isti princip za razmenu poruka.

### JMS API sadrži tri dela:

- **General API** — radi i sa queue i sa topic destinacijama
- **Point-to-Point API** — specifičan za queue komunikaciju
- **Publish-and-Subscribe API** — specifičan za topic komunikaciju

---

## 6. JMS General API — Glavni interfejsi

### ConnectionFactory

Fabrika za kreiranje konekcija ka brokeru. **Administered object** — administrator ga konfiguriše na brokeru, aplikacija ga pronalazi preko JNDI lookup-a. Ne kreira se ručno u kodu.

```java
ConnectionFactory cf = (ConnectionFactory) ctx.lookup("cf");
```

### Destination

Apstrakcija koja predstavlja Queue ili Topic. Takođe administered object — pronalazi se preko JNDI.

```java
Queue queue = (Queue) ctx.lookup("queue");
Topic topic = (Topic) ctx.lookup("topic");
```

### Connection

Fizička konekcija ka brokeru. Kreira se iz ConnectionFactory. Mora se eksplicitno startovati pre primanja poruka i zatvoriti na kraju.

```java
Connection cnx = cf.createConnection();
cnx.start();   // obavezno pre receive
cnx.close();   // obavezno na kraju
```

### Session

Jednonitnio okruženje za rad sa porukama. Iz sesije se kreiraju produceri, consumeri i poruke.

```java
Session sess = cnx.createSession(false, Session.AUTO_ACKNOWLEDGE);
```

`AUTO_ACKNOWLEDGE` — broker automatski prima potvrdu čim consumer preuzme poruku.

### MessageProducer

Šalje poruke na destinaciju.

```java
MessageProducer prod = sess.createProducer(null); // null = destination se navodi pri slanju
prod.send(queue, msg);
prod.send(topic, msg);
```

### MessageConsumer

Prima poruke sa destinacije. Može raditi:
- **sinhrono** — `receiver.receive()` blokira dok poruka ne stigne
- **asinhrono** — postaviš `MessageListener`, callback se poziva kad poruka stigne

```java
MessageConsumer recv = sess.createConsumer(queue);
recv.setMessageListener(new MsgListener("Queue"));
```

### Redosled u kodu (uvek isti):

```
JNDI lookup → ConnectionFactory → Connection → Session → Producer/Consumer → start() → send()/receive() → close()
```

---

## 7. Zaglavlje poruke (Message Header)

Svaka JMS poruka ima tri dela: **zaglavlje**, **propertiji** i **payload**. Zaglavlje je uvek prisutno.

Postoje dve grupe polja u zaglavlju, podeljene po tome ko ih setuje:

### Podrazumevano zaglavlje (setuje JMS automatski):

| Polje | Opis |
|---|---|
| `JMSDestination` | Odredište poruke — setuje JMS pri slanju |
| `JMSDeliveryMode` | `PERSISTENT` (upisuje na disk, preživljava pad brokera) ili `NON_PERSISTENT` (brže, ali se gubi pri padu) |
| `JMSMessageID` | Jedinstveni ID poruke, generiše broker |
| `JMSTimestamp` | Vreme slanja |

### Podešivo zaglavlje (setuje developer):

| Polje | Opis |
|---|---|
| `JMSPriority` | 0–4 normalne poruke, 5–9 hitne. Broker pokušava da isporuči hitnije prve. |
| `JMSReplyTo` | Destinacija na koju consumer treba da pošalje odgovor. Korisno za request-reply pattern. |
| `JMSType` | Opcionalni string tip poruke (npr. `"OrderMessage"`). Većina brokera ga ignoriše. |
| `JMSExpiration` | Vreme isteka poruke. Poruka se uklanja iz reda nakon isteka. |

---

## 8. Propertiji poruke (Message Properties)

Propertiji su dodatna zaglavlja — key-value parovi koji se prilapu uz poruku. Tri vrste:

### Aplikacijsko specifični
Developer ih definiše i setuje kako hoće. Koriste se za filtriranje na consumer strani.

```java
msg.setStringProperty("customerType", "premium");
msg.setIntProperty("orderValue", 500);
```

### JMS definisani
Standardizovani od strane JMS specifikacije, setuje ih provider automatski:
- `JMSXGroupID` — ID grupe poruka
- `JMSXGroupSeq` — redni broj poruke u grupi
- `JMSXUserID` — ko je poslao poruku
- `JMSXAppID` — koja aplikacija je poslala poruku

### Provider specifični
Broker dodaje svoje interne informacije. Nisu standardizovani između različitih brokera.

**Tipovi vrednosti propertija:** `String`, `boolean`, `byte`, `double`, `int`, `long`, `float`.

---

## 9. Payload (Teret poruke)

Stvarni sadržaj poruke. JMS definiše 6 tipova:

| Tip | Opis | Kada koristiti |
|---|---|---|
| `Message` | Prazna poruka, nema tela | Signalizacija događaja — "posao završen", "refresh cache" |
| `TextMessage` | String sadržaj | Plain text, JSON, XML — najčešće korišćen |
| `BytesMessage` | Sirovi niz bajtova | Binarne podatke, slike, fajlove |
| `StreamMessage` | Tok Java primitiva, čita se sekvencijalno | Streaming numeričkih podataka |
| `MapMessage` | Key-value mapa, ključevi su stringovi | Strukturirani podaci bez potrebe za serijalizacijom objekta |
| `ObjectMessage` | Serijalizovani Java objekat | Kompleksni Java objekti (mora implementovati `Serializable`) |

> **Važno:** Interfejs `Message` može se proširiti za podršku dodatnih tipova, npr. XML poruka.

---

## 10. Message Selectors (Filtriranje poruka)

Consumer ne mora da prima sve poruke sa destinacije — može da postavi filter i primati samo poruke koje zadovoljavaju određeni uslov.

### Sintaksa

Filter je string u **SQL-92 sintaksi** koji se prosleđuje pri kreiranju consumera:

```java
String filter = "customerType = 'premium' AND JMSPriority > 5";
TopicSubscriber subscriber = session.createSubscriber(topic, filter, true);
```

### Važna ograničenja

- Filter se piše nad **zaglavljem i propertijima** — **ne može se filtrirati po sadržaju payloada**
- Poređenje uvek vraća Boolean vrednost

### Podržani operatori

`=`, `>`, `<`, `>=`, `<=`, `<>`, `LIKE`, `BETWEEN`, `IN`, `NOT`, `IS NULL`

### Gde se može primeniti

Filter se može postaviti na: `QueueReceiver`, `QueueBrowser`, `TopicSubscriber`.

### Razlika Topic vs Queue filtriranje

**Topic:** Svaki subscriber ima svoj filter nezavisno. Ista poruka može biti prosleđena jednom subscriberu a blokirana drugom — ne utiče na ostale.

**Queue:** Kada consumer sa filterom preuzme poruku, ona se **uklanja iz reda** i više nije dostupna drugom consumeru. Ako nijedan consumer nema odgovarajući filter, poruka ostaje u redu. Poruke sa višim `JMSPriority` se filtriraju i isporučuju prve.

---

## Kratki pregled za ispit

| Pojam | Jedna rečenica |
|---|---|
| MOM | Softverski posrednik koji omogućava asinhronu razmenu poruka između distribuiranih komponenti |
| Broker | Konkretna implementacija MOM-a koja čuva, rutira i isporučuje poruke |
| JMS | Apstraktni Java API za rad sa MOM brokerima — ne kompletan sistem |
| Queue | P2P destinacija, poruku prima tačno jedan consumer |
| Topic | Pub/Sub destinacija, poruku primaju svi subscriberi |
| Persistent | Poruka se upisuje na disk i preživljava pad brokera |
| Message Selector | SQL-92 filter na consumer strani, nad zaglavljem i propertijima |
| Session | Jednonitnio okruženje iz kojeg se kreiraju produceri, consumeri i poruke |