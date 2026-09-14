# JMS Blanketi — Redosled vežbanja

> Strategija: **skorašnji rokovi imaju prednost** (indikator šta se trenutno pojavljuje), pa onda starija ponavljanja po učestalosti. **Jedan predstavnik po porodici — replike preskaču** (vide [SABLONI.md](SABLONI.md)).

## Glavni redosled

| # | Porodica | Rokovi (svi) | Napomena |
| --- | --- | --- | --- |
| 1 | 📦 Aplikacija za obavestenja (3×) | Jun 2026, Apr 2026, Jun 2025 | početna vežba — urađena |
| 2 | 💬 Razmena poruka (1×) | Jun 2 2026 | najnoviji jedinstveni — jak signal; usmeravanje po imenu, više klijenata istog imena = isti korisnik (JMS verzija četa) |
| 3 | 💣 Minesweeper (1×) | Okt 2025 | jak signal; multi-instanca, selector po koordinatama, agregacija odgovora |
| 4 | 🌡️ Temperatura (4×) | Apr 2025, Okt 2 2025, Sept 2023, Okt 2023 | urađena (Apr/Okt 2 2025, Sept 2023); selector + više kanala + skica kanala; Okt 2023 je delta varijanta sa "dopuniti" (bez qAkcija) |
| 5 | 🎓 Ispitivanje (4×) | Jan 2025, Sept 2025, Sept 2024, Okt 2024 | funkcije u tekstu (Pokreni, PosaljiPitanje), uloge nastavnik/student |
| 6 | 📧 Mail (3×) | Jun 2023, Okt 2 2023, Jan 2022 | topic obavezan + više primalaca + JMSTimestamp; Jan 2022 je durable varijanta |
| 7 | 🏭 Lanac stanica (3×) | Jan 2023, Dec 2022, Okt 2 2022 | ObjectMessage sa menjanjem stanja, prosleđivanje lancem |
| 8 | 🎯 Prodavnica (3×) | Jun 2 2022, Jun 2022, Apr 2022 | selector majstorija (AND/BETWEEN), property uz ObjectMessage |
| 9 | 🏭 Pomoć-u-mreži (1×) | Apr 2023 | necentralizovana mreža, funkcija "pomoć", sve async |
| 10 | 🔀 Merge sort (2×) | Okt 2022, Jun 2 2023 | final boss — rekurzija porukama, agregacija dva dela |

## Delta vežbe (brze, posle odgovarajućeg glavnog)

- posle #4 → **Oktobar 2023**: ista temperatura, "dopuniti" varijanta (bez slanja zahteva)
- posle #6 → **Januar 2022**: isti mail, durable varijanta (`createDurableSubscriber` + `tc.setClientID`)

## Mapa porodica (25 rokova = 10 zadataka)

| Porodica | Rokovi | Ključni mehanizam |
| --- | --- | --- |
| 🏭 Lanac stanica (3×) | Okt 2 2022, Dec 2022, Jan 2023 | ObjectMessage menja stanje, prosleđuje se lancem |
| 🎯 Prodavnica (3×) | Apr 2022, Jun 2022, Jun 2 2022 | selector + AND/BETWEEN + property uz objekat |
| 🌡️ Temperatura (4×) | Okt 2023, Sept 2023, Apr 2025, Okt 2 2025 | selector po lokaciji, odlučivanje klijenta |
| 📧 Mail (3×) | Jan 2022, Jun 2023, Okt 2 2023 | topic, više primalaca, durable/non-durable varijante |
| 📦 Aplikacija za obavestenja (3×) | Jun 2025, Jun 2026, Apr 2026 | sender/receiver + MapMessage |
| 💬 Razmena poruka (1×) | Jun 2 2026 | usmeravanje po imenu (selector), više primalaca istog imena = isti korisnik |
| 🎓 Ispitivanje (4×) | Sept 2024, Okt 2024, Jan 2025, Sept 2025 | funkcije u tekstu, uloge nastavnik/student |
| 🏭 Pomoć-u-mreži (1×) | Apr 2023 | topic + selector po tipu posla, sve async |
| 💣 Minesweeper (1×) | Okt 2025 | multi-instanca + agregacija odgovora brojačem |
| 🔀 Merge sort (2×) | Okt 2022, Jun 2 2023 | rekurzija porukama + agregacija dva dela |

Napomena: porodice 1–7 pokrivaju 19 od 25 rokova (76%), a 1–8 čak 22/25 (88%). Folderi G2019, G2020 i G2021 postoje ali su prazni — ako se nađu tekstovi, mapa se dopunjuje.
