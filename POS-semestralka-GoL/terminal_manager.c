#include "stdio.h"
#include "terminal_manager.h"

/**
 * Pokial na vstupe nedostane cislo, cyklicky vyzyva uzivatela o zadanie korektneho vstupu (cisla).
 * @return cislo od pouzivatela
 */
int nacitajCislo() {
    int cislo;
    while (1) {
        char vstup[50];
        fgets(vstup, 5 * sizeof(char), stdin);
        int jeCislo = sscanf(vstup, "%d", &cislo);
        if (jeCislo)
            return cislo;
        else {
            printf("\nVas vstup nie je cislom! Opakujte volbu cisla > ");
        }
    }
}

/**
 * @return true - nahodne, false - manualne
 */
int vyberSposobInicializacie() {
    int volba, vysledok;
    printf("\nAkym sposobom chcete vytvorit vzor? (zadajte cislo)"
           "\n 1 - nahodne generovat\n 2 - poskytnut vzor ulozeny v subore\n 3 - manualne zadat hodnotu kazdej bunky\nVasa volba > ");

    do {
        volba = nacitajCislo();
        if (volba > 0 & volba < 4)
            break;
        else {
            printf("\nTakato moznost neexistuje! Vasa volba > ");
        }
    } while (1);

    return volba;
}
