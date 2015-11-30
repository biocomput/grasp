#include "graspSolucaoAleatoria.h"

void geraSolucaoInicialAleatoria(char **MSA, int k, int maiorSeq, Sequencia *seq) {
	int  i, j, l, n, q, r, ni, *nBuracos, novosBuracos, *qtdeBuracos;
	char *aux;

	qtdeBuracos = (int *)  malloc(sizeof(int)  * 3*maiorSeq);
	aux 				= (char *) malloc(sizeof(char) * 3*maiorSeq);
	nBuracos		= (int *)  malloc(sizeof(int)  * k);

	memset(qtdeBuracos, 0, sizeof(int)  * 3*maiorSeq);
	memset(aux,				  0, sizeof(char) * 3*maiorSeq);

	srand(time(NULL));

	/* Copia sequencias para o MSA */
	for (i = 0; i < k; i++) {
		strcpy(MSA[i], seq[i].valor);
	}

	/* Passo 1: Seja ai a sequencia mais longa do MSA, de tamanho ni. Para cada outra sequencia aj, de tamanho nj,
		 nBuracos = ni-nj equivale ao numero de buracos necessarios para deixar a sequencia aj do tamanho de ai. Alem
		 disso, inclui um numero aleatorio de buracos, limitado pelo dobro do tamanho da maior sequencia */
	novosBuracos = rand() % (int)(2*maiorSeq);
	for (i = 0; i < k; i++) {
		ni 			 		= strlen(seq[i].valor);
		nBuracos[i] = maiorSeq - ni + novosBuracos;
	}

	n = maiorSeq + novosBuracos; // Tamanho de cada sequencia no MSA final

	/* Passo 2: Distribui os buracos em posicoes aleatorias do MSA */
	for (i = 0; i < k; i++) {
		while (nBuracos[i] > 0) {
			qtdeBuracos[rand()%(n-nBuracos[i]--)]++;
		}

		for (r = 0, j = 0, q = 0; r < n; j++) {
			if (qtdeBuracos[j] > 0) {
				for (l = 0; l < qtdeBuracos[j]; l++) {
					aux[r++] = '-';
				}
				qtdeBuracos[j] = 0;
			}
			else {
				if (MSA[i][q] != 0) {
					aux[r++] = MSA[i][q];
				}
				else {
					aux[r++] = '-';
				}
				q++;
			}
		}
		aux[r] = 0;
		strcpy(MSA[i], aux);
	}

	free(nBuracos);
	free(aux);
	free(qtdeBuracos);
}
