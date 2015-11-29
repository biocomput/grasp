/**
star-multiple-alignment.c

Implementa rotinas para realizar o alinhamento multiplo entre duas sequencias com a heuristica de alinhamento
estrela.

Autor: Guilherme Colucci Pereira
*/

#include "star-multiple-alignment.h"
#include "utils.h"

/**
void buildDistanceMatrix(float **dist, float gap, float match, float mismatch, int k, int max, Sequencia *sequences)

Calcula a matriz de distancias entre k sequencias, utilizando a pontuacao de alinhamento fornecida.

dist:      matriz de floats kxk ja inicializada, onde serao armazenadas as distancias entre cada par de sequencias
gap:       pontuacao utilizada para gaps
match:     pontuacao utilizada para matches
mismatch:  pontuacao utilizada para mismatches
max:       tamanho da maior sequencia fornecida
sequences: vetor de elementos do tipo Sequencia, contendo as sequencias a serem alinhadas

A matriz preenchida sera armazenada na variavel dist.
*/
void buildDistanceMatrix(float **dist, float gap, float match, float mismatch, int k, int max, Sequencia *sequences) {
  int   i,j,l,m,n;
  float **M = NULL;

  /* Constroi matriz de pontuacao */
  for (i = 0; i < k-1; i++) {
    for (j = i+1; j < k; j++) {
      n = strlen(sequences[i].valor);
      m = strlen(sequences[j].valor);

      M = (float **) realloc(M, sizeof(float *) * (n+1));
      for (l = 0; l <= n; l++) {
        M[l] = NULL;
      }

      for (l = 0; l <= n; l++) {
        M[l] = (float *) realloc(M[l], sizeof(float) * (m+1));
      }

      dist[i][j] = dist[j][i] = align(M, sequences[i].valor, n, sequences[j].valor, m, gap, match, mismatch)*-1;
    }
  }

  /* Libera espaço alocado */
  for (i = 0; i <= n; i++) {
    free(M[i]);
  }
  free(M);
}

/**
int calculaPontuacao(char **MSA, int k, int n) 

Calcula a pontuacao do alinhamento multiplo, a partir do criterio usado pelo BAliBase: matches tem pontuacao 1 e 
nao sao realizados descontos de pontuacao

MSA: matriz que contem o alinhamento multiplo
k:	 numero de sequencias
n:   comprimento das sequencias

Retorna a pontuacao
*/

