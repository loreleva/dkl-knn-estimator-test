#ifndef print_h
#define print_h

/*print an array a of doubles with size elements*/
void print_array_double(double a[], int size);

/*print a gsl vector with size elements*/
void print_gsl_vector(gsl_vector *a, int size);

/*print a gsl matrix with size h x w*/
void print_gsl_matrix(gsl_matrix *m, int h, int w);

#endif