#include "text_input.h"

#include <stdlib.h>

#include "../../error_handler/error_handler.h"

#define INPUT_BUFFER_INCREMENT 16

char* text_input(FILE* fp, size_t size) {
    char* str = malloc(size);
    if (!str) {
        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_MEMORY_ALLOCATION
        });
        return NULL;
    }

    size_t len = 0;
    int ch;

    while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
        if (len + 1 == size) {
            char* new_str = realloc(str, size += INPUT_BUFFER_INCREMENT);
            if (!new_str) {
                free(str);
                handle_error(&(AppError) {
                    LEVEL_ERROR,
                    ERROR_MEMORY_ALLOCATION
                });
                return NULL;
            }
            str = new_str;
        }
        str[len++] = ch;
    }

    str[len] = '\0';
    return str;
}