/**
void starAlign(char **MSA, float gap, float match, float mismatch, int k, int maiorSeq, Sequencia *sequences)

Realiza o alinhamento multiplo de k sequencias utilizando o algoritmo estrela.

MSA:       matriz ja inicializada, onde sera armazenado o alinhamento multiplo final
gap:       pontuacao utilizada para gaps
match:     pontuacao utilizada para matches
mismatch:  pontuacao utilizada para mismatches
maiorSeq:  tamanho da maior sequencia fornecida
sequences: vetor de elementos do tipo Sequencia, contendo as sequencias a serem alinhadas

O alinhamento preenchido sera armazenado na variavel MSA.
*/
void starAlign(char **MSA, float gap, float match, float mismatch, int k, int maiorSeq, Sequencia *sequences) {
  int   f, g, h, i, j, l, m, n, nextRow, larguraMSA, larguraSA, ancora, max;
  float menorSoma, soma, **M, **dist;
  char  *a, *b, *aux, *ancoraOriginal;

  /* Inicializacao e alocacao */
  M = NULL;
  a = NULL;
  b = NULL;
  aux = (char *) malloc(sizeof(char) * 3*maiorSeq);
  ancoraOriginal = (char *) malloc(sizeof(char) * 3*maiorSeq);
  larguraMSA = 0;
  nextRow = 1;

  /* Constroi matriz de distancias */
  dist = (float **) malloc(sizeof(float *) * k);
  for (i = 0; i < k; i++) {
    dist[i] = (float *) malloc(sizeof(float) * k);
  }
  buildDistanceMatrix(dist, gap, match, mismatch, k, maiorSeq, sequences);

  /* Escolhe sequencia ancora */;
  for (i = 0; i < k; i++) {
    soma = 0;
    for (j = 0; j < k; j++) {
      if (i != j) {
        soma += dist[i][j];
      }
    }

    if (i == 0) {
      ancora = i;
      menorSoma = soma;
    }

    if (soma < menorSoma){
      ancora = i;
      menorSoma = soma;
    }
  }

  /* Libera espaco da matriz de distancia */
  for (i = 0; i < k; i++) {
    free(dist[i]);
  }
  free(dist);

  n = strlen(sequences[ancora].valor);

  /* Constroi alinhamento */
  for (i = 0; i < k; i++) { // Em cada etapa, incluímos a sequencia i no alinhamento multiplo
    if (i != ancora) {
      m = strlen(sequences[i].valor);
      max = n > m? n: m;

      /* Alocacoes */
      M = (float **) malloc(sizeof(float *) * (n+1));
      for (l = 0; l <= n; l++) {
        M[l] = (float *) malloc(sizeof(float) * (m+1));
      }

      a = (char *) realloc(a, sizeof(char)*2*max);
      b = (char *) realloc(b, sizeof(char)*2*max);
      memset(a, 0, sizeof(char)*2*max); // Alocacao maior que o necessario
      memset(b, 0, sizeof(char)*2*max); // Alocacao maior que o necessario

      /* Computa o alinhamento da sequencia ancora com a sequencia i */
      align(M, sequences[ancora].valor,n,sequences[i].valor,m,gap,match,mismatch);
      larguraSA = getUpmostAlignment(M,a,b,sequences[ancora].valor,n,sequences[i].valor,m,gap,match,mismatch);

      for (l = 0; l <= n; l++) {
        free(M[l]);
      }
      free(M);

      reverteString(a);
      reverteString(b);

      if (i == 0 || (i == 1 && ancora == 0)) { /* Primeiro alinhamento */
        larguraMSA = larguraSA;
        strcpy(MSA[ancora],a); // Copia alinhamento gerado para MSA
        strcpy(MSA[i],b);

        if (i < ancora && i+1 == ancora) {
          nextRow = i+2; // Variavel nextRow indica qual sera a proxima linha ocupada por uma sequencia no MSA
        }
        else {
          nextRow = i+1;
        }
      }
      else {
        /* Inclui sequencia i no MSA para ser posteriormente ajustada */
        strcpy(MSA[nextRow],b);

        j = 0; // Indice j aponta a j-esima sequencia do MSA

        memset(ancoraOriginal, 0, sizeof(char)*2*maiorSeq);
        strcpy(ancoraOriginal,MSA[ancora]); // Salva configuracao atual da ancora no MSA para uso futuro

        /* Atualizacao do MSA */
        for (j = 0; j < k; j++) {
          if (MSA[j][0] == 0) {
            continue; // Posicao nao preenchida
          }

          f = 0; // Indice f percorre os elementos da sequencia j no MSA atual
          h = 0; // Indice h percorre a ancora no MSA atualmente
          l = 0; // Indice l percorre a ancora produzida pelo alinhamento com a sequencia i
          g = 0; // Indice g preenche os elementos da j-esima sequencia

          while (h < larguraMSA || l < larguraSA) {
            if (ancoraOriginal[h] == a[l]) { // Se nao e um buraco novo
              aux[g] = MSA[j][f];
              h++;
              l++;
              f++;
            }
            else if (j == nextRow) { // Caso 1: atualizando a sequencia b recem-inserida
              if (ancoraOriginal[h] == '-') { // Se e um buraco novo
                aux[g] = '-';
                h++;
              }
              else {
                aux[g] = MSA[j][f];
                f++;
                l++;
              }
            }
            else if (j == ancora) { // Caso 2: atualizando a ancora
              if (ancoraOriginal[h] == '-') {
                h++;
                f++;
              }
              else {
                l++;
              }

              aux[g] = '-';
            }
            else { // Caso 3: atualizando o resto do MSA
              if (ancoraOriginal[h] == '-') { // Se ja havia um buraco
                aux[g] = MSA[j][f];
                f++;
                h++;
              }
              else {
                aux[g] = '-';
                l++;
              }
            }

            g++;
          }

          if (strlen(aux) > larguraMSA) {
            larguraMSA = strlen(aux);
          }

          strcpy(MSA[j],aux);
        }

        if (i < ancora && i+1 == ancora) {
          nextRow = i+2;
        }
        else {
          nextRow = i+1;
        }
      }
    }
  }

  /* Libera espaço alocado */
  free(aux);
  free(a);
  free(b);
  free(ancoraOriginal);
}
