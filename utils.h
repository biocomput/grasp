#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "limits.h"

typedef struct sequencia {
  char *nome;
  char *valor;
  struct sequencia* prox;
} Sequencia;

void leNomesSequencias (FILE *f, Sequencia *seq, int k, int *max);
void reverteString(char *s);
float pontuacao(char **MSA, float gap, float match, float mismatch, int k);

#endif
