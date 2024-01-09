#include "pole_stringov.h"

/**
 * Pripravi strukturu na ukladanie c-stringov
 * @param data
 * @param kapacita maximalny pocet stringov na ulozenie
 */
void init_pole(STR_POLE* data, size_t kapacita) {
    if (data == NULL)
        return;

    data->offset = 0;
    data->kapacita = kapacita;
    data->neplatna_hodnota = NULL;
    data->prvy_volny = NEPLATNY_INDEX;
    data->baza = (char**) calloc(data->kapacita, sizeof (char*));
}

/**
 * Dealokuje vsetky stringy.
 * @param data pole na znicenie
 */
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

/**
 * Prida dynamicky alokovany string do pola. Ak sa prekrocila velkost pola, pole sa zvacsovacou politikou zvacsi.
 * @param str_data Predpoklada sa, ze su DYNAMICKY alokovane!
 * @param vysledok
 */
_Bool pridaj_prvok_do_pola(char* str_data, STR_POLE* pole) {
    if (pole == NULL) {
        return false;
    }
    if (pole->offset < pole->kapacita)
    {
        *(pole->baza + pole->offset) = str_data;
        pole->offset++;
        return true;
    }
    else
    {
        size_t novaKapacita = pole->kapacita * ((pole->kapacita > 12) ? 1.5 : 2);
        char** pom;
        pom = (char**) realloc(pole->baza, novaKapacita * sizeof(char*));

        if (pom != NULL) {
            pole->baza = pom;
            pom = NULL;
            pole->kapacita = novaKapacita;
            *(pole->baza + pole->offset) = str_data;
            pole->offset++;
            return true;
        }
    }
    return false;
}

/**
 * Sluzi na najdenie prveho indexu, ktory sa zhoduje s hladanym stringom.
 * @param pole pole, v ktorom ma hladat string
 * @param index sem ulozi hodnotu, na ktorom prvku pola sa nasla prva zhoda
 * @param hladany_str tento vzor sa snazi najst
 * @param dlzka_str dlzka vzoru na hladanie
 * @param zaciatocnyIndex od ktoreho indexu pola ma zacat prehladavat
 */
void vyhladaj_index_podla_hodnoty(STR_POLE* pole, int* index, char* hladany_str, size_t dlzka_str, int zaciatocnyIndex) {
    if (pole == NULL || index == NULL) {
        *index = NEPLATNY_INDEX;
        return;
    }
    int i = (zaciatocnyIndex > 0 && zaciatocnyIndex < pole->offset) ? zaciatocnyIndex : 0;
//    while (i < pole->offset && *(pole->baza + i) != hladanaHodnota) {
    while (i < pole->offset && strncmp(*(pole->baza + i), hladany_str, dlzka_str) != 0) {
        ++i;
    }
    *index = (i < pole->offset) ? i : NEPLATNY_INDEX;
}
/**
 * Hlada poziciu hladaneho retazca. Ak ho najde, nastavi tento prvok na neplatnu hodnotu a postara sa o dealokaciu stringu.
 * @param pole
 * @param hladany_str string na odstranenie
 * @param dlzka_str dlzka hladaneho stringu na odstranenie
 */
void zneplatni_prvy_s_hodnotou(STR_POLE* pole, char* hladany_str, size_t dlzka_str ) {
    if (pole == NULL)
        return;

    int i = 0;
    vyhladaj_index_podla_hodnoty(pole, &i, hladany_str, dlzka_str, 0);

    if (i != NEPLATNY_INDEX) {
        free(*(pole->baza + i));
        *(pole->baza + i) = pole->neplatna_hodnota;
        pole->prvy_volny = (pole->prvy_volny == NEPLATNY_INDEX)
                           ? i
                           : ( (i < pole->prvy_volny) ? i : pole->prvy_volny);
    }
}

/**
 * Ak existuje v poli neplatny prvok, ktory ma skorsiu poziciu ako posledny vlozeny prvok, potom vyuzije tuto poziciu
 * na nastavenie nahradneho stringu. Inak prida string na koniec pola.
 * @param pole
 * @param nahradnaHodnota
 */
void nahrad_prvy_neplatny(STR_POLE* pole, char* nahradnaHodnota) {
    if (pole == NULL) {
        return;
    }

    if (pole->prvy_volny != NEPLATNY_INDEX) {
        *(pole->baza + pole->prvy_volny) = nahradnaHodnota;
        int i = pole->prvy_volny;
        while (i < pole->offset && *(pole->baza + i) != NULL) // najdi nasledovny volny
            ++i;
        pole->prvy_volny = (i < pole->offset) ? i : NEPLATNY_INDEX;
    }
    else {
        pridaj_prvok_do_pola(nahradnaHodnota, pole);
    }
}