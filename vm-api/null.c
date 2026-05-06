#include <stdlib.h>
#include <stdio.h>

typedef struct 
{
    int *data;
    int size;
    int capacity;

} vector_t;


vector_t* vector_create(int capacity) {
    vector_t *vector = malloc(sizeof(vector_t));
    if (!vector) return NULL;

    
    vector->capacity = capacity;
    vector->data = malloc(sizeof(int) * capacity);
    if (!vector->data) return NULL;
    vector->size = 0;
    
    return vector;
}


void vector_push(vector_t* vector, int value) {
    if (vector->size == vector->capacity) {
        int new_capacity = vector->capacity * 2;
        int *new_data = realloc(vector->data, sizeof(int) * new_capacity);

        if (!new_data) return; // or handle error, not important for the exercise

        vector->data = new_data;
        vector->capacity = new_capacity;
    }

    vector->data[vector->size++] = value;
}

int main() {

    vector_t* vector = vector_create(5);

    vector_push(vector, 10);
    vector_push(vector, 10);
    vector_push(vector, 10);
    vector_push(vector, 10);
    vector_push(vector, 10);

    printf("vector size is: %d\n", vector->size);
    printf("vector capacity is: %d\n", vector->capacity);

    vector_push(vector, 2);
    printf("vector size is: %d\n", vector->size);
    printf("vector capacity is: %d\n", vector->capacity);
    printf("value of first index of v is: %d\n", vector->data[0]);

    free(vector->data);
    free(vector);
    // int *x = NULL;
    // y = *x;

    //int* x = malloc(sizeof(int) * 5);
    //x[0] = 42;

    // int *data = malloc(sizeof(int) * 100);

    // for(int i = 0; i < 99; i++) {
    //     data[i] = i;
    // }
    // data[100] = 0; this is obv not correct, bc of indexes and it uses a not allocated space


    // free(data+3); invalid pointer, aborted

    // printf("value of freed space in array: %d\n", data[50]);
}

// gdb shows instruction adress of crash, type of error 

// valgind shows size of memory leak if memory was not freed before exiting the program