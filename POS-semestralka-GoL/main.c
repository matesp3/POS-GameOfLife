#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "svet.h"
#include "file_manager.h"


void spracuj_signal(int param) {
    char c;
    printf("\n>>> STOP! <<<\n");
    printf("Vyberte moznost:\n - koniec (k)\n - pokracovat (p)\n - reverzia (r)\nVasa volba > ");
    scanf("%c", &c);
}

void nastavRozmery(POLE_DATA* data, int argc, char *argv[]) {
    if (argc > 2) {
        int vysledok;
        vysledok = atoi(argv[1]);
        data->dim1 = vysledok > 0 ? vysledok : 15; // ak tam je nezmysel, vrati 0
        vysledok = atoi(argv[2]);
        data->dim2 = vysledok > 0 ? vysledok : 20;
    }
    else {
        data->dim1 = 15;
        data->dim2 = 20;
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    signal(SIGINT, spracuj_signal);

    POLE_DATA svet = {NULL, 0, 0 };
    nastavRozmery(&svet, argc, argv);
//     Vytvorenie sveta s vybraným spôsobom inicializácie
    vytvorSvet(&svet,  vyberSposobInicializacie());

//    if(!nacitajVzor("/tmp/tmp.Y27gDGX4qB/pokusnySubor.txt", &svet))
//        return 0;
//

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
