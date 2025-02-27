#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#pragma once

typedef enum {
    LEVEL_INFO,
    LEVEL_WARNING,
    LEVEL_ERROR,
    LEVEL_ERROR_PANIC
} ERROR_LEVEL;

typedef enum {
    INFO_MESSAGE_EMPTY_STACK,
    INFO_MESSAGE_NOT_FOUND,
    WARNING_POSSIBLE_MEMORY_LEAK,
    ERROR_MEMORY_ALLOCATION,
    ERROR_READ_DATA_FROM_POINTER,
    ERROR_PUSHING_TO_THE_STACK,
    ERROR_IMPLEMENTATION_INVALID,
    ERROR_INVALID_USER_INPUT,
    ERROR_FAILED_TO_OPEN_FILE,
    ERROR_FAILED_TO_READ_DATA_FROM_FILE,
    ERROR_FAILED_TO_READ_NUMBER_FROM_SCANF,
    ERROR_EXIT
} ERRORS_MAP;

typedef struct AppError {
    ERROR_LEVEL level;
    ERRORS_MAP code;
} AppError;

void handle_error(AppError* error);

#endif //ERROR_HANDLER_H