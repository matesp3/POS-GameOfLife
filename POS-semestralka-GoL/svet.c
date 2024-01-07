#include <stdio.h>

#include "stdlib.h"
#include "stdbool.h"

#include "svet.h"

/**
 * V dynamickej pamati vytvori nove pole podla zadanych parametrov. Treba potom DEALOKOVAT!
 */
void vytvorPole(POLE_DATA* data) {
    data->baza = (_Bool*) calloc(data->dim1 * data->dim2, sizeof (_Bool));
//    for (int i = 0; i < (data->dim1 * data->dim2); i++) {
//        *(data->baza + i) = false;
//    }
}

void znicPole(POLE_DATA* data) {
    free(data->baza);
}

void vykresliPole(POLE_DATA* data) {
    for (int i = 0; i < data->dim1; ++i) {
        for (int j = 0; j < data->dim2; ++j) {
            char znak = '.';
//            if (*(data->baza + (i * data->dim2) + j))
            if (getPolicko(i, j, data))
                znak = '*';
            printf(" %c",  znak);
        }
        printf("\n");
    }
}

_Bool indexySuValidne(int index1, int index2, POLE_DATA* data) {
    return index1 >= 0 && index1 < data->dim1 && index2 >= 0 && index2 < data->dim2;
}

_Bool getPolicko(int index1, int index2, POLE_DATA* data) {
    if (indexySuValidne(index1, index2, data)) {
        int offset = (index1 * data->dim2) + index2;
        return *(data->baza + offset);
    }

    return INVALID_INDEX;
}
_Bool setPolicko(int index1, int index2, POLE_DATA* data, _Bool zivaBunka) {
    if (indexySuValidne(index1, index2, data)) {
        int offset = (index1 * data->dim2) + index2;
        *(data->baza + offset) = zivaBunka;
        return true;
    }
    return false;
}

/**
 * @return true - nahodne, false - manualne
 */
_Bool vyberSposobInicializacie() {
    char volba;
    printf("Vyberte prosím spôsob generovania: n - nahodné nastavenie | m -manuálne nastavanie  (n/m): ");
    scanf("%c", &volba);

    return (volba == 'n' || volba == 'N');
}

void vytvorSvet(POLE_DATA* svet, bool nahodne) {

    vytvorPole(svet);

    if (nahodne) {
        for (int i = 0; i < svet->dim1; ++i) {
            for (int j = 0; j < svet->dim2; ++j) {
                setPolicko(i, j, svet, rand() % 2 == 0);
            }
        }
    } else {
        // cout << "Manuálne nastavenie farieb:" << endl; // cout je pre c++, pre c je printf()
        printf("Manuálne nastavenie farieb:\n");
        for (int i = 0; i < svet->dim1; ++i) {
            for (int j = 0; j < svet->dim2; ++j) {
                char volba;
                printf("Bunka [%d][%d]: Má byť bunka živá? (a/n): ", i, j);
                scanf("%c", &volba);
                setPolicko(i, j, svet, (volba == 'a' || volba == 'A'));
            }
        }
        // tuto else vetvu by som spravil inak: podla mna by sa mal program spytat, ktoru bunku chce uzivatel ozivit.
        // Toto by program cyklicky opakoval, kym by uzivatel nezadal nejaky ukoncovaci znak. Ostatne policka by
        // boli nastavene na false - mrtve
    }
}

void aktualizujSvet(POLE_DATA* stary_data) {
    _Bool* nova_baza = (_Bool*) calloc(stary_data->dim1 * stary_data->dim2, sizeof (_Bool ));
    POLE_DATA aktualizovane_pole = { nova_baza, stary_data->dim1, stary_data->dim2 };

    for (int i = 0; i < stary_data->dim1; ++i) {
        for (int j = 0; j < stary_data->dim2; ++j) {
            int ziviSusedia = 0;

            // Kontrola okolitých buniek
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (x == 0 && y == 0) continue;
                    int ni = i + x;
                    int nj = j + y;

                    // Overenie či susedné bunky nevypadajú
                    if (ni >= 0 && ni < stary_data->dim2 && nj >= 0 && nj < stary_data->dim1) {
                        ziviSusedia += getPolicko(ni, nj, stary_data) ? 1 : 0;
                    }
                }
            }

            // Aplikácia pravidiel Game of Life
            if (getPolicko(i, j, stary_data)) {
                // Živá bunka
                //  1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
                //  2. Any live cell with two or three live neighbours lives on to the next generation.
                //  3. Any live cell with more than three live neighbours dies, as if by overpopulation.
                if (ziviSusedia == 2 || ziviSusedia == 3) { // Prežitie
                    setPolicko(i, j, &aktualizovane_pole, true);
                } else {                                    // Smrť
                    setPolicko(i, j, &aktualizovane_pole, false);
                }
            } else {
                // Mŕtva bunka
                // 4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                if (ziviSusedia == 3) {                     // Oživenie
                    setPolicko(i, j, &aktualizovane_pole, true);
                } else {
                    setPolicko(i, j, &aktualizovane_pole, false);
                }
            }
        }
    }
    znicPole(stary_data);
    stary_data->baza = nova_baza;
}