#include "grasp.h"

void geraSolucaoInicialAleatoria(char **MSA, float gap, float match, float mismatch, int k, 
																 int maiorSeq, Sequencia *seq) {

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
			printf("toto : %s\n",shortStr);
			tempStr = insertGap(shortStr,i,block->end-block->beginning+1);
			if(tempStr != NULL){
				strcpy(buff, ferr->MSA[ferr->index]);
				strcpy(ferr->MSA[ferr->index], tempStr);
				printf("TEST: %s\n",tempStr);
				//free(tempStr);
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
	if(temp->prox != NULL){
		do{
			if(i>temp->beginning && i<= temp->end){
				//printf("I : %d, beg %d end %d\n",i,temp->beginning,temp->end);
				ret = 1;
				break;
			}
			temp = temp->prox;
		}while(temp->prox != NULL);
	}
	return ret;
}
GapBlock* findGapBlocks(char* seq){
 	int i, size = strlen(seq);
 	char* str = malloc(size*sizeof(char));
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
	char* newStr = malloc((strlen(sentence)+n)*sizeof(char)),*gapString = malloc(n*sizeof(char)), *gap = "-";
	int count = 0;
	while(count<n){
		count++;
		strcat(gapString,gap);
		printf("gapString : %s\n", gapString);
	}
	strncpy(newStr,sentence,position);
	strcat(newStr,gapString);
	strncpy(newStr+position+n, sentence+position,strlen(sentence)-position+1);
	return newStr;
}