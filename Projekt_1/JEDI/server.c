/*
    DI Andreas JEDLICKA
    Server (Ausgabe aller Zeichen die Empfangen werden)
    07.04.2005
*/

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")    // f�r MSV C++

#define SERVER_PORT 5432
#define MAX_PENDING 1


int main()
{
    struct sockaddr_in sin;
    char buf[256];
    int s, new_s;
    int len;
    WSADATA wData;

    WSAStartup(MAKEWORD(1, 0), &wData);

    // Initialisiere die Datenstruktur f�r die Adresse
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);

    // Richte passives �ffnen ein
    if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket\n");
        return 1;
    }
    if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        fprintf(stderr, "bind\n");
        return 2;
    }
    listen(s, MAX_PENDING);

    // Warte auf Verbindung, dann Daten empfangen und ausgeben
    if((new_s = accept(s, (struct sockaddr *)&sin, &len)) < 0)
    {
        fprintf(stderr, "accept\n");
        return 3;
    }
    while((len = recv(new_s, buf, sizeof(buf), 0)) > 0)
    {
        puts(buf);
    }
    closesocket(new_s);
    WSACleanup();
    return 0;
}
