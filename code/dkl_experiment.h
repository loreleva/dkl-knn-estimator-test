#ifndef dkl_experiment_h
#define dkl_experiment_h

#include <stdbool.h>

//RUN THE D_KL ESTIMATION, FOR EACH DIMENSION, FOR EACH PARAMETERS OF THE DISTRIBUTIONS, THE TESTS ARE RUNNED UNTIL THE DIFFERENCE OF 5 CONSECUTIVE EXECUTION ARE NO MORE DIFFERENT OF 0,1
void run_experiment_convergence(int start_dim, int max_dim, int init_samples, int epochs, int k, int max_updates, double param_update, int max_size_stack_convergence, bool write_csv, bool write_backup, int n_stack_values, double *stack_values);

void run_experiment_bias_variance_convergence(int init_samples, int k);

double slope_linear_regression(double *x, double *y, double n);

void intHandler(int dummy);

char * format_time(int seconds);

double theoretical_variance(int N, int M);

double variance(double a[], int n);

#endif