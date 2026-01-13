============================
README - Programmeringsprojekt
============================

1) INSTRUKTION

Förutsättningar:
- SDL3, SDL3_image och SDL3_ttf måste vara installerade på datorn.
- Projektet följer filstrukturen enligt kurskraven.

Bygga projektet:
- Öppna terminalen i projektmappen.
- Kör kommandot: make
- Det kompilerade programmet hamnar i: build/debug/play

Köra projektet:
- Starta spelet genom att köra: build/debug/play

Sökvägar till resursfiler:
- Alla bilder finns i: resources/images/
- Alla ljudfiler finns i: resources/sounds/
- Alla typsnitt finns i: resources/fonts/
- Sökvägar till resursfiler definieras i include/Constants.h, t.ex.:
  const std::string player_charart = gResPath + "images/Walk.png";


2) TESTBESKRIVNING

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
- Hålla sig vid liv så länge som möjligt.
- Hälsan minskar vid kollision med fiender, spelet avslutas vid 0 hälsa.

Testa att:
- Spelaren kan röra sig och hoppa.
- Fiender och plattformar spawnar korrekt.
- Projektiler kan skjutas och träffar fiender.
- Kollisioner fungerar och påverkar spelet.
- Spelet avslutas korrekt vid "Game Over".
