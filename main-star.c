/**
main_star.c

Implementa rotina main para alinhamento estrela

Autor: Guilherme Colucci Pereira
*/

#include "star-multiple-alignment.h"
#include "utils.h"

/** Funcao principal */
int main(int argc,char *argv[]) {
  int i, k, max = 0;
  float valorOtimo, gap = -5, match = 3, mismatch = -2;
  char **MSA;
  Sequencia *seq;
  FILE *f;

  if (argc < 2) {
    printf("Uso correto: ./star <filename>\n");
    exit(1);
  }

  if ((f = fopen(argv[1], "rt")) == NULL) {
    printf("Arquivo inexistente.\n");
    exit(1);
  }
	printf("eap\n");
	fscanf(f, "%d %f", &k, &valorOtimo);

  seq = (Sequencia *) malloc(sizeof(Sequencia) * k);

  leNomesSequencias(f, seq, k, &max);

  MSA = (char **) malloc(sizeof(char *) * k);
  for (i = 0; i < k; i++) {
    MSA[i] = (char *) malloc(sizeof(char) * 3*max); // Exagero de espaco, mas utilizado para evitar problemas com realloc
    memset(MSA[i], 0, sizeof(char)*3*max);
  }

  starAlign(MSA, gap, match, mismatch, k, max, seq);
	printf("Pontuacao: %.2lf\n", pontuacao(MSA, gap, match, mismatch, k));

  for (i = 0; i < k; i++) {
		printf("%s\n", MSA[i]);
	  free(seq[i].nome);
    free(seq[i].valor);
    free(MSA[i]);
  }
  free(seq);
  free(MSA);

  fclose(f);

  return 0;
}
