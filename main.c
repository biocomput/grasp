/**
main_star.c

Implementa rotina main

Autor: Guilherme Colucci Pereira
*/

#include "star-multiple-alignment.h"
#include "grasp.h"

/** Funcao principal */
int main(int argc,char *argv[]) {
  int 					 i, k, max = 0;
  float		  		 valorOtimo, a = -5, b = -1;
  char 			     **MSA, *fName;
  Sequencia			 *seq;
  FILE 					 *f, *f2;
	struct timeval start, end;

	/* Parametros de entrada */
  if (argc < 3 || (strcmp(argv[1], "star") != 0 && strcmp(argv[1], "grasp") != 0)) {
    printf("Uso correto: ./proj <star/grasp> <filename>\n");
    exit(1);
  }

  if ((f = fopen(argv[2], "rt")) == NULL) {
    printf("Arquivo inexistente.\n");
    exit(1);
  }

	/* Leitura do arquivo */
	fscanf(f, "%d %f", &k, &valorOtimo);

  seq = (Sequencia *) malloc(sizeof(Sequencia) * k);
  leNomesSequencias(f, seq, k, &max);

	/* Alocacao do MSA */
  MSA = (char **) malloc(sizeof(char *) * k);
  for (i = 0; i < k; i++) {
    MSA[i] = (char *) malloc(sizeof(char) * 3*max); // Exagero de espaco, mas utilizado para evitar problemas com realloc
    memset(MSA[i], 0, sizeof(char)*3*max);
  }

	/* Invocacao da funcao principal */
	gettimeofday(&start, NULL);
	if (strcmp(argv[1], "star") == 0) {
	  starAlign(MSA, a, b, k, max, seq);
	}
	else {
		grasp(MSA, a, b, k, max, seq);
	}
	gettimeofday(&end, NULL);

	/* Escrita dos resultados */
	fName = (char *) malloc(sizeof(char) * (strlen(argv[2])+11));

	strcpy(fName, argv[2]);
	strcat(fName, ".");
	strcat(fName, argv[1]);
	strcat(fName, ".out");

	f2 = fopen(fName, "wt");

	fprintf(f2, "Tempo gasto (ms):\n%ld\n", (end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
	fprintf(f2, "\nPontuacao em relacao ao esquema utilizado:\n%.2lf\n", pontuacaoMSA(MSA, a, b, k));
	fprintf(f2, "\nPontuacao em relacao ao alinhamento de referencia:\n%d\n", pontuacaoReferencia(MSA, argv[2], k));
	fprintf(f2, "\nAlinhamento produzido:\n");
  for (i = 0; i < k; i++) {
		fprintf(f2, "%s : %s\n", seq[i].nome, MSA[i]);
	}
	
	/* Liberacao da memoria alocada */
	for (i = 0; i < k; i++) {
	  free(seq[i].nome);
    free(seq[i].valor);
    free(MSA[i]);
  }
  free(seq);
  free(MSA);
	free(fName);
  fclose(f);
	fclose(f2);

  return 0;
}
