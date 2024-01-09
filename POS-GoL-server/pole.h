#ifndef SERVER_TRIAL_MOJE_POLE_H
#define SERVER_TRIAL_MOJE_POLE_H
#include "stddef.h"
#include "stdbool.h"

#define GENERUJ_POLE(typ, privlastok) \
typedef struct pole_##privlastok { \
    typ* baza; \
    size_t kapacita; \
    size_t offset; \
} POLE_##privlastok; \
\
static void init_pole_##privlastok(struct pole_##privlastok* data, size_t kapacita) { \
    if (data == NULL) \
        return; \
\
    data->offset = 0; \
    data->kapacita = kapacita; \
    data->baza = (typ*) calloc(data->kapacita, sizeof (typ)); \
} \
\
static void znic_pole_##privlastok(struct pole_##privlastok* data) { \
    if (data == NULL) \
        return; \
\
    data->offset = 0; \
    data->kapacita = 0; \
    free(data->baza); \
} \
\
static _Bool pridaj_prvok_do_pola_##privlastok(typ noveData, struct pole_##privlastok* pole) { \
    if (pole == NULL) \
        return false;  \
\
    if (pole->offset < pole->kapacita) \
    { \
        *(pole->baza + pole->offset) = noveData; \
        pole->offset++; \
        return true;    \
    } \
    else \
    { \
        size_t novaKapacita = pole->kapacita * ((pole->kapacita > 12) ? 1.5 : 2); \
        typ* pom; \
        pom = (typ*) realloc(pole->baza, novaKapacita * sizeof(typ)); \
\
        if (pom != NULL) {  \
            pole->baza = pom; \
            pom = NULL; \
            pole->kapacita = novaKapacita; \
            *(pole->baza + pole->offset) = noveData; \
            pole->offset++; \
            return true;    \
        } \
    } \
    return false; \
} \
\
static void nastav_prvok_pola_##privlastok(typ noveData, size_t index, struct pole_##privlastok* pole) { \
    if (index >= pole->kapacita) \
        return; \
    *(pole->baza + index) = noveData; \
} \
\
static void spristupni_prvok_pola_##privlastok(size_t index, struct pole_##privlastok* pole, typ* hodnota_prvku) { \
    if (index >= pole->kapacita) {     \
        return;  \
        }\
    *hodnota_prvku = *(pole->baza + index); \
}    \

#endif //SERVER_TRIAL_MOJE_POLE_H