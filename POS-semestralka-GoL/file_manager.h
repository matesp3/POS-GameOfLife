#ifndef SEMESTRALKA_GAMEOFLIFE_FILE_MANAGER_H
#define SEMESTRALKA_GAMEOFLIFE_FILE_MANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include "svet.h"

void ulozVzor(const char* cesta_k_suboru, POLE_DATA* data);
_Bool nacitajVzor(const char* cesta_k_suboru, POLE_DATA* data);

#endif //SEMESTRALKA_GAMEOFLIFE_FILE_MANAGER_H
