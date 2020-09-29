/**
 * Austin Van Braeckel - 1086829 - avanbrae@uoguelph.ca
 * 2020-09-25
 * CIS2520 Assignment 2
 * Contains all function prototypes for the array ADT for Assignment 2
 */

#include "array.h"

// ------------------------------ BASIC FUNCTIONS ------------------------------

/**
 * Allocates sufficient memory for a Performance structure, and initializes it.
 */
struct Performance* newPerformance() {
    struct Performance* p = malloc(sizeof(struct Performance));
    if ( p == NULL ) { // allocation failed
        fprintf( stderr, "ERROR: Memory allocation failure\n");
        exit(-1);
    }

    p->frees = 0;
    p->mallocs = 0;
    p->reads = 0;
    p->writes = 0;

    return p;
}

/**
 * Allocates sufficient memory for an Array structure, and initializes it with
 * the given values for width and capacity.
 */
struct Array* newArray(struct Performance* performance, unsigned int width, unsigned int capacity) {
    struct Array* arr = malloc(sizeof(struct Array));
    if ( arr == NULL ) { // allocation failed
        fprintf( stderr, "ERROR: Memory allocation failure\n");
        // Free Performance struct before exiting
        free(performance);
        exit(-1);
    }

    arr->width = width;
    arr->capacity = capacity;
    arr->nel = 0;
    arr->data = malloc(width*capacity);

    // Increment mallocs in the Performance struct
    performance->mallocs++;

    return arr;
}

/**
 * Reads the data from the given Array struct and copies to the specified destination.
 */
void readItem(struct Performance* performance, struct Array* array, unsigned int index, void* dest) {
    if ( index >= array->nel ) { // index is not in bounds of array
        fprintf( stderr, "ERROR: Read index is out of bounds\n");
        // Free Array and Performance structs before exiting
        freeArray(performance, array);
        free(performance);
        exit(-1);
    }

    memcpy(dest, (char*)(array->data) + (index * array->width), array->width);

    // Increment reads in Performance struct
    performance->reads++;
}

/**
 * Writes the data from the given source to the given Array at the specified index.
 */
void writeItem(struct Performance* performance, struct Array* array, unsigned int index, void* src) {

    if ( index > array->nel || index >= array->capacity ) { // index is not in bounds of array's current and/or maximum capacity
        fprintf( stderr, "ERROR: Write index is out of bounds - %d\n", index);
        // Free Array and Performance structs before exiting
        freeArray(performance, array);
        free(performance);
        exit(-1);
    }

    memcpy( (char*)(array->data) + (index * array->width), src, array->width);

    // Increases number of elements in the array and increments writes in Performance struct
    if ( index == array->nel ) {
        array->nel++;
    }
    performance->writes++;
}

/**
 * Decrements the given Array's number of elements by one.
 */
void contract(struct Performance *performance, struct Array* array) {
    if ( array->nel == 0 ) { // Array has 0 elements
        fprintf( stderr, "ERROR: Contract failed since Array is empty\n");
        // Free Array and Performance structs before exiting
        freeArray(performance, array);
        free(performance);
        exit(-1);
    }

    array->nel--;
}

/**
 * Frees both the given Array structure itself as well as the data within it,
 * incrementing the number of frees in the given performance struct by one.
 */
void freeArray(struct Performance* performance, struct Array* array) {
    free(array->data);
    free(array);
    performance->frees++;
}

// ------------------------------ DERIVED FUNCTIONS ------------------------------

/**
 * Adds an element to the end of the array
 */
void appendItem(struct Performance* performance, struct Array* array, void *src) {
    writeItem(performance, array, array->nel, src);
}

/**
 * Moves the elements int he array at the given index position and higher, one
 * further back, and then write the given data from the source at the same specified 
 * index in the array.
 */
void insertItem(struct Performance* performance, struct Array* array, unsigned int index, void* src) {
    int i;
    char prev[array->width];
    char next[array->width];

    readItem(performance, array, index, prev);
    for (i = index; i < array->nel; i++ ) {
        if ( i < array->nel - 1 ) {
            readItem(performance, array, i + 1, next);
            writeItem(performance, array, i + 1, prev);

            memcpy(prev, next, array->width);
        } else { // last item - written onto the end
            writeItem(performance, array, i + 1, prev);
            i = array->capacity + 1; // End loop
        }
    }

    writeItem(performance, array, index, src); // Write the given value to be inserted at the given index after shifting
}

/**
 * Inserts data from the given source at the index of 0 in the given array.
 */
void prependItem(struct Performance* performance, struct Array* array, void* src) {
    insertItem(performance, array, 0, src);
}

/**
 * Deletes the element at the given index.
 */
void deleteItem(struct Performance* performance, struct Array* array, unsigned int index) {
    int i;
    char prev[array->width];
    char next[array->width];

    readItem(performance, array, array->nel - 1, prev);
    for (i = array->nel - 1; i > index; i-- ) {
        readItem(performance, array, i - 1, next);
        writeItem(performance, array, i - 1, prev);
        memcpy(prev, next, array->width);
    }

    contract(performance, array); // Remove the duplicate last entry
}


/**
 * Finds the index at which the element in the array matches the given target, using the given compar function.
 * It returns the index of the matching element, and if no match is found, it returns -1. (Linear search)
 */
int findItem(struct Performance* performance, struct Array* array, int (*compar)(const void*, const void*), void* target) {
    int i;
    char element[array->width];

    for ( i = 0; i < array->nel; i++ ) {
        readItem(performance, array, i, element);
        if ( compar(element, target) == 0 ) { // Match found
            return i;
        } 
    }

    return -1; //No match found
}

/**
 * Searches the given array for the index at which the element in the array matches the given target, using the compar function.
 * It returns the index of the matching element, and if no match is found, it returns -1. (Binary search - recursive)
 */
int searchItem(struct Performance* performance, struct Array* array, int (*compar)(const void*, const void*), void* target) {
    int result = binarySearch(performance, array, compar, target, 0, array->nel - 1);
    return result;
}

// -------------------- Helper recursive function for searchItem ----------------------
int binarySearch(struct Performance* performance, struct Array* array, int (*compar)(const void*, const void*), void* target, int start, int end) {
    char element[array->width];
    int result = 0;
    
    if ( end < start) {
        return -1;
    } else if ( end == start ) {
        readItem(performance, array, start, element);
        return compar(element, target) ? -1 : start;
    } else {  

        int mid = (start + (end - 1)) / 2; // midpoint (rounded down to lower half)

        readItem(performance, array, mid, element); 
        result = compar(element, target);

        // If the element is the midpoint 
        if ( result == 0 ) { 
            return mid; 
        } else if ( result > 0 ) { // mid element value is less than the target, so search upper half
            return binarySearch(performance, array, compar, target, start, mid - 1); 
        } else { // otherwise mid element value is greater than the target, so search lower half
            return binarySearch(performance, array, compar, target, mid + 1, end); 
        }

    }
}

