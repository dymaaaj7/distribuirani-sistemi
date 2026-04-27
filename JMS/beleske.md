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

## Korisni linkovi

- [Nazad na JMS README](README.md)
- [Teorija — MOM i JMS rezime](Vezbe/MOM_JMS.md)
