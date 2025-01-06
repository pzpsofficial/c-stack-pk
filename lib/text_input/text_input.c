#include "text_input.h"

#include <stdlib.h>

#include "../../error-handler/error_handler.h"

char *text_input(FILE* fp, size_t size){
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(*str)*size);

    if(!str) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_MEMORY_ALLOCATION
        });

        return str;
    };

    while ((ch = getchar()) != '\n' && ch != EOF){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(*str)*(size+=16));

            if(!str) {
                handle_error(&(AppError){
                    LEVEL_ERROR,
                    ERROR_MEMORY_ALLOCATION
                });

                return str;
            };
        }
    }
    str[len++]='\0';
    return realloc(str, sizeof(*str)*len);
}
