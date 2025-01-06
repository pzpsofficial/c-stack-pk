#ifndef STUDENT_H
#define STUDENT_H
#include <stdbool.h>
#include <stdio.h>

static const char *fields_of_studies[] = {
    "Computer Science",
    "Machine Learning",
    "Artificial Intelligence"
};

enum FIELD_OF_STUDY {
    COMPUTER_SCIENCE = 1,
    MACHINE_LEARNING,
    ARTIFICIAL_INTELLIGENCE
};

typedef struct Student {
    int birth_year;
    enum FIELD_OF_STUDY field_of_study;
    char *last_name;
} Student;

void *student_init(char *last_name, int birth_year, enum FIELD_OF_STUDY field_of_study);
void student_free(void *ptr);
void *student_push(char *last_name, int birth_year, enum FIELD_OF_STUDY field_of_study);
void student_display(void *ptr);
void student_print(void *ptr);
void student_serialize(void *ptr, FILE *file);
void *student_deserialize(FILE *file);
void print_fields_of_studies();
bool validate_field_of_study();
bool student_match_full(void *data, void *search_data);
int student_search_last_name(void *current_data, void *search_data);

#endif //STUDENT_H
