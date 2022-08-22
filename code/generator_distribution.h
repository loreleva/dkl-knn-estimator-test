#ifndef generator_distribution_h
#define generator_distribution_h

struct multivariate {
	gsl_vector * mu;
	gsl_matrix * cov;
};
typedef struct multivariate multivariate;

/*generate a multivariate structure with:
	-	mu: vector of means
	-	cov: covariance matrix */
multivariate *generate_gaussian_multivariate(int dim, double param[][2]);

#endif