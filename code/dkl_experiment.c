#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>
#include <gsl/gsl_fit.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include "stack.h"
#include "generator_distribution.h"
#include "sample_distribution.h"
#include "knn_divergence.h"
#include <sys/stat.h>

#include "dkl_experiment.h"
#include "print.h"

static FILE *fpt_convergence=NULL, *fpt_convergence_variance=NULL, *fpt_backup_convergence=NULL;

void sig_int_handler(int signum, siginfo_t *info, void *ptr){
	if (fpt_convergence!=NULL) fclose(fpt_convergence);
	if (fpt_convergence_variance!=NULL) fclose(fpt_convergence_variance);
	exit(0);
}

void catch_sigint(){
	static struct sigaction _sigact;

	memset(&_sigact, 0, sizeof(_sigact));
	_sigact.sa_sigaction = sig_int_handler;
	_sigact.sa_flags = SA_SIGINFO;

	sigaction(SIGINT, &_sigact, NULL);
}

double slope_linear_regression(double *x, double *y, double n){
	double cov00, cov01, cov11, sumsq;
	double c0 = 0, c1 = 0;
	gsl_fit_linear(x, 1, y, 1, n, &c0, &c1, &cov00, &cov01, &cov11, &sumsq);
	double slope = ((2*c1) + c0) - (c1 + c0); // rise in y (points x=1 and x=2) divided by run in x
	return slope;
}

double theoretical_variance(int N, int M){
	return (1./N) + ( (pow(log(M),4) * pow(log(M + N), 2)) / M);
}

double variance(double a[], int n){
	double mean=0;
	for (int i=0; i<n ;i++){
		mean = mean + a[i];
	}
	mean = mean/n;

	double variance = 0;
	for (int i=0; i<n; i++){
		variance = variance + pow(a[i] - mean, 2);
	}
	variance = variance / n;
	return variance;
}

void run_experiment_bias_variance_convergence(int init_samples, int k){
	catch_sigint();
	int epochs = 10;
	double f[] = {1.0, 0.0};
	double g[] = {1.0, 0.0};
	int seed = 2;
	int N = init_samples, M = init_samples;
	double res=0;
	fpt_convergence_variance = fopen("csv/variance_convergence/variance.csv", "w+");
	fprintf(fpt_convergence_variance, "N; Empirical Variance; Theoretical Variance; Constant\n");
	double results_variance[5000];
	int j=0;
	while(1){
		double results[epochs];
		double th_variance = theoretical_variance(N, M);
		printf("N = %d, ", N);
		for (int i=0; i<epochs; i++){
			double *uni_samples[] = {sample_gaussian_univariate(N, f, seed), sample_gaussian_univariate(M, g, (unsigned long int) pow(seed,2))};
			res = kNN_divergence_1D(k, uni_samples[0], N, uni_samples[1], M);
			results[i] = res;

			free(uni_samples[0]);
			free(uni_samples[1]);
			seed += 2;
		}

		results_variance[j] = variance(results, epochs);
		double range[j+1];
		for (int w=0; w<j+1; w++){
			range[w] = w;
		}

		double empirical_variance = fabs(slope_linear_regression(range, results_variance, j+1));

		fprintf(fpt_convergence_variance, "%d; %f; %f; %f\n", N, empirical_variance, th_variance, empirical_variance/th_variance);
		printf("emp var = %f, th var = %f, constant = %f\n", empirical_variance, th_variance, empirical_variance/th_variance);
		N = M = N + 1000;
		j+=1;
	}
}

char * format_time(int seconds){
	int h=0, m=0, s=0;
	m = seconds / 60;
	s = seconds - (m * 60);
	if (m>=60){
		h = m / 60;
		m = m - (h * 60);
	}
	char *res = (char *) calloc(40, sizeof(char));
	snprintf(res, 40, "%d H %d M %d S", h, m, s);
	return res;
}

