#include <math.h>
#include <gsl/gsl_vector.h>

#include "lp_distance.h"
#include "knn_divergence.h"

int p = 2; //p value for the Lp distances

double kNN_divergence_1D(int k, double X_samples[], int N, double Y_samples[], int M){
	double sum = 0;
	for (int i=0; i< N; i++){
		double x_i = X_samples[i]; //x_i = ith sample

		//computing distances between x_i and each sample in X_samples
		//X_negh array of distance (i.e., neighbours) of x_i wrt samples in X_samples
		double X_neigh[N-1];

		for (int j=0; j<i; j++){
			X_neigh[j] = lp_distance_1D(x_i, X_samples[j], p);
		}
		for (int j=i+1; j<N; j++){
			X_neigh[j-1] = lp_distance_1D(x_i, X_samples[j], p);
		}

		//kth with for loop
		double X_kth = 0; //will contain the kth NN
		int min_index=0; //index of the last lower NN found

		//until i find the kth NN
		for (int j=0; j<k; j++){
			for (int idx=0; idx<(N-1); idx++){
				if (X_neigh[idx] < X_neigh[min_index]){
					min_index = idx;
				}
			}

			//set to infinity in the array of neigh's the new NN found and save it
			X_kth = X_neigh[min_index];
			X_neigh[min_index] = INFINITY;
		}
	
		//computing distances between X_i and each sample of X
		double Y_neigh[M];
		for (int j=0; j<M; j++){
			Y_neigh[j] = lp_distance_1D(x_i, Y_samples[j], p);
		}

		double Y_kth = 0;
		min_index=0;
		for (int j=0; j<k; j++){
			for (int idx=0; idx<M; idx++){
				if (Y_neigh[idx] < Y_neigh[min_index]){
					min_index = idx;
				}
			}
			Y_kth = Y_neigh[min_index];
			Y_neigh[min_index] = INFINITY;
		}

		sum = sum + log(Y_kth/X_kth) + log((M / (double) (N-1)));
	}
	double DL = (((double) 1)/N) * sum;
	return DL;
}

double kNN_divergence_ND(int dim, int k, gsl_vector **X_samples, int N, gsl_vector **Y_samples, int M){
	double sum=0;
	for (int i=0; i<N; i++){
		gsl_vector *x_i = X_samples[i];

		//computing distances for samples of X
		double X_neigh[N-1];
		double X_distances = 0;
		
		for (int j=0; j<i; j++){
			X_neigh[j] = lp_distance_ND(x_i, X_samples[j], dim, p);
		}
		for (int j=i+1; j<N; j++){
			X_neigh[j-1] = lp_distance_ND(x_i, X_samples[j], dim, p);
		}

		//kth with for loop
		double X_kth = 0;
		int min_index=0;
		for (int j=0; j<k; j++){
			for (int idx=0; idx<(N-1); idx++){
				if (X_neigh[idx] < X_neigh[min_index]){
					min_index = idx;
				}
			}
			X_kth = X_neigh[min_index];
			X_neigh[min_index] = INFINITY;
		}

		double Y_neigh[M];
		double Y_distances = 0;
		for (int j=0; j<M; j++){
			Y_neigh[j] = lp_distance_ND(x_i, Y_samples[j], dim, p);
		}
		
		double Y_kth = 0;
		min_index=0;
		for (int j=0; j<k; j++){
			for (int idx=0; idx<M; idx++){
				if (Y_neigh[idx] < Y_neigh[min_index]){
					min_index = idx;
				}
			}
			Y_kth = Y_neigh[min_index];
			Y_neigh[min_index] = INFINITY;
		}

		sum = sum + log(Y_kth/X_kth);
		
	}
	sum = sum + (N * log(M / (double) (N-1)));

	//computing the result of the estimation of the DL divergence
	double DL =   (((double) dim)/N) * sum;
	return DL;
}