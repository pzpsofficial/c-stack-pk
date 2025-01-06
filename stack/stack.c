#include "stack.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../constants.h"
#include "../error-handler/error_handler.h"
#include "../lib/root_file_path_constructor/root_file_path_constructor.h"

static Stack *last_item = NULL;
FreeData free_data_pointer;

void stack_init(const FreeData *p_free_data) {
    free_data_pointer = *p_free_data;
    last_item = NULL;
}

void stack_free() {
    Stack *current = last_item;

    while (current != NULL) {
        Stack *temp = current;
        current = current->prev;

        if (current && current->destroy) {
            current->destroy(current->data);
        } else if (current) {
            handle_error(&(AppError){
                LEVEL_WARNING,
                WARNING_POSSIBLE_MEMORY_LEAK
            });
        }

        free(temp);
    }

    last_item = NULL;
}

void stack_display() {
    Stack *current = last_item;

    while (current != NULL) {
        if (current->print != NULL) {
            if (current != last_item) {
                printf(OUTPUT_DELIMITER_LIGHT);
            }

            current->print(current->data);
        } else {
            handle_error(&(AppError){
                LEVEL_ERROR_PANIC,
                ERROR_IMPLEMENTATION_INVALID
            });
        }

        current = current->prev;
    }
}

Stack *stack_push(void *data, void (*print)(void *data), void (*serialize)(void *data, FILE *file), void *(*deserialize)(FILE *file), void (*destroy)(void *data)) {
    Stack *new_stack_item = (Stack *)malloc(sizeof(Stack));
    if (new_stack_item == NULL) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_MEMORY_ALLOCATION
        });

        return NULL;
    }

    new_stack_item->prev = last_item;
    new_stack_item->data = data;
    new_stack_item->print = print;
    new_stack_item->serialize = serialize;
    new_stack_item->deserialize = deserialize;

    if (destroy) {
        new_stack_item->destroy = destroy;
    } else {
        new_stack_item->destroy = NULL;
    }

    last_item = new_stack_item;

    return new_stack_item;
}

void *stack_pop() {
    if (last_item == NULL) {
        handle_error(&(AppError){
            LEVEL_INFO,
            INFO_MESSAGE_EMPTY_STACK
        });

        return NULL;
    }

    Stack *p = last_item;
    void *data = p->data;

    last_item = last_item->prev;
    free(p);

    return data;
}

void *stack_search(bool (*match)(void *data, void *search_data), void *search_data) {
    Stack *current = last_item;

    while (current != NULL) {
        if (match(current->data, search_data)) {
            return current->data;
        }

        current = current->prev;
    }

    return NULL;
}

void *stack_save_to_file(char *fileName) {
    char *file_path = construct_root_file_path("./../", fileName, ".bin");
    FILE *file = fopen(file_path, "wb");

    if (!file) {
        free(file_path);

        handle_error(&(AppError){
            LEVEL_ERROR_PANIC,
            ERROR_EXIT
        });
    }

    Stack *current = last_item;
    while (current != NULL) {
        if (current->serialize) {
            current->serialize(current->data, file);
        } else {
            handle_error(&(AppError){
               LEVEL_ERROR_PANIC,
               ERROR_IMPLEMENTATION_INVALID
           });
        }
        current = current->prev;
    }

    fclose(file);

}

void *stack_read_from_file(char *fileName, void (*print)(void *data), void (*serialize)(void *data, FILE *file), void *(*deserialize)(FILE *file), void (*destroy)(void *data)) {
    char *file_path = construct_root_file_path("./../", fileName, ".bin");
    FILE *file = fopen(file_path, "rb");
    free(file_path);

    if (!file) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_FAILED_TO_OPEN_FILE
        });

        return NULL;
    }

    last_item = NULL;

    while (!feof(file)) {
        void *data = deserialize(file);
        if (!data) {
            break;
        }

        stack_push(data, print, serialize, deserialize, destroy);
    }

    fclose(file);
}

void stack_reverse() {
    if (last_item == NULL) {
        return;
    }

    Stack *prev = NULL;
    Stack *current = last_item;
    Stack *next = NULL;

    while (current != NULL) {
        next = current->prev;
        current->prev = prev;
        prev = current;
        current = next;
    }

    last_item = prev;
}