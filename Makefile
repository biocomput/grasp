all:	star grasp
star:	main-star.c needleman-wunsch.c star-multiple-alignment.c utils.c
	gcc needleman-wunsch.c star-multiple-alignment.c utils.c main-star.c -o star -lm -Wall -fsanitize=address -g
grasp:	main-grasp.c grasp.c utils.c
	gcc main-grasp.c grasp.c utils.c -o grasp -lm -Wall -fsanitize=address -g
clean:
	rm *.o star grasp
