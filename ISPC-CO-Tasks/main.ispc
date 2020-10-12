typedef float row[SIZE];

export uniform float vector_mult (uniform float A[], uniform float B[], uniform int count) {
	uniform float mult;
	float partial = 0.0f;
	foreach (index = 0 ... count) {
		float A_elem = A[index];
		float B_elem = B[index];
		
		partial += A_elem * B_elem;
	}

	mult = reduce_add(partial);

	return mult;
}

static task void part_mult (uniform row A[], uniform row B[], uniform row mult_T[], uniform int elems_in_partition[], uniform float min_elem_in_task[], uniform int min_row_in_task[], uniform int min_col_in_task[]) {
	uniform int index;
	uniform int start;

	index = taskIndex;
	start = 0;
	if (index > 0) {
		for (uniform int j=0; j < index; j++) {
			start += elems_in_partition[j];
		}
	}
	for (uniform int i=start; i < start+elems_in_partition[index]; i++) {
		for (uniform int j=0; j < SIZE; j++) {
			//mult_T[i][j] = 0;
			//for (uniform int k=0; k < SIZE; k++) {
			//	mult_T[i][j] += A[i][k] * B[j][k];
			//}
			mult_T[i][j] = vector_mult(A[i], B[j], SIZE);
			if (mult_T[i][j] < min_elem_in_task[index]) {
				min_elem_in_task[index] = mult_T[i][j];
				min_row_in_task[index] = i;
				min_col_in_task[index] = j;
			}
		}
	}
}

export void matrix_mult_ISPC_tasks(uniform row A[], uniform row B[], uniform row mult_T[], uniform int elems_in_partition[], uniform float min_elem_in_task[], uniform int min_row_in_task[], uniform int min_col_in_task[]) {
	launch[NUM_OF_TASKS] part_mult(A, B, mult_T, elems_in_partition, min_elem_in_task, min_row_in_task, min_col_in_task);
	sync;
}

static task void transpose_row (uniform row B[], uniform row B_T[], uniform int elems_in_partition[]) {
	uniform int index;
	uniform int start;

	index = taskIndex;
	start = 0;
	if (index > 0) {
		for (uniform int j=0; j < index; j++) {
			start += elems_in_partition[j];
		}
	}
	for (uniform int i=start; i < start+elems_in_partition[index]; i++) {
		for (uniform int j=0; j < SIZE; j++) {
			B_T[i][j] = B[j][i];
		}
	}
}

export void matrix_transpose (uniform row B[], uniform row B_T[], uniform int elems_in_partition[]) {
	launch[NUM_OF_TASKS] transpose_row(B, B_T, elems_in_partition);
	sync;
}
