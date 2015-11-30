#include "utils.h"

void 			computeAll		 (PontuacaoFerramentas* ferr);
void 			computeSequence(PontuacaoFerramentas* ferr);
void 			moveBlock			 (PontuacaoFerramentas* ferr, GapBlock* block);
int 			isInBlock			 (GapBlock* block, int i);
GapBlock* findGapBlocks	 (char* seq);
char* 		removePart		 (char* sentence, int beginning, int end);
char* 		insertGap			 (char* sentence,int position, int n);
