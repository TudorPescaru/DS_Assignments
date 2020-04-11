// Copyright 2020 Pescaru Tudor-Mihai 315CA
#include <stdio.h>
#include <stdlib.h>

// Hashing function for integers
unsigned int hash(void *a) {
	unsigned int uint_a = *((unsigned int *)a);
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
	uint_a = (uint_a >> 16u) ^ uint_a;
	return uint_a;
}

int main(int argc, char **argv) {
    int m, *M, n, j, x, bits, k, count_used = 0, E;
    unsigned int h;
    double Z = 0, alpha;
    FILE *f;
    // Check if apropriate argument was given
    if (argc < 2) {
		fprintf(stderr, "Expected usage: ./hll <file_name>\n");
		exit(0);
	}
	// Set initial division for index and 0 count portions
	k = 11;
    m = 1 << k;
    // Allocate HLL vector of elements
    M = (int*)calloc(m, sizeof(int));
	f = fopen(argv[1], "r");
	// Read inputs from given file
	while (!feof(f)) {
		fscanf(f, "%d\n", &n);
		// Compute hash
		h = hash(&n);
		// Get index bits using mask and increment used vector element
		j = ((1 << k) - 1) & (h >> (32 - k));
		// Apply mask to keep only bits for 0 count portion
		h = ((1 << (32 - k)) - 1) & h;
		// Find postion of leftmost 1 bit using indexing starting with 1
		x = 1;
		bits = 32 - k;
		while (bits) {
			if ((h & (1 << (bits - 1))) == 0) {
				x++;
			} else {
				// Break at first 1 bit
				break;
			}
			bits--;
		}
		// Add postion to HLL vector if bigger than existing
		if (x > M[j]) {
			M[j] = x;
		}
	}
	// Compute agregation of HLL vector values
	for (j = 0; j < m; j++) {
		// Use only used buckets
		if (M[j] != 0) {
			Z = Z + (1.00 / (1 << M[j]));
			count_used++;
		}
	}
	Z = 1.00 / Z;
	// Compute atenuation factor
	alpha = 0.7213 / (1 + 1.079 / count_used);
	// Compute probable number of distinct elements
	E = alpha * count_used * count_used * Z;
	// Print and free memory
	printf("%d\n", E);
	fclose(f);
    free(M);
    return 0;
}
