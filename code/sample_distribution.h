#ifndef sample_distribution_h
#define sample_distribution_h

/*structure multivariate with 2 fields:
	- gsl_vector mu (mean)
	- gsl_matrix cov (covariance)
*/
typedef struct multivariate multivariate;

/*sample N samples from gaussian distribution f=(mu, cov)*/
double * sample_gaussian_univariate(int N, double f[], unsigned long int seed);

/*sample N samples, each of dimension dim, from a multivariate gaussian distribution distr (distr.mu mean vector, dist.cov covariance matrix)*/
gsl_vector ** sample_gaussian_multivariate(int dim, int N, multivariate *distr, unsigned long int seed);

#endif