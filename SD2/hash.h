// Copyright 2020 Pescaru Tudor-Mihai 315CA
#ifndef HASH_H_
#define HASH_H_

// Struct that defines the contents of a bucket
struct info {
    void *key;
    int count;
};

// Struct for Hashtable
struct Hashtable {
    int hmax;
    // Array of buckets
    struct info *buckets;
};

// Fucntion that initailises Hashtable
void init(struct Hashtable *h, int nr_buckets);

// Function that adds to hashtable
void put(struct Hashtable *h, void *key, int bytes);

// Function that prints Hashtable elements
void get_all(struct Hashtable *h);

// Function that frees memory allocated for Hashtable
void free_ht(struct Hashtable *h);

// Function to compare keys
int compare(void *a, void *b);

// Hashing function
unsigned int hash(void *a);

#endif  // HASH_H_
