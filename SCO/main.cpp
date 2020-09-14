///////////////////////////////
// Sequential code to do matrix multiplication of two arrays
///////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#ifndef SIZE
	#define SIZE 10
#endif

int main() {
	float mat_A[SIZE][SIZE] = {};
	float mat_B[SIZE][SIZE] = {};
	float mat_B_T[SIZE][SIZE] = {};
	float mult[SIZE][SIZE] =  {};
	float mult_T[SIZE][SIZE] =  {};
	struct timeval start_time, stop_time;

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
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mult[i][j] = 0;
			for (int k=0; k < SIZE; k++) {
				mult_T[i][j] += mat_A[i][k] * mat_B_T[j][k]; 
			}
			//printf("%f\t", mult[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");

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
	
	gettimeofday(&stop_time, NULL);
	
	long int start_time_final, stop_time_final;
	start_time_final = (long int)start_time.tv_sec * 1000000 + (long int)start_time.tv_usec;
	stop_time_final = (long int)stop_time.tv_sec * 1000000 + (long int)stop_time.tv_usec;
	printf("start final time is %ld \n", start_time_final);
	printf("stop final time is %ld \n", stop_time_final);
	
	printf("multiplication time: %ld microseconds\n", stop_time_final - start_time_final);

}