
README


INSTRUKTION

Förutsättningar:
- SDL3, SDL3_image och SDL3_ttf måste vara installerade på datorn.
- Projektet följer filstrukturen.

Bygga projektet:
- Kör kommandot: make

Köra projektet:
- Kör: build/debug/play eller titta i den mappen för .exe

Sökvägar till resursfiler:
- Alla bilder finns i resources/images/
- Alla ljudfiler finns i resources/sounds/ - Använde inga dock
- Alla typsnitt finns i: resources/fonts/ - Använde inga dock
- Sökvägar till resursfiler definieras i include/Constants.h, t.ex.


TESTBESKRIVNING

Syfte:
- Testa att spelet startar och att alla resurser laddas korrekt.
- Kontrollera att spelaren kan styra sin figur och att spelet reagerar på input.

Spelkontroller:
- A/D: Flytta spelaren vänster/höger
- W: Hoppa (om på marken)
- Vänster musknapp: Skjut projektil

Vad spelaren förväntas göra:
- Hoppa mellan plattformar och undvika fiender.
- Skjuta projektiler mot fiender.
- Hålla sig vid liv så länge som möjligt/vill (finns inte något riktigt slut i spelet/ett mål att uppnå).
- Hälsan minskar vid kollision med fiender, spelet avslutas vid 0 health.

Testa att:
- Spelaren kan röra sig och hoppa.
- Fiender och plattformar spawnar korrekt.
- Projektiler kan skjutas och träffar fiender.
- Kollisioner fungerar och påverkar spelet, både mellan spelare och ledges samt spelare och motståndare.
- Spelet avslutas korrekt vid "Game Over" när health är mindre än noll.
