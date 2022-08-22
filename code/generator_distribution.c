#include <stdlib.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

#include "generator_distribution.h"

multivariate * generate_gaussian_multivariate(int dim, double param[][2]){
	multivariate *X = (multivariate *) calloc(1, sizeof(multivariate));
	X->cov = gsl_matrix_alloc(dim, dim);
	gsl_matrix_set_zero(X->cov);
	X->mu = gsl_vector_alloc(dim);
	gsl_vector_set_zero(X->mu);

	for (int i=0; i<dim; i++){
		gsl_matrix_set(X->cov, i, i, pow(param[i][0], 2));
		gsl_vector_set(X->mu, i, param[i][1]);
	}
	gsl_linalg_cholesky_decomp1(X->cov);
	return X;

}