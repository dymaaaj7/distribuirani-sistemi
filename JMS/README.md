# JMS — Java Message Service

Ovaj folder sadrži materijale vezane za **JMS** deo predmeta Distribuirani Sistemi: vežbe sa časova, laboratorijske zadatke, rešenja ispitnih zadataka (blanketa) i pomoćne biblioteke.

---

## 📁 Struktura

```
JMS/
├── README.md                    # Ovaj fajl
├── beleske.md                   # Kratke beleške za ispit (transakcije, sintaksa)
├── lib/
│   └── javax.jms-api-2.0.1.jar  # JMS API biblioteka
├── Vezbe/                       # Vežbe sa časova
│   ├── MOM_JMS.md              # Teorijski rezime: MOM, broker, P2P, Pub/Sub, JMS API
│   ├── JMS_07/                 # Osnovni JMS API
│   │   ├── Primer_1/           # Queue + Topic (General API)
│   │   ├── Primer_2/           # Point-to-Point (Queue)
│   │   └── Primer_3/           # Publish/Subscribe (Topic) sa transakcijama
│   ├── JMS_08/                 # Centralizovana arhitektura (placeholder)
│   │   └── Centralizovana/
│   └── JMS_09/                 # Necentralizovana arhitektura — lekar/sestra
│       └── Necentralizovana/
│           ├── Lekar.java
│           ├── Sestra.java
│           └── SestraML.java
├── Lab/                         # Laboratorijske vežbe
└── Blanketi/                    # Rešenja ispitnih zadataka (placeholder)
```

---

## 📚 Pregled vežbi

### Primer 1 — JMS osnovni API (Queue + Topic)

Demonstracija osnovnih JMS koncepta (connection, session, message, listener) kroz **oba komunikaciona modela** — i Point-to-Point (`Queue`) i Publish/Subscribe (`Topic`).

| Fajl | Opis |
|------|------|
| [`Producer.java`](Vezbe/JMS_07/Primer_1/Producer.java) | JMS producent koji šalje identične poruke i na `queue` i na `topic` istovremeno. |
| [`Subscriber.java`](Vezbe/JMS_07/Primer_1/Subscriber.java) | Kreira `MessageConsumer` za **oba** `queue` i `topic`, postavlja `MessageListener` na svaki, i istovremeno prima poruke sa oba izvora. |
| [`MsgListener.java`](Vezbe/JMS_07/Primer_1/MsgListener.java) | Reusable `MessageListener` implementacija za asinhrono prihvatanje JMS poruka (označava izvor — Queue ili Topic). |

### Primer 2 — Point-to-Point (Queue)

Direktna komunikacija zasnovana na redovima (queue). Svaka poruka se obrađuje tačno jednom.

| Fajl | Opis |
|------|------|
| [`Sender.java`](Vezbe/JMS_07/Primer_2/Sender.java) | JMS pošiljalac koji šalje 10 tekstualnih poruka u queue. |
| [`Receiver.java`](Vezbe/JMS_07/Primer_2/Receiver.java) | JMS primalac koji izvlači 10 poruka iz queue-a i ispisuje ih. |

### Primer 3 — Publish/Subscribe (Topic) sa transakcijama

Pub/Sub model zasnovan isključivo na `Topic` sa Topic-specifičnim API-jem (`TopicConnection`, `TopicSession`, `TopicPublisher`, `TopicSubscriber`). Sesija je **transakciona** — poruke se šalju u batch-u i potvrđuju eksplicitnim `commit()`.

| Fajl | Opis |
|------|------|
| [`Publisher.java`](Vezbe/JMS_07/Primer_3/Publisher.java) | Topic publisher koji šalje 10 poruka, svaku dva puta, a zatim izvršava `ts.commit()` da potvrdi transakciju. |
| [`Subscriber.java`](Vezbe/JMS_07/Primer_3/Subscriber.java) | Topic subscriber koji se pretplaćuje na topic i postavlja `MessageListener` za asinhrono prihvatanje. |
| [`MsgListener.java`](Vezbe/JMS_07/Primer_3/MsgListener.java) | `MessageListener` implementacija za ispis primljenih tekstualnih poruka. |

### JMS_08 — Centralizovana arhitektura

*U izradi...* — folder `JMS_08/Centralizovana/` rezervisan za centralizovane JMS primere.

### JMS_09 — Necentralizovana arhitektura (Lekar / Sestra)

Primer necenzurisane (decentralizovane) komunikacije između lekara i sestre preko dva queue-a (`qObevestiLekara` i `qStampa`). Sestra obaveštava lekara da je pacijent stigao; lekar prihvata pacijenta i šalje potvrdu nazad.

| Fajl | Opis |
|------|------|
| [`Sestra.java`](Vezbe/JMS_09/Necentralizovana/Sestra.java) | Glavna klasa sestre. Šalje poruke na `qObevestiLekara` sa `StringProperty("Lekar", ...)` i sluša odgovore na `qStampa` preko `SestraML` listener-a. Koristi transakcionu sesiju (`true`, `AUTO_ACKNOWLEDGE`). |
| [`SestraML.java`](Vezbe/JMS_09/Necentralizovana/SestraML.java) | `MessageListener` implementacija za ispis tekstualnih poruka koje stižu sestri. |
| [`Lekar.java`](Vezbe/JMS_09/Necentralizovana/Lekar.java) | Lekar se pretplaćuje na `qObevestiLekara` (sa mogućnošću filtera po lekaru), prima pacijenta i šalje potvrdu na `qStampa`. Koristi transakcionu sesiju. |

---

## 🧪 Laboratorijske vežbe

*U izradi...*

---

## 📝 Blanketi — Ispitni zadaci

*U izradi...*

---

## 📖 Teorija i reference

| Fajl | Opis |
|------|------|
| [`Vezbe/MOM_JMS.md`](Vezbe/MOM_JMS.md) | Rezime teorije: MOM, broker, sinhrona/asinhrona komunikacija, P2P vs Pub/Sub, JMS API interfejsi, zaglavlje/propertiji/payload, Message Selectors. |
| [`beleske.md`](beleske.md) | Kratke beleške za ispit — šta se ne piše (importi, try-catch), ponašanje `createSession(true/false, ...)`. |

---

## 📝 Napomene

- Biblioteka [`lib/javax.jms-api-2.0.1.jar`](lib/javax.jms-api-2.0.1.jar) je potrebna za kompilaciju i izvršavanje JMS programa.
- Primeri koriste GlassFish/Payara `jms/__defaultConnectionFactory` i JNDI lookup za `queue` / `topic` resurse.
- Na ispitu **ne treba pisati importove** ni **try-catch** blokove.
- `createSession(true, ...)` zahteva ručan `qs.commit()` prilikom slanja; `createSession(false, ...)` šalje automatski.
