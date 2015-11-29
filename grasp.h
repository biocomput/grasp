#include "utils.h"

void geraSolucaoInicialAleatoria(char **MSA, float gap, float match, float mismatch, int k, 
																 int maiorSeq, Sequencia *seq);
void grasp(char **MSA, float gap, float match, float mismatch, int k, int maiorSeq, Sequencia *seq);
void constructList();
void improveAlignment();
GapBlock *findGapBlocks(char* seq);
void showGapBlocks(GapBlock* gapArray);
void printGapBlock(GapBlock* toShow);
char* removePart(char* sentence, int beginning, int end);
char* insertGap(char* sentence,int position, int n);
void computeAll(PontuacaoFerramentas* ferr);
void computeSequence(PontuacaoFerramentas* ferr);
void moveBlock(PontuacaoFerramentas* ferr, GapBlock* block);
int isInBlock(GapBlock* block, int i);
