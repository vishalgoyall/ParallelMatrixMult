///////////////////////////////
// Cache-optimised Sequential code to do matrix multiplication of two arrays
///////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits>

#ifndef SIZE
	#define SIZE 10
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
			mat_A[i][j] = rand()/(float)1147483648;
			mat_B[i][j] = rand()/(float)1147483648;
		}
	}

	// Getting start time
	gettimeofday(&start_time, NULL);

	// Transposing the matrix B
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
	for (int i=0; i < SIZE; i++) {
		for (int j=0; j < SIZE; j++) {
			mult_T[i][j] = 0;
			for (int k=0; k < SIZE; k++) {
				mult_T[i][j] += mat_A[i][k] * mat_B_T[j][k]; 
			}
			if (mult_T[i][j] < min) {
				min = mult_T[i][j];
				min_row = i;
				min_col = j;
			}
			//printf("%f\t", mult_T[i][j]);
		}
		//printf("\n");
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
	printf("SCO code\n");
	printf("Min value is %f at coordinates (%d, %d)\n", min, min_row, min_col);
	long int start_time_final, stop_time_final;
	start_time_final = (long int)start_time.tv_sec * 1000000 + (long int)start_time.tv_usec;
	stop_time_final = (long int)stop_time.tv_sec * 1000000 + (long int)stop_time.tv_usec;
	
	printf("multiplication time: %ld microseconds\n", stop_time_final - start_time_final);
	printf("\n*****************************\n");

}
