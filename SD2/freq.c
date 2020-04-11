// Copyright 2020 Pescaru Tudor-Mihai 315CA
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *v, x, i;
    // Dynamic allocation of frequency vector using given max element
    v = (int*)calloc(2000002, sizeof(int));
    // Reading inputs and incrementing their respective frequency value
    while (!feof(stdin)) {
        fscanf(stdin, "%d\n", &x);
        v[x]++;
    }
    // Printing elements alongside their frequency
    for (i = 0; i <= 2000000; i++) {
        if (v[i] != 0) {
            printf("%d %d\n", i, v[i]);
        }
    }
    free(v);
    return 0;
}
