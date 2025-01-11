#include "student.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../error_handler/error_handler.h"

#pragma warning (disable: 4996)

void student_print(void* ptr)
{
    Student* student = (Student*)ptr;

    if (!student) {
        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_READ_DATA_FROM_POINTER,
        });

        return;
    }

    printf("Lastname          : %s\n", student->last_name);
    printf("Birth year        : %d\n", student->simple_fields->birth_year);
    printf("Field of study    : %s\n", fields_of_studies[student->simple_fields->field_of_study - 1]);
}

void* student_init(char* last_name, int birth_year, enum FIELD_OF_STUDY field_of_study) {
    Student* student = (Student*)malloc(sizeof(Student));
    StudentSimpleFields* simple_fields = (StudentSimpleFields*)malloc(sizeof(StudentSimpleFields));

    if (!student || !simple_fields) {
        free(student);
        free(simple_fields);

        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_MEMORY_ALLOCATION
        });

        return NULL;
    }

    student->last_name = malloc(strlen(last_name) + 1);

    if (!student->last_name) {
        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_MEMORY_ALLOCATION
        });

        free(student);
        free(simple_fields);
        return NULL;
    }

    strcpy(student->last_name, last_name);
    simple_fields->birth_year = birth_year;
    simple_fields->field_of_study = field_of_study;
    simple_fields->last_name_length = strlen(student->last_name) + 1;
    student->simple_fields = simple_fields;

    return (void*)(student);
}

void* student_push(char* last_name, int birth_year, enum FIELD_OF_STUDY field_of_study) {
    return student_init(last_name, birth_year, field_of_study);
}

void student_free(void* ptr) {
    Student* student = (Student*)ptr;

    if (!student) {
        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_READ_DATA_FROM_POINTER,
        });
    }
    else {
        if (student->last_name) {
            free(student->last_name);
        }

        if (student->simple_fields) {
            free(student->simple_fields);
        }

        free(student);
    }
}

void print_fields_of_studies() {
    for (size_t i = 1; i <= ARTIFICIAL_INTELLIGENCE; i++) {
        printf("%d - %s\n", (int)i, fields_of_studies[i - 1]);
    }
}

void student_serialize(void* data, FILE* file) {
    //SF Patrz kanal "Studia niestacjonarne"
    Student* student = (Student*)data;

    if (fwrite(student->simple_fields, sizeof(StudentSimpleFields), 1, file) != 1) {
        handle_error(&(AppError) {
            LEVEL_ERROR_PANIC,
            ERROR_EXIT
        });
    }

    if (fwrite(student->last_name, sizeof(char), student->simple_fields->last_name_length, file)
        != student->simple_fields->last_name_length)
    {
        handle_error(&(AppError) {
            LEVEL_ERROR_PANIC,
            ERROR_EXIT
        });
    }
}

void* student_deserialize(FILE* file) {
    Student* student = malloc(sizeof(Student));
    StudentSimpleFields* student_simple_fields = malloc(sizeof(StudentSimpleFields));

    if (!student || !student_simple_fields) {
        free(student);
        free(student_simple_fields);

        handle_error(&(AppError) {
            LEVEL_ERROR_PANIC,
            ERROR_MEMORY_ALLOCATION,
        });

        return NULL;
    }

    student->last_name = NULL;
    student->simple_fields = student_simple_fields;

    if (fread(student->simple_fields, sizeof(StudentSimpleFields), 1, file) != 1) {
        if (feof(file)) {
            student_free(student);
            return NULL;
        }

        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_FAILED_TO_READ_DATA_FROM_FILE,
        });

        student_free(student);
        return NULL;
    }

    student->last_name = malloc(student->simple_fields->last_name_length);

    if (!student->last_name) {
        student_free(student);

        handle_error(&(AppError) {
            LEVEL_ERROR_PANIC,
            ERROR_MEMORY_ALLOCATION,
        });

        return NULL;
    }

    if (fread(student->last_name, sizeof(char), student->simple_fields->last_name_length, file) != student->simple_fields->last_name_length) {
        if (feof(file)) {
            student_free(student);
            return NULL;
        }

        handle_error(&(AppError) {
            LEVEL_ERROR,
            ERROR_FAILED_TO_READ_DATA_FROM_FILE,
        });

        student_free(student);
        return NULL;
    }

    return student;
}

bool student_match_full(void* data, void* search_data) {
    Student* student = (Student*)data;
    Student* search_criteria = (Student*)search_data;

    if (search_criteria->last_name != NULL && strcmp(student->last_name, search_criteria->last_name) != 0) {
        return false;
    }

    if (search_criteria->simple_fields->birth_year != 0 && student->simple_fields->birth_year != search_criteria->simple_fields->birth_year) {
        return false;
    }

    if (student->simple_fields->field_of_study != search_criteria->simple_fields->field_of_study) {
        return false;
    }

    return true;
}

bool validate_field_of_study(int field_of_study) {
    return field_of_study >= COMPUTER_SCIENCE && field_of_study <= ARTIFICIAL_INTELLIGENCE;
}