#include "utils.h"

float pontuacao(char **MSA, float gap, float match, float mismatch, int k) {
	int i, j, l, n;
	float soma = 0;

	n = strlen(MSA[0]);

	for (i = 0; i < n; i++) {
		for (j = 0; j < k; j++) {
			for (l = j; l < k; l++) {
				if (MSA[j][i] != '-') {
					if (MSA[j][i] == MSA[l][i]) {
						soma += match;
					}
					else if (MSA[l][i] == '-') {
						soma += gap;
					}
					else {
						soma += mismatch;
					}
				}
			}
    }
  }

	return soma;	
}

/**
void reverteString(char *s)

Reverte os caracteres de uma string.

s: string a ser revertida.

A string revertida e armazenada em s.
*/
void reverteString(char *s) {
  int  i, j, n;
  char aux;

  n = strlen(s);

  for (i = 0, j = n-1; i < j; i++, j--) {
    aux = s[i];
    s[i] = s[j];
    s[j] = aux;
  }
}

/**
void leNomesSequencias (FILE *f, Sequencia *seq, int k, int *max)

Le as sequencias fornecidas no arquivo f.

f:   ponteiro para um arquivo
seq: vetor de elementos do tipo Sequencia ja inicializado
k:   numero de sequencias a serem lidas
max: tamanho da maior sequencia

As sequencias lidas sao armazenadas em seq e o tamanho da maior, em max.
*/
void leNomesSequencias (FILE *f, Sequencia *seq, int k, int *max) {
  int i, j, size;
  char c;

  i = 0;
  *max = 0;
  while (i < k) {
    /* Leitura do nome */
    fscanf(f, "%d", &size);
    seq[i].nome = (char *) malloc(sizeof(char) * (size+1));

    fgetc(f); // Elimina espaco

    j = 0;
    while (j < size) {
      c = fgetc(f);
      seq[i].nome[j++] = c;
    }
    seq[i].nome[j] = 0;

    /* Leitura da sequencia */
    fgetc(f); fgetc(f); fgetc(f); // Elimina espaços e dois pontos
    fscanf(f, "%d", &size);
    fgetc(f);

    seq[i].valor = (char *) malloc(sizeof(char) * (size+1));
    j = 0;
    while (j < size) {
      c = fgetc(f);
      seq[i].valor[j++] = c;
    }
    seq[i].valor[j] = 0;

    /* Atualizacao do tamanho da maior sequencia */
    if (size > *max) {
      *max = size;
    }

    i++;
  }
}