void run_experiment_convergence(int start_dim, int max_dim, int init_samples, int epochs, int k, int max_updates, double param_update, int max_size_stack_convergence, bool write_csv, bool write_backup, int n_stack_values, double *stack_values){
	catch_sigint();

	if (write_csv){
		mkdir("csv", 0777);
	}

	int size_filename = 80;
	char dirpath_convergence[size_filename];
	memset(dirpath_convergence, 0, sizeof(dirpath_convergence));
	char filename_convergence[size_filename];
	memset(filename_convergence, 0, sizeof(filename_convergence));
	char backup_filename_convergence[size_filename];
	memset(backup_filename_convergence, 0, sizeof(backup_filename_convergence));
	char *open_mode = "a+";

	int seconds = 0;
	double range_stack_linear_regression[max_size_stack_convergence];
	for (int i=0; i<max_size_stack_convergence; i++){
		range_stack_linear_regression[i] = i;
	}
	for (int dim=start_dim; dim <= max_dim; dim = 10*dim){
		printf("Test for dim = %d\n", dim);

		double f[dim][2];
		double g[dim][2];
		double param[] = {1.0, 0.0}; //{sigma, mu}
		for (int i=0; i<dim; i++){
			f[i][0] = param[0];
			f[i][1] = param[1];
			g[i][0] = param[0]; 
			g[i][1] = param[1];
		}

		for (int num_updates=0; num_updates<=max_updates; num_updates++){
			int seed = 2;
			double res = 0;
			int num_results = 0;

			int N = init_samples;
			int M = N;

			stack *s = (stack *) calloc(1, sizeof(stack));
			if (n_stack_values != 0){
				int num_stack_values = 0;
				while (num_stack_values<n_stack_values){
					push(s, stack_values[num_stack_values]);
					num_stack_values++;
				}
			}
			char *content_file = (char *) calloc(14, sizeof(char));
			if (write_csv){
				snprintf(dirpath_convergence, size_filename, "csv/dim_%d", dim);
				snprintf(filename_convergence, size_filename, "%s/%d_updates.csv", dirpath_convergence, num_updates);
				mkdir(dirpath_convergence, 0777);
				fpt_convergence = fopen(filename_convergence, open_mode);
				fprintf(fpt_convergence, "N; DKL; Time\n");
				
				if (write_backup){
					strcpy(content_file, "N; DKL; Time\n");
				}
			}

			int num_backup=0;
			while(1){
				double results[epochs];
				int times[epochs];

				for (int w=0; w<epochs; w++){
					seconds = (int) time(NULL);
					switch (dim){
						case 1:
							double *uni_samples[] = {sample_gaussian_univariate(N, f[0], seed), sample_gaussian_univariate(M, g[0], (unsigned long int) pow(seed,2))};
							res = kNN_divergence_1D(k, uni_samples[0], N, uni_samples[1], M);
							//printf("Result: %f\n",res);
							free(uni_samples[0]);
							free(uni_samples[1]);

					default:
						multivariate *distr[] = {generate_gaussian_multivariate(dim, f), generate_gaussian_multivariate(dim, g)}; //distr[0] = X, distr[1] = Y, distr[i].mu, distr[i].cov
					
					//sampling from the multivariates
						gsl_vector **samples[] = {sample_gaussian_multivariate(dim, N, distr[0], seed), sample_gaussian_multivariate(dim, M, distr[1], (unsigned long int) pow(seed,2))}; //samples[0] vector of samples of X, samples[1] ...
						res = kNN_divergence_ND(dim, k, samples[0], N, samples[1], M);
						//printf("\t\tResult: %g\n", res);
						for (int i=0; i<2; i++){
							gsl_matrix_free(distr[i]->cov);
							gsl_vector_free(distr[i]->mu);
						}
		
						for (int i=0; i<N; i++){
							gsl_vector_free(samples[0][i]);
						}
	
						for (int i=0; i<M; i++){
							gsl_vector_free(samples[1][i]);
						}
						
						free(distr[0]);
						free(distr[1]);
						free(samples[0]);
						free(samples[1]);
					}

					results[w] = res;
					times[w] = (int) (time(NULL) - seconds);
					seed+=1;								
				}
				double mean_results = 0;
				int mean_seconds = 0;
				for (int w=0; w<epochs; w++){
					mean_results += results[w];
					mean_seconds += times[w];
				}

				char *res_time = format_time(mean_seconds/epochs);
				res = mean_results/epochs;

				printf("Dimension: %d, Updates: %d, DKL: %f, N: %d, Time: %s\n, g_mu=%f, g_sigma=%f\n", dim, num_updates, res, N, res_time, g[0][1], g[0][0]);
				
				if (write_csv){
					int size_result = 6 + 20 + strlen(res_time) + 6;
					char temp_res[size_result];
					snprintf(temp_res, size_result, "%d; %f; %s\n", N, res, res_time);
					fputs(temp_res, fpt_convergence);
					
					if (write_backup){
						content_file = (char *) realloc(content_file, strlen(content_file) + strlen(temp_res) + 1);
						strcat(content_file, temp_res);
						snprintf(backup_filename_convergence, size_filename, "%s/%d_updates_backup_%d.csv", dirpath_convergence, num_updates, num_backup);
						fpt_backup_convergence = fopen(backup_filename_convergence, open_mode);
						fputs(content_file, fpt_backup_convergence);
						fclose(fpt_backup_convergence); 
						fpt_backup_convergence = NULL;
						memset(backup_filename_convergence, 0, sizeof(backup_filename_convergence));
					}


				}
				free(res_time);
				push(s, res);
				//IMPLEMENT THE AUGMENTING OF NUMBER OF SAMPLES UNTIL SLOPE OF LAST RESULTS IS NEAR 0
				if (s->size == max_size_stack_convergence){
					double *stack_values = get_stack_values(s);
					double slope = fabs(slope_linear_regression(range_stack_linear_regression, stack_values, max_size_stack_convergence));
					if (slope < 0.01){
						print_stack(s);
						printf("Slope: %f\n\n", slope);
						free(stack_values);
						break;
					}
					print_stack(s);
					free(stack_values);
					pop(s);
				}

				num_backup += 1;
				N = M = N + 1000;
			}

			free_stack(s);
			for (int i=0; i<dim; i++){
				g[i][0] = g[0][0] + param_update;
				g[i][1] = g[0][1] + param_update;
			}

			if (write_csv){
				fclose(fpt_convergence);
				fpt_convergence = NULL;
				memset(filename_convergence, 0, sizeof(filename_convergence));
				memset(dirpath_convergence, 0, sizeof(dirpath_convergence));
			}
			free(content_file);
		}
	}
}