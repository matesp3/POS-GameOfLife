#include "stdlib.h"
#include "string.h"
#include "file_manager.h"

/**
 * Ukladanie 0 a 1, ako mrtve a zive bunky.
 * @param cesta_k_suboru - cesta pre subor na zapisanie vzoru
 * @param data - data na zapisanie vzoru
 * @return - uspesnost zapisu
 */
void ulozVzor(const char* cesta_k_suboru, POLE_DATA* data) {
    FILE * file_ptr = fopen(cesta_k_suboru, "w"); // ak subor neexistuje, vytvori ho. Ak existuje, tak ho prepise

    fprintf(file_ptr, "%d %d\n", data->dim1, data->dim2); // prvy riadok sluzi na informaciu, kolko riadkov a stlpcov bude pole mat

    for (int i = 0; i < data->dim1; ++i) {
        for (int j = 0; j < data->dim2; ++j) {
            fprintf(file_ptr, "%d ", getPolicko(i, j, data) ? 1 : 0);
        }
        fprintf(file_ptr, "\n");
    }

    fclose(file_ptr);
}

int zistiIndexDalsiehoOddelovaca(int indexZaciatku, const char * string, const char delimiter) {
    int i = indexZaciatku < 0 ? 0 : indexZaciatku;
    while (string[i] != delimiter) {
        i++;
    }
    return i;
}

/**
 *
 * @param cesta_k_suboru -> odkial ma subor nacitat
 * @param data -> Nezabudnut dealokovat ulozenu pamat!
 * @return uspesnost nacitania vzoru
 */
_Bool nacitajVzor(const char* cesta_k_suboru, POLE_DATA* data) {
    FILE * file_ptr = fopen(cesta_k_suboru, "r");
    if (file_ptr == NULL) {
        printf("Subor '%s' so vzorom sa nepodarilo nacitat...\n", cesta_k_suboru);
        return false;
    }

    char parametre[10];
    char cislo1[4], cislo2[4];
    memset(cislo1, 0, sizeof (char) * 4);
    memset(cislo2, 0, sizeof (char) * 4);

    fgets(parametre, 10, file_ptr);

    int i, a;
    i = zistiIndexDalsiehoOddelovaca(0, parametre, ' ');
    memcpy(cislo1, parametre, i++);

    a = zistiIndexDalsiehoOddelovaca(i, parametre, '\n');
    memcpy(cislo2, parametre + i, a - i);

    data->dim1 = atoi(cislo1);
    data->dim2 = atoi(cislo2);

    int velkost_buf = 2 * data->dim2 + 2;
    data->baza  = (_Bool*) calloc(data->dim1 * data->dim2, sizeof (_Bool));
    char* riadok = (char*) calloc(velkost_buf, sizeof (char ));

    for (int q = 0; q < data->dim1; ++q) {
        fgets(riadok, velkost_buf, file_ptr);
        char* token = strtok(riadok, " "); // https://www.educative.io/answers/splitting-a-string-using-strtok-in-c

        int r = 0;
        while (token != NULL && strcmp(token, "\n") != 0) {
            setPolicko(q, r, data, strcmp(token, "1") == 0);
            token = strtok(NULL, " "); // dalsia bunka v riadku
            r++;
        }
    }
    free(riadok);

    fclose(file_ptr);
    return true;
}