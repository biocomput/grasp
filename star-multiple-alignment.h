#ifndef STAR_MULTIPLE_ALIGNMENT_H
#define STAR_MULTIPLE_ALIGNMENT_H

#include "needleman-wunsch.h"
#include "utils.h"

void buildDistanceMatrix(float **dist, float gap, float match, float mismatch, int k, int max, Sequencia *sequences);
void starAlign(char **MSA, float gap, float match, float mismatch, int k, int maiorSeq, Sequencia *sequences);
void reverteString(char *s);
int calculaPontuacao(char **MSA, int k, int n);

#endif
