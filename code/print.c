#include <stdio.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

#include "print.h"


void print_array_double(double a[], int size){
	printf("[ ");
	int i = 0;
	for (i; i<size-1; i++){
		printf("%f, ",a[i]);
	}
	if (size>0){
		printf("%f ]\n", a[i]);
	}
	else printf(" ]\n");
}

void print_gsl_vector(gsl_vector *a, int size){
	printf("[ ");
	int i = 0;
	for (i; i<size-1; i++){
		printf("%f, ", gsl_vector_get(a, i));
	}
	if (size > 0){
		printf("%f ]\n", gsl_vector_get(a, i));
	}
	else printf(" ]\n");
}

void print_gsl_matrix(gsl_matrix *m, int h, int w){
	printf("[");
	for (int y=0; y<h; y++){
		printf(" [");
		for (int x=0; x<w; x++){
			printf(" %f", gsl_matrix_get(m, y, x));
		}
		printf(" ],\n");
	}
	printf("]\n");
}