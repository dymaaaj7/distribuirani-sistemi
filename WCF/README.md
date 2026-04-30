# WCF — Windows Communication Foundation

Ovaj folder sadrži materijale vezane za **WCF** deo predmeta Distribuirani Sistemi: vežbe sa časova, laboratorijske zadatke i rešenja ispitnih zadataka (blanketa).

> [← Nazad na root README](../README.md)

---

## 📁 Struktura

```
WCF/
├── README.md               # Ovaj fajl
├── beleske.md              # Beleške: InstanceContextMode, SessionMode, primeri
├── Vezbe/                  # Vežbe sa časova
│   ├── WCF_10/             # WCF 10 — Koliko je sati (Single / PerSession)
│   │   ├── WcfKolikoJeSati/
│   │   │   ├── IkolikoJeSati.cs   # ServiceContract interfejs
│   │   │   └── KolikoJeSati.cs    # Implementacija servisa
│   │   ├── WcfKolikoJeSatiClient/
│   │   │   └── Client.cs          # WCF klijent
│   │   └── CisternaClient/        # Placeholder
│   └── WCF_11/             # WCF 11 — (rezervisan)
├── Lab/                    # Laboratorijske vežbe (trenutno prazan)
└── Blanketi/               # Rešenja ispitnih zadataka (trenutno prazan)
```

---

## 📚 Pregled vežbi

### WCF_10 — Koliko je sati

Vežba pokazuje različite načine instanciranja WCF servisa i upotrebu DataContract/DataMember atributa.

#### Interfejs — ServiceContract

| Fajl | Opis |
|------|------|
| [`WcfKolikoJeSati/IKolikoJeSati.cs`](Vezbe/WCF_10/WcfKolikoJeSati/IKolikoJeSati.cs) | Definiše `ServiceContract` sa `SessionMode.Allowed` i `OperationContract` metodama. Sadrži `DataContract` klasu `PristupLog`. |
| [`WcfKolikoJeSati/KolikoJeSati.cs`](Vezbe/WCF_10/WcfKolikoJeSati/KolikoJeSati.cs) | Implementacija servisa sa `[ServiceBehavior(InstanceContextMode = InstanceContextMode.Single)]`. Jedna instanca za sve klijente — stanje (`pristupio`) se deli. |
| [`WcfKolikoJeSatiClient/Client.cs`](Vezbe/WCF_10/WcfKolikoJeSatiClient/Client.cs) | Klijentski program koji poziva `VratiVreme()`, `ZadnjiPristup()` i `ObaviOperaciju()`. |

---

## 🧪 Laboratorijske vežbe

*U izradi...*

---

## 📝 Blanketi — Ispitni zadaci

*U izradi...*

---

## 📖 Reference

| Fajl | Opis |
|------|------|
| [`beleske.md`](beleske.md) | Detaljne beleške: `InstanceContextMode` (PerCall / PerSession / Single), `SessionMode` (NotAllowed / Allowed / Required), primeri kombinacija, `ConcurrencyMode`. |

---

## 📝 Napomene

- WCF (Windows Communication Foundation) je Microsoft-ov framework za izgradnju servisno-orijentisanih aplikacija.
- Vežba `WCF_10` demonstrira osnovne koncepte: `ServiceContract`, `OperationContract`, `DataContract`, `DataMember`, `InstanceContextMode` i `SessionMode`.
