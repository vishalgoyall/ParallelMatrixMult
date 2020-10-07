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
