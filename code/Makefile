main: main.o dkl_experiment.o generator_distribution.o knn_divergence.o lp_distance.o print.o sample_distribution.o stack.o
	gcc *.o -o main -lm -lgsl -lgslcblas

main.o: main.c
	gcc -c main.c

generator_distribution.o: generator_distribution.h generator_distribution.c
	gcc -c generator_distribution.c

knn_divergence.o: knn_divergence.h knn_divergence.c
	gcc -c knn_divergence.c

lp_distance.o: lp_distance.h lp_distance.c
	gcc -c lp_distance.c

print.o: print.h print.c
	gcc -c print.c

sample_distribution.o: sample_distribution.h sample_distribution.c
	gcc -c sample_distribution.c

stack.o: stack.h stack.c
	gcc -c stack.c

dkl_experiment.o: dkl_experiment.h dkl_experiment.c
	gcc -c dkl_experiment.c

clean:
	@(rm main *.o > /dev/null 2>&1  && echo All cleaned!) || (echo No files to remove)
	