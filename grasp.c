#include "grasp.h"

void geraSolucaoInicialAleatoria(char **MSA, float gap, float match, float mismatch, int k, 
																 int maiorSeq, Sequencia *seq) {

	int i, j, ni, nBuracos, pos, nMax = 0;

	srand(time(NULL));

	/* Copia sequencias para o MSA */
	for (i = 0; i < k; i++) {
		strcpy(MSA[i], seq[i].valor);
	}

	/* Passo 1: Seja ai a sequencia mais longa do MSA, de tamanho ni. Para cada outra sequencia aj, de tamanho nj,
		 inclui ni-nj buracos no fim de aj. */
	for (i = 0; i < k; i++) {
		ni = strlen(seq[i].valor);
		nBuracos = maiorSeq - ni;

		if (nBuracos > nMax) {
			nMax = nBuracos;
		}

		for (j = ni; j < ni + nBuracos; j++) {
			MSA[i][j] = '-';
		}
	}

	/* Passo 2: Inclui mais um numero aleatorio de buracos nas sequencias, limitado pelo maior numero de buracos */
	nBuracos = rand() % (int)(2*(nMax+1));

	for (i = 0; i < k; i++) {
		for (j = maiorSeq; j < maiorSeq + nBuracos; j++) {
			MSA[i][j] = '-';
		}
	}

	/* Passo 3: Move um buraco do fim para uma posicao aleatoria do MSA */
	for (i = 0; i < k; i++) {
		nBuracos = strlen(MSA[i]) - strlen(seq[i].valor);

		while (nBuracos > 0) {
			pos = rand()%(strlen(MSA[i])-nBuracos);

			j = strlen(MSA[i]) - nBuracos;
			for (; j >= pos + 1; j--) {
				MSA[i][j] = MSA[i][j-1];
			}

			MSA[i][pos] = '-';
			nBuracos--;
		}
	}
}

void grasp(char **MSA, float gap, float match, float mismatch, int k, int maiorSeq, Sequencia *seq) {
	float melhorPontuacao = LONG_MIN, score;
	char **aux, **melhorSolucao;
	int i, igual, rodadasSemMelhorar;

	aux = (char **) malloc(sizeof(char *) * k);
	melhorSolucao = (char **) malloc(sizeof(char *) * k);

	for (i = 0; i < k; i++) {
		aux[i] = (char *) malloc(sizeof(char) * 3*maiorSeq);
		melhorSolucao[i] = (char *) malloc(sizeof(char) * 3*maiorSeq);
		aux[i][0] = '-';
	}

	rodadasSemMelhorar = 0;
	while (rodadasSemMelhorar < 10) {
		/* Escolhe solucao aleatoria */	
		do {
			for (i = 0; i < k; i++) {
				memset(MSA[i], 0, sizeof(char)*3*maiorSeq);
			}

			geraSolucaoInicialAleatoria(MSA, gap, match, mismatch, k, maiorSeq, seq);
			igual = 1;

			for (i = 0; i < k; i++) {
				if (strcmp(aux[i], MSA[i]) != 0) {
					igual = 0;
					break;
				}
			}
		}
		while (igual); // Verifica se a solucao nao foi explorada na ultima rodada

		for (i = 0; i < k; i++) {
			memset(aux[i], 0, sizeof(char)*3*maiorSeq);
			strcpy(aux[i], MSA[i]);
		}

		/* Realiza busca local */

		/* Atualiza valor da solucao */
		score = pontuacao(MSA, gap, match, mismatch, k);
		if (score >= melhorPontuacao) {
			melhorPontuacao = score;

			for (i = 0; i < k; i++) {
				memset(melhorSolucao[i], 0, sizeof(char)*3*maiorSeq);
				strcpy(melhorSolucao[i], MSA[i]);
			}
			rodadasSemMelhorar = 0;
		}
		else {
			rodadasSemMelhorar++;
		}
	}

	for (i = 0; i < k; i++) {
		memset(MSA[i], 0, sizeof(char)*3*maiorSeq);
		strcpy(MSA[i], melhorSolucao[i]);
		free(melhorSolucao[i]);
		free(aux[i]);
	}

	free(melhorSolucao);
	free(aux);
}
