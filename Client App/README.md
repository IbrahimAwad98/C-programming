# Enkel web client i c med (Winsock)

Laborationen är ett exempel på hur man implementerar en enkel HTTP-klient i C-språk för att ansluta till en webserver.

## Syfte

- Förstå hur socket fungerar i C. Socket består av en ip-address och port t.ex. 127.0.0.1:2000
- hur man skapar och använder socket(); , connect(); , send(); , recv();
- skicka och ta emot HTTP-förfrågor (GET/POST). ObS: GET är enklare och lättare då mindre hantering av sträng-längd
- förstå hur Winsock fungerar på Windows eller macOS/Linux

### Instruktioner(Allt sker via kod)

- Initiera nätverk (Winsock2)
- Skapa socket
- Ange serverAdress
- Ansluta till server
- Skicka en HTTP-förfrågan
- Ta emot svar
- Stäng anslutning

#### Kompilering (Via terminal)

-gcc "namnet på filen t.ex. client.c" -lws2_32 -o "namnet på filen t.ex. client.exe".
-gcc client.o -lws2_32 -o client.exe

#### Resultat

- Om anslutning lyckades via en IP och Port ska man få ett svar från server som t.ex. WELL DONE!
- Annars om anslutning är misslyckades då får man ett ERROR som t.ex. Try Again!
