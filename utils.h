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
  struct sequencia* ante;
} Sequencia;
typedef struct gapBlock {
  int beginning;
  int end;
  struct gapBlock* prox;
  struct gapBlock* ante;
} GapBlock;
typedef struct pontuacaoFerramentas {
  char** MSA;
  int maiorSeq;
  float gap;
  float match;
  float mismatch;
  int size;
  float score;
  int index;
} PontuacaoFerramentas;
void leNomesSequencias (FILE *f, Sequencia *seq, int k, int *max);
void reverteString(char *s);
float pontuacao(char **MSA, float gap, float match, float mismatch, int k);

#endif
