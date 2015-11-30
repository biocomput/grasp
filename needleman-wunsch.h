#ifndef NEEDLEMAN_WUNSCH_H
#define NEEDLEMAN_WUNSCH_H

#include "utils.h"

float getUpmostAlignment(float **M, char *aa1, char *aa2, char* a1, int n1, char *a2, int n2, float gap);
float align						  (float **M, char* a1, int n1, char *a2, int n2, float gap);

#endif
