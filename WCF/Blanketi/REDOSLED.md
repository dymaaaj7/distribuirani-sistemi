# WCF Blanketi — Redosled vežbanja

> Strategija: **skorašnji rokovi imaju prednost** (indikator šta se trenutno pojavljuje), pa onda starija ponavljanja po učestalosti. **Jedan predstavnik po porodici — replike preskaču** (SABLONI.md tek kad se dodaju implementacije).

## Glavni redosled

| # | Porodica | Rokovi (svi) | Napomena |
| --- | --- | --- | --- |
| 1 | 🕒 Tačno vreme (2×) | Sept 2026, Jun 2 2026 | početna vežba — trenutno vreme + brojač poziva po sesiji (kao vežba KolikoJeSati iz WCF_10) |
| 2 | 🧮 Kalkulator (4×) | Jun 2026, Apr 2026, Okt 2 2025, Jan 2025 | full-duplex, callback šalje izraz; Jan 2025 i Okt 2 2025 su replike Jun 2026 |
| 3 | 💬 Čet (4×) | Jun 2025, Sept 2025, Apr 2025, Jun 2023 | duplex, registracija nadimka, slanje po nadimku; prikaz ko/kad/sadržaj |
| 4 | 🔢 Matrice (3×) | Okt 2025, Okt 2 2023, Jan 2023 | server-side stanje + zajednička Result struktura (flag greške, opis, rezultat); Jan 2023 je loše skenirana kopija |
| 5 | 🚗 Registracija vozila (2×) | Sept 2024, Okt 2024 | vlasnik + entitet + istorija; broj registracije dodeljuje server |
| 6 | 🏗️ Zakup skladišta (1×) | Jun 2024 | ista šema kao vozila (vlasnik + entitet + istorija + liste) |
| 7 | 🏭 Fabrika sokova (3×) | Apr 2023, Jun 2022, Apr 2022 | stateful mikser — gustina se računa kao ponder po zapreminama |
| 8 | 🗺️ Parcele (1×) | Jun 2 2023 | poligon granica → računanje površine, filtriranje po arima |
| 9 | 📝 Prijava ispita (1×) | Sept 2023 | prijava/odjava + liste + brojači; lakša varijanta "prijavne" šeme |
| 10 | 🎓 Turnir (1×) | Okt 2023 | final boss — prijava/odjava + grupe od 16 sa rebalansiranjem po proseku |

## Delta vežbe (brze, posle odgovarajućeg glavnog)

- posle #1 → **Jun 2 2026**: isto tačno vreme — brza provera bez ponovnog pisanja
- posle #2 → **April 2026**: isti kalkulator, varijanta **bez duplex-a** + prosleđivanje izuzetka klijentu (FaultException)
- posle #3 → **April 2025**: isti čet, varijanta gde re-registracija nadimka **poništava** staru sesiju (Jun 2025: stara sesija i dalje važi)
- posle #3 → **Jun 2023**: čet + broadcast rezervisana reč `SVI` (samo aktivni) + istorija poruka za period (DateTime from/to)
- posle #2 → **Oktobar 2 2025**: isti kalkulator — brza provera bez ponovnog pisanja
- posle #4 → **Januar 2023** i **Oktobar 2 2023**: iste matrice — brza provera bez ponovnog pisanja
- posle #7 → **April 2022** i **Jun 2022**: isti sokovi — brza provera bez ponovnog pisanja

## Mapa porodica (22 roka = 10 zadataka)

| Porodica | Rokovi | Ključni mehanizam |
| --- | --- | --- |
| 🕒 Tačno vreme (2×) | Jun 2 2026, Sept 2026 | sesijski brojač poziva po korisniku (per-session state) |
| 🧮 Kalkulator (4×) | Jan 2025, Okt 2 2025, Apr 2026, Jun 2026 | sesijsko stanje, duplex callback uz izraz / FaultException varijanta |
| 💬 Čet (4×) | Jun 2023, Apr 2025, Jun 2025, Sept 2025 | duplex, rečnik nadimak→callback, usmeravanje poruke primaocu |
| 🔢 Matrice (3×) | Jan 2023, Okt 2 2023, Okt 2025 | server-side stanje (setovana matrica) + zajednička Result struktura |
| 🚗 Registracija vozila (2×) | Sept 2024, Okt 2024 | vlasnik + vozilo + istorija registracija, server dodeljuje broj |
| 🏗️ Zakup skladišta (1×) | Jun 2024 | vlasnik + skladište + istorija zakupa, liste aktivnih |
| 🏭 Fabrika sokova (3×) | Apr 2022, Jun 2022, Apr 2023 | stateful mikser, gustina kao ponder zapremina sastojaka |
| 🗺️ Parcele (1×) | Jun 2 2023 | poligon → površina parcele, filtriranje po zadatim arima |
| 📝 Prijava ispita (1×) | Sept 2023 | prijava/odjava po roku, liste i brojači, povratna info/greška |
| 🎓 Turnir (1×) | Okt 2023 | prijava/odjava + balansiranje grupa (16) po proseku, rebalans |

Napomena: porodice 1–4 pokrivaju 13 od 22 roka (~59%), 1–6 čak 18/22 (~82%), a sa sokovima 21/22 (95%). Ako se nađu stariji tekstovi (G2021 i ranije), mapa se dopunjuje.
