# JMS — Java Message Service

Ovaj folder sadrži materijale vezane za **JMS** deo predmeta Distribuirani Sistemi: vežbe sa časova, laboratorijske zadatke, rešenja ispitnih zadataka (blanketa) i pomoćne biblioteke.

---

## 📁 Struktura

```
JMS/
├── README.md                    # Ovaj fajl
├── lib/
│   └── javax.jms-api-2.0.1.jar  # JMS API biblioteka
├── Vezbe/                       # Vežbe sa časova
│   └── JMS_07/
│       ├── Primer_1/            # JMS osnovni API (Queue + Topic)
│       │   ├── Producer.java
│       │   ├── Subscriber.java
│       │   └── MsgListener.java
│       └── Primer_2/            # P2P model (Queue)
│           ├── Sender.java
│           └── Receiver.java
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

---

## 🧪 Laboratorijske vežbe

*U izradi...*

---

## 📝 Blanketi — Ispitni zadaci

*U izradi...*

---

## 📝 Napomene

- Biblioteka [`lib/javax.jms-api-2.0.1.jar`](lib/javax.jms-api-2.0.1.jar) je potrebna za kompilaciju i izvršavanje JMS programa.
- Primeri koriste GlassFish/Payara `jms/__defaultConnectionFactory` i JNDI lookup za `queue` / `topic` resurse.
