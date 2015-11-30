#include "grasp.h"

void grasp(char **MSA, float a, float b, int k, int maiorSeq, Sequencia *seq) {
	float melhorPontuacao = LONG_MIN, score;
	char 	**aux, **melhorSolucao;
	int 	i, igual, rodadasSemMelhorar;

	aux 					= (char **) malloc(sizeof(char *) * k);
	melhorSolucao = (char **) malloc(sizeof(char *) * k);

	for (i = 0; i < k; i++) {
		aux[i]					 = (char *) malloc(sizeof(char) * 3*maiorSeq);
		melhorSolucao[i] = (char *) malloc(sizeof(char) * 3*maiorSeq);

		memset(aux[i], 0, sizeof(char) * 3*maiorSeq);
	}

	rodadasSemMelhorar = 0;
	while (rodadasSemMelhorar < 10) {
		/* Escolhe solucao aleatoria  */
		do {
			for (i = 0; i < k; i++) {
				memset(MSA[i], 0, sizeof(char)*3*maiorSeq);
			}

			geraSolucaoInicialAleatoria(MSA, k, maiorSeq, seq);
			igual = 1;

			for (i = 0; i < k; i++) {
				if (strcmp(aux[i], MSA[i]) != 0) {
					igual = 0;
					break;
				}
			}
		}	while (igual); // Verifica se a solucao nao foi explorada na ultima rodada

		for (i = 0; i < k; i++) {
			memset(aux[i], 0, sizeof(char)*3*maiorSeq);
			strcpy(aux[i], MSA[i]);
		}

		/* Realiza busca local */
		PontuacaoFerramentas* ferr = malloc(sizeof(PontuacaoFerramentas));
		ferr->MSA = MSA;
		ferr->a = a;
		ferr->b = b;
		ferr->maiorSeq = maiorSeq;
		ferr->size = strlen(MSA[0]);
		ferr->numeroSequencia = k;
		ferr->score = pontuacaoMSA(MSA, a, b, k);;
		computeAll2(ferr);
		score = ferr->score;

		/* Atualiza valor da solucao  */
		score = pontuacaoMSA(MSA, a, b, k);
		if (score >= melhorPontuacao) {
			melhorPontuacao = score;

			for (i = 0; i < k; i++) {
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
