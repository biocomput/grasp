all:	
	gcc global-alignment.c star-multiple-alignment.c utils.c grasp.c graspBuscaLocal.c graspBuscaLocalOld.c graspSolucaoAleatoria.c main.c -o proj -lm -Wall -fsanitize=address -g
clean:
	rm *o star grasp
