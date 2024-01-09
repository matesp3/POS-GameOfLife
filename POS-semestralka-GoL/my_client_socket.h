#ifndef POS_SEMESTRALKA_GOL_MY_SOCKET_H
#define POS_SEMESTRALKA_GOL_MY_SOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define SOCKET_TERMINATE_CHAR '\0'       // oznamenie konca spravy
#define POTVRDENIE_PRECITANIA "all:ok"   // potvrdenie precitania celej spravy serverom
#define SOCKET_UKONCENIE      "sock:end" // ukoncenie celej komunikacie

int vytvoritSpojenie(char* adresa, char* port);
_Bool poslatSpravu(int sockfd, char* sprava, size_t velkostSpravy);
_Bool prijatOdozvu(int sockfd);
void ukoncitSpojenie(int sockfd);
void komunikujSoServerom();

#endif //POS_SEMESTRALKA_GOL_MY_SOCKET_H
