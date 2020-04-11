// Copyright 2020 Pescaru Tudor-Mihai 315CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./hash.h"

// Function to initialise Hashtable
void init(struct Hashtable *h, int nr_buckets) {
    h->hmax = nr_buckets;
    h->buckets = (struct info*)calloc(nr_buckets, sizeof(struct info));
}

// Function to compare two string keys
int compare(void *a, void *b) {
    char *str_a = (char*)a;
    char *str_b = (char*)b;
    return strcmp(str_a, str_b);
}

// Hashing function
unsigned int hash(void *a) {
    unsigned char *puchar_a = (unsigned char*)a;
    unsigned int hash_value = 5381;
    int c;
    while ((c = *puchar_a++)) {
        hash_value = ((hash_value << 5u) + hash_value) + c;
    }
    return hash_value;
}

// Function that adds to Hashtable
void put(struct Hashtable *h, void *key, int bytes) {
    unsigned int ind;
    if (h == NULL || h->buckets == NULL) {
        return;
    }
    // Get bucket index using hash function
    ind = hash(key) % h->hmax;
    // Check if bucket is empty and use it if so
    if (h->buckets[ind].count == 0) {
        h->buckets[ind].count++;
        h->buckets[ind].key = malloc(bytes);
        memcpy(h->buckets[ind].key, key, bytes);
    } else {
    	// If bucket is not empty check if it contains the same element
        if (compare(key, h->buckets[ind].key) == 0) {
        	// Increment frequency
            h->buckets[ind].count++;
        } else {
        	// Find next available bucket
            ind++;
            // Loop to first index if we reach max index
            if ((int)ind == h->hmax) {
                ind = 0;
            }
            // Run until empty bucket is found
            while (h->buckets[ind].count != 0) {
            	// Check if bucket contains same element
                if (compare(h->buckets[ind].key, key) == 0) {
                    h->buckets[ind].count++;
                    return;
                }
                ind++;
				// Loop to first index if we reach max index
                if ((int)ind == h->hmax) {
                    ind = 0;
                }
            }
            // Empty bucket is found, add to it
            h->buckets[ind].count++;
            h->buckets[ind].key = malloc(bytes);
            memcpy(h->buckets[ind].key, key, bytes);
        }
    }
}

// Function to print all elemnts and their frequency
void get_all(struct Hashtable *h) {
    int i;
    if (h == NULL || h->buckets == NULL) {
        return;
    }
    for (i = 0; i < h->hmax; i++) {
        if (h->buckets[i].count != 0) {
            printf("%s %d\n", (char*)h->buckets[i].key, h->buckets[i].count);
        }
    }
}

// Function that frees the memory allocated to Hashtable
void free_ht(struct Hashtable *h) {
    int i;
    if (h == NULL || h->buckets == NULL) {
        return;
    }
    for (i = 0; i < h->hmax; i++) {
        free(h->buckets[i].key);
    }
    free(h->buckets);
    free(h);
}

int main() {
    struct Hashtable *h;
    char *str;
    int count = 0;
    // Allocate memory for Hashtable
    h = (struct Hashtable*)malloc(sizeof(struct Hashtable));
    // Allocate memory for reading buffer
    str = (char*)calloc(100,  sizeof(char));
    // Read inputs while possible and count them
    while (!feof(stdin)) {
        fscanf(stdin, "%s\n", str);
        count++;
    }
    // Initialise buckets using nr of inputs as nr of buckets
    init(h, count);
    // Move back to start of inputs
    fseek(stdin, 0, SEEK_SET);
    // Read inputs again and add them to Hashtable
    while (!feof(stdin)) {
        fscanf(stdin, "%s\n", str);
        put(h, str, strlen(str) + 1);
    }
    // Print elements and free all alocated memory
    free(str);
    get_all(h);
    free_ht(h);
    return 0;
}
