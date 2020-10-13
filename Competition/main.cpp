///////////////////////////////
// 
///////////////////////////////

#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits>

#include "main_ispc.h"
//#include "tasksys.h"
using namespace ispc;

#ifndef SIZE
	#define SIZE 10
#endif

#ifndef NUM_OF_TASKS
	#define NUM_OF_TASKS 4
#endif

#define PADDING_SIZE 8

float mat_A[SIZE][SIZE] = {};
float mat_B[SIZE][SIZE] = {};
float mat_B_T[SIZE][SIZE] = {};
float mult[SIZE][SIZE] =  {};

int main() {
	struct timeval start_time, stop_time;
	int min_row = 0;
	int min_col = 0;
	float min = std::numeric_limits<float>::max();

	int min_elem_per_task = SIZE/NUM_OF_TASKS;
	int remainder_elems = (SIZE%NUM_OF_TASKS);

	int elems_in_partition[NUM_OF_TASKS] = {};
	float min_elem_in_task[NUM_OF_TASKS] = {};
	int min_row_in_task[NUM_OF_TASKS] = {};
	int min_col_in_task[NUM_OF_TASKS] = {};

	for (int i=0; i < NUM_OF_TASKS; i++) {
		min_elem_in_task[i] = min;
	}
	// Array initialization
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mat_A[i][j] = rand()/(float)1147483648;
			mat_B[i][j] = rand()/(float)1147483648;
		}
	}

	gettimeofday(&start_time, NULL);
	
	// Splitting work-load
	for (int i=0; i<NUM_OF_TASKS; i++) {
		elems_in_partition[i] = min_elem_per_task + (int)(remainder_elems != 0);
		if (remainder_elems > 0)
			remainder_elems--;
	}

	// Transpose matrix
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
	matrix_mult_ISPC_tasks( mat_A, mat_B_T, mult, elems_in_partition, min_elem_in_task, min_row_in_task, min_col_in_task);

	// Finding min element among the group of min elems
	for (int i=0; i < NUM_OF_TASKS; i++) {
		if (min_elem_in_task[i] < min) {
			min = min_elem_in_task[i];
			min_row = min_row_in_task[i];
			min_col = min_col_in_task[i];
		}
	}
	gettimeofday(&stop_time, NULL);

	/*
	// print B_T
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			printf("%f\t", mult[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	
	
	printf("\n*****************************\n");
	printf("Competition code\n");
	printf("Min value is %f at coordinates (%d, %d)\n", min, min_row, min_col);
	long int start_time_final, stop_time_final;
	start_time_final = (long int)start_time.tv_sec * 1000000 + (long int)start_time.tv_usec;
	stop_time_final = (long int)stop_time.tv_sec * 1000000 + (long int)stop_time.tv_usec;
	//printf("start final time is %ld \n", start_time_final);
	//printf("stop final time is %ld \n", stop_time_final);
	
	printf("multiplication time: %ld microseconds\n", stop_time_final - start_time_final);
	printf("\n*****************************\n");

}
