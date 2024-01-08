#ifndef SEMESTRALKA_GAMEOFLIFE_SVET_H
#define SEMESTRALKA_GAMEOFLIFE_SVET_H

#define INVALID_INDEX       (-1)
#define GENEROVAT_NAHODNE    1
#define GENEROVAT_ZO_SUBORU  2
#define GENEROVAT_MANUALNE   3

typedef struct pole_data {
    _Bool* baza;
    unsigned dim1;
    unsigned dim2;
} POLE_DATA;

void vytvorPole(POLE_DATA* data);
void znicPole(POLE_DATA* data);
void vykresliPole(POLE_DATA* data);
_Bool indexySuValidne(int index1, int index2, POLE_DATA* data);
_Bool getPolicko(int index1, int index2, POLE_DATA* data);
_Bool setPolicko(int index1, int index2, POLE_DATA* data, _Bool zivaBunka);
int vyberSposobInicializacie();
void vytvorSvet(POLE_DATA* svet, int sposobGenerovania);
void aktualizujSvet(POLE_DATA* stary_data);

#endif //SEMESTRALKA_GAMEOFLIFE_SVET_H
