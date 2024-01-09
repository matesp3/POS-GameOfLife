#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "my_client_socket.h"

/**
 * Pokusi sa pripojit k serveru pomocou socketu.
 * @param port
 * @param adresa
 * @return socket file descriptor
 */
int vytvoritSpojenie(char* adresa, char* port) {
    if (port == NULL) {
        fprintf(stderr,"Port nebol zadany!\n");
        return -1;
    }
    if (adresa == NULL) {
        fprintf(stderr,"Adresa nebola zadana!\n");
        return -2;
    }

    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[256];

    server = gethostbyname(adresa);
    if (server == NULL)
    {
        fprintf(stderr, "Chyba, takyto host neexistuje!\n");
        return -3;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(atoi(port));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket sa nepodarilo vytvorit!");
        return -4;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Chyba pri pripajani sa na socket");
        return -5;
    }
    return sockfd;
}


void ukoncitSpojenie(int sockfd) {
    close(sockfd);
}

#define BUF_SIZE 256
/**
 *
 * @param sprava obsah na poslanie. Nesmie presiahnut dlzku BUF_SIZE! (256 bytov)
 * @return uspesnost odoslania spravy
 */
_Bool poslatSpravu(int sockfd, char* sprava, size_t velkostSpravy) {
    char buffer[BUF_SIZE];

    bzero(buffer, BUF_SIZE); // inicializuj buffer
    memcpy(buffer, sprava, velkostSpravy);
    buffer[velkostSpravy] = SOCKET_TERMINATE_CHAR;

    int n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Chyba pri zapisovani do socketu!\n");
        return false;
    }
    return true;
}

/**
 *
 * @param sockfd ID socketu, z ktoreho citat odpoved
 * @return ci bola odoslana sprava uspesne precitana
 */
_Bool prijatOdozvu(int sockfd) {
    char buffer[BUF_SIZE];

    bzero(buffer, 256);
    int n = read(sockfd, buffer, BUF_SIZE - 1);
    if (n < 0) {
        perror("Chyba pri citani zo socketu");
        return false;
    }
    return strncmp(buffer, POTVRDENIE_PRECITANIA, 6) == 0;
}

void komunikujSoServerom(int argc, char* argv[]) {
    int sockfd;
    if (argc > 3) {
        sockfd = vytvoritSpojenie(argv[3], argv[4]);
    }
    else {
        char adresa[] = "frios2.fri.uniza.sk";
        char port[] = "17834";
        sockfd = vytvoritSpojenie("frios2.fri.uniza.sk", "17834");
    }
    if (sockfd < 0){
        return;
    }

    char sprava[] = "sock:end";
    if (poslatSpravu(sockfd,sprava, sizeof (sprava))) {
        if (prijatOdozvu(sockfd))
            printf("Komunikacia prebehla OK..");
        else
            printf("Komunikacia ZLYHALA...");
    }

    ukoncitSpojenie(sockfd);
}