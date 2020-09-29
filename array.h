/**
 * Austin Van Braeckel - 1086829 - avanbrae@uoguelph.ca
 * 2020-09-25
 * CIS2520 Assignment 2
 * Header file containing all function protoypes, declarations, and #includes
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Structs

/**
 * Represents an array data structure, where width represents the size in bytes of each element in the array,
 * nel represents the number of elements currently in the array, capacity represents the total number of
 * elements that can be stored in the array, and data is a pointer to the contents of the array.
 */
struct Array {
    unsigned int width;
    unsigned int nel;
    unsigned int capacity;
    void *data;
};

/**
 * A structure that is used to measure the performance of the code, counting the number
 * of memory read, memory write, malloc and free operations
 */
struct Performance {
    unsigned int reads;
    unsigned int writes;
    unsigned int mallocs;
    unsigned int frees;
};

// Basic function prototypes
struct Performance* newPerformance();
struct Array* newArray(struct Performance* performance, unsigned int width, unsigned int capacity);
void readItem(struct Performance* performance, struct Array* array, unsigned int index, void* dest);
void writeItem(struct Performance* performance, struct Array* array, unsigned int index, void* src);
void contract(struct Performance *performance, struct Array* array);
void freeArray(struct Performance* performance, struct Array* array);

// Derived function prototypes
void appendItem(struct Performance* performance, struct Array* array, void *src);
void insertItem(struct Performance* performance, struct Array* array, unsigned int index, void* src);
void prependItem(struct Performance* performance, struct Array* array, void* src);
void deleteItem(struct Performance* performance, struct Array* array, unsigned int index);

int findItem(struct Performance* performance, struct Array* array, int (*compar)(const void*, const void*), void* target);
int searchItem(struct Performance* performance, struct Array* array, int (*compar)(const void*, const void*), void* target);

// Helper functions not outlined in the Assignment Description
int binarySearch(struct Performance* performance, struct Array* array, int (*compar)(const void*, const void*), void* target, int start, int end);

#endif