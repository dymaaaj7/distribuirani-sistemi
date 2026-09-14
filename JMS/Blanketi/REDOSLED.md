# JMS Blanketi — Redosled vežbanja

> Strategija: **skorašnji rokovi imaju prednost** (indikator šta se trenutno pojavljuje), pa onda starija ponavljanja po učestalosti. **Jedan predstavnik po porodici — replike preskaču** (vide [SABLONI.md](SABLONI.md)).

## Glavni redosled

| # | Porodica | Rokovi (svi) | Napomena |
| --- | --- | --- | --- |
| 1 | 📦 Aplikacija za obavestenja (3×) | Jun 2026, Apr 2026, Jun 2025 | početna vežba — urađena |
| 2 | 💣 Minesweeper (1×) | Okt 2025 | najnoviji jedinstveni — jak signal; multi-instanca, selector po koordinatama, agregacija odgovora |
| 3 | 🌡️ Temperatura (3×) | Apr 2025, Sept 2023, Okt 2023 | selector + više kanala + skica kanala; Okt 2023 je varijanta sa "dopuniti" |
| 4 | 🎓 Ispitivanje (2×) | Jan 2025, Sept 2024 | funkcije u tekstu (Pokreni, PosaljiPitanje), uloge nastavnik/student |
| 5 | 📧 Mail (3×) | Jun 2023, Okt 2 2023, Jan 2022 | topic obavezan + više primalaca + JMSTimestamp; Jan 2022 je durable varijanta |
| 6 | 🏭 Lanac stanica (3×) | Jan 2023, Dec 2022, Okt 2 2022 | ObjectMessage sa menjanjem stanja, prosleđivanje lancem |
| 7 | 🎯 Prodavnica (3×) | Jun 2 2022, Jun 2022, Apr 2022 | selector majstorija (AND/BETWEEN), property uz ObjectMessage |
| 8 | 🏭 Pomoć-u-mreži (1×) | Apr 2023 | necentralizovana mreža, funkcija "pomoć", sve async |
| 9 | 🔀 Merge sort (2×) | Okt 2022, Jun 2 2023 | final boss — rekurzija porukama, agregacija dva dela |

## Delta vežbe (brze, posle odgovarajućeg glavnog)

- posle #3 → **Oktobar 2023**: ista temperatura, "dopuniti" varijanta (bez slanja zahteva)
- posle #5 → **Januar 2022**: isti mail, durable varijanta (`createDurableSubscriber` + `tc.setClientID`)

## Mapa porodica (21 rok = 9 zadataka)

| Porodica | Rokovi | Ključni mehanizam |
| --- | --- | --- |
| 🏭 Lanac stanica (3×) | Okt 2 2022, Dec 2022, Jan 2023 | ObjectMessage menja stanje, prosleđuje se lancem |
| 🎯 Prodavnica (3×) | Apr 2022, Jun 2022, Jun 2 2022 | selector + AND/BETWEEN + property uz objekat |
| 🌡️ Temperatura (3×) | Okt 2023, Sept 2023, Apr 2025 | selector po lokaciji, odlučivanje klijenta |
| 📧 Mail (3×) | Jan 2022, Jun 2023, Okt 2 2023 | topic, više primalaca, durable/non-durable varijante |
| 📦 Aplikacija za obavestenja (3×) | Jun 2025, Jun 2026, Apr 2026 | sender/receiver + MapMessage |
| 🎓 Ispitivanje (2×) | Sept 2024, Jan 2025 | funkcije u tekstu, uloge nastavnik/student |
| 🏭 Pomoć-u-mreži (1×) | Apr 2023 | topic + selector po tipu posla, sve async |
| 💣 Minesweeper (1×) | Okt 2025 | multi-instanca + agregacija odgovora brojačem |
| 🔀 Merge sort (2×) | Okt 2022, Jun 2 2023 | rekurzija porukama + agregacija dva dela |

Napomena: porodice 1–6 pokrivaju 17 od 21 rokova (~81%). Folderi G2019, G2020 i G2021 postoje ali su prazni — ako se nađu tekstovi, mapa se dopunjuje.
