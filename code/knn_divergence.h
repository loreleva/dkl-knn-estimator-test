#ifndef knn_divergence_h
#define knn_divergence_h

double kNN_divergence_1D(int k, double *X_samples, int N, double *Y_samples, int M);

double kNN_divergence_ND(int dim, int k, gsl_vector **X_samples, int N, gsl_vector **Y_samples, int M);

#endif