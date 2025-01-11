#include "stack.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../constants.h"
#include "../error_handler/error_handler.h"
#include "../lib/root_file_path_constructor/root_file_path_constructor.h"

static Stack* last_item = NULL;
FreeData free_data_pointer;

void stack_init(const FreeData* p_free_data) {
    free_data_pointer = *p_free_data;
    last_item = NULL;
}

void stack_free() {
    Stack* current = last_item;

    while (current != NULL) {
        Stack* temp = current->prev;

        if (current->destroy) {
            current->destroy(current->data);
        }
        else {
            handle_error(&(AppError) {
                LEVEL_WARNING,
                WARNING_POSSIBLE_MEMORY_LEAK
            });
        }

        free(current);
        current = temp;
    }

    last_item = NULL;
}

void stack_display() {
    Stack* current = last_item;

    while (current != NULL) {
        if (current->print != NULL) {
            if (current != last_item) {
                printf(OUTPUT_DELIMITER_LIGHT);
            }

            current->print(current->data);
        }
        else {
            handle_error(&(AppError) {
                LEVEL_ERROR_PANIC,
                ERROR_IMPLEMENTATION_INVALID
            });
        }

        current = current->prev;
    }
}

Stack* stack_push(void* data, void (*print)(void* data), void (*serialize)(void* data, FILE* file), void* (*deserialize)(FILE* file), void (*destroy)(void* data)) {
    Stack* new_stack_item = (Stack*)malloc(sizeof(Stack));
    if (new_stack_item == NULL) {
        handle_error(&(AppError) {
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
    }
    else {
        new_stack_item->destroy = NULL;
    }

    last_item = new_stack_item;

    return new_stack_item;
}

void* stack_pop() {
    if (last_item == NULL) {
        handle_error(&(AppError) {
            LEVEL_INFO,
            INFO_MESSAGE_EMPTY_STACK
        });

        return NULL;
    }

    Stack* p = last_item;
    void* data = p->data;

    last_item = last_item->prev;
    free(p);

    return data;
}

void** stack_search(bool (*match)(void* data, void* searchData), void* searchData, size_t* matchCount) {
    size_t capacity = 2;
    Stack* current = last_item;
    *matchCount = 0;

    void** results = malloc(capacity * sizeof(void*));

    if (!results) {
        handle_error(&(AppError) {
            LEVEL_ERROR_PANIC,
            ERROR_MEMORY_ALLOCATION
        });

        return NULL;
    }

    while (current != NULL) {
        if (!match(current->data, searchData)) {
            current = current->prev;
            continue;
        }

        if (*matchCount == capacity) {
            // I like this concept of capacity from Go standard library.
            // In append function implementation it's done the same way.
            // In order to prevent wasting memory, after scaling capacity to 256, we multiply by smaller value.
            if (capacity >= 256) {
                capacity = (size_t)floor(capacity * 1.25);
            }
            else {
                capacity *= 2;
            }

            void** resized_results = realloc(results, capacity * sizeof(void*));

            if (!resized_results) {
                free(results);

                handle_error(&(AppError) {
                    LEVEL_ERROR_PANIC,
                    ERROR_MEMORY_ALLOCATION
                });

                return NULL;
            }

            results = resized_results;
        }


        results[*matchCount] = current->data;
        *matchCount += 1;
        current = current->prev;
    }

    if (*matchCount == 0) {
        free(results);
        return NULL;
    }

    return results;
}

void* stack_save_to_file(char* fileName) {
    char* file_path = construct_root_file_path("./", fileName, ".bin");
    FILE* file = fopen(file_path, "wb");

    if (!file) {
        free(file_path);

        handle_error(&(AppError) {
            LEVEL_ERROR_PANIC,
            ERROR_EXIT
        });

        return NULL;
    }

    Stack* current = last_item;
    while (current != NULL) {
        if (current->serialize) {
            current->serialize(current->data, file);
        }
        else {
            handle_error(&(AppError) {
                LEVEL_ERROR_PANIC,
                ERROR_IMPLEMENTATION_INVALID
            });
        }
        current = current->prev;
    }

    free(file_path);
    fclose(file);

    //SF: warning C4033: 'stack_save_to_file' must return a value
    return NULL;
}

void* stack_read_from_file(char* fileName, void (*print)(void* data), void (*serialize)(void* data, FILE* file), void* (*deserialize)(FILE* file), void (*destroy)(void* data)) {
    char* file_path = construct_root_file_path("./", fileName, ".bin");
    FILE* file = fopen(file_path, "rb");
    free(file_path);

    if (!file) {
        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_FAILED_TO_OPEN_FILE
        });

        return NULL;
    }

    last_item = NULL;

    while (true) {
        void* data = deserialize(file);

        if (data == NULL) {
            break;
        }

        stack_push(data, print, serialize, deserialize, destroy);
    }

    fclose(file);
    //SF: warning C4715: 'stack_read_from_file': not all control paths return a value

    return NULL;
}

void stack_reverse() {
    if (last_item == NULL) {
        return;
    }

    Stack* prev = NULL;
    Stack* current = last_item;
    Stack* next = NULL;

    while (current != NULL) {
        next = current->prev;
        current->prev = prev;
        prev = current;
        current = next;
    }

    last_item = prev;
}