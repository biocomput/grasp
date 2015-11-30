#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>
#include <time.h>

typedef struct sequencia {
  char *nome;
  char *valor;
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
  float a;
	float b;
  int size;
  float score;
  int index;
  int numeroSequencia;
  int numeroBuracos;
} PontuacaoFerramentas;

void  leNomesSequencias	 (FILE *f, Sequencia *seq, int k, int *max);
void  reverteString			 (char *s);
float pontuacaoMSA			 (char **MSA, float a, float b, int k);
int 	pontuacaoCaracteres(char a, char b);
int		pontuacaoReferencia(char **MSA, char *nome, int k);

#endif
