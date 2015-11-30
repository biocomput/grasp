/**
needleman-wunsch.c

Implementa rotinas para realizar o alinhamento global entre duas sequencias com o algoritmo de Needleman-Wunsch.

Autor: Guilherme Colucci Pereira
*/

#include "needleman-wunsch.h"

/**
int getUpmostAlignment(float **M, char *aa1, char *aa2, char* a1, int n1, char *a2, int n2, int max, float gap)

Dada uma matriz de alinhamento, calcula o alinhamento correspondente atraves do criterio upmost.

M:        matriz de alinhamento
aa1:      string ja alocada onde sera guardado o valor da sequencia a1 no alinhamento
aa2:      string ja alocada onde sera guardado o valor da sequencia a2 no alinhamento
a1:       sequencia alinhada
n1:       tamanho de a1
a2:       sequencia alinhada
n2:       tamanho de a2
gap:      pontuacao utilizada para gaps

Valores de a1 e a2 no alinhamento sao guardados nas variaveis aa1 e aa2, respectivamente.
*/
float getUpmostAlignment(float **M, char *aa1, char *aa2, char* a1, int n1, char *a2, int n2, float gap) {
  int i, j, k;

  i = n1;
  j = n2;
  k = 0;

  while (i > 0 || j > 0) {
    if (i > 0 && M[i][j] == M[i-1][j] + gap) {
      aa1[k] = a1[i-1];
      aa2[k] = '-';
      i--;
    }
    else if (i > 0 && j > 0 && M[i][j] == M[i-1][j-1] + pontuacaoCaracteres(a1[i-1], a2[j-1])) {
      aa1[k] = a1[i-1];
      aa2[k] = a2[j-1];
      i--;
      j--;
    }
    else if (j > 0) {
      aa1[k] = '-';
      aa2[k] = a2[j-1];
      j--;
    }
    k++;
  }

  aa1[k] = aa2[k] = 0;
  return(strlen(aa1));
}

/**
float align(float **M, char* a1, int n1, char *a2, int n2, float gap, float match, float mismatch)

Dadas duas sequencias de caracteres, calcula a matriz de alinhamento global correspondente usando o algoritmo
de Needleman-Wunsch.

M:   matriz de alinhamento ja inicializada
a1:  sequencia alinhada
n1:  tamanho de a1
a2:  sequencia alinhada
n2:  tamanho de a2
gap: pontuacao utilizada para gaps

Retorna o valor do alinhamento com a pontuacao fornecida.
*/
float align(float **M, char* a1, int n1, char *a2, int n2, float gap) {
  float max, aux;
  int   i, j;

  for (i = 0; i <= n1; i++) {
    M[i][0] = gap * i;
  }

  for (j = 0; j <= n2; j++) {
    M[0][j] = gap * j;
  }

  for (i = 1; i <= n1; i++) {
    for (j = 1; j <= n2; j++) {
      max = M[i][j-1] + gap;

      aux = pontuacaoCaracteres(a1[i-1], a2[j-1]);
      if (max < M[i-1][j-1] + aux) {
        max 	= M[i-1][j-1] + aux;
      }

      if (max < M[i-1][j] + gap) {
        max 	= M[i-1][j] + gap;
      }

      M[i][j] = max;
    }
  }

  return M[n1][n2];
}
