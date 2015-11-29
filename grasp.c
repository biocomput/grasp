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
		PontuacaoFerramentas* ferr = malloc(sizeof(PontuacaoFerramentas));
		ferr->MSA = MSA;
		ferr->gap = gap;
		ferr->mismatch = mismatch;
		ferr->match = match;
		ferr->maiorSeq = maiorSeq;
		ferr->size = k;
		ferr->score = 0;
		computeAll(ferr);
		printf("Score : %f\n",ferr->score);
		score = ferr->score;
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
void computeAll(PontuacaoFerramentas* ferr){
	int i;
	char**  MSA = (char **) malloc(sizeof(char *) * ferr->size);
  	for (i = 0; i < ferr->size; i++) {
   	 	MSA[i] = (char *) malloc(sizeof(char) * 3*ferr->maiorSeq); // Exagero de espaco, mas utilizado para evitar problemas com realloc
    	memset(MSA[i], 0, sizeof(char)*3*ferr->maiorSeq);
    	strcpy(MSA[i],ferr->MSA[i]);
 	 }
	for(i=0;i<ferr->size;i++){
		ferr->index = i;
		computeSequence(ferr);
	}
}
void computeSequence(PontuacaoFerramentas* ferr){
	GapBlock* blocks = findGapBlocks(ferr->MSA[ferr->index]),*temp, *before;
	temp = blocks;
	while(temp->prox != NULL){
		moveBlock(ferr,temp);
		temp = temp->prox;
	}
	printf("(On sort?)\n");
}
void moveBlock(PontuacaoFerramentas* ferr, GapBlock* block){
	int i;
	float score;
	char* shortStr = removePart(ferr->MSA[ferr->index],block->beginning, block->end+1), *tempStr, *buff ;
	buff = malloc(ferr->maiorSeq);
	GapBlock* blockShort = findGapBlocks(shortStr);
	for(i=0;i<=strlen(shortStr);i++){

		if(isInBlock(blockShort,i) == 0){
			tempStr = insertGap(shortStr,i,block->end-block->beginning+1);
			if(tempStr != NULL){
				strcpy(buff, ferr->MSA[ferr->index]);
				strcpy(ferr->MSA[ferr->index], tempStr);
				printf("TEST: %s\n",tempStr);
				free(tempStr);
				score = pontuacao(ferr->MSA,ferr->gap, ferr->match,ferr->mismatch,ferr->size);
				if(ferr->score <score){
					ferr->score = score;
					printf("new Score : %f\n", score);
				}else{
					strcpy(ferr->MSA[ferr->index], buff);
				}
			}
			
			//printf("etape %d : %s\n",i,tempStr);
		}
	}
}
int isInBlock(GapBlock* block, int i){
	GapBlock* temp = block;
	int ret = 0;
	do{
		printf("deb\n");
		if(i>temp->beginning && i<= temp->end){
			//printf("I : %d, beg %d end %d\n",i,temp->beginning,temp->end);
			ret = 1;
			break;
		}
		temp = temp->prox;
		printf("fin\n");
	}while(temp->prox != NULL);
	return ret;
}
GapBlock* findGapBlocks(char* seq){
 int i, size = strlen(seq);
 char* str = malloc(size);
 strcpy(str, seq);
 GapBlock *current, *gapArray, *before;
 gapArray = malloc(sizeof(GapBlock));
  gapArray->prox = NULL;
  current = gapArray;
 for(i = 0;i<size;i++){
 	if(str[i]=='-'){
 		current->beginning = i;
 		do{
 			i++;
 		}while(str[i] =='-');
 		
 		current->end = i-1;
 		current->prox = malloc(sizeof(GapBlock));
 		before = current;
 		current = current->prox;
 		current->prox = NULL;
 	}
 }
 return gapArray;
}
char* removePart(char* sentence, int beginning, int end){
	int strSize = strlen(sentence);
	if(beginning<=end && strSize>=end){
		int size = end-beginning;
		char *newStr = malloc((strlen(sentence)-size+1)*sizeof(char));
		strncpy(newStr,sentence,beginning);
		strncpy(newStr+beginning, sentence+end,strlen(sentence)-end+1);
		return newStr;
	}
	return NULL;
	
}
char* insertGap(char* sentence,int position, int n){
	printf("T  : %d\n", strlen(sentence));
	char* newStr = malloc(strlen(sentence)+n),*gapString = malloc(n), *gap = "-";
	printf("A\n");

	int count = 0;
	printf("G\n");

	while(count<n){
		count++;
	printf("E\n");

		strcat(gapString,gap);
	}
	printf("O\n");

	strncpy(newStr,sentence,position);
	strcat(newStr,gapString);
	strncpy(newStr+position+n, sentence+position,strlen(sentence)-position+1);
	printf("i\n");

	return newStr;
}