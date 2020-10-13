///////////////////////////////
//
///////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
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

typedef struct {
	float (*A)[SIZE];
	float (*B)[SIZE];
	float (*mult_T)[SIZE];
	int start_row;
	int num_rows;
	int thread_no;
	float min_elem;
	int min_elem_row;
	int min_elem_col;
} args;

void *sub_matrix_mult (void* thread_args) {
	args *local_args = (args*)thread_args;
	local_args->min_elem = std::numeric_limits<float>::max();
	local_args->min_elem_row = local_args->start_row;
	local_args->min_elem_col = 0;
	for (int i=local_args->start_row; i < (local_args->start_row + local_args->num_rows); i++) {
		for (int j=0; j<SIZE; j++) {
			local_args->mult_T[i][j] = vector_mult(local_args->A[i], local_args->B[j], SIZE);
			// Finding the minimum element for this thread's sub-section
			if (local_args->mult_T[i][j] < local_args->min_elem) {
				local_args->min_elem = local_args->mult_T[i][j];
				local_args->min_elem_row = i;
				local_args->min_elem_col = j;
			}
		}
	}
	//local_args->mult_T[local_args->start_row][0] = local_args->thread_no;
}

int main() {
	struct timeval start_time, stop_time;
	int min_row = 0;
	int min_col = 0;
	float min = std::numeric_limits<float>::max();

	args thread_args[NUM_OF_THREADS];
	pthread_t threads[NUM_OF_THREADS];
	
	int min_elem_per_thread = SIZE/NUM_OF_THREADS;
	int remainder_elems = (SIZE%NUM_OF_THREADS);
	
	// Array initialization
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mat_A[i][j] = rand()/(float)1147483648;
			mat_B[i][j] = rand()/(float)1147483648;
		}
	}


	int elems_in_partition[NUM_OF_THREADS] = {};
	int elems_processed = 0;

	gettimeofday(&start_time, NULL);

	for (int i=0; i<NUM_OF_THREADS; i++) {
		elems_in_partition[i] = min_elem_per_thread + (int)(remainder_elems != 0);
		if (remainder_elems > 0)
			remainder_elems--;
	}

	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mat_B_T[j][i] = mat_B[i][j];
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
	
	// Multiplication
	for (int i=0; i < NUM_OF_THREADS; i++) {
		thread_args[i].A = mat_A;
		thread_args[i].start_row = elems_processed;
		thread_args[i].num_rows = elems_in_partition[i];
		//printf("elements in this partition is %d and \n", elems_in_partition[i]);
		//printf ("for thread %d address of A is %x or %x\n", i, &mat_A , mat_A);
		thread_args[i].B = mat_B_T;
		thread_args[i].mult_T = mult_T;
		thread_args[i].thread_no = i;
		//printf ("in loop %d , thread_args->A : %x \n", i, thread_args[i].A);
		pthread_create(&threads[i], NULL, sub_matrix_mult, (void *) &thread_args[i]);
		elems_processed += elems_in_partition[i];
	}
	
	// Join different threads
	for (int i=0; i< NUM_OF_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	
	// Finding minimum elements among different threads' partitions
	for (int i=0; i< NUM_OF_THREADS; i++) {
		if (thread_args[i].min_elem < min) {
			min = thread_args[i].min_elem;
			min_row = thread_args[i].min_elem_row;
			min_col = thread_args[i].min_elem_col;
		}
	}
	
	gettimeofday(&stop_time, NULL);

	/*
	// print mult_T
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			printf("%f\t", mult_T[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	
	// Final results
	printf("Min value is %f at coordinates (%d, %d)\n", min, min_row, min_col);
	long int start_time_final, stop_time_final;
	start_time_final = (long int)start_time.tv_sec * 1000000 + (long int)start_time.tv_usec;
	stop_time_final = (long int)stop_time.tv_sec * 1000000 + (long int)stop_time.tv_usec;
	//printf("start final time is %ld \n", start_time_final);
	//printf("stop final time is %ld \n", stop_time_final);
	
	printf("multiplication time: %ld microseconds\n", stop_time_final - start_time_final);

}
