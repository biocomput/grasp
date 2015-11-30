#ifndef STAR_MULTIPLE_ALIGNMENT_H
#define STAR_MULTIPLE_ALIGNMENT_H

#include "global-alignment.h"
#include "utils.h"

void buildDistanceMatrix(float **dist, float a, float b, int k, int max, Sequencia *seq);
void starAlign				  (char **MSA, float a, float b, int k, int maiorSeq, Sequencia *sequences);
void reverteString			(char *s);
int  calculaPontuacao		(char **MSA, int k, int n);

#endif
