#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>
/*
Dessa är headers för nätverk för MacOS/Linux
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
*/
#define LENGTH 1024

int main(int argc, char **argv)
{
	// Steg 1: starta winsock (initiera nätverk)
	// Winsock består av struck som innehållr web version
	WSADATA wsa;
	printf("\nInitialising Winsock...");
	// hantera misslyckades initiering
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) // parametern makeword består av webversion 2 och pekare till struct.
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return EXIT_FAILURE;
	}
	printf("Initialised.");
	printf("Web Client 0.1\n");

	// Steg 2: skapa socket
	SOCKET sock = socket(PF_INET, SOCK_STREAM, 0); // PF_INET är för IPv4 (Standard) och SOCK_STREAM är TCP (ej UDP).
	// hantera misslyckades socket
	if (sock == -1)
	{
		return EXIT_FAILURE;
	}
	printf("*** SOCKET Established...\n");

	// Steg 3: Ange serveradress och port VIKTIGT!!
	struct sockaddr_in addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("130.229.175.91"); // IP för Google (publik)
	addr.sin_port = htons(8080);						// port OCH htons konverterar till rättformat.
	// hantera misslyckades information
	if (connect(sock, (const struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		printf("Connect Failed! Error Code: %d\n", WSAGetLastError());
		return 2;
	}
	printf("*** CONNECT Success...\n");

	// Steg 4: Välj rätt metod (GET/POST)
	/*

	*/
	//  === ALTERNATIV 1: GET ===
	char buffer[LENGTH];
	sprintf(buffer, "GET /IbrahimAwad HTTP/1.1\r\n" // meddelandet (t.ex. Google ger datum)
					"Host: 130.229.175.91\r\n"		// server eller länk
					"Connection: close\r\n"
					"\r\n");
	printf("*** SENDING \n[%s]\n", buffer);
	size_t len = strlen(buffer);

	// === ALTERNATIV 2: POST ===
	/*
	char text[] = "{name : Anders}";
	sprintf(buffer,
	"POST /api/1.0/pass/8533 HTTP/1.1\r\n"
	"Host: 127.0.0.1\r\n"
	"Content-Type: application/json\r\n"
	"Accept: application/json\r\n"
	"Content-Length: %d\r\n"
	"Connection: close\r\n"
	"\r\n"
	"%s",
	(int)strlen(text), text);
	*/

	// Steg 5: skicka data (sokcet, bufferfer, längd)
	send(sock, buffer, len, 0);
	printf("*** SEND Success...\n");

	// Steg 6: Ta emot svaret. OBS: välj en av dem.

	//  === ALTERNATIV 1: Lätt svar ===
	/*
	len = recv(sock, buffer, sizeof(buffer), 0);
	buffer[len] = '\0';
	if (len)
	{
		printf("***[%s](%d)\n", buffer, (int)len);
	}
	*/

	//  === ALTERNATIV 2: allmänt svar (Rekommenderat) ===
	while ((len = recv(sock, buffer, sizeof(buffer) - 1, 0)) > 0)
	{
		buffer[len] = '\0';
		printf("%s", buffer);
	}

	// steg 7: Stänga anslutning
	closesocket(sock);
	WSACleanup();
	printf("*** CONNECTION CLOSED!\n");
	printf("*** DONE...\n");
	return EXIT_SUCCESS;
}
