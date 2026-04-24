# Distribuirani Sistemi — Materijali sa vežbi i ispita

Ovaj repozitorijum sadrži materijale, vežbe, laboratorijske zadatke i ispitna rešenja iz predmeta **Distribuirani Sistemi**.

Fokus je na pismenom delu ispita i obuhvata više tehnologija: **MPI**, **JMS**, i **WCF**.

---

## 📁 Struktura projekta

```
.
├── MPI/                          # MPI vežbe, labovi i blanketi
│   ├── README.md                 # Detaljan opis MPI dela
│   ├── SPISAKFUNKCIJA.md         # Referenca MPI funkcija
│   ├── template.c                # Šablon za MPI programe
│   ├── Vezbe/
│   ├── Lab/
│   └── Blanketi/
│
├── JMS/                          # JMS vežbe, labovi i blanketi
│   ├── README.md                 # Detaljan opis JMS dela
│   ├── lib/
│   ├── Vezbe/
│   ├── Lab/
│   └── Blanketi/
│
├── WCF/                          # WCF vežbe, labovi i blanketi (placeholder)
│   └── README.md
│
├── ispit-zadaci.md               # Praćenje odrađenih zadataka sa ispita po rokovima
├── Distribuirani Sistemi_PrezentacijeSpojene.pdf
└── .gitignore
```

---

## 🚀 Brzi linkovi

| Tehnologija | Opis |
| ------------- | ------ |
| [MPI](MPI/README.md) | Paralelno programiranje sa Message Passing Interface |
| [JMS](JMS/README.md) | Java Message Service — asinhrona komunikacija |
| [WCF](WCF/README.md) | Windows Communication Foundation (placeholder) |
| [Praćenje zadataka](ispit-zadaci.md) | Evidencija odrađenih zadataka po ispitnim rokovima |

---

## 📝 Napomene

- Folder `MPI/Lab/PS-NP-Lab1PTP` sadrži implementaciju zadataka sa prve laboratorijske vežbe sa predmeta Paralelni Sistemi po **novoj akreditaciji** i pomenuti zadaci odgovaraju prvom terminu MPI vežbi na predmetu Distribuirani Sistemi.
- Kod nekih vežbi i zadataka **nedostaje implementacija** — ostavljeni su samo komentari sa tekstom zadatka.
- Fajl [`MPI/template.c`](MPI/template.c) sadrži osnovni šablon za MPI programe sa inicijalizacijom, dobijanjem ranga i veličine komunikatora, kao i finalizacijom.

---

*Autor: Dimitrije Janković*
