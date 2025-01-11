#include "root_file_path_constructor.h"

#include <stdlib.h>
#include <string.h>

#include "../../error_handler/error_handler.h"

#pragma warning (disable: 4996)

char* construct_root_file_path(const char* root_relative_path, const char* file_name, const char* file_ext) {
    size_t buffer_size = strlen(root_relative_path) + strlen(file_name) + strlen(file_ext) + 2;
    char* file_path = malloc(buffer_size);

    if (!file_path) {
        handle_error(&(AppError) {
            LEVEL_ERROR_PANIC,
            ERROR_MEMORY_ALLOCATION
        });

        return NULL;
    }

    strcpy(file_path, root_relative_path);
    strcat(file_path, file_name);
    strcat(file_path, file_ext);

    return file_path;
}