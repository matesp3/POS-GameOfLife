#ifndef POS_GOL_SERVER_POLE_STRINGOV_H
#define POS_GOL_SERVER_POLE_STRINGOV_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NEPLATNY_INDEX -1

typedef struct str_pole {
    char** baza;
    char* neplatna_hodnota;
    int prvy_volny;  // prve policko, na ktorom bola zneplatnena hodnota, pricom existuju nasledovne policka
    size_t kapacita;
    size_t offset;   // aktualna velkost

} STR_POLE;

void init_pole(STR_POLE* data, size_t kapacita);
void znic_pole(STR_POLE* data);
_Bool pridaj_prvok_do_pola(char* str_data, STR_POLE* pole);
void vyhladaj_index_podla_hodnoty(STR_POLE* pole, int* index, char* hladany_str, size_t dlzka_str, int zaciatocnyIndex);
void zneplatni_prvy_s_hodnotou(STR_POLE* pole, char* hladany_str, size_t dlzka_str);
void nahrad_prvy_neplatny(STR_POLE* pole, char* nahradnaHodnota);

#endif //POS_GOL_SERVER_POLE_STRINGOV_H
