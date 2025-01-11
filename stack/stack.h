#ifndef STACK_H
#define STACK_H
#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

typedef struct Stack {
    void* data;
    struct Stack* prev;
    void (*print)(void* data);
    void (*serialize)(void* data, FILE* file);
    void* (*deserialize)(FILE* file);
    void (*destroy)(void* data);
} Stack;

typedef void (*FreeData)(void* dataPointer);
typedef int (*CompareData)(void* data1, void* data2);

void stack_init(const FreeData* freeDataPointer);
void stack_free();
void stack_reverse();
void stack_display();
void* stack_save_to_file(char* fileName);
void* stack_read_from_file(char* fileName, void (*print)(void* data), void (*serialize)(void* data, FILE* file), void* (*deserialize)(FILE* file), void (*destroy)(void* data));
void* stack_pop();
void** stack_search(bool (*match)(void* data, void* searchData), void* searchData, size_t* matchCount);
Stack* stack_push(void* data, void (*print)(void* data), void (*serialize)(void* data, FILE* file), void* (*deserialize)(FILE* file), void (*destroy)(void* data));


#endif //STACK_H