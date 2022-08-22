#include <stdlib.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>

#include "sample_distribution.h"
#include "generator_distribution.h"

double * sample_gaussian_univariate(int N, double f[], unsigned long int seed){
	double *samples = (double *) calloc(N, sizeof(double));
	if (samples == NULL){
		printf("ERROR! memory for samples not allocated.\n");
		exit(1);
	}

	//init random number generator taus
	gsl_rng *r = gsl_rng_alloc(gsl_rng_taus);

	gsl_rng_set(r, seed);
	for (int i=0; i<N; i++){
		//sample from a gaussian distribution with variance f[0] and mean f[1]
		samples[i] = gsl_ran_gaussian(r, f[0]) + f[1];
	}

	gsl_rng_free(r);
	return samples;
}

gsl_vector ** sample_gaussian_multivariate(int dim, int N, multivariate *distr, unsigned long int seed){
	gsl_vector **samples = (gsl_vector **) calloc(N, sizeof(gsl_vector *));
	if (samples == NULL){
		printf("ERROR! memory for samples not allocated.\n");
		exit(1);
	}

	//init random number generator taus
	gsl_rng *r = gsl_rng_alloc(gsl_rng_taus);
	gsl_rng_set(r, seed);

	gsl_vector *temp_samples = 0;
	for (int i=0; i<N; i++){
		temp_samples = gsl_vector_calloc(dim);
		gsl_ran_multivariate_gaussian(r, distr->mu, distr->cov, temp_samples);
		samples[i] = temp_samples;
	}

	gsl_rng_free(r);
	return samples;
}