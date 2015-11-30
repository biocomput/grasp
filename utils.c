#include "utils.h"

int blosum62[20][20] = {
/*			0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 18 19
        C  S  T  P  A  G  N  D  E  Q  H  R  K  M  I  L  V  F  Y  W */
/*C*/ { 9,-1,-1,-3, 0,-3,-3,-3,-4,-3,-3,-3,-3,-1,-1,-1,-1,-2,-2,-2},
/*S*/ {-1, 4, 1,-1, 1, 0, 1, 0, 0, 0,-1,-1, 0,-1,-2,-2,-2,-2,-2,-3},
/*T*/ {-1, 1, 5,-1, 0,-2, 0,-1,-1,-1,-2,-2,-1,-2,-3,-3,-2,-4,-3,-4},
/*P*/ {-3,-1,-1, 7,-1,-2,-2,-1,-1,-1,-2,-2,-1,-2,-3,-3,-2,-4,-3,-4},
/*A*/ { 0, 1, 0,-1, 4, 0,-2,-2,-1,-1,-2,-1,-1,-1,-1,-1, 0,-2,-2,-2},
/*G*/ {-3, 0,-2,-2, 0, 6, 0,-1,-2,-2,-2,-2,-2,-3,-4,-4,-3,-3,-3,-2},
/*N*/ {-3, 1, 0,-2,-2, 0, 6, 1, 0, 0, 1, 0, 0,-2,-3,-3,-3,-3,-2,-4},
/*D*/ {-3, 0,-1,-1,-2,-1, 1, 6, 2, 0,-1,-2,-1,-3,-3,-4,-3,-3,-3,-4},
/*E*/ {-4, 0,-1,-1,-1,-2, 0, 2, 5, 2, 0, 0, 1,-2,-3,-3,-2,-3,-2,-3},
/*Q*/ {-3, 0,-1,-1,-1,-2, 0, 0, 2, 5, 0, 1, 1, 0,-3,-2,-2,-3,-1,-2},
/*H*/ {-3,-1,-2,-2,-2,-2, 1,-1, 0, 0, 8, 0,-1,-2,-3,-3,-3,-1, 2,-2},
/*R*/ {-3,-1,-2,-2,-1,-2, 0,-2, 0, 1, 0, 5, 2,-1,-3,-2,-3,-3,-2,-3},
/*K*/ {-1, 0,-1,-1,-1,-2, 0,-1, 1, 1,-1, 2, 5,-1,-3,-2,-2,-3,-2,-3},
/*M*/ {-1,-1,-2,-2,-1,-3,-2,-3,-2, 0,-2,-1,-1, 5, 1, 2, 1, 0,-1,-1},
/*I*/ {-1,-2,-3,-3,-1,-4,-3,-3,-3,-3,-3,-3,-3, 1, 4, 2, 3, 0,-1,-3},
/*L*/ {-1,-2,-3,-3,-1,-4,-3,-4,-3,-2,-3,-2,-2, 2, 2, 4, 1, 0,-1,-2},
/*V*/ {-1,-2,-2,-2, 0,-3,-3,-3,-2,-2,-3,-3,-2, 1, 3, 1, 4,-1,-1,-3},
/*F*/ {-2,-2,-4,-4,-2,-3,-3,-3,-3,-3,-1,-3,-3, 0, 0, 0,-1, 6, 3, 1},
/*Y*/ {-2,-2,-3,-3,-2,-3,-2,-3,-2,-1, 2,-2,-2,-1,-1,-1,-1, 3, 7, 2},
/*W*/ {-2,-3,-4,-4,-2,-2,-4,-4,-3,-2,-2,-3,-3,-1,-3,-2,-3, 1, 2, 11}};

/*						  A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y  Z */
int map[26] = { 4,-1, 0, 7, 8,17, 5,10,14,-1,12,15,13, 6,-1, 3, 9,11, 1, 2,-1,16,19,-1,18,-1};

int pontuacaoCaracteres(char a, char b) {
	return blosum62[ map[(int)a-65] ][ map[(int)b-65] ];
}

int pontuacaoReferencia(char **MSA, char *nome, int k) {
	FILE *f;
	char *fname, **MSARef;
	int	 i, j, l, m, n, p, soma = 0;

	/* Abertura do arquivo com o MSA de referencia do BAliBase */
	fname = (char *) malloc(sizeof(char) * (strlen(nome)+5));

	strcpy(fname, nome);
	strcat(fname, "MULT");

	f = fopen(fname, "rt");

	fscanf(f, "%d", &n);

	/* Leitura do arquivo */
	MSARef = (char **) malloc(sizeof(char *) * k);
	for (i = 0; i < k; i++) {
		fgetc(f);
		MSARef[i] = (char *) malloc(sizeof(char) * (n+1));

		for (j = 0; j < n; j++) {
			MSARef[i][j] = fgetc(f);
		}
		MSARef[i][j] = 0;
	}

	/* Comparacao */
	m = strlen(MSA[0]);
	for (i = 0; i < k; i++) {
		l = 0;

		for (j = 0; j < m; j++) {
			if (MSA[i][j] != '-') { // Busca cada caracter do MSA produzido
				while(l < n && MSA[i][j] != MSARef[i][l]) { // Procura a coluna onde o caracter aparece no MSA de referencia
					l++;
				}

				for (p = i+1; p < k; p++) {
					if (MSARef[p][l] == MSA[p][j] && MSA[p][j] != '-') {
						soma++;
					}
				}
			}
		}
	}

	for (i = 0; i < k; i++) {
		free(MSARef[i]);
	}
	free(MSARef);
	free(fname);
	fclose(f);

	return soma;
}

/**
float pontuacaoMSA(char **MSA, float gap, int k) 

Calcula a pontuacao do alinhamento multiplo em relacao ao esquema de pontuacao pre-definido

MSA: matriz que contem o alinhamento multiplo
gap: penalidade para blocos
k:	 numero de sequencias

Retorna a pontuacao
*/
float pontuacaoMSA(char **MSA, float a, float b, int k) {
	int i, j, l, n, *h;
	float soma = 0;

	n = strlen(MSA[0]);
	h = (int *) malloc(sizeof(int) * k);

	memset(h, 0, sizeof(int)*k);

	for (i = 0; i < n; i++) {
		for (j = 0; j < k; j++) {
			if (MSA[j][i] == '-') {
				if (h[j] == 0) {
					soma += a;
				}
				soma += b;
				h[j]++;
			}
			else {
				h[j] = 0;

				for (l = j+1; l < k; l++) {
					if (MSA[l][i] != '-') {
						soma += pontuacaoCaracteres(MSA[j][i], MSA[l][i]);
					}
				}
			}
    }
  }

	free(h);

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
