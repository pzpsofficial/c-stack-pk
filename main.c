#include <stdio.h>
#include <stdlib.h>

#include "app_interface/app_interface.h"
#include "constants.h"

int main(void) {
    int operation = 0;

    while (1) {
        menu();
        printf(SELECT_OPERATION_MESSAGE);

        if (scanf("%d", &operation) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (operation) {
            case APP_INTERFACE_DISPLAY:
                display();
            break;
            case APP_INTERFACE_PUSH:
                push();
            break;
            case APP_INTERFACE_POP:
                pop();
            break;
            case APP_INTERFACE_SEARCH:
                search();
            break;
            case APP_INTERFACE_SAVE_TO_FILE:
                save_to_file();
            break;
            case APP_INTERFACE_READ_FROM_FILE:
                read_from_file();
            break;
            case APP_INTERFACE_EXIT:
                app_exit();
                return 0;
            default:
                printf(OPERATION_NOT_SUPPORTED_MESSAGE);
        }
    }

    return 0;
}