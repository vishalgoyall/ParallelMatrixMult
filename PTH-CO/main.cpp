///////////////////////////////
// Sequential code to do matrix multiplication of two arrays
///////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#ifndef SIZE
	#define SIZE 10
#endif

#ifndef NUM_OF_THREADS
	#define NUM_OF_THREADS 4
#endif

typedef struct {
	float (*A)[SIZE];
	float (*B)[SIZE];
	float (*mult_T)[SIZE];
	int start_row;
	int num_rows;
	int thread_no;
} args;

void *sub_matrix_mult (void* thread_args) {
	args *local_args = (args*)thread_args;
	//args local_args = (args)thread_args;
	//printf("%d first elem of A is %f\n", local_args->thread_no, local_args->A[local_args->start_row][0]);
	//printf("thread %d first element of A is %d\n", local_args->thread_no, local_args->A);
	for (int i=local_args->start_row; i < (local_args->start_row + local_args->num_rows); i++) {
		for (int j=0; j<SIZE; j++) {
			for (int k=0; k<SIZE; k++) {
				local_args->mult_T[i][j] += local_args->A[i][k] * local_args->B[j][k];
			}
		}
	}
	//local_args->mult_T[local_args->start_row][0] = local_args->thread_no;
}

int main() {
	float mat_A[SIZE][SIZE] = {};
	float mat_B[SIZE][SIZE] = {};
	float mat_B_T[SIZE][SIZE] = {};
	float mult_T[SIZE][SIZE] =  {};
	struct timeval start_time, stop_time;
	args thread_args[NUM_OF_THREADS];

	pthread_t threads[NUM_OF_THREADS];
	
	int min_elem_per_thread = SIZE/NUM_OF_THREADS;
	int remainder_elems = (SIZE%NUM_OF_THREADS);

	int elems_in_partition[NUM_OF_THREADS] = {};
	for (int i=0; i<NUM_OF_THREADS; i++) {
		elems_in_partition[i] = min_elem_per_thread + (int)(remainder_elems != 0);
		if (remainder_elems > 0)
			remainder_elems--;
	}
	int elems_processed = 0;

	// Array initialization
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mat_A[i][j] = (float)(rand() % 10);
			mat_B[i][j] = (float)(rand() % 10);
		}
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
	
	gettimeofday(&start_time, NULL);

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
	
	for (int i=0; i< NUM_OF_THREADS; i++) {
		pthread_join(threads[i], NULL);
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
	long int start_time_final, stop_time_final;
	start_time_final = (long int)start_time.tv_sec * 1000000 + (long int)start_time.tv_usec;
	stop_time_final = (long int)stop_time.tv_sec * 1000000 + (long int)stop_time.tv_usec;
	//printf("start final time is %ld \n", start_time_final);
	//printf("stop final time is %ld \n", stop_time_final);
	
	printf("multiplication time: %ld microseconds\n", stop_time_final - start_time_final);

}
