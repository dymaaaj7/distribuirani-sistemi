# JMS — Kratke beleške za ispit

> Brzi podsjetnik za pisanje JMS koda na ispitu.

---

## Šta se NE piše na ispitu

- **Ne treba pisati importove**
- **Ne treba pisati try-catch** blokove

---

## Transakcije u `createSession`

```java
// Transakciona sesija — poruke se šalju eksplicitno commit-om
QueueSession qs = qc.createQueueSession(true, Session.AUTO_ACKNOWLEDGE);

sender.send(msg);
qs.commit();  // obavezno pozvati da poruka ode

//u sustini ovo je kao da sa send stavljamo u neki bafer, a kadase commituju
//tek onda se posalju sve poruke zajedno
```

```java
// Netransakciona sesija — poruka se šalje automatski
QueueSession qs = qc.createQueueSession(false, Session.AUTO_ACKNOWLEDGE);

sender.send(msg);  // nema potrebe za commit
```

| Prvi parametar | Ponašanje |
|----------------|-----------|
| `true` | Transakciona sesija — `qs.commit()` je **obavezan** prilikom slanja poruke |
| `false` | Netransakciona sesija — poruka se šalje **automatski** čim se stavi na slanje |

---

## Queue ili topic? (prva odluka u svakom zadatku)

| | **queue** | **topic** |
| --- | --- | --- |
| Ko dobija poruku | **tačno jedan** primalac (load balancing među onima koji prođu filter) | **svi aktivni pretinerci** dobijaju kopiju |
| Poruka čeka neaktivne? | ✅ čeka u queue-u | ❌ propuštaju je (osim durable) |
| Tipičan tekst zadatka | „šalje **zahtev**", „**samo jedan** obrađuje", „isti korisnik" | „**svi** korisnici", „obaveštava **sve**", „svi prisutni" |

- Više primalaca na istom queue-u **sa istim selectorom** = load balancing → poruku dobija jedan od njih.
  Ako su „isti korisnik" (Jun 2 2026) — to je tačno ono što se traži!
- Durable varijanta (`createDurableSubscriber` + `tc.setClientID`) postoji **samo za topic** — poruke čekaju neaktivne (Mail, Jan 2022).

### API asimetrija (kompajlerske zamke)

| | queue | topic |
| --- | --- | --- |
| primač | `qs.createReceiver(q, selector)` — 2 arg | `ts.createSubscriber(t, selector, false)` — **3 arg** (`noLocal`) |
| pošiljalac | `sender.send(msg)` | `publisher.publish(msg)` |
| sesija poruke | poruku pravi sesija kanala na koji šalješ (`qs.createTextMessage()` za queue slanje, `ts...` za topic) | |

---

## Slanje vs primanje — gde šta ide (najčešća zbrka)

```java
// FUNKCIJA ZA SLANJE: create + set + send
TextMessage msg = qs.createTextMessage();
msg.setText(tekst);
msg.setStringProperty("Ime", ime);   // property = JEDINO po čemu selector filtrira
sender.send(msg);

// LISTENER (onMessage): cast + get + ispis — poruku NIKAD ne praviš ovde, ona stiže kao parametar
TextMessage msg = (TextMessage) message;
String t = msg.getText();
```

---

## Korisni linkovi

- [Nazad na JMS README](README.md)
- [Teorija — MOM i JMS rezime](Vezbe/MOM_JMS.md)
