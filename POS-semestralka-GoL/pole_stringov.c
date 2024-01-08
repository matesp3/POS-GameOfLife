#include "pole_stringov.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEPLATNY_INDEX -1
#define MAX_DLZKA_RETEZCA 100

typedef struct {
    char** baza;
    size_t offset;
    size_t kapacita;
    int prvy_volny;
    char* neplatna_hodnota;
} STR_POLE;

void init_pole(STR_POLE* data, size_t kapacita) {
    if (data == NULL)
        return;

    data->offset = 0;
    data->kapacita = kapacita;
    data->neplatna_hodnota = NULL;
    data->prvy_volny = NEPLATNY_INDEX;
    data->baza = (char**)calloc(data->kapacita, sizeof(char*));
}

void znic_pole(STR_POLE* data) {
    if (data == NULL)
        return;

    for (int i = 0; i < data->offset; ++i) {
        free(*(data->baza + i));
    }
    free(data->baza);
    data->offset = 0;
    data->kapacita = 0;
    data->prvy_volny = NEPLATNY_INDEX;
    data->neplatna_hodnota = NULL;
}

_Bool pridaj_prvok_do_pola(const char* str_data, STR_POLE* pole) {
    if (pole == NULL || str_data == NULL) {
        return false;
    }

    if (pole->offset < pole->kapacita) {
        char* kopie_retezca = strdup(str_data);

        if (kopie_retezca == NULL) {
            return false;
        }

        *(pole->baza + pole->offset) = kopie_retezca;
        pole->offset++;
        return true;
    } else {
        size_t novaKapacita = pole->kapacita * ((pole->kapacita > 12) ? 1.5 : 2);
        char** pom = (char**)realloc(pole->baza, novaKapacita * sizeof(char*));

        if (pom != NULL) {
            pole->baza = pom;
            pom = NULL;
            pole->kapacita = novaKapacita;

            char* kopie_retezca = strdup(str_data);

            if (kopie_retezca == NULL) {
                return false;
            }

            *(pole->baza + pole->offset) = kopie_retezca;
            pole->offset++;
            return true;
        }
    }
    return false;
}

void vytvor_novy_svet(STR_POLE* svet, int krok) {
    // reťazec reprezentujúci nový svet
    char novy_svet[MAX_DLZKA_RETEZCA];
    snprintf(novy_svet, MAX_DLZKA_RETEZCA, "Novy svet - Krok %d", krok);


    pridaj_prvok_do_pola(novy_svet, svet);

    // Vyhladaj a zneplatni neplatne prvky
    zneplatni_prvy_s_hodnotou(svet, "nezadouci", strlen("nezadouci"));
    nahrad_prvy_neplatny(svet, "nahradny");
}

int main() {
    STR_POLE svet;
    init_pole(&svet, 10);

    vytvor_novy_svet(&svet, 1);
    vytvor_novy_svet(&svet, 2);

    for (int i = 0; i < svet.offset; ++i) {
        printf("%s\n", *(svet.baza + i));
    }

    // Uvoľnenie pamäte
    znic_pole(&svet);

    return 0;
}
