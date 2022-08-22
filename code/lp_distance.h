#ifndef lp_distance_h
#define lp_distance_h

/*compute the lp distance in 1D*/
double lp_distance_1D(double a, double b, int p);

/*compute the lp distance in ND*/
double lp_distance_ND(gsl_vector *a, gsl_vector *b, int size, int p);

#endif