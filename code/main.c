#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_fit.h>
#include <signal.h>

#include "print.h"
#include "generator_distribution.h"
#include "sample_distribution.h"
#include "knn_divergence.h"
#include "dkl_experiment.h"
#include "stack.h"

double bias(double a[], int n, double target){
	double bias = 0;
	for (int i=0; i<n; i++){
		bias = bias + fabs(a[i] - target);
	}
	return bias/n;
}

double *from_string_to_double_array(char *s){
	if (s[0]!='0'){
		double *res = (double *) calloc(strlen(s)/3, sizeof(double));
		int t=0;
		int i=1;
		while(1){
			char temp_double[9];
			int k=0;
			while (s[i] != ',' && s[i] != ']'){
				temp_double[k] = s[i];
				k++;
				i++;
			}
			printf("%s\n", temp_double);
			res[t] = atof(temp_double);
			if (s[i] == ']'){
				break;
			}
			t++;
			i++;
		}
		return res;
	}
	return 0;
}
void wrong_number_arguments(){
	fprintf(stderr, "Wrong number of arguments!\nFor HELP run ./main help\n");
	exit(-1);
}

int main(int argc, char *argv[]){
	int align = 35;
	if (argc == 1 | argc > 13){
		wrong_number_arguments();
	}
	if (strncasecmp(argv[1], "help", 4) == 0){
		printf("Usage ./main:\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n"
			"\t%*s\t%*s\n",
			-align,"(int) starting dimension",
			align,"Starting number of distributions' dimensions of the experiment",

			-align,"(int) max dimension",
			align,"Order of magnitude of the maximum dimension (10^max)",

			-align,"(int) starting samples",
			align,"Starting number of samples for each dimension in the experiment",

			-align,"(int) epochs",
			align,"Number of execution of an experiment, for a dimension and a number of samples",

			-align,"(int) k",
			align,"Value of k for the k Nearest Neighbours algorithm",

			-align,"(int) max updates",
			align,"Maximum number of updates of the parameters of the second distribution Y",

			-align,"(double) parameter update",
			align,"Value to add to the mean and std deviation of the distribution Y",

			-align,"(int) max size stack convergence",
			align,"Number of results to compute the linear regression for the stop criterion (i.e., slope < 0.01)",

			-align,"(int) write csv",
			align,"Choose if you want the results in a csv",

			-align,"(int) write backup",
			align,"Choose if you want to save backup files",

			-align,"(int) n stack values",
			align,"number of stack values",

			-align,"(double *) stack_values",
			align,"Starting values of the stack");
		return 0;
	}
	if (argc < 13){
		wrong_number_arguments();
	}

	run_experiment_convergence(
		//int start_dim
		atoi(argv[1]),
		//int max dim
		pow(10, atoi(argv[2])),
		//int init_samples
		atoi(argv[3]),
		//int epochs
		atoi(argv[4]),
		//int k
		atoi(argv[5]),
		//int max_updates
		atoi(argv[6]),
		//double param_update
		atof(argv[7]),
		//int max_size_stack_convergence
		atoi(argv[8]),
		//write_csv
		atoi(argv[9]),
		//write backup files
		atoi(argv[10]),
		//number of stack values
		atoi(argv[11]),
		//stack_values
		from_string_to_double_array(argv[12]));
	return 0;
}


