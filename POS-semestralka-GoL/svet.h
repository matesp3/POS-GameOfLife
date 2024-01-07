#ifndef SEMESTRALKA_GAMEOFLIFE_SVET_H
#define SEMESTRALKA_GAMEOFLIFE_SVET_H

#define INVALID_INDEX -1;

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
_Bool vyberSposobInicializacie();
void vytvorSvet(POLE_DATA* svet, _Bool nahodne);
void aktualizujSvet(POLE_DATA* stary_data);

#endif //SEMESTRALKA_GAMEOFLIFE_SVET_H
