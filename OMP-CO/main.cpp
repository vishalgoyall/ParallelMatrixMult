///////////////////////////////
// Cache-optimised Sequential code to do matrix multiplication of two arrays
///////////////////////////////

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits>

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

	// Array initialization
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mat_A[i][j] = (float)(rand() % 10);
			mat_B[i][j] = (float)(rand() % 10);
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
	
	// print B_T
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			printf("%f\t", mat_B_T[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	
	gettimeofday(&start_time, NULL);
	
	// Transposing the matrix
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mat_B_T[j][i] = mat_B[i][j];
		}
	}


	// Multiplication
	omp_set_num_threads(NUM_OF_THREADS);
	int num_actual_threads;
	float thread_min_val [NUM_OF_THREADS][8];
	int thread_min_row [NUM_OF_THREADS][8];
	int thread_min_col [NUM_OF_THREADS][8];
	for (int i=0; i < NUM_OF_THREADS; i++) thread_min_val[i][0] = min;

	// Case 1:
	/*
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mult_T[i][j] = 0;
			#pragma omp parallel
			{
				int thread_id = omp_get_thread_num();
				int total_threads = omp_get_num_threads();
				if (id == 0) num_actual_threads = total_threads;

				part_sum[id][0] = parallel_vect_mult(id, total_threads, A, B);
			}
			for (int k=0; k<num_actual_threads; k++) mult_T[i][j] += part_sum[id][0];
		}
	}
	*/

	// Case 2:
	int i;
	#pragma omp parallel for
	for (i=0; i< SIZE; i++) {
		int thread_id = omp_get_thread_num();
		for (int j=0; j<SIZE; j++) {
			for (int k=0; k<SIZE; k++) {
				mult_T[i][j] += mat_A[i][k] * mat_B_T[j][k];
			}
			// Trying to find minimum element for each thread
			if (mult_T[i][j] < thread_min_val[thread_id][0]) {
				thread_min_val[thread_id][0] = mult_T[i][j];
				thread_min_row[thread_id][0] = i;
				thread_min_col[thread_id][0] = j;
			}
			//printf("value of mult at %d and %d is %f calculated by %d thread\n", i, j, mult_T[i][j], thread_id);
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
	
	printf("Min value is %f at coordinates (%d, %d)\n", min, min_row, min_col);
	long int start_time_final, stop_time_final;
	start_time_final = (long int)start_time.tv_sec * 1000000 + (long int)start_time.tv_usec;
	stop_time_final = (long int)stop_time.tv_sec * 1000000 + (long int)stop_time.tv_usec;
	//printf("start final time is %ld \n", start_time_final);
	//printf("stop final time is %ld \n", stop_time_final);
	
	printf("multiplication time: %ld microseconds\n", stop_time_final - start_time_final);

}
