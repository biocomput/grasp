#include "graspBuscaLocal.h"

void computeAll(PontuacaoFerramentas* ferr) {
	int i;
	char**  MSA = (char **) malloc(sizeof(char *) * ferr->size);

 	for (i = 0; i < ferr->size; i++) {
	 	MSA[i] = (char *) malloc(sizeof(char) * 3*ferr->maiorSeq);
   	memset(MSA[i], 0, sizeof(char)*3*ferr->maiorSeq);
   	strcpy(MSA[i],ferr->MSA[i]);
  }
	
	for(i = 0; i < ferr->size; i++){
		ferr->index = i;
		computeSequence(ferr);
	}
}

void computeSequence(PontuacaoFerramentas* ferr) {
	GapBlock* blocks = findGapBlocks(ferr->MSA[ferr->index]),*temp;
	temp = blocks;

	while(temp->prox != NULL){
		moveBlock(ferr,temp);
		temp = temp->prox;
	}
}

void moveBlock(PontuacaoFerramentas* ferr, GapBlock* block) {
	int i;
	float score;
	char* shortStr = removePart(ferr->MSA[ferr->index],block->beginning, block->end+1), *tempStr, *buff ;

	buff = malloc((ferr->maiorSeq+1)*sizeof(char)*5);
	GapBlock* blockShort = findGapBlocks(shortStr);

	for(i = 0; i <= strlen(shortStr); i++){
		if(isInBlock(blockShort,i) == 0){
			tempStr = insertGap(shortStr,i,block->end-block->beginning+1);

			if(tempStr != NULL){
				strcpy(buff, ferr->MSA[ferr->index]);
				strcpy(ferr->MSA[ferr->index], tempStr);
				//free(tempStr);
				score = pontuacaoMSA(ferr->MSA, ferr->a, ferr->b, ferr->size);

				if(ferr->score <score){
					ferr->score = score;
					//printf("new Score : %f\n", score);
				}
				else{
					strcpy(ferr->MSA[ferr->index], buff);
				}
			}			
			//printf("etape %d : %s\n",i,tempStr);
		}
	}
}

int isInBlock(GapBlock* block, int i) {
	GapBlock* temp = block;
	int ret = 0;

	if(temp->prox != NULL) {
		do {
			if(i > temp->beginning && i <= temp->end) {
				//printf("I : %d, beg %d end %d\n",i,temp->beginning,temp->end);
				ret = 1;
				break;
			}
			temp = temp->prox;
		} while(temp->prox != NULL);
	}
	return ret;
}

GapBlock* findGapBlocks(char* seq) {
	int i, size = strlen(seq);
	char* str = malloc((size+1)*sizeof(char));

	strcpy(str, seq);
	GapBlock *current, *gapArray;
	gapArray = malloc(sizeof(GapBlock));
	gapArray->prox = NULL;
	current = gapArray;

  for(i = 0;i<size;i++){
 		if(str[i]=='-'){
 			current->beginning = i;

 			do{
 				i++;
 			} while(str[i] =='-');

	 		current->end = i-1;
	 		current->prox = malloc(sizeof(GapBlock));
	 		current = current->prox;
	 		current->prox = NULL;
	 	}
 }

 return gapArray;
}

char* removePart(char* sentence, int beginning, int end) {
	int strSize = strlen(sentence), size;
	char *newStr;
	
	if(beginning<=end && strSize>=end){
		size = end-beginning;
		newStr = (char*) malloc((strlen(sentence)-size+2)*sizeof(char));
		strncpy(newStr,sentence,beginning);
		strncpy(newStr+beginning, sentence+end,strlen(sentence)-end+1);

		return newStr;
	}

	return NULL;	
}

char* insertGap(char* sentence,int position, int n) {
	char* newStr = malloc((strlen(sentence)+n+1)*sizeof(char)),*gapString = malloc((n+1)*sizeof(char));
	int count = 0;

	while(count < n){
		gapString[count] = '-';
		count++;
		gapString[count] = '\0';
	}

	strncpy(newStr,sentence,position);
	strncpy(newStr+position,gapString,n);
	strncpy(newStr+position+n, sentence+position,strlen(sentence)-position+1);
	
	return newStr;
}
