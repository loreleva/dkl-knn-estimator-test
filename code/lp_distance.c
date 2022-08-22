#include <math.h>
#include <gsl/gsl_vector.h>

#include "lp_distance.h"

double lp_distance_1D(double a, double b, int p){
	return pow(pow((a - b), p), (double) 1/p);
}

double lp_distance_ND(gsl_vector *a, gsl_vector *b, int size, int p){
	double sum = 0;
	for (int i=0; i<size; i++){
		sum = sum + pow(fabs(gsl_vector_get(a, i) - gsl_vector_get(b, i)), p);
	}
	return pow(sum, 1/ (double) p);
}