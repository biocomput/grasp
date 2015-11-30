/**
needleman-wunsch.c

Implementa rotinas para realizar o alinhamento global entre duas sequencias com o algoritmo de Needleman-Wunsch.

Autor: Guilherme Colucci Pereira
*/

#include "global-alignment.h"

/**
void getUpmostAlignment(float **M, char *aa1, char *aa2, char* a1, int n1, char *a2, int n2, int max, float gap)

Dada uma matriz de alinhamento, calcula o alinhamento correspondente atraves do criterio upmost.

Mdiag: matriz de alinhamento ja inicializada
Mleft: matriz de alinhamento ja inicializada
Mup:   matriz de alinhamento ja inicializada
aa1:   string ja alocada onde sera guardado o valor da sequencia a1 no alinhamento
aa2:   string ja alocada onde sera guardado o valor da sequencia a2 no alinhamento
a1:    sequencia alinhada
n1:    tamanho de a1
a2:    sequencia alinhada
n2:    tamanho de a2
a:     penalidade para abrir um bloco de buracos
b: 		 penalidade para extender um bloco de buracos

Valores de a1 e a2 no alinhamento sao guardados nas variaveis aa1 e aa2, respectivamente.
*/
int getUpmostAlignment(float **Mdiag, float **Mleft, float **Mup, char *aa1, char *aa2, 
												 char* a1, 			int n1, 			 char *a2, 		int n2, 	 float a,   float b) {

	int 	i, j, k, mAtual;
	float **M, max, aux;

	i = n1;
	j = n2;
	k = 0;

	max 	 = Mdiag[n1][n2];
	M 	 	 = Mdiag;
	mAtual = 1;

	if (Mleft[n1][n2] > max) {
		max 	 = Mleft[n1][n2];
		M 		 = Mleft;
		mAtual = 2;
	}
	if (Mup[n1][n2] > max) {
		max 	 = Mup[n1][n2];
		M 	 	 = Mup;
		mAtual = 3;
	}
	
	while (i > 0 || j > 0) {
		if (mAtual == 3) {
			if (i > 0) {
				aa1[k] = a1[i-1];
				aa2[k] = '-';

				if(M[i][j] == Mup[i-1][j]+b) {
					M 		 = Mup;
					mAtual = 3;
				}
				else if (M[i][j] == Mdiag[i-1][j]+a+b) {
					M 		 = Mdiag;
					mAtual = 1;
				}
				else {
					M 		 = Mleft;
					mAtual = 2;
				}
				i--;
			}
		}
		else if (mAtual == 1) {
			if (i > 0 && j > 0) {
				aa1[k] = a1[i-1];
				aa2[k] = a2[j-1];

				aux = pontuacaoCaracteres(a1[i-1], a2[j-1]);

				if (M[i][j] == Mup[i-1][j-1]+aux) {
					M 		 = Mup;
					mAtual = 3;
				}
				else if (M[i][j] == Mdiag[i-1][j-1]+aux) {
					M 		 = Mdiag;
					mAtual = 1;
				}
				else {
					M 		 = Mleft;
					mAtual = 2;
				} 
				i--;
				j--;
			}
		}
		else {
			if (j > 0) {
				aa1[k] = '-';
				aa2[k] = a2[j-1];

				if (M[i][j] == Mup[i][j-1]+a+b) {
					M 		 = Mup;
					mAtual = 3;
				}
				else if (M[i][j] == Mdiag[i][j-1]+a+b) {
					M 		 = Mdiag;
					mAtual = 1;
				}
				else {
					M 		 = Mleft;
					mAtual = 2;
				}
				j--;
			}
		}
		k++;
	}

	return k;
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
float align(float **Mdiag, float **Mleft, float **Mup, char *a1, int n1, char *a2, int n2, float a, float b) {
	int i, j;
	float max;
	
	Mdiag[0][0] = 0;
	Mleft[0][0] = LONG_MIN;
	Mup[0][0] = LONG_MIN;

	for (i = 1; i <= n1; i++) {
		Mdiag[i][0] = LONG_MIN;
		Mleft[i][0] = LONG_MIN;
		Mup[i][0] = a+b*i;
	}

	for (i = 1; i <= n2; i++) {
		Mdiag[0][i] = LONG_MIN;
		Mleft[0][i] = a+b*i;
		Mup[0][i] = LONG_MIN;
	}

	for (i = 1; i <= n1; i++) {
		for (j = 1; j <= n2; j++) {
			/* Atualizacao da matriz diagonal */
			max = Mdiag[i-1][j-1]; 
			if (Mleft[i-1][j-1] > max) {
				max = Mleft[i-1][j-1]; 
			}
			if (Mup[i-1][j-1] > max) {
				max = Mup[i-1][j-1];
			}
			Mdiag[i][j] = pontuacaoCaracteres(a1[i-1], a2[j-1]) + max;

			/* Atualizacao da matriz esquerda */
			max = Mdiag[i][j-1]+a+b;
			if (Mleft[i][j-1]+b > max) {
				max = Mleft[i][j-1]+b;
			}
			if (Mup[i][j-1]+a+b > max) {
				max = Mup[i][j-1]+a+b;
			}
			Mleft[i][j] = max;

			/* Atualizacao da matriz pra cima */
			max = Mdiag[i-1][j]+a+b;
			if (Mleft[i-1][j]+a+b > max) {
				max = Mleft[i-1][j]+a+b;
			}
			if (Mup[i-1][j]+b > max) {
				max = Mup[i-1][j]+b;
			}
			Mup[i][j] = max;
		}
	}

	max = Mdiag[n1][n2];

	if (Mleft[n1][n2] > max) {
		max = Mleft[n1][n2];
	}
	if (Mup[n1][n2] > max) {
		max = Mup[n1][n2];
	}

	return max;
}
