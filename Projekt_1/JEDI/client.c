/*
    DI Andreas JEDLICKA
    Client (Senden von Zeichen an den Server)
    07.04.2005
*/

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")    // f�r MSV C++

#define SERVER_PORT 5432


int main(int argc, char *argv[])
{
    struct hostent *hp;
    struct sockaddr_in sin;
    char buf[256];
    int s;
    int len;
    WSADATA wData;

    WSAStartup(MAKEWORD(1, 0), &wData);

    if(argc != 2)
    {
        fprintf(stderr, "host / ip-addr ?\n");
        return 1;
    }

    // Initialisiere die Datenstruktur f�r die Adresse
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);

    // �bersetze angegebene IP-Adresse
    if((sin.sin_addr.s_addr = inet_addr(argv[1])) == -1)
    {
        // �bersetze Host-Name in IP-Adresse des Partners
        if(!(hp = gethostbyname(argv[1])))
        {
            fprintf(stderr, "unknown host: %s\n", argv[1]);
            return 2;
        }
        memcpy(&sin.sin_addr, hp->h_addr, hp->h_length);
    }
//    printf("0x%08x   %d.%d.%d.%d\n", sin.sin_addr.s_addr,
//        (int)sin.sin_addr.s_net, (int)sin.sin_addr.s_host, (int)sin.sin_addr.s_lh, (int)sin.sin_addr.s_impno);

    // Aktives �ffnen
    if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket\n");
        return 3;
    }
    if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        fprintf(stderr, "connect\n");
        closesocket(s);
        return 4;
    }

    // Hauptschleife: lesen und senden
    for(;;)
    {
        gets(buf);
        len = strlen(buf) + 1;
        send(s, buf, len, 0);
    }
    closesocket(s);
    WSACleanup();
    return 0;
}
