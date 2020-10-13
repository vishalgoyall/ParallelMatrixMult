///////////////////////////////
//
///////////////////////////////

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits>

#include "main_ispc.h"
using namespace ispc;

#ifndef SIZE
	#define SIZE 10
#endif

#ifndef NUM_OF_THREADS
	#define NUM_OF_THREADS 4
#endif

float mat_A[SIZE][SIZE] = {};
float mat_B[SIZE][SIZE] = {};
float mat_B_T[SIZE][SIZE] = {};
float mult_T[SIZE][SIZE] =  {};

int main() {
	struct timeval start_time, stop_time;
	int min_row = 0;
	int min_col = 0;
	float min = std::numeric_limits<float>::max();

	int num_actual_threads;
	float thread_min_val [NUM_OF_THREADS][8]; // Padding to avoid false sharing
	int thread_min_row [NUM_OF_THREADS][8]; // Padding to avoid false sharing
	int thread_min_col [NUM_OF_THREADS][8]; // Padding to avoid false sharing

	// Array initialization
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mat_A[i][j] = rand()/(float)1147483648;
			mat_B[i][j] = rand()/(float)1147483648;
		}
	}

	/*
	// print A
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			printf("%f\t", mat_A[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	// print B
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			printf("%f\t", mat_B[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	*/
	
	gettimeofday(&start_time, NULL);
	
	// Transposing the matrix
	int ii;
	#pragma omp parallel for
	for (ii=0; ii < SIZE; ii++) {
		for (int j=0; j < SIZE; j++) {
			// Making sure that each thread works on one row of the output matrix;
			// that way there is no false sharing as data to the same row is not 
			// written by different threads
			mat_B_T[ii][j] = mat_B[j][ii];
		}
	}

	/*
	// print B_T
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			printf("%f\t", mat_B_T[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/


	// Multiplication
	omp_set_num_threads(NUM_OF_THREADS);
	for (int i=0; i < NUM_OF_THREADS; i++) thread_min_val[i][0] = min;

	int i;
	#pragma omp parallel for
	for (i=0; i< SIZE; i++) {
		int thread_id = omp_get_thread_num();
		for (int j=0; j<SIZE; j++) {
			mult_T[i][j] = vector_mult(mat_A[i], mat_B_T[j], SIZE);
			// Trying to find minimum element for each thread
			if (mult_T[i][j] < thread_min_val[thread_id][0]) {
				thread_min_val[thread_id][0] = mult_T[i][j];
				thread_min_row[thread_id][0] = i;
				thread_min_col[thread_id][0] = j;
			}
		}
	}

	// IMPLICIT BARRIER
	
	// Once all the threads have done the work,
	// time to find the min element from the pool
	// of minimum numbers
	for (int i=0; i< NUM_OF_THREADS; i++) {
		if (thread_min_val[i][0] < min) {
			min = thread_min_val[i][0];
			min_row = thread_min_row[i][0];
			min_col = thread_min_col[i][0];
		}
	}

	//printf("\n");
	gettimeofday(&stop_time, NULL);

	/*
	// print B_T
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			printf("%f\t", mult_T[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	
	printf("\n*****************************\n");
	printf("OMP-CO-ISPC code\n");
	printf("Min value is %f at coordinates (%d, %d)\n", min, min_row, min_col);
	long int start_time_final, stop_time_final;
	start_time_final = (long int)start_time.tv_sec * 1000000 + (long int)start_time.tv_usec;
	stop_time_final = (long int)stop_time.tv_sec * 1000000 + (long int)stop_time.tv_usec;
	
	printf("multiplication time: %ld microseconds\n", stop_time_final - start_time_final);
	printf("\n*****************************\n");

}
