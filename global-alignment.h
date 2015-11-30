#ifndef GLOBAL_ALIGNMENT_H
#define GLOBAL_ALIGNMENT_H

#include "utils.h"

float align						  (float **Mdiag, float **Mleft, float **Mup, char* a1,  int n1, char *a2, int n2, float a, float b);
int getUpmostAlignment  (float **Mdiag, float **Mleft, float **Mup, char *aa1, char *aa2, char* a1, int n1, char *a2, int n2, 											 	 float a, float b);

#endif
