#include "error_handler.h"

#include <stdio.h>
#include <stdlib.h>

#include "../stack/stack.h"

static const char *default_error_message = "An unknown error has occurred";
static const char *error_messages[] = {
    "The stack is empty",
    "No item was found for given conditions",
    "Possible memory leak, make sure that all of your data types are deallocated properly (they have proper destructror passed as dependency to generic stack_push method)",
    "Error when allocating memory has occurred",
    "Invalid pointer error has occurred",
    "Error while pushing to the stack",
    "Implementation error, some data type has not print method associated with it",
    "Incorrect data was provided",
    "Error while trying to open file",
    "Failed to read data from file",
    "Fatal error has occurred"
};

void handle_error(AppError *error) {
    const char *message = error_messages[error->code] ? error_messages[error->code] : default_error_message;

    puts(message);

    if (error->level == LEVEL_ERROR_PANIC) {
        stack_free();
        system("pause");
    }
}
