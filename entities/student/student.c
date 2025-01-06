#include "student.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../error-handler/error_handler.h"

void student_print(void *ptr)
{
    Student *student = (Student *)ptr;

    if (!student) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_READ_DATA_FROM_POINTER,
        });

        return;
    }

    printf("Lastname          : %s\n", student->last_name);
    printf("Birth year        : %d\n", student->birth_year);
    printf("Field of study    : %s\n", fields_of_studies[student->field_of_study - 1]);
}

void *student_init(char *last_name, int birth_year, enum FIELD_OF_STUDY field_of_study) {
    Student *student = (Student *) malloc(sizeof(Student));

    if (student == NULL) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_MEMORY_ALLOCATION
        });

        return NULL;
    }

    student->last_name = malloc(strlen(last_name) + 1);

    if (!student->last_name) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_MEMORY_ALLOCATION
        });

        free(student);
        return NULL;
    }

    strcpy(student->last_name, last_name);
    student->birth_year = birth_year;
    student->field_of_study = field_of_study;

    return (void *)(student);
}

void *student_push(char *last_name, int birth_year, enum FIELD_OF_STUDY field_of_study) {
    return student_init(last_name, birth_year, field_of_study);
}

void student_free(void *ptr) {
    Student *student = (Student *)ptr;

    if (!student) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_READ_DATA_FROM_POINTER,
        });
    } else {
        free(student->last_name);
        free(student);
    }
}

void print_fields_of_studies() {
    for (size_t i = 1; i<=ARTIFICIAL_INTELLIGENCE; i++) {
        printf("%lu - %s\n", i, fields_of_studies[i - 1]);
    }
}

void student_serialize(void *data, FILE *file) {
    Student *student = (Student *)data;

    fwrite(&student->birth_year, sizeof(int), 1, file);

    fwrite(&student->field_of_study, sizeof(enum FIELD_OF_STUDY), 1, file);

    size_t last_name_length = strlen(student->last_name) + 1;
    fwrite(&last_name_length, sizeof(size_t), 1, file);

    fwrite(student->last_name, sizeof(char), last_name_length, file);
}

void *student_deserialize(FILE *file) {
    Student *student = malloc(sizeof(Student));
    if (!student) {
        handle_error(&(AppError){
            LEVEL_ERROR_PANIC,
            ERROR_MEMORY_ALLOCATION,
        });

        return NULL;
    }

    if (fread(&student->birth_year, sizeof(int), 1, file) != 1) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_FAILED_TO_READ_DATA_FROM_FILE,
        });

        student_free(student);
        return NULL;
    }

    if (fread(&student->field_of_study, sizeof(enum FIELD_OF_STUDY), 1, file) != 1) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_FAILED_TO_READ_DATA_FROM_FILE,
        });

        student_free(student);
        return NULL;
    }

    size_t last_name_length;
    if (fread(&last_name_length, sizeof(size_t), 1, file) != 1) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_FAILED_TO_READ_DATA_FROM_FILE,
        });

        student_free(student);
        return NULL;
    }

    student->last_name = malloc(last_name_length);
    if (!student->last_name) {
        handle_error(&(AppError){
           LEVEL_ERROR_PANIC,
           ERROR_MEMORY_ALLOCATION,
       });

        return NULL;
    }

    if (fread(student->last_name, sizeof(char), last_name_length, file) != last_name_length) {
        handle_error(&(AppError){
            LEVEL_ERROR,
            ERROR_FAILED_TO_READ_DATA_FROM_FILE,
        });

        student_free(student);
        return NULL;
    }

    return student;
}

bool student_match_full(void *data, void *search_data) {
    Student *student = (Student *)data;
    Student *search_criteria = (Student *)search_data;

    if (search_criteria->last_name != NULL && strcmp(student->last_name, search_criteria->last_name) != 0) {
        return false;
    }

    if (search_criteria->birth_year != 0 && student->birth_year != search_criteria->birth_year) {
        return false;
    }

    if (student->field_of_study != search_criteria->field_of_study) {
        return false;
    }

    return true;
}

bool validate_field_of_study(int field_of_study) {
    return field_of_study >= COMPUTER_SCIENCE && field_of_study <= ARTIFICIAL_INTELLIGENCE;
}