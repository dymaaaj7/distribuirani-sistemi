# WCF — Windows Communication Foundation

## InstanceContextMode

Određuje kako se kreira i održava instanca servisa.

### PerCall (podrazumevano)

```csharp
[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerCall)]
```

- Instanca servisa se kreira **prilikom svakog poziva metoda** od strane klijenta.
- Nakon završetka poziva, instanca se uništava.
- Bez stanja između poziva.

### PerSession

```csharp
[ServiceBehavior(InstanceContextMode = InstanceContextMode.PerSession)]
```

- Instanca se kreira **prilikom uspostavljanja sesije** i traje koliko i sesija.
- Svaki klijent ima svoju instancu.
- Ako pokrenemo 3 klijenta, svaki ima svoj životni vek instance.
- **Zahteva binding koji podržava sesije** (npr. `NetTcpBinding`, `WSHttpBinding` sa sigurnošću, `NetNamedPipeBinding`).

### Single

```csharp
[ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)]
```

- Jedna instanca za **sve klijente** — traje koliko i sam servis.
- Svi klijenti dele istu instancu i isto stanje.
- Pogodno za singleton servise ili kada je potrebno deljenje stanja među klijentima.

---

## SessionMode (na ServiceContract)

`SessionMode` se definiše na **interfejsu** (`[ServiceContract]`), a ne na klasi servisa. Određuje da li servis zahteva, dozvoljava ili zabranjuje sesije.

```csharp
[ServiceContract(SessionMode = SessionMode.Allowed)]
public interface IKolikoJeSati
{
    [OperationContract]
    DateTime VratiVreme();
}
```

| Vrednost | Značenje |
|----------|----------|
| `SessionMode.NotAllowed` | Sesije su **zabranjene**. Servis neće uspostaviti sesiju ni sa jednim bindingom. |
| `SessionMode.Allowed` | Sesije su **dozvoljene** ako binding podržava sesije. |
| `SessionMode.Required` | Sesije su **obavezne**. Servis će baciti grešku ako binding ne podržava sesije. |

> **Važno:** Ako je `InstanceContextMode.PerSession`, a `SessionMode.NotAllowed`, servis neće raditi ispravno jer ne može da kreira sesiju. Uvek koristi `Allowed` ili `Required` sa `PerSession`.

---

## Primer iz vežbi (WCF_10)

### Interfejs — definicija ugovora

```csharp
using System;
using System.Runtime.Serialization;
using System.ServiceModel;

namespace WcfKolikoJeSati
{
    [ServiceContract(SessionMode = SessionMode.Allowed)]
    public interface IKolikoJeSati
    {
        [OperationContract]
        DateTime VratiVreme();

        [OperationContract]
        PristupLog ZadnjiPristup();

        [OperationContract]
        decimal ObaviOperaciju(decimal op1, decimal op2, string op);
    }

    [DataContract]
    public class PristupLog
    {
        [DataMember]
        public DateTime Timestamp { get; set; }

        [DataMember]
        public string HostName { get; set; }
    }
}
```

### Implementacija servisa

```csharp
using System;
using System.ServiceModel;

namespace WcfKolikoJeSati
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)]
    public class KolikoJeSati : IKolikoJeSati
    {
        DateTime pristupio = DateTime.Now;

        public PristupLog ZadnjiPristup()
        {
            return new PristupLog()
            {
                HostName = Environment.MachineName,
                Timestamp = pristupio,
            };
        }

        public DateTime VratiVreme()
        {
            pristupio = DateTime.Now;
            return DateTime.Now;
        }

        public decimal ObaviOperaciju(decimal a, decimal b, string op)
        {
            switch (op)
            {
                case "+": return a + b;
                case "-": return a - b;
                case "*": return a * b;
                case "/":
                    if (b == 0)
                        throw new Exception("Deljenje nulom");
                    return a / b;
                default:
                    throw new Exception("Nepoznata operacija.");
            }
        }
    }
}
```

### Šta se dešava u primeru?

- `SessionMode.Allowed` na interfejsu → sesije su dozvoljene, ali nisu obavezne.
- `InstanceContextMode.Single` na servisu → **jedna instanca za sve klijente**.
- Polje `pristupio` se deli među svim klijentima — svaki poziv `VratiVreme()` ažurira isto polje.

---

## Brzi pregled kombinacija

| `InstanceContextMode` | `SessionMode` | Rezultat |
|----------------------|---------------|----------|
| `PerCall` | `NotAllowed` | ✅ Svaki poziv = nova instanca. Bez stanja. |
| `PerCall` | `Allowed` | ✅ Svaki poziv = nova instanca (sesija ne utiče). |
| `PerCall` | `Required` | ✅ Svaki poziv = nova instanca unutar sesije. |
| `PerSession` | `NotAllowed` | ❌ **Nemoguće** — nema sesije, nema instance po sesiji. |
| `PerSession` | `Allowed` | ✅ Instanca po sesiji, ako binding podržava. |
| `PerSession` | `Required` | ✅ Instanca po sesiji, obavezno. |
| `Single` | bilo koji | ✅ Jedna instanca za sve. Sesija ne utiče na kreiranje instance. |

---

## Napomena: ConcurrencyMode

Pored `InstanceContextMode`, postoji i `[ServiceBehavior(ConcurrencyMode = ...)]` koji određuje da li više niti može istovremeno da pristupa istoj instanci servisa.

- `ConcurrencyMode.Single` (podrazumevano) — jedna nit po instanci.
- `ConcurrencyMode.Multiple` — više niti može istovremeno da pristupa.
- `ConcurrencyMode.Reentrant` — nit može da "izađe" i ponovo uđe (npr. poziv drugog servisa).

> Na faksu trenutno nije obrađeno, ali je dobro znati da postoji kao dodatni sloj kontrole.
