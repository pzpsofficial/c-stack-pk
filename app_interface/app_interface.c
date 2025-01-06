#include "app_interface.h"

#include <stdlib.h>

#include "../constants.h"
#include "../entities/student/student.h"
#include "../error-handler/error_handler.h"
#include "../lib/text_input/text_input.h"
#include "../stack/stack.h"

static const char *menu_items[] = {
    "0 - Display",
    "1 - Push",
    "2 - Pop",
    "3 - Search",
    "4 - Save to file",
    "5 - Read from file",
    "6 - Exit"
};

void menu() {
    printf(OUTPUT_DELIMITER);
    printf("Menu:\n");
    printf(OUTPUT_DELIMITER);

    for (size_t i = 0; i<=APP_INTERFACE_EXIT; ++i) {
        printf("%s\n", menu_items[i]);
    }

    printf(OUTPUT_DELIMITER);
}

void display() {
    stack_display();
}

void push() {
    int birth_year, field_of_study;

    printf(BIRTH_YEAR_LABEL);
    scanf("%d", &birth_year);

    printf(FIELDS_OF_STUDIES_LABEL);
    print_fields_of_studies();
    printf(YOUR_CHOICE_LABEL);
    scanf("%d", &field_of_study);

    while (getchar() != '\n') {};
    printf(LAST_NAME_LABEL);
    char *last_name = text_input(stdin, 10);

    if (!validate_field_of_study(field_of_study)) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_INVALID_USER_INPUT
        });

        return;
    }

    void *data = student_push(last_name, birth_year, field_of_study);
    if (!stack_push(data, student_print, student_serialize, student_deserialize, student_free)) {
        handle_error(&(AppError){
              LEVEL_ERROR,
        ERROR_PUSHING_TO_THE_STACK
        });
    }

    free(last_name);
}

void pop() {
    Student *found_student = (Student *)stack_pop();

    if (found_student == NULL) {
        return;
    }

    printf(STACK_ITEM_POPPED);
    student_print(found_student);
    student_free(found_student);
}

void search() {
    int birth_year, field_of_study;

    printf(SEARCH_BIRTH_YEAR_LABEL);
    scanf("%d", &birth_year);

    printf(SEARCH_FIELDS_OF_STUDIES_LABEL);
    print_fields_of_studies();
    printf(YOUR_CHOICE_LABEL);
    scanf("%d", &field_of_study);

    while (getchar() != '\n') {};
    printf(SEARCH_LAST_NAME_LABEL);
    char *last_name = text_input(stdin, 10);

    Student search_criteria = {
        .last_name = last_name,
        .birth_year = birth_year,
        .field_of_study = field_of_study,
    };

    Student *found_student = (Student *)stack_search(student_match_full, &search_criteria);

    if (found_student != NULL) {
        student_print(found_student);
    } else {
        handle_error(&(AppError){
            LEVEL_INFO,
            INFO_MESSAGE_NOT_FOUND
        });
    }
}

void save_to_file() {
    while (getchar() != '\n') {};
    printf(ENTER_FILE_NAME);
    char *filename = text_input(stdin, 10);

    stack_save_to_file(filename);
}

void read_from_file() {
    while (getchar() != '\n') {};
    printf(ENTER_FILE_NAME);
    char *filename = text_input(stdin, 10);

    stack_free();
    stack_read_from_file(filename, student_print, student_serialize, student_deserialize, student_free);
    stack_reverse();
}

void app_exit() {
    stack_free();
    printf(THANK_YOU_MESSAGE);
}