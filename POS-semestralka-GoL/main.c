#include <time.h>
#include <stdlib.h>
#include <stdio.h>
//#include <windows.h>
#include "svet.h"
#include "file_manager.h"
#include <unistd.h>

void* vstup(void* data) {

}

void* vystup(void* data) {
    while (true) {
        sleep(1000);
        printf("1");
    }
}
#include "signal.h"
_Bool pokracuj = 1;

void spracuj_signal(int param) {
    pokracuj = 0;
}

//int main(int argc, char *argv[]) {
//    signal(SIGINT, spracuj_signal);
//    while (pokracuj) {
//        printf("Stale bezim...\n");
//        sleep(3);
//    }
//    printf("KONIEC:)\n");

//        char            name[20] = {0}; // in case of single character input
//        fd_set          input_set;
//        struct timeval  timeout;
//        int             ready_for_reading = 0;
//        int             read_bytes = 0;
//        /* Empty the FD Set */
//        FD_ZERO(&input_set );
//        /* Listen to the input descriptor */
//        FD_SET(STDIN_FILENO, &input_set);
//
//        timeout.tv_sec = WAIT;    // WAIT seconds
//        timeout.tv_usec = 0;    // 0 milliseconds
//        printf("Time start now!!!\n");
//        /* Listening for input stream for any activity */
//        ready_for_reading = select(1, &input_set, NULL, NULL, &timeout);
//
//        if (ready_for_reading == -1) {
//            printf("Unable to read your input\n");
//            return -1;
//        }
//
//        if (ready_for_reading) {
//            read_bytes = read(0, name, 19);
//            if(name[read_bytes-1]=='\n'){
//                --read_bytes;
//                name[read_bytes]='\0';
//            }
//            if(read_bytes==0){
//                printf("You just hit enter\n");
//            } else {
//                printf("Read, %d bytes from input : %s \n", read_bytes, name);
//            }
//            printf("You just hit enter\n");
//        } else {
//            printf(" %d Seconds are over - no data input \n", WAIT);
//        }
//        sleep(3);
//}

int main(int argc, char *argv[]) {

//    char predpona[] = "C:\\Users\\MatejPol\\Downloads\\";

    srand(time(NULL));
    int riadky, stlpce;
    if (argc > 2) {
        riadky = atoi(argv[1]);
        stlpce = atoi(argv[2]);
    }
    else {
        riadky = 15;
        stlpce = 20;
    }
    POLE_DATA svet = {NULL, riadky,stlpce };
    _Bool nahodne = vyberSposobInicializacie();

//     Vytvorenie sveta s vybraným spôsobom inicializácie
    vytvorSvet(&svet, nahodne);

//    if(!nacitajVzor("C:\\Users\\MatejPol\\Downloads\\pokusnySubor.txt", &svet))
//        return 0;
    printf("dostal som sa sem\n");
    printf("Vychodzia pozicia %d:\n", 0);
    vykresliPole(&svet);

    for (int krok = 1; krok < 20; ++krok) {
        printf("Krok %d:\n", krok);
        aktualizujSvet(&svet);
        vykresliPole(&svet);

        sleep(1);
    }
    ulozVzor("/tmp/tmp.Y27gDGX4qB/pokusnySubor.txt", &svet);

    znicPole(&svet);
    return 0;
}